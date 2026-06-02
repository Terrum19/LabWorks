#include <cassert>
#include <iostream>
#include "player/my_observer.hpp"
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cmath>

#include "../src/player/my_player.hpp"

using namespace ttt::game;
using namespace ttt::my_player;

// ===== Original existing tests =====

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
    assert(mv.x == 2 && mv.y == 0);
}

void test_center_on_empty_board() {
    std::cout << "test_center_on_empty_board\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    MyPlayer p("test");
    Point mv = p.make_move(s);
    assert(mv.x == 1 && mv.y == 1);
}

void test_last_empty_cell() {
    std::cout << "test_last_empty_cell\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);
  
    // Fill board except (2,2)
    assert(s.process_move(Sign::X, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 0, 1) == MoveResult::OK);
    assert(s.process_move(Sign::X, 0, 2) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 0) == MoveResult::OK);
    assert(s.process_move(Sign::X, 1, 1) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 2) == MoveResult::OK);
    assert(s.process_move(Sign::X, 2, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 2, 1) == MoveResult::WIN);

    MyPlayer p("test");
    Point mv = p.make_move(s);
    assert(mv.x == 2 && mv.y == 2);
}

// ===== New unit tests for extracted functions =====

void test_winning_move_O() {
    std::cout << "test_winning_move_O\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    // O has two in a row on diagonal: (0,0) and (1,1), it's O's turn
    assert(s.process_move(Sign::X, 0, 1) == MoveResult::OK);
    assert(s.process_move(Sign::O, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::X, 1, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 1) == MoveResult::OK);
    assert(s.process_move(Sign::X, 0, 2) == MoveResult::OK);

    const auto &opts_ref = s.get_opts();
    int L = opts_ref.win_len;
    const auto &lines = get_cached_lines(3, 3, 3);

    std::vector<Point> empty_cells;
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            if (s.get_value(x, y) == Sign::NONE)
                empty_cells.push_back({x, y});

    std::vector<LineInfo> line_infos(lines.size());
    int total_threats = 0;
    analyze_lines(lines, s, Sign::O, Sign::X, L, line_infos, total_threats);

    Point tie_move{-1, -1};
    Point win = find_winning_move(empty_cells, lines, line_infos,
                                  Sign::O, L, total_threats, tie_move);
    // O should win by completing diagonal at (2,2)
    assert(win.x == 2 && win.y == 2);
}

void test_winning_move_X_absolute() {
    std::cout << "test_winning_move_X_absolute\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    // X has (0,0) and (1,1) on a line, O has no threats
    assert(s.process_move(Sign::X, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 0, 1) == MoveResult::OK);
    assert(s.process_move(Sign::X, 1, 1) == MoveResult::OK);
    assert(s.process_move(Sign::O, 2, 0) == MoveResult::OK);

    const auto &opts_ref = s.get_opts();
    int L = opts_ref.win_len;
    const auto &lines = get_cached_lines(3, 3, 3);

    std::vector<Point> empty_cells;
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            if (s.get_value(x, y) == Sign::NONE)
                empty_cells.push_back({x, y});

    std::vector<LineInfo> line_infos(lines.size());
    int total_threats = 0;
    analyze_lines(lines, s, Sign::X, Sign::O, L, line_infos, total_threats);

    Point tie_move{-1, -1};
    Point win = find_winning_move(empty_cells, lines, line_infos,
                                  Sign::X, L, total_threats, tie_move);
    // X should win at (2,2) with no remaining O threats
    assert(win.x == 2 && win.y == 2);
    assert(tie_move.x == -1 && tie_move.y == -1);
}

void test_blocking_move_single() {
    std::cout << "test_blocking_move_single\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    // O threatens at (2,0): O has (0,0), (1,0)
    assert(s.process_move(Sign::X, 0, 1) == MoveResult::OK);
    assert(s.process_move(Sign::O, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::X, 0, 2) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 0) == MoveResult::OK);
    assert(s.process_move(Sign::X, 1, 1) == MoveResult::OK);

    const auto &opts_ref = s.get_opts();
    int L = opts_ref.win_len;
    const auto &lines = get_cached_lines(3, 3, 3);

    std::vector<Point> empty_cells;
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            if (s.get_value(x, y) == Sign::NONE)
                empty_cells.push_back({x, y});

    std::vector<LineInfo> line_infos(lines.size());
    int total_threats = 0;
    analyze_lines(lines, s, Sign::X, Sign::O, L, line_infos, total_threats);

    int max_blocked = -1;
    Point block = find_blocking_move(empty_cells, lines, line_infos,
                                     L, 3, 3, max_blocked);
    // Should block at (2,0), blocking exactly 1 threat
    assert(block.x == 2 && block.y == 0);
    assert(max_blocked == 1);
}

void test_fork_move() {
    std::cout << "test_fork_move\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    // Fork setup: X at (0,0) and (2,2), O at (1,1).
    // X's turn: playing at (0,2) creates threats on row 0 and col 2.
    assert(s.process_move(Sign::X, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 1) == MoveResult::OK);
    assert(s.process_move(Sign::X, 2, 2) == MoveResult::OK);

    const auto &opts_ref = s.get_opts();
    int L = opts_ref.win_len;
    const auto &lines = get_cached_lines(3, 3, 3);

    std::vector<Point> empty_cells;
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            if (s.get_value(x, y) == Sign::NONE)
                empty_cells.push_back({x, y});

    std::vector<LineInfo> line_infos(lines.size());
    int total_threats = 0;
    analyze_lines(lines, s, Sign::X, Sign::O, L, line_infos, total_threats);

    Point fork = find_fork_move(empty_cells, lines, line_infos, L, 3, 3);
    // (0,2) creates a fork (threats on row 0 and col 2)
    assert(fork.x == 0 && fork.y == 2);
}

void test_build_weights_table() {
    std::cout << "test_build_weights_table\n";
    std::vector<int> w;
    build_weights_table(3, w);
    assert(w.size() == 4); // indices 0..3
    assert(w[0] == 0);
    assert(w[1] == 10);
    assert(w[2] == 50'000);
    assert(w[3] == 1'000'000);
}

void test_select_best_cell() {
    std::cout << "test_select_best_cell\n";
    // 3 empty cells on 3x3 board
    std::vector<Point> cells = {{0,0}, {1,1}, {2,2}};
    // atk and def arrays 3x3=9, only indices 0,4,8 matter
    std::vector<int> atk(9, 0), def(9, 0);
    atk[4] = 100;  // center (1,1) has highest attack value
    def[4] = 50;
    atk[8] = 90;   // (2,2) less
    def[8] = 40;

    Point best = select_best_cell(cells, atk, def, 1.0, 3, 3);
    // center should win
    assert(best.x == 1 && best.y == 1);
}

void test_analyze_lines() {
    std::cout << "test_analyze_lines\n";
    State::Opts opts{3, 3, 3, 9};
    State s(opts);

    assert(s.process_move(Sign::X, 0, 0) == MoveResult::OK);
    assert(s.process_move(Sign::O, 1, 1) == MoveResult::OK);

    const auto &lines = get_cached_lines(3, 3, 3);
    std::vector<LineInfo> infos(lines.size());
    int threats = -1;
    analyze_lines(lines, s, Sign::X, Sign::O, 3, infos, threats);

    // threats should be 0 since O doesn't have 2-in-a-row
    assert(threats == 0);

    // Main diagonal (index 6 for 3x3 L=3: 3 hor + 3 ver + 1 SE diag): X at (0,0), O at (1,1), empty at (2,2)
    assert(infos[6].my == 1);
    assert(infos[6].opp == 1);
    assert(infos[6].empty == 1);
    assert(infos[6].has_wall == false);
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

  // Original integration tests
  test_block_opponent_win();
  test_block_opponent();
  test_center_on_empty_board();
  test_last_empty_cell();

  // New unit tests for extracted functions
  test_analyze_lines();
  test_winning_move_O();
  test_winning_move_X_absolute();
  test_blocking_move_single();
  test_fork_move();
  test_build_weights_table();
  test_select_best_cell();

  std::cout << "All my_player tests passed!\n";
  return 0;
}