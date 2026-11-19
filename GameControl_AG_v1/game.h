#pragma once
#include <array>
#include <string>
#include <vector>
// NO RAYLIB DEPENDENCY

enum class Half {Top, Bottom};
enum class HitType {Single = 1, Double = 2, Triple = 3, HomeRun = 4};

struct Team {
    std::string name;
    int score = 0;
    int hits = 0;
    int errors = 0;
    int walks = 0;
};

// Plain structure, you can copy it
struct GameState {
    int inning = 1;
    Half half = Half::Top;
    int balls = 0, strikes = 0, outs = 0;
    std::array<bool, 3> bases{};  // [0]=first, [1]=second, [2]=third
    Team away{"AWAY"};
    Team home{"HOME"};
    bool gameOver = false;
};

class Game {
public:
    Game(); // game class interface

    const GameState& state() const {return s_;}

    // Pitch outcomes
    void ball();
    void strike();
    void foul();
    void hitByPitch();         // also use for catcher's interference

    // Ball in play's outcomes
    void hit(HitType type);
    void batterOut();          // fly out, ground out, etc (ends the at-bat)

    // Runner events (base index: 0=first, 1=second, 2=third)
    void stealBase(int from);
    void advanceRunner(int from);  // tag-up, wild pitch, etc. Scores if from == 2
    void tagOut(int base);         // runner thrown out or tagged out

    void addError();                 // charged to the fielding team

    // Utilities
    void undo();
    void reset();

private:
    GameState s_;
    std::vector<GameState> history_;

    Team& batting();
    Team& fielding();

    void save();               // push state for undo
    void walk();               // force-advance runners, reset count
    void addRun(int n = 1);
    void addOut();
    void resetCount();
    void endHalfInning();
    void startHalfInning();    // ghost runner after the 9th inning
};