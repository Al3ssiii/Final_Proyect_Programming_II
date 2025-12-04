#include "controller.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>

std::string leer_linea()
{
    std::string s;
    std::getline(std::cin, s);
    return s;
}

std::pair<int,int> seleccionar_unidad(const Map& map)
{
    std::vector<std::pair<int,int>> unidades;

    std::cout << "\n--- Tus Unidades ---\n";

    for (int r = 0; r < map.num_rows(); r++)
    {
        for (int c = 0; c < map.num_cols(); c++)
        {
            const Tile &t = map.at(r, c);
            Unit *u = t.getUnit();

            if (u != nullptr && u->get_owner() == Owner::J1)
            {
                unidades.push_back({r, c});

                std::cout << " " << unidades.size() << ". "
                          << u->get_name() << " [" << (u->get_owner() == Owner::J1 ? "J1" : "J2") << "]"
                          << " HP:" << u->get_hp()
                          << " ATK:" << u->get_atk()
                          << " DEF:" << u->get_def()
                          << " (r:" << r << ", c:" << c << ")\n";
            }
        }
    }

    if (unidades.empty()) {
        std::cout << "No tienes unidades en el mapa.\n";
        return {-1, -1};
    }

    std::cout << "Selecciona una unidad (1 - " << unidades.size() << "): ";
    std::string s = leer_linea();

    int idx = 0;
    try {
        idx = std::stoi(s);
    } catch (...) {
        std::cout << "Selección inválida.\n";
        return {-1, -1};
    }

    if (idx < 1 || idx > unidades.size()) {
        std::cout << "Número de unidad fuera de rango.\n";
        return {-1, -1};
    }

    return unidades[idx-1];
}

Controller::Controller(GameEngine &engine) : eng(engine) {}

void Controller::loop()
{
    while (!eng.quit_requested())
    {
        eng.render();
        std::cout << "Acción (M/R/B/E/Q): ";
        std::string line = leer_linea();
        process_line(line);
    }
}

void Controller::process_line(const std::string &line)
{
    std::string cmd = line;
    if (line.empty()) return;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "q")
    {
        eng.request_quit();
        return;
    }

    if (cmd == "e")
    {
        eng.end_turn();
        return;
    }

    if (cmd == "m")
    {
        std::pair<int, int> pos = seleccionar_unidad(eng.get_map());
        if (pos.first == -1) return;

        int r = pos.first;
        int c = pos.second;

        std::cout << "Mover a (fila columna): ";
        std::string s = leer_linea();

        int nr = 0, nc = 0;
        std::istringstream iss(s);
        if (!(iss >> nr >> nc)) {
            std::cout << "Coordenadas invalidas.\n";
            return;
        }

        eng.player_move(r, c, nr, nc);
        return;
    }

    if (cmd == "r")
    {
        std::cout << "Donde reclutar (fila columna): ";
        std::string s = leer_linea();

        int r = 0, c = 0;
        std::istringstream iss(s);
        if (!(iss >> r >> c)) {
            std::cout << "Coordenadas invalidas.\n";
            return;
        }

        std::map<std::string, UnitType> unit_map = {
            {"s", UnitType::S}, {"a", UnitType::A}, {"c", UnitType::C},
            {"m", UnitType::M}, {"i", UnitType::I}
        };

        std::cout << "--- Tipos de Unidad (Costo Comida/Metal) ---\n";

        for (const auto& pair : unit_map) {
            UnitType ut = pair.second;
            Unit *temp_unit = create_unit(ut, Owner::J1);
            if (temp_unit) {
                std::cout << " " << pair.first << " - " << temp_unit->get_name()
                          << " (Com:" << temp_unit->get_cost_comida()
                          << ", Met:" << temp_unit->get_cost_metal() << ")\n";
                delete temp_unit;
            }
        }

        std::cout << "Tipo (S/A/C/M/I): ";
        std::string tipo = leer_linea();
        std::transform(tipo.begin(), tipo.end(), tipo.begin(), ::tolower);

        UnitType ut = UnitType::NONE_U;
        if (unit_map.count(tipo)) {
            ut = unit_map[tipo];
        } else {
            std::cout << "Tipo desconocido.\n";
            return;
        }

        eng.recruit(r, c, ut);
        return;
    }

    if (cmd == "b")
    {
        std::cout << "Donde construir (fila columna): \n";
        std::string s = leer_linea();

        int r = 0, c = 0;
        std::istringstream iss2(s);
        if (!(iss2 >> r >> c)) {
            std::cout << "Coordenadas invalidas.\n";
            return;
        }

        std::cout << "--- Edificios (Costo Metal/Energia) ---\n";
        std::cout << " Gr - Granja  (Metal:1, Eng:1)\n";
        std::cout << " To - Torre   (Metal:2, Eng:1)\n";

        std::cout << "Tipo (Gr / To): ";
        std::string tipo = leer_linea();
        std::transform(tipo.begin(), tipo.end(), tipo.begin(), ::tolower);

        BuildingType bt = BuildingType::NONE_B;
        if (tipo == "gr") bt = BuildingType::GR;
        else if (tipo == "to") bt = BuildingType::TO;
        else {
            std::cout << "Tipo inválido.\n";
            return;
        }

        eng.build(r, c, bt);
        return;
    }

    std::cout << "Comando desconocido.\n";
}