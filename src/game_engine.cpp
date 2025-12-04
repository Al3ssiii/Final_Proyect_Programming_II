#include "game_engine.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

GameEngine::GameEngine()
    : map(12, 12),
      renderer(),
      turno(1),
      pa(MAX_PA),
      comida(START_COMIDA),
      metal(START_METAL),
      energia(START_ENERGIA),
      quit_flag(false)
{
    generar_mapa_aleatorio(map);
}

void GameEngine::render()
{
    renderer.render(map, turno, pa, comida, metal, energia);
}

bool GameEngine::can_move_unit(int r, int c, int nr, int nc)
{
    if (!((r == nr && std::abs(c - nc) == 1) || (c == nc && std::abs(r - nr) == 1)))
        return false;

    Tile &src = map.at(r, c);
    if (src.getUnit() == UnitType::NONE_U)
        return false;

    if (map.at(nr, nc).getTerrain() == TerrainType::AG)
        return false;

    return true;
}

void GameEngine::player_move(int r, int c, int nr, int nc)
{
    if (pa <= 0)
    {
        std::cout << "Sin PA.\n";
        return;
    }
    if (!map.in_bounds(r, c) || !map.in_bounds(nr, nc))
    {
        std::cout << "Fuera de rango.\n";
        return;
    }

    if (map.at(r, c).getUnitOwner() != Owner::J1)
    {
        std::cout << "Esa unidad no es tuya.\n";
        return;
    }

    if (!can_move_unit(r, c, nr, nc))
    {
        std::cout << "Movimiento invalido.\n";
        return;
    }

    Tile &src = map.at(r, c);
    Tile &dst = map.at(nr, nc);

    if (dst.getUnit() != UnitType::NONE_U)
    {
        resolve_combat(r, c, nr, nc);
    }
    else
    {
        dst.setUnit(src.getUnit());
        dst.setUnitOwner(src.getUnitOwner());
        src.clearUnit();
    }
    pa -= 1;
}

void GameEngine::resolve_combat(int r, int c, int nr, int nc)
{
    Tile &src = map.at(r, c);
    Tile &dst = map.at(nr, nc);

    if (src.getUnitOwner() == dst.getUnitOwner())
    {
        std::cout << "Destino ocupado por aliado.\n";
        return;
    }

    UnitStats atkStats = get_unit_stats(src.getUnit());
    UnitStats defStats = get_unit_stats(dst.getUnit());

    std::cout << "Combate! Tu " << get_unit_name(src.getUnit())
              << " ataca a " << get_unit_name(dst.getUnit()) << "\n";

    int dmgToDef = std::max(0, atkStats.atk - defStats.def);
    int dmgToAtk = std::max(0, defStats.atk - atkStats.def);

    atkStats.hp -= dmgToAtk;
    defStats.hp -= dmgToDef;

    if (defStats.hp <= 0)
    {
        std::cout << "Enemigo eliminado!\n";
        dst.setUnit(src.getUnit());
        dst.setUnitOwner(src.getUnitOwner());
        src.clearUnit();
        if (dst.getBuilding() != BuildingType::NONE_B)
            dst.setBuildingOwner(dst.getUnitOwner());
    }
    else if (atkStats.hp <= 0)
    {
        std::cout << "Tu unidad murio en el ataque.\n";
        src.clearUnit();
    }
    else
    {

        std::cout << "Empate tecnico. Ambos sobreviven.\n";
    }
}

void GameEngine::recruit(int r, int c, UnitType ut)
{
    if (pa <= 0)
    {
        std::cout << "Sin PA.\n";
        return;
    }
    if (!map.in_bounds(r, c))
    {
        std::cout << "Fuera de rango.\n";
        return;
    }
    Tile &t = map.at(r, c);
    if (t.getUnit() != UnitType::NONE_U)
    {
        std::cout << "Casilla ocupada.\n";
        return;
    }

    bool allowed = false;
    if (t.getOwner() == Owner::J1)
        allowed = true;
    if (t.getBuilding() == BuildingType::CU && t.getBuildingOwner() == Owner::J1)
        allowed = true;

    if (!(t.getBuilding() == BuildingType::CU && t.getBuildingOwner() == Owner::J1))
    {
        std::cout << "Solo puedes reclutar en un Cuartel (Cu) aliado.\n";
        return;
    }

    UnitStats stats = get_unit_stats(ut);

    if (comida < stats.cost_comida || metal < stats.cost_metal)
    {
        std::cout << "Recursos insuficientes.\n";
        return;
    }

    comida -= stats.cost_comida;
    metal -= stats.cost_metal;
    t.setUnit(ut);
    t.setUnitOwner(Owner::J1);
    pa -= 1;

    std::cout << "Reclutado " << get_unit_name(ut) << " en (" << r << "," << c << ").\n";
}

void GameEngine::build(int r, int c, BuildingType bt)
{
    if (pa <= 0)
    {
        std::cout << "Sin PA.\n";
        return;
    }
    if (!map.in_bounds(r, c))
    {
        std::cout << "Fuera de rango.\n";
        return;
    }
    Tile &t = map.at(r, c);

    if (t.getUnit() != UnitType::NONE_U)
    {
        std::cout << "Desocupa la casilla antes de construir.\n";
        return;
    }
    if (t.getBuilding() != BuildingType::NONE_B)
    {
        std::cout << "Ya existe edificio.\n";
        return;
    }

    int cost_metal = 1, cost_energy = 1;
    if (bt == BuildingType::TO)
    {
        cost_metal = 2;
        cost_energy = 1;
    }

    if (metal < cost_metal || energia < cost_energy)
    {
        std::cout << "Recursos insuficientes.\n";
        return;
    }

    metal -= cost_metal;
    energia -= cost_energy;
    t.setBuilding(bt);
    t.setBuildingOwner(Owner::J1);
    pa -= 1;

    std::cout << "Construido edificio en (" << r << "," << c << ").\n";
}

void GameEngine::cpu_turn()
{
    for (int r = 0; r < map.num_rows(); ++r)
    {
        for (int c = 0; c < map.num_cols(); ++c)
        {
            Tile &t = map.at(r, c);
            if (t.getBuilding() == BuildingType::CU && t.getBuildingOwner() == Owner::J2)
            {
                if (t.getUnit() == UnitType::NONE_U)
                {
                    t.setUnit(UnitType::S);
                    t.setUnitOwner(Owner::J2);
                }
                else
                {
                    if (map.in_bounds(r - 1, c) && map.at(r - 1, c).getUnit() == UnitType::NONE_U)
                    {
                        map.at(r - 1, c).setUnit(UnitType::S);
                        map.at(r - 1, c).setUnitOwner(Owner::J2);
                    }
                }
            }
        }
    }

    world_phase();
}

void GameEngine::end_turn()
{
    turno++;
    pa = MAX_PA;
    cpu_turn();

    comida += 2;
    metal += 1;
    energia += 1;
}

void GameEngine::world_phase()
{

    for (int r = 0; r < map.num_rows(); ++r)
    {
        for (int c = 0; c < map.num_cols(); ++c)
        {
            Tile &t = map.at(r, c);
            if (t.getUnitOwner() == Owner::J2 && t.getUnit() != UnitType::NONE_U)
            {
                int nr = r;
                int nc = c - 1;

                if (!map.in_bounds(nr, nc))
                {
                    nc = c;
                    nr = r - 1;
                }

                if (map.in_bounds(nr, nc))
                {
                    Tile &target = map.at(nr, nc);

                    if (target.getUnit() == UnitType::NONE_U && target.getTerrain() != TerrainType::AG)
                    {
                        target.setUnit(t.getUnit());
                        target.setUnitOwner(Owner::J2);
                        t.clearUnit();
                    }
                    else if (target.getUnitOwner() == Owner::J1)
                    {
                        resolve_combat(r, c, nr, nc);
                    }
                }
            }
        }
    }
}

void GameEngine::request_quit() { quit_flag = true; }
bool GameEngine::quit_requested() const { return quit_flag; }
