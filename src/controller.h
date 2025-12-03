#ifndef FINAL_PROYECT_PROGRAMMING_II_CONTROLLER_H
#define FINAL_PROYECT_PROGRAMMING_II_CONTROLLER_H

#include "map.h"
#include "game_engine.h"
#include <string>

class Controller
{
public:
    Controller(GameEngine &engine);
    void loop(); // main input loop

private:
    GameEngine &eng;
    void process_line(const std::string &line);
};


struct Unit
{
    UnitType type;
    Owner owner;
    int hp;
    int atk;
    int def;
    int move; // movement points (abstract)
    Unit(UnitType t = UnitType::NONE_U, Owner o = Owner::NONE);
    std::string to_string() const;
};

#endif //FINAL_PROYECT_PROGRAMMING_II_CONTROLLER_H
