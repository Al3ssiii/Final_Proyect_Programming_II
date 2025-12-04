#include "controller.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm> // for tolower

// -------------------------------------------------------------
// Helper: Safe Input Reading
// -------------------------------------------------------------
std::string leer_linea()
{
    std::string s;
    std::getline(std::cin, s);
    return s;
}

// -------------------------------------------------------------
// UnitInfo Implementation (Display Logic)
// -------------------------------------------------------------
UnitInfo::UnitInfo(UnitType t, Owner o) : type(t), owner(o) {
    stats = get_unit_stats(t);
}

std::string UnitInfo::to_string() const {
    std::ostringstream oss;
    oss << get_unit_name(type)
        << " [" << (owner == Owner::J1 ? "J1" : "J2") << "]"
        << " HP:" << stats.hp
        << " ATK:" << stats.atk
        << " DEF:" << stats.def
        << " MOV:" << stats.move;
    return oss.str();
}

// -------------------------------------------------------------
// Helper: Select Unit from List
// -------------------------------------------------------------
std::pair<int,int> seleccionar_unidad(const Map& map)
{
    std::vector<std::pair<int,int>> unidades;

    std::cout << "\n--- Tus Unidades ---\n";

    for (int r = 0; r < map.num_rows(); r++)
    {
        for (int c = 0; c < map.num_cols(); c++)
        {
            const Tile& t = map.at(r, c);

            if (t.getUnit() != UnitType::NONE_U &&
                t.getUnitOwner() == Owner::J1)
            {
                unidades.push_back({r, c});
                UnitInfo u(t.getUnit(), t.getUnitOwner());

                std::cout << " " << unidades.size() << ". " << u.to_string()
                          << " en (" << r << ", " << c << ")\n";
            }
        }
    }

    if (unidades.empty())
    {
        std::cout << "No tienes unidades.\n";
        return {-1, -1};
    }

    std::cout << "Elige #: ";
    std::string s = leer_linea();
    int elec = 0;
    try {
        elec = std::stoi(s);
    } catch (...) {
        std::cout << "Entrada invalida.\n";
        return {-1, -1};
    }

    if (elec < 1 || elec > (int)unidades.size()) {
        std::cout << "Opcion fuera de rango.\n";
        return {-1, -1};
    }

    return unidades[elec - 1];
}

// -------------------------------------------------------------
// Controller Logic
// -------------------------------------------------------------
Controller::Controller(GameEngine &engine) : eng(engine) {}

void Controller::loop()
{
    while (!eng.quit_requested())
    {
        eng.render();
        std::cout << "Comando: ";

        std::string line = leer_linea();

        if (line.empty())
            continue;

        process_line(line);

        // Pause to let user read messages if not quitting
        if (!eng.quit_requested()) {
            std::cout << "Presiona Enter para continuar...";
            leer_linea();
        }
    }
}

void Controller::process_line(const std::string &line)
{
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    // Normalize command to lowercase
    for (auto &ch : cmd) ch = tolower(ch);

    // Aliases
    if (cmd == "move" || cmd == "mv") cmd = "m";
    if (cmd == "re" || cmd == "recruit") cmd = "r";
    if (cmd == "bu" || cmd == "build") cmd = "b";
    if (cmd == "end" || cmd == "endturn") cmd = "e";
    if (cmd == "quit" || cmd == "exit") cmd = "q";

    // --- MOVE ---
    if (cmd == "m")
    {
        auto [r, c] = seleccionar_unidad(eng.get_map());
        if (r == -1) return;

        std::cout << "Destino (fila columna): ";
        std::string s = leer_linea();

        int nr = 0, nc = 0;
        std::istringstream iss2(s);
        if (!(iss2 >> nr >> nc)) {
            std::cout << "Coordenadas invalidas.\n";
            return;
        }

        eng.player_move(r, c, nr, nc);
        return;
    }

    // --- RECRUIT ---
    if (cmd == "r")
    {
        std::cout << "Casilla del Cuartel (fila columna): ";
        std::string s = leer_linea();
        int r = 0, c = 0;
        std::istringstream iss2(s);
        if (!(iss2 >> r >> c)) {
            std::cout << "Coordenadas invalidas.\n";
            return;
        }

        // Get stats dynamically to show current costs
        UnitStats s_stats = get_unit_stats(UnitType::S);
        UnitStats a_stats = get_unit_stats(UnitType::A);
        UnitStats c_stats = get_unit_stats(UnitType::C);
        UnitStats m_stats = get_unit_stats(UnitType::M);
        UnitStats i_stats = get_unit_stats(UnitType::I);

        std::cout << "--- Reclutamiento ---\n";
        std::cout << " S - Soldado   (Com:" << s_stats.cost_comida << ", Met:" << s_stats.cost_metal << ")\n";
        std::cout << " A - Arquero   (Com:" << a_stats.cost_comida << ", Met:" << a_stats.cost_metal << ")\n";
        std::cout << " C - Caballero (Com:" << c_stats.cost_comida << ", Met:" << c_stats.cost_metal << ")\n";
        std::cout << " M - Mago      (Com:" << m_stats.cost_comida << ", Met:" << m_stats.cost_metal << ")\n";
        std::cout << " I - Ingeniero (Com:" << i_stats.cost_comida << ", Met:" << i_stats.cost_metal << ")\n";

        std::cout << "Elige tipo (S/A/C/M/I): ";
        std::string tipo = leer_linea();

        UnitType ut = UnitType::NONE_U;
        char code = toupper(tipo[0]);

        if (code == 'S') ut = UnitType::S;
        else if (code == 'A') ut = UnitType::A;
        else if (code == 'C') ut = UnitType::C;
        else if (code == 'M') ut = UnitType::M;
        else if (code == 'I') ut = UnitType::I;
        else {
            std::cout << "Tipo desconocido.\n";
            return;
        }

        eng.recruit(r, c, ut);
        return;
    }

    // --- BUILD ---
    if (cmd == "b")
    {
        std::cout << "Donde construir (fila columna): ";
        std::string s = leer_linea();

        int r = 0, c = 0;
        std::istringstream iss2(s);
        if (!(iss2 >> r >> c)) {
            std::cout << "Coordenadas invalidas.\n";
            return;
        }

        std::cout << "--- Edificios ---\n";
        std::cout << " Cu - Cuartel (Permite reclutar) [Metal:1, Eng:1]\n";
        std::cout << " To - Torre   (Defensa)          [Metal:2, Eng:1]\n";
        std::cout << " Gr - Granja  (Neutral placeholder) [Metal:1, Eng:1]\n";

        std::cout << "Tipo (Cu / To / Gr): ";
        std::string tipo = leer_linea();

        BuildingType bt = BuildingType::NONE_B;
        if (tipo == "Cu" || tipo == "cu") bt = BuildingType::CU;
        else if (tipo == "To" || tipo == "to") bt = BuildingType::TO;
        else if (tipo == "Gr" || tipo == "gr") bt = BuildingType::GR;
        else {
            std::cout << "Tipo inválido.\n";
            return;
        }

        eng.build(r, c, bt);
        return;
    }

    // --- END TURN ---
    if (cmd == "e") {
        eng.end_turn();
        std::cout << "Turno finalizado.\n";
        return;
    }

    // --- QUIT ---
    if (cmd == "q") {
        eng.request_quit();
        return;
    }

    std::cout << "Comando no reconocido. Usa: m, r, b, e, q\n";
}