#ifndef FINAL_PROYECT_PROGRAMMING_II_MAP_H
#define FINAL_PROYECT_PROGRAMMING_II_MAP_H

#include <vector>
#include <string>
#include <algorithm>

enum class Owner { NONE, J1, J2, NEUTRAL };
enum class UnitType { NONE_U, S, A, C, M, I };
enum class BuildingType { NONE_B, CU, TO, GR };
enum class TerrainType { LL, BO, MO, AG, PA };

class Unit
{
protected:
    int hp;
    int max_hp;
    int atk;
    int def;
    int move_range;
    int cost_comida;
    int cost_metal;
    Owner owner;

public:
    Unit(int hp, int atk, int def, int mov, int c_food, int c_metal, Owner own)
        : hp(hp), max_hp(hp), atk(atk), def(def), move_range(mov),
          cost_comida(c_food), cost_metal(c_metal), owner(own) {}

    virtual ~Unit() = default;

    virtual std::string get_name() const = 0;
    virtual UnitType get_type() const = 0;

    virtual int get_movement_cost(TerrainType t) const { return 1; }
    virtual int get_defense_bonus(TerrainType t) const { return 0; }

    int get_hp() const { return hp; }
    int get_atk() const { return atk; }
    int get_def() const { return def; }
    int get_cost_comida() const { return cost_comida; }
    int get_cost_metal() const { return cost_metal; }
    Owner get_owner() const { return owner; }

    void take_damage(int damage) { hp -= damage; }
    void heal(int amount) { hp = std::min(hp + amount, max_hp); }
};

class Soldier : public Unit {
public:
    Soldier(Owner o) : Unit(10, 2, 1, 2, 1, 1, o) {}
    std::string get_name() const override { return "Soldado"; }
    UnitType get_type() const override { return UnitType::S; }
};

class Archer : public Unit {
public:
    Archer(Owner o) : Unit(8, 3, 0, 2, 1, 1, o) {}
    std::string get_name() const override { return "Arquero"; }
    UnitType get_type() const override { return UnitType::A; }

    int get_movement_cost(TerrainType t) const override {
        return (t == TerrainType::BO) ? 0 : 1;
    }
};

class Knight : public Unit {
public:
    Knight(Owner o) : Unit(12, 3, 2, 2, 1, 1, o) {}
    std::string get_name() const override { return "Caballero"; }
    UnitType get_type() const override { return UnitType::C; }

    int get_movement_cost(TerrainType t) const override {
        return (t == TerrainType::BO || t == TerrainType::MO) ? 2 : 1;
    }
};

class Mage : public Unit {
public:
    Mage(Owner o) : Unit(7, 4, 0, 2, 2, 1, o) {}
    std::string get_name() const override { return "Mago"; }
    UnitType get_type() const override { return UnitType::M; }
};

class Engineer : public Unit {
public:
    Engineer(Owner o) : Unit(9, 2, 1, 2, 1, 1, o) {}
    std::string get_name() const override { return "Ingeniero"; }
    UnitType get_type() const override { return UnitType::I; }
};

Unit* create_unit(UnitType t, Owner o);

class Tile
{
private:
    TerrainType terrain;
    Owner owner;
    Unit* unit_ptr;
    BuildingType building;
    Owner building_owner;

public:
    Tile();
    ~Tile();

    TerrainType getTerrain() const { return terrain; }
    void setTerrain(TerrainType t) { terrain = t; }

    Owner getOwner() const { return owner; }
    void setOwner(Owner o) { owner = o; }

    Unit* getUnit() const { return unit_ptr; }
    void setUnit(Unit* u);
    void clearUnit();
    Unit* releaseUnit(); // Nuevo: Cede el puntero sin eliminar la unidad

    Owner getUnitOwner() const { return unit_ptr ? unit_ptr->get_owner() : Owner::NONE; }

    BuildingType getBuilding() const { return building; }
    void setBuilding(BuildingType b) { building = b; }

    Owner getBuildingOwner() const { return building_owner; }
    void setBuildingOwner(Owner o) { building_owner = o; }

    void clearBuilding() { building = BuildingType::NONE_B; building_owner = Owner::NONE; }

    std::string code() const;
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
    int num_rows() const;
    int num_cols() const;

    void set_terrain(int r, int c, TerrainType t);
};

void generar_mapa_aleatorio(Map &map);

class ConsoleRenderer {
public:
    void render(const Map& map, int turno, int pa, int comida, int metal, int energia) const;
};

#endif