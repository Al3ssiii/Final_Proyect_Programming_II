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
    map.at(0, 0).setUnit(create_unit(UnitType::S, Owner::J1));
}

void GameEngine::render() {
    renderer.render(map, turno, pa, comida, metal, energia);
}

void GameEngine::try_capture_tile(int r, int c, Owner new_owner) {
    Tile &t = map.at(r, c);

    t.setOwner(new_owner);

    if (t.getBuilding() != BuildingType::NONE_B && t.getBuildingOwner() != new_owner) {
        t.setBuildingOwner(new_owner);
        std::cout << " * ¡IMPORTANTE! El jugador " << (new_owner == Owner::J1 ? "J1" : "J2")
                  << " ha capturado el " << (t.getBuilding() == BuildingType::CU ? "CUARTEL" : "EDIFICIO")
                  << " en (" << r << ", " << c << ")!\n";
    }
}

void GameEngine::check_win_conditions() {
    const Tile& j2_base = map.at(11, 11);
    if (j2_base.getBuilding() == BuildingType::CU && j2_base.getBuildingOwner() == Owner::J1) {
        std::cout << "\n======================================================\n";
        std::cout << "               ¡VICTORIA! J1 HA CAPTURADO EL CUARTEL DE J2.\n";
        std::cout << "======================================================\n";
        request_quit();
        return;
    }

    const Tile& j1_base = map.at(0, 0);
    if (j1_base.getBuilding() == BuildingType::CU && j1_base.getBuildingOwner() == Owner::J2) {
        std::cout << "\n======================================================\n";
        std::cout << "               ¡DERROTA! J2 HA CAPTURADO TU CUARTEL (0,0).\n";
        std::cout << "======================================================\n";
        request_quit();
        return;
    }
}

bool GameEngine::can_move_unit(int r, int c, int nr, int nc) {
    if (!((r == nr && std::abs(c - nc) == 1) || (c == nc && std::abs(r - nr) == 1)))
        return false;

    Tile &src = map.at(r, c);
    Unit *unit_to_move = src.getUnit();
    if (unit_to_move == nullptr) return false;

    if (map.at(nr, nc).getTerrain() == TerrainType::AG) return false;

    int move_cost = unit_to_move->get_movement_cost(map.at(nr, nc).getTerrain());
    if (move_cost > pa) {
        std::cout << "El movimiento cuesta " << move_cost << " PA. Solo tienes " << pa << " PA.\n";
        return false;
    }

    return true;
}

void GameEngine::player_move(int r, int c, int nr, int nc) {
    if (pa <= 0) { std::cout << "Sin PA.\n"; return; }
    if (!map.in_bounds(r, c) || !map.in_bounds(nr, nc)) { std::cout << "Coordenadas fuera de limites.\n"; return; }

    Tile &src = map.at(r, c);
    Tile &dest = map.at(nr, nc);
    Unit *unit_to_move = src.getUnit();

    if (unit_to_move == nullptr || unit_to_move->get_owner() != Owner::J1) {
        std::cout << "No hay unidad tuya en el origen.\n";
        return;
    }
    if (!can_move_unit(r, c, nr, nc)) { return; }

    int move_cost = unit_to_move->get_movement_cost(dest.getTerrain());

    if (dest.getUnit() == nullptr) {

        // FIX: Uso releaseUnit para transferir el puntero
        Unit* moved_unit = src.releaseUnit();
        dest.setUnit(moved_unit);

        pa -= move_cost;
        std::cout << unit_to_move->get_name() << " movido a (" << nr << ", " << nc << "). PA restante: " << pa << "\n";
        try_capture_tile(nr, nc, Owner::J1);
        check_win_conditions();
    }
    else if (dest.getUnit()->get_owner() != Owner::J1) {
        pa -= 1;
        std::cout << unit_to_move->get_name() << " ataca a " << dest.getUnit()->get_name() << " en (" << nr << ", " << nc << ").\n";
        resolve_combat(r, c, nr, nc);
    }
    else {
        std::cout << "No puedes moverte a una casilla con una unidad amiga.\n";
    }
}

void GameEngine::recruit(int r, int c, UnitType ut) {
    if (pa <= 0) { std::cout << "Sin PA.\n"; return; }
    if (!map.in_bounds(r, c)) { std::cout << "Coordenadas fuera de limites.\n"; return; }

    Tile &t = map.at(r, c);

    if (t.getBuilding() != BuildingType::CU || t.getBuildingOwner() != Owner::J1) {
        std::cout << "Solo puedes reclutar en tu Cuartel.\n";
        return;
    }
    if (t.getUnit() != nullptr) {
        std::cout << "Casilla ocupada por otra unidad.\n";
        return;
    }

    Unit *new_unit = create_unit(ut, Owner::J1);
    if (new_unit == nullptr) {
        std::cout << "Tipo de unidad no válido.\n";
        return;
    }

    if (comida < new_unit->get_cost_comida() || metal < new_unit->get_cost_metal()) {
        std::cout << "Recursos insuficientes. Necesitas Comida:" << new_unit->get_cost_comida()
                  << " Metal:" << new_unit->get_cost_metal() << ".\n";
        delete new_unit;
        return;
    }

    comida -= new_unit->get_cost_comida();
    metal -= new_unit->get_cost_metal();

    t.setUnit(new_unit);
    pa -= 1;
    std::cout << new_unit->get_name() << " reclutado en (" << r << ", " << c << "). PA restante: " << pa << "\n";
}

void GameEngine::build(int r, int c, BuildingType bt) {
    if (pa <= 0) { std::cout << "Sin PA.\n"; return; }
    if (!map.in_bounds(r, c)) { std::cout << "Coordenadas fuera de limites.\n"; return; }

    Tile &t = map.at(r, c);

    if (t.getBuilding() != BuildingType::NONE_B) {
        std::cout << "Ya hay un edificio aquí.\n";
        return;
    }
    if (t.getOwner() != Owner::J1) {
        std::cout << "Debes controlar el territorio para construir.\n";
        return;
    }

    int cost_metal = 0, cost_energia = 0;

    switch (bt) {
        case BuildingType::GR:
            cost_metal = 1; cost_energia = 1; break;
        case BuildingType::TO:
            cost_metal = 2; cost_energia = 1; break;
        default:
            std::cout << "Tipo de edificio inválido.\n";
            return;
    }

    if (metal < cost_metal || energia < cost_energia) {
        std::cout << "Recursos insuficientes. Necesitas Metal:" << cost_metal
                  << " Energia:" << cost_energia << ".\n";
        return;
    }

    metal -= cost_metal;
    energia -= cost_energia;

    t.setBuilding(bt);
    t.setBuildingOwner(Owner::J1);
    pa -= 1;
    std::cout << "Edificio construido en (" << r << ", " << c << "). PA restante: " << pa << "\n";
}

void GameEngine::resolve_combat(int r, int c, int nr, int nc) {
    Tile &src = map.at(r, c);
    Tile &dest = map.at(nr, nc);

    Unit *attacker = src.getUnit();
    Unit *defender = dest.getUnit();

    if (!attacker || !defender) return;

    int def_bonus = defender->get_defense_bonus(dest.getTerrain());
    int effective_def = defender->get_def() + def_bonus;

    int damage = std::max(1, attacker->get_atk() - effective_def);

    defender->take_damage(damage);

    std::cout << " -> " << attacker->get_name() << " (HP:" << attacker->get_hp() << ") vs "
              << defender->get_name() << " (HP:" << defender->get_hp() << "). Daño: " << damage << ".\n";

    if (defender->get_hp() <= 0) {
        std::cout << "¡" << defender->get_name() << " derrotado!\n";

        // FIX: Uso releaseUnit para transferir el atacante sin borrarlo
        Unit* moved_unit = src.releaseUnit();
        dest.setUnit(moved_unit);

        try_capture_tile(nr, nc, Owner::J1);
        check_win_conditions();

    } else {
        std::cout << defender->get_name() << " resiste (HP restante: " << defender->get_hp() << ").\n";
    }
}

void GameEngine::cpu_turn() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)) + turno);
    std::cout << "\n--- Turno de la IA (J2) ---\n";

    int rows = map.num_rows();
    int cols = map.num_cols();
    std::vector<std::pair<int, int>> j2_units;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (map.at(r, c).getUnitOwner() == Owner::J2) {
                j2_units.push_back({r, c});
            }
        }
    }

    for (auto pos : j2_units) {
        if (quit_requested()) return;

        int r = pos.first;
        int c = pos.second;
        Tile &t = map.at(r, c);
        Unit *unit = t.getUnit();

        if (unit == nullptr) continue;

        int dr[] = {0, 0, 1, -1};
        int dc[] = {1, -1, 0, 0};

        bool moved = false;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (map.in_bounds(nr, nc)) {
                Tile &target = map.at(nr, nc);

                if (target.getUnit() == nullptr && target.getTerrain() != TerrainType::AG) {

                    // FIX: Uso releaseUnit para transferir la unidad sin borrarla
                    Unit* moved_unit = t.releaseUnit();
                    target.setUnit(moved_unit);

                    try_capture_tile(nr, nc, Owner::J2);
                    check_win_conditions();
                    if (quit_requested()) return;

                    moved = true;
                    break;
                }
                else if (target.getUnit() != nullptr && target.getUnit()->get_owner() == Owner::J1) {

                    Unit *attacker = unit;
                    Unit *defender = target.getUnit();

                    int def_bonus = defender->get_defense_bonus(target.getTerrain());
                    int effective_def = defender->get_def() + def_bonus;
                    int damage = std::max(1, attacker->get_atk() - effective_def);
                    defender->take_damage(damage);

                    std::cout << " -> J2 ataca. " << attacker->get_name() << " (HP:" << attacker->get_hp() << ") vs "
                            << defender->get_name() << " (HP:" << defender->get_hp() << "). Daño: " << damage << ".\n";

                    if (defender->get_hp() <= 0) {
                        std::cout << "¡" << defender->get_name() << " derrotado por J2!\n";

                        // FIX: Uso releaseUnit para transferir el atacante sin borrarlo
                        Unit* moved_unit = t.releaseUnit();
                        target.setUnit(moved_unit);

                        try_capture_tile(nr, nc, Owner::J2);
                        check_win_conditions();
                        if (quit_requested()) return;

                    } else {
                        std::cout << defender->get_name() << " resiste (HP restante: " << defender->get_hp() << ").\n";
                    }

                    moved = true;
                    break;
                }
            }
        }
    }
    std::cout << "--- Fin Turno de la IA (J2) ---\n";
    world_phase();
}

void GameEngine::end_turn() {
    turno++;
    pa = MAX_PA;

    int maintenance_comida = 0;
    int granja_bonus_comida = 0;
    int granja_bonus_metal = 0;

    for (int r = 0; r < map.num_rows(); ++r) {
        for (int c = 0; c < map.num_cols(); ++c) {
            const Tile &t = map.at(r, c);

            if (t.getUnit() != nullptr && t.getUnit()->get_owner() == Owner::J1) {
                maintenance_comida += t.getUnit()->get_cost_comida();
            }

            if (t.getBuilding() == BuildingType::GR && t.getBuildingOwner() == Owner::J1) {
                granja_bonus_comida += 2;
                granja_bonus_metal += 1;
            }
        }
    }

    comida -= maintenance_comida;

    comida += 2 + granja_bonus_comida;
    metal += 1 + granja_bonus_metal;
    energia += 1;

    if (comida < 0) {
        std::cout << "¡DERROTA! Te quedaste sin comida. Mantenimiento total: " << maintenance_comida << "\n";
        request_quit();
        return;
    }

    check_win_conditions();
    if (quit_requested()) return;

    cpu_turn();
}

void GameEngine::world_phase() {

}

void GameEngine::request_quit() {
    quit_flag = true;
}

bool GameEngine::quit_requested() const {
    return quit_flag;
}