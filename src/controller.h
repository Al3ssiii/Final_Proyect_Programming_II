#ifndef FINAL_PROYECT_PROGRAMMING_II_CONTROLLER_H
#define FINAL_PROYECT_PROGRAMMING_II_CONTROLLER_H

#include "map.h"
#include "game_engine.h"
#include <string>

class Controller
{
public:
    Controller(GameEngine &engine);
    void loop();

private:
    GameEngine &eng;
    void process_line(const std::string &line);
};

// Helper struct for display only
struct UnitInfo
{
    UnitType type;
    Owner owner;
    UnitStats stats;

    UnitInfo(UnitType t, Owner o);
    std::string to_string() const;
};

#endif