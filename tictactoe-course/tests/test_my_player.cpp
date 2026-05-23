#include <cassert>
#include <iostream>
#include "player/my_observer.hpp"
// #include "player/my_player.hpp"
#include <cstdio>
#include <cstdlib>
#include <iomanip>


#include "../src/player/my_player.hpp"

using namespace ttt::game;
using namespace ttt::my_player;

void test_block_opponent_win() {
    std::cout << "test_block_opponent_win\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    // Set up: O threatens to win at (2,1); it's X's turn and should block
    assert(s.process_move(Sign::X, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 0, 1) == MoveResult::OK);
    assert(s.process_move(Sign::X, 2, 2) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 1) == MoveResult::OK);

    MyPlayer p("test");
    Point mv = p.make_move(s);
    assert(mv.x == 2 && mv.y == 1);
}

void test_block_opponent() {
    std::cout << "Opponent block test\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    assert(s.process_move(Sign::X, 0, 1) == MoveResult::OK); // X
    assert(s.process_move(Sign::O, 2, 2) == MoveResult::OK); // O
    assert(s.process_move(Sign::X, 1, 0) == MoveResult::OK); // X
    assert(s.process_move(Sign::O, 2, 1) == MoveResult::OK); // O

    MyPlayer p("test");
    Point mv = p.make_move(s);
    // std::cout << "Chosen move: (" << mv.x << ", " << mv.y << ")\n";
    assert(mv.x == 2 && mv.y == 0);
}

void test_center_on_empty_board() {
    std::cout << "test_center_on_empty_board\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    MyPlayer p("test");
    Point mv = p.make_move(s);
    // Prefer center on an empty 3x3 board
    assert(mv.x == 1 && mv.y == 1);
}

void test_last_empty_cell() {
    std::cout << "test_last_empty_cell\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);
  
    // Fill board except (2,2)
    // Moves: X(0,0),O(0,1),X(0,2),O(1,0),X(1,1),O(1,2),X(2,0),O(2,1)
    assert(s.process_move(Sign::X, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 0, 1) == MoveResult::OK);
    assert(s.process_move(Sign::X, 0, 2) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 0) == MoveResult::OK);
    assert(s.process_move(Sign::X, 1, 1) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 2) == MoveResult::OK);
    assert(s.process_move(Sign::X, 2, 0) == MoveResult::OK);
    // accidentally winning, which doesnt worsen the test of last empty cel in any way
    assert(s.process_move(Sign::O, 2, 1) == MoveResult::WIN);

      // Now only (2,2) is free and it's X's turn
      MyPlayer p("test");
      Point mv = p.make_move(s);
      assert(mv.x == 2 && mv.y == 2);
}

int main(int argc, char *argv[]) {
  std::cout << "Hello!\n";
  if (argc >= 2) {
    std::srand(atoi(argv[1]));
  }

  ttt::game::State::Opts opts;
  opts.rows = opts.cols = 20;
  opts.win_len = 20;
  opts.max_moves = 0;

  auto field_initializer = ttt::game::RandomObstaclesFI(0.75, 50, 1);

  ttt::my_player::MyPlayer p1("p1");
  ttt::my_player::MyPlayer p2("p2");
  ttt::my_player::ConsoleWriter obs;

  ttt::game::Game game(opts, &field_initializer);
  game.add_player(ttt::game::Sign::X, &p1);
  game.add_player(ttt::game::Sign::O, &p2);
  game.add_observer(&obs);

  obs.print_game_state(game.get_state());
  while (game.process() == ttt::game::MoveResult::OK) {
    obs.print_game_state(game.get_state());
  }
  obs.print_game_state(game.get_state());

  test_block_opponent_win();
  test_block_opponent();
  test_center_on_empty_board();
  test_last_empty_cell();

  std::cout << "All my_player tests passed!\n";
  return 0;
}