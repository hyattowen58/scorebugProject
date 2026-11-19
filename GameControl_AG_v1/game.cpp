// game.cpp
#include "game.h"

Game::Game() {}

Team& Game::batting()  { return s_.half == Half::Top ? s_.away : s_.home; }
Team& Game::fielding() { return s_.half == Half::Top ? s_.home : s_.away; }

void Game::save() { history_.push_back(s_); }

void Game::undo() {
    if (history_.empty()) return;
    s_ = history_.back();
    history_.pop_back();
}

void Game::reset() {
    Team away = s_.away, home = s_.home;  // keep team names
    away.score = away.hits = away.errors = away.walks = 0;
    home.score = home.hits = home.errors = home.walks = 0;
    s_ = GameState{};
    s_.away = away;
    s_.home = home;
    history_.clear();
}

void Game::resetCount() { s_.balls = 0; s_.strikes = 0; }

// ---------- Pitches ----------

void Game::ball() {
    if (s_.gameOver) return;
    save();
    if (++s_.balls >= 4) {
        batting().walks++;
        walk();
    }
}

void Game::strike() {
    if (s_.gameOver) return;
    save();
    if (++s_.strikes >= 3) {
        resetCount();
        addOut();
    }
}

void Game::foul() {
    if (s_.gameOver) return;
    save();
    if (s_.strikes < 2) s_.strikes++;  // a foul never causes strike three
}

void Game::hitByPitch() {
    if (s_.gameOver) return;
    save();
    walk();
}

/*
todo
-baseball in play
-four runner events (inc error)
-walks
-adding runs
-starting the next half of the innning
-ending the game
*/