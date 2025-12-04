#include "map.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

UnitStats get_unit_stats(UnitType t)
{
    switch (t)
    {
    case UnitType::S:
        return {10, 2, 1, 2, 1, 1}; // HP, ATK, DEF, MOV, FOOD, METAL
    case UnitType::A:
        return {8, 3, 0, 2, 1, 2};
    case UnitType::C:
        return {12, 3, 2, 2, 2, 2};
    case UnitType::M:
        return {7, 4, 0, 2, 2, 1};
    case UnitType::I:
        return {9, 2, 1, 2, 1, 1};
    default:
        return {0, 0, 0, 0, 0, 0};
    }
}

std::string get_unit_name(UnitType t)
{
    switch (t)
    {
    case UnitType::S:
        return "Soldado";
    case UnitType::A:
        return "Arquero";
    case UnitType::C:
        return "Caballero";
    case UnitType::M:
        return "Mago";
    case UnitType::I:
        return "Ingeniero";
    default:
        return "Ninguno";
    }
}

Tile::Tile()
    : terrain(TerrainType::LL),
      owner(Owner::NONE),
      unit(UnitType::NONE_U),
      unit_owner(Owner::NONE),
      building(BuildingType::NONE_B),
      building_owner(Owner::NONE) {}

TerrainType Tile::getTerrain() const { return terrain; }
void Tile::setTerrain(TerrainType t) { terrain = t; }

Owner Tile::getOwner() const { return owner; }
void Tile::setOwner(Owner o) { owner = o; }

UnitType Tile::getUnit() const { return unit; }
void Tile::setUnit(UnitType u) { unit = u; }

Owner Tile::getUnitOwner() const { return unit_owner; }
void Tile::setUnitOwner(Owner o) { unit_owner = o; }

BuildingType Tile::getBuilding() const { return building; }
void Tile::setBuilding(BuildingType b) { building = b; }

Owner Tile::getBuildingOwner() const { return building_owner; }
void Tile::setBuildingOwner(Owner o) { building_owner = o; }

void Tile::clearUnit()
{
    unit = UnitType::NONE_U;
    unit_owner = Owner::NONE;
}
void Tile::clearBuilding()
{
    building = BuildingType::NONE_B;
    building_owner = Owner::NONE;
}

std::string Tile::code() const
{
    auto terr = [](TerrainType t)
    {
        switch (t)
        {
        case TerrainType::LL:
            return "LL";
        case TerrainType::BO:
            return "BO";
        case TerrainType::MO:
            return "MO";
        case TerrainType::AG:
            return "AG";
        case TerrainType::PA:
            return "PA";
        }
        return "??";
    }(terrain);

    auto pref = [](Owner o)
    {
        switch (o)
        {
        case Owner::J1:
            return "J1";
        case Owner::J2:
            return "J2";
        case Owner::NEUTRAL:
            return "Ne";
        default:
            return "..";
        }
    };

    std::string owner_str = "..";
    std::string unit_str = "";
    std::string building_str = "";
    std::string terrain_str = terr;

    if (unit != UnitType::NONE_U)
    {
        owner_str = pref(unit_owner);
        switch (unit)
        {
        case UnitType::S:
            unit_str = "S";
            break;
        case UnitType::A:
            unit_str = "A";
            break;
        case UnitType::C:
            unit_str = "C";
            break;
        case UnitType::M:
            unit_str = "M";
            break;
        case UnitType::I:
            unit_str = "I";
            break;
        default:
            unit_str = "?";
            break;
        }
    }

    if (building != BuildingType::NONE_B)
    {
        if (unit == UnitType::NONE_U)
            owner_str = pref(building_owner);
        switch (building)
        {
        case BuildingType::CU:
            building_str = "Cu";
            break;
        case BuildingType::TO:
            building_str = "To";
            break;
        case BuildingType::GR:
            building_str = "Gr";
            break;
        default:
            building_str = "??";
            break;
        }
    }

    if (!unit_str.empty() && !building_str.empty())
    {
        return owner_str + unit_str + "+" + building_str;
    }
    else if (!unit_str.empty())
    {
        return owner_str + unit_str + " " + terrain_str;
    }
    else if (!building_str.empty())
    {
        return owner_str + building_str + " " + terrain_str;
    }
    else
    {
        return " .." + std::string(" ") + terrain_str;
    }
}

Map::Map(int r, int c) : rows(r), cols(c), data(r, std::vector<Tile>(c)) {}

bool Map::in_bounds(int r, int c) const { return r >= 0 && c >= 0 && r < rows && c < cols; }
Tile &Map::at(int r, int c) { return data[r][c]; }
const Tile &Map::at(int r, int c) const { return data[r][c]; }
const std::vector<std::vector<Tile>> &Map::grid() const { return data; }
int Map::num_rows() const { return rows; }
int Map::num_cols() const { return cols; }

void Map::set_terrain_row(int row, TerrainType t)
{
    if (row < 0 || row >= rows)
        return;
    for (int c = 0; c < cols; ++c)
        data[row][c].setTerrain(t);
}

void Map::clear()
{
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            data[r][c] = Tile();
}

ConsoleRenderer::ConsoleRenderer() {}

void ConsoleRenderer::render(const Map &map, int turno, int pa, int comida, int metal, int energia)
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    std::cout << "\x1B[2J\x1B[H";
#endif

    std::cout << "========================================================\n";
    std::cout << " Turno: " << turno
              << " | PA: " << pa
              << " | Com: " << comida
              << " | Met: " << metal
              << " | Eng: " << energia << "\n";
    std::cout << "========================================================\n";

    const int rows = map.num_rows();
    const int cols = map.num_cols();

    std::cout << "    ";
    for (int c = 0; c < cols; ++c)
        std::cout << std::left << std::setw(8) << c;
    std::cout << "\n";

    for (int r = 0; r < rows; ++r)
    {
        std::cout << std::right << std::setw(2) << r << "  ";
        for (int c = 0; c < cols; ++c)
        {
            std::cout << std::left << std::setw(8) << map.at(r, c).code();
        }
        std::cout << "\n";
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << "[M] Move [R] Recruit [B] Build [E] End Turn [Q] Quit\n";
    std::cout << "--------------------------------------------------------\n";
}

void generar_mapa_aleatorio(Map &map)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int r = 0; r < map.num_rows(); ++r)
    {
        for (int c = 0; c < map.num_cols(); ++c)
        {
            int rnd = std::rand() % 100;
            if (rnd < 60)
                map.at(r, c).setTerrain(TerrainType::LL);
            else if (rnd < 75)
                map.at(r, c).setTerrain(TerrainType::BO);
            else if (rnd < 90)
                map.at(r, c).setTerrain(TerrainType::MO);
            else
                map.at(r, c).setTerrain(TerrainType::AG);
        }
    }

    // Bases
    map.at(0, 0).setBuilding(BuildingType::CU);
    map.at(0, 0).setBuildingOwner(Owner::J1);
    map.at(map.num_rows() - 1, map.num_cols() - 1).setBuilding(BuildingType::CU);
    map.at(map.num_rows() - 1, map.num_cols() - 1).setBuildingOwner(Owner::J2);

    // Initial Units
    map.at(1, 0).setUnit(UnitType::S);
    map.at(1, 0).setUnitOwner(Owner::J1);
    map.at(map.num_rows() - 2, map.num_cols() - 1).setUnit(UnitType::S);
    map.at(map.num_rows() - 2, map.num_cols() - 1).setUnitOwner(Owner::J2);
}
