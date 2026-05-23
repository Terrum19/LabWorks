#include "player/my_player.hpp"
#include "core/baseline.hpp"
#include "test_stats.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    std::cout << "Testing MyPlayer vs baseline easy player\n";
    if (argc >= 2) {
        std::srand(atoi(argv[1]));
    }

    
    ttt::my_player::MyPlayer p1("MyPlayer"); ////поместите вашего игрока сюда
    ttt::game::IPlayer *p2 = ttt::baseline::get_easy_player("BaselineEasy"); //здесь вы можете выбрать между базовыми игроками: сложным и лёгким
    ttt::game::IPlayer *p3 = ttt::baseline::get_harder_player("BaselineHard"); //здесь вы можете выбрать между базовыми игроками: сложным и лёгким

    auto resultEasy = ttt::test::run_game_tests(p1, *p2, 100); //здесь вы можете изменить количество тестовых итераций ~~ 100
    auto resultHard = ttt::test::run_game_tests(p1, *p3, 100); //здесь вы можете изменить количество тестовых итераций ~~ 100


    ttt::test::print_test_results(resultEasy, "MyPlayer", "BaselineEasy");
    ttt::test::print_test_results(resultHard, "MyPlayer", "BaselineHard");

    delete p2;
    return 0;
}