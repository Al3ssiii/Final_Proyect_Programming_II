#include "map.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <sstream>

Unit* create_unit(UnitType t, Owner o) {
    switch (t) {
        case UnitType::S: return new Soldier(o);
        case UnitType::A: return new Archer(o);
        case UnitType::C: return new Knight(o);
        case UnitType::M: return new Mage(o);
        case UnitType::I: return new Engineer(o);
        default: return nullptr;
    }
}

Tile::Tile()
        : terrain(TerrainType::LL),
          owner(Owner::NONE),
          unit_ptr(nullptr),
          building(BuildingType::NONE_B),
          building_owner(Owner::NONE)
{}

Tile::~Tile() {
    clearUnit();
}

void Tile::setUnit(Unit *u) {
    clearUnit();
    unit_ptr = u;
}

void Tile::clearUnit() {
    if (unit_ptr) {
        delete unit_ptr;
        unit_ptr = nullptr;
    }
}

Unit* Tile::releaseUnit() {
    Unit* u = unit_ptr;
    unit_ptr = nullptr;
    return u;
}

std::string get_terrain_code(TerrainType t) {
    switch (t) {
        case TerrainType::LL: return "LL";
        case TerrainType::BO: return "BO";
        case TerrainType::MO: return "MO";
        case TerrainType::AG: return "AG";
        case TerrainType::PA: return "PA";
        default: return "??";
    }
}

std::string get_unit_code(UnitType t) {
    switch (t) {
        case UnitType::S: return "S";
        case UnitType::A: return "A";
        case UnitType::C: return "C";
        case UnitType::M: return "M";
        case UnitType::I: return "I";
        default: return " ";
    }
}

std::string get_building_code(BuildingType b) {
    switch (b) {
        case BuildingType::CU: return "Cu";
        case BuildingType::TO: return "To";
        case BuildingType::GR: return "Gr";
        default: return "  ";
    }
}

std::string get_owner_code(Owner o) {
    switch (o) {
        case Owner::J1: return "1";
        case Owner::J2: return "2";
        case Owner::NEUTRAL: return "N";
        default: return " ";
    }
}

std::string Tile::code() const {
    std::string s;

    if (unit_ptr) {
        s += get_unit_code(unit_ptr->get_type());
        s += get_owner_code(unit_ptr->get_owner());
    } else {
        s += "  ";
    }

    if (building != BuildingType::NONE_B) {
        s += get_building_code(building);
        s += get_owner_code(building_owner);
    } else {
        s += get_terrain_code(terrain);
    }

    s += get_owner_code(owner);

    return s;
}

Map::Map(int r, int c)
    : rows(r), cols(c), data(r, std::vector<Tile>(c))
{}

bool Map::in_bounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

Tile &Map::at(int r, int c) {
    if (!in_bounds(r, c)) {
        throw std::out_of_range("Map coordinates out of bounds");
    }
    return data[r][c];
}

const Tile &Map::at(int r, int c) const {
    if (!in_bounds(r, c)) {
        throw std::out_of_range("Map coordinates out of bounds");
    }
    return data[r][c];
}

int Map::num_rows() const {
    return rows;
}

int Map::num_cols() const {
    return cols;
}

void Map::set_terrain(int r, int c, TerrainType t) {
    if (in_bounds(r, c)) {
        data[r][c].setTerrain(t);
    }
}

void ConsoleRenderer::render(const Map& map, int turno, int pa, int comida, int metal, int energia) const
{
    const int rows = map.num_rows();
    const int cols = map.num_cols();

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "Turno: " << turno << " | PA: " << pa;
    std::cout << " | Comida: " << comida << " | Metal: " << metal << " | Energia: " << energia << "\n";
    std::cout << "--------------------------------------------------------\n";

    std::cout << "   ";
    for (int c = 0; c < cols; ++c)
    {
        std::cout << std::right << std::setw(8) << c;
    }
    std::cout << "\n";

    for (int r = 0; r < rows; ++r)
    {
        std::cout << std::right << std::setw(2) << r << "  ";
        for (int c = 0; c < cols; ++c)
        {
            std::cout << std::left << std::setw(8) << map.at(r,c).code();
        }
        std::cout << "\n";
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << "[M] Move [R] Recruit [B] Build [E] End Turn [Q] Quit\n";
    std::cout << "--------------------------------------------------------\n";
}

void generar_mapa_aleatorio(Map &map) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int r = 0; r < map.num_rows(); ++r) {
        for (int c = 0; c < map.num_cols(); ++c) {
            int rnd = std::rand() % 100;
            if (rnd < 60) map.at(r, c).setTerrain(TerrainType::LL);
            else if (rnd < 75) map.at(r, c).setTerrain(TerrainType::BO);
            else if (rnd < 90) map.at(r, c).setTerrain(TerrainType::MO);
            else map.at(r, c).setTerrain(TerrainType::AG);
        }
    }

    map.at(0, 0).setBuilding(BuildingType::CU);
    map.at(0, 0).setBuildingOwner(Owner::J1);
    map.at(11, 11).setBuilding(BuildingType::CU);
    map.at(11, 11).setBuildingOwner(Owner::J2);

    map.at(11, 11).setUnit(create_unit(UnitType::S, Owner::J2));
}