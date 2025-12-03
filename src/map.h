#ifndef FINAL_PROYECT_PROGRAMMING_II_MAP_H
#define FINAL_PROYECT_PROGRAMMING_II_MAP_H

#include <vector>
#include <string>

enum class Owner
{
    NONE,
    J1,
    J2,
    NEUTRAL
};
enum class UnitType
{
    NONE_U,
    S,
    A,
    C,
    M,
    I
};
enum class BuildingType
{
    NONE_B,
    CU,
    TO,
    GR
};
enum class TerrainType
{
    LL,
    BO,
    MO,
    AG,
    PA
};

class Tile
{
private:
    TerrainType terrain;
    Owner owner;
    UnitType unit;
    Owner unit_owner;
    BuildingType building;
    Owner building_owner;

public:
    Tile();

    // getters / setters
    TerrainType getTerrain() const;
    void setTerrain(TerrainType t);

    Owner getOwner() const;
    void setOwner(Owner o);

    UnitType getUnit() const;
    void setUnit(UnitType u);

    Owner getUnitOwner() const;
    void setUnitOwner(Owner o);

    BuildingType getBuilding() const;
    void setBuilding(BuildingType b);

    Owner getBuildingOwner() const;
    void setBuildingOwner(Owner o);

    void clearUnit();
    void clearBuilding();

    std::string code() const; // e.g. "J1S/LL" or "..../AG"
};

class Map
{
private:
    int rows;
    int cols;
    std::vector<std::vector<Tile>> data;

public:
    Map(int r = 12, int c = 12);

    bool in_bounds(int r, int c) const;
    Tile &at(int r, int c);
    const Tile &at(int r, int c) const;
    const std::vector<std::vector<Tile>> &grid() const;
    int num_rows() const;
    int num_cols() const;

    void set_terrain_row(int row, TerrainType t);
    void clear();
};

class ConsoleRenderer
{
public:
    ConsoleRenderer();
    void render(const Map &map, int turno, int pa, int comida, int metal, int energia);
};


struct Building
{
    BuildingType type;
    Owner owner;
    int level;
    Building(BuildingType t = BuildingType::NONE_B, Owner o = Owner::NONE);
    std::string to_string() const;
};

#endif //FINAL_PROYECT_PROGRAMMING_II_MAP_H
