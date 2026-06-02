#pragma once

#include "../core/game.hpp"
#include <vector>

namespace ttt::my_player {

using game::Event;
using game::IPlayer;
using game::Point;
using game::Sign;
using game::State;

struct Line {
    std::vector<Point> cells;
};

// Ленивое кэширование линий (используется также в тестах)
const std::vector<Line>& get_cached_lines(int rows, int cols, int L);

// Структура для хранения информации об одной линии
struct LineInfo {
    int my = 0, opp = 0, empty = 0;
    bool has_wall = false;
};

// Заполняет line_infos и подсчитывает total_opp_winning_threats
void analyze_lines(
    const std::vector<Line>& lines,
    const State& state,
    Sign my_sign,
    Sign opp_sign,
    int L,
    std::vector<LineInfo>& line_infos,
    int& total_opp_winning_threats);

// Поиск победного хода с учётом асимметрии правил
// Возвращает точку для немедленной победы, либо {-1,-1} если такой нет.
// Заполняет out_best_tie_win_move — ход, дающий линию, но ведущий к ничьей (для X).
Point find_winning_move(
    const std::vector<Point>& empty_cells,
    const std::vector<Line>& lines,
    const std::vector<LineInfo>& line_infos,
    Sign my_sign,
    int L,
    int total_opp_winning_threats,
    Point& out_best_tie_win_move);

// Поиск лучшего блокирующего хода против угроз противника
Point find_blocking_move(
    const std::vector<Point>& empty_cells,
    const std::vector<Line>& lines,
    const std::vector<LineInfo>& line_infos,
    int L,
    int rows, int cols,
    int& out_max_blocked);

// Поиск и построение собственных вилок (две угрозы за один ход)
Point find_fork_move(
    const std::vector<Point>& empty_cells,
    const std::vector<Line>& lines,
    const std::vector<LineInfo>& line_infos,
    int L,
    int rows, int cols);

// Построение таблицы весов для позиционной оценки
void build_weights_table(int L, std::vector<int>& weights);

// Накопление весов атаки и защиты для позиционной оценки
void accumulate_positional_scores(
    const std::vector<Line>& lines,
    const std::vector<LineInfo>& line_infos,
    const State& state,
    const std::vector<int>& weights,
    int rows, int cols,
    int L,
    std::vector<int>& atk,
    std::vector<int>& def);

// Выбор лучшей клетки по сумме взвешенных атакующих/защитных весов
Point select_best_cell(
    const std::vector<Point>& empty_cells,
    const std::vector<int>& atk,
    const std::vector<int>& def,
    double def_coeff,
    int rows, int cols);

class MyPlayer : public IPlayer {
  Sign m_sign = Sign::NONE;
  const char *m_name;

public:
  MyPlayer(const char *name) : m_sign(Sign::NONE), m_name(name) {}
  void set_sign(Sign sign) override;
  Point make_move(const State &game) override;
  const char *get_name() const override;
};

}; // namespace ttt::my_player