#ifndef FINAL_PROYECT_PROGRAMMING_II_GAME_ENGINE_H
#define FINAL_PROYECT_PROGRAMMING_II_GAME_ENGINE_H

#include "map.h"

static const int START_COMIDA = 18;
static const int START_METAL = 12;
static const int START_ENERGIA = 7;
static const int MAX_PA = 2;

class GameEngine
{
private:
    Map map;
    ConsoleRenderer renderer;

    int turno;
    int pa;
    int comida, metal, energia;
    bool quit_flag;

    void world_phase();
    bool can_move_unit(int r, int c, int nr, int nc);
    void resolve_combat(int r, int c, int nr, int nc);
    void cpu_turn();

public:
    GameEngine();

    void render();

    // Jugador
    void player_move(int r, int c, int nr, int nc);
    void recruit(int r, int c, UnitType ut);
    void build(int r, int c, BuildingType bt);
    void end_turn();

    // Control del juego
    void request_quit();
    bool quit_requested() const;
    const Map &get_map() const
    {
        return map;
    }
    Map &get_map()
    {
        return map;
    }
};

#endif
