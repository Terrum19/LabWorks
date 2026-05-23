#include "my_player.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

namespace ttt::my_player {

struct Line {
    std::vector<Point> cells;
};

// Генерация всех возможных линий длины L на поле rows*cols
static std::vector<Line> generate_lines(int rows, int cols, int L) {
    std::vector<Line> lines;
    lines.reserve(rows * cols * 4);

    // Горизонтали
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c <= cols - L; ++c) {
            Line ln; ln.cells.reserve(L);
            for (int i = 0; i < L; ++i) ln.cells.push_back({c + i, r});
            lines.push_back(std::move(ln));
        }
    // Вертикали
    for (int c = 0; c < cols; ++c)
        for (int r = 0; r <= rows - L; ++r) {
            Line ln; ln.cells.reserve(L);
            for (int i = 0; i < L; ++i) ln.cells.push_back({c, r + i});
            lines.push_back(std::move(ln));
        }
    // Диагональ на юго-восток
    for (int r = 0; r <= rows - L; ++r)
        for (int c = 0; c <= cols - L; ++c) {
            Line ln; ln.cells.reserve(L);
            for (int i = 0; i < L; ++i) ln.cells.push_back({c + i, r + i});
            lines.push_back(std::move(ln));
        }
    // Диагональ на юго-запад
    for (int r = 0; r <= rows - L; ++r)
        for (int c = L - 1; c < cols; ++c) {
            Line ln; ln.cells.reserve(L);
            for (int i = 0; i < L; ++i) ln.cells.push_back({c - i, r + i});
            lines.push_back(std::move(ln));
        }
    return lines;
}

// Ленивое кэширование линий для предотвращения TLE на больших полях
static const std::vector<Line>& get_cached_lines(int rows, int cols, int L) {
    static int cached_rows = 0, cached_cols = 0, cached_L = 0;
    static std::vector<Line> cached_lines;
    
    if (rows != cached_rows || cols != cached_cols || L != cached_L) {
        cached_lines = generate_lines(rows, cols, L);
        cached_rows = rows; cached_cols = cols; cached_L = L;
    }
    return cached_lines;
}

inline Sign get_opponent(Sign s) {
    return s == Sign::X ? Sign::O : Sign::X;
}

void MyPlayer::set_sign(Sign sign) { m_sign = sign; }
const char* MyPlayer::get_name() const { return m_name; }

Point MyPlayer::make_move(const State &state) {
    const auto &opts = state.get_opts();
    const int rows = opts.rows;
    const int cols = opts.cols;
    const int L = opts.win_len;
    
    const Sign my_sign = state.get_current_player();
    const Sign opp_sign = get_opponent(my_sign);

    const auto &lines = get_cached_lines(rows, cols, L);
    
    std::vector<Point> empty_cells;
    empty_cells.reserve(rows * cols);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (state.get_value(x, y) == Sign::NONE) {
                empty_cells.push_back({x, y});
            }
        }
    }

    if (empty_cells.empty()) return {0, 0}; 

    // Легковесная структура анализа линий без динамических векторов внутри
    struct LineInfo {
        int my = 0, opp = 0, empty = 0;
        bool has_wall = false;
    };
    std::vector<LineInfo> line_infos(lines.size());

    int total_opp_winning_threats = 0;

    // Первичный легковесный анализ игрового поля
    for (size_t li = 0; li < lines.size(); ++li) {
        auto &info = line_infos[li];
        for (const auto &c : lines[li].cells) {
            Sign v = state.get_value(c.x, c.y);
            if (v == Sign::WALL) { info.has_wall = true; break; }
            if (v == my_sign) info.my++;
            else if (v == opp_sign) info.opp++;
            else info.empty++;
        }
        // Подсчёт критических угроз противника длины L-1
        if (!info.has_wall && info.my == 0 && info.opp == L - 1) {
            total_opp_winning_threats++;
        }
    }

    // Таблица весов для позиционной эвристики
    std::vector<int> weights(L + 1, 0);
    weights[L] = 1'000'000;          
    if (L > 1) weights[L - 1] = 50'000; 
    for (int i = 1; i < L - 1; ++i) {
        weights[i] = static_cast<int>(std::pow(10, i));
    }

    constexpr double DEF_COEFF = 0.85;

    // Поиск победного хода с учётом асимметрии правил
    Point best_tie_win_move = {-1, -1};

    for (const auto &p : empty_cells) {
        bool completes_my_line = false;
        int opp_threats_blocked_by_p = 0;

        for (size_t li = 0; li < lines.size(); ++li) {
            const auto &info = line_infos[li];
            if (info.has_wall) continue;

            // Проверяем принадлежит ли клетка текущей линии
            bool contains_p = false;
            for (const auto &c : lines[li].cells) {
                if (c.x == p.x && c.y == p.y) { contains_p = true; break; }
            }
            if (!contains_p) continue;

            if (info.opp == 0 && info.my == L - 1) {
                completes_my_line = true;
            }
            if (info.my == 0 && info.opp == L - 1) {
                opp_threats_blocked_by_p++;
            }
        }

        if (completes_my_line) {
            if (my_sign == Sign::O) {
                // Нолики побеждают немедленно и безусловно
                return p;
            } else {
                // Стратегия для крестиков проверяем, не приведёт ли ход к ничьей
                int remaining_opp_threats = total_opp_winning_threats - opp_threats_blocked_by_p;
                if (remaining_opp_threats == 0) {
                    // Чистая абсолютная победа для X т.к. у O нет шанса ответить
                    return p;
                } else {
                    // Этот ход даёт линию но O гарантированно закроет свою на последнем ходу что есть ничья
                    if (best_tie_win_move.x == -1) best_tie_win_move = p;
                }
            }
        }
    }

    
    // Блокирование немедленного проигрыша
    if (total_opp_winning_threats > 0) {
        Point best_blocking_move = {-1, -1};
        int max_blocked = -1;
        int min_dist = INT_MAX;

        for (const auto &p : empty_cells) {
            int blocked_here = 0;
            for (size_t li = 0; li < lines.size(); ++li) {
                if (line_infos[li].has_wall || line_infos[li].my > 0 || line_infos[li].opp != L - 1) continue;
                
                for (const auto &c : lines[li].cells) {
                    if (c.x == p.x && c.y == p.y) { blocked_here++; break; }
                }
            }

            if (blocked_here > max_blocked) {
                max_blocked = blocked_here;
                best_blocking_move = p;
                min_dist = std::max(std::abs(p.x - cols / 2), std::abs(p.y - rows / 2));
            } else if (blocked_here == max_blocked && max_blocked > 0) {
                // Расстояние Чебышёва для защиты центра при равном блоке
                int dist = std::max(std::abs(p.x - cols / 2), std::abs(p.y - rows / 2));
                if (dist < min_dist) {
                    best_blocking_move = p;
                    min_dist = dist;
                }
            }
        }

        // Стратегический выбор: если мы можем заблокировать ВСЕ угрозы противника, 
        // лучше продолжить игру ради чистой победы, чем соглашаться на ничью через best_tie_win_move.
        if (max_blocked == total_opp_winning_threats && best_blocking_move.x != -1) {
            return best_blocking_move;
        }
        // Если у противника двойная вилка на победу т.е нельзя заблокировать всё разом
        // но у нас есть ход, сводящий игру в гарантированную ничью - берём ничью.
        if (best_tie_win_move.x != -1) {
            return best_tie_win_move;
        }
        // В худшем случае блокируем максимум возможного
        if (best_blocking_move.x != -1) {
            return best_blocking_move;
        }
    }

    // Поиск и построение собственных вилок 
    if (total_opp_winning_threats == 0) {
        Point best_fork_move = {-1, -1};
        int min_fork_dist = INT_MAX;

        for (const auto &p : empty_cells) {
            int created_threats = 0;
            for (size_t li = 0; li < lines.size(); ++li) {
                const auto &info = line_infos[li];
                if (info.has_wall || info.opp > 0 || info.my != L - 2) continue;

                for (const auto &c : lines[li].cells) {
                    if (c.x == p.x && c.y == p.y) { created_threats++; break; }
                }
            }

            if (created_threats >= 2) {
                int dist = std::max(std::abs(p.x - cols / 2), std::abs(p.y - rows / 2));
                if (best_fork_move.x == -1 || dist < min_fork_dist) {
                    best_fork_move = p;
                    min_fork_dist = dist;
                }
            }
        }
        if (best_fork_move.x != -1) return best_fork_move;
    }

    // Позиционная оценка 
    std::vector<int> atk(rows * cols, 0);
    std::vector<int> def(rows * cols, 0);

    for (size_t li = 0; li < lines.size(); ++li) {
        const auto &info = line_infos[li];
        if (info.has_wall) continue;

        // Накопление весов атаки
        if (info.opp == 0 && info.my < L) {
            int w = weights[info.my + 1];
            for (const auto &c : lines[li].cells) {
                if (state.get_value(c.x, c.y) == Sign::NONE) {
                    atk[c.y * cols + c.x] += w;
                }
            }
        }
        // Накопление весов защиты
        if (info.my == 0 && info.opp > 0 && info.opp < L) {
            int w = weights[info.opp + 1];
            for (const auto &c : lines[li].cells) {
                if (state.get_value(c.x, c.y) == Sign::NONE) {
                    def[c.y * cols + c.x] += w;
                }
            }
        }
    }

    // Выбор лучшей клетки по сумме весов
    Point best_move = empty_cells[0];
    long long best_score = LLONG_MIN;

    for (const auto &p : empty_cells) {
        int idx = p.y * cols + p.x;
        long long score = static_cast<long long>(atk[idx]) + 
                          static_cast<long long>(DEF_COEFF * def[idx]);

        if (score > best_score) {
            best_score = score;
            best_move = p;
        } else if (score == best_score) {
            // Метрика Чебышёва для оценки близости к геометрическому центру поля
            int dist_new = std::max(std::abs(p.x - cols / 2), std::abs(p.y - rows / 2));
            int dist_best = std::max(std::abs(best_move.x - cols / 2), std::abs(best_move.y - rows / 2));
            if (dist_new < dist_best) {
                best_move = p;
            }
        }
    }

    return best_move;
}

} // namespace ttt::my_player