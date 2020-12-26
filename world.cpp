#include "world.h"

#include <array>
#include <ostream>
#include <iostream>
#include <stdexcept>

World::World(point_t size, bool saveLastStepChanges, NeighborCountingPolicy neighborCountingPolicy)
    : _current_world_ptr(createEmptyMatrix(size)),
      _next_world_ptr(createEmptyMatrix(size)),
      _saveLastStepChages(saveLastStepChanges)
{
    std::tie(_x, _y) = size;
    setNeighborCountingPolicy(neighborCountingPolicy);
}


World::World(World &&temporaryWorld) {
    _current_world_ptr = std::move(temporaryWorld._current_world_ptr);
    _next_world_ptr = std::move(temporaryWorld._next_world_ptr);

    _saveLastStepChages = temporaryWorld._saveLastStepChages;
    _lastStepCellsChanged = std::move(temporaryWorld._lastStepCellsChanged);

    _x = temporaryWorld._x;
    _y = temporaryWorld._y;

    setNeighborCountingPolicy(temporaryWorld._neighborCountingPolicy);
}

World::World(const World &other) {
    _current_world_ptr = std::make_unique<matrix_t>(*other._current_world_ptr);
    _next_world_ptr = std::make_unique<matrix_t>(*other._next_world_ptr);

    _saveLastStepChages = other._saveLastStepChages;
    _lastStepCellsChanged = other._lastStepCellsChanged;

    _x = other._x;
    _y = other._y;

    setNeighborCountingPolicy(other._neighborCountingPolicy);
}

World &World::operator=(World &&other) {
    if(this == &other) {
        return *this;
    }

    _current_world_ptr = std::move(other._current_world_ptr);
    _next_world_ptr = std::move(other._next_world_ptr);

    _saveLastStepChages = other._saveLastStepChages;
    _lastStepCellsChanged = std::move(other._lastStepCellsChanged);

    _x = other._x;
    _y = other._y;

    setNeighborCountingPolicy(other._neighborCountingPolicy);

    return *this;
}

World &World::operator=(World &other) {
    if(this == &other) {
        return *this;
    }

    _current_world_ptr = std::make_unique<matrix_t>(*other._current_world_ptr);
    _next_world_ptr = std::make_unique<matrix_t>(*other._next_world_ptr);

    _saveLastStepChages = other._saveLastStepChages;
    _lastStepCellsChanged = other._lastStepCellsChanged;

    _x = other._x;
    _y = other._y;

    setNeighborCountingPolicy(other._neighborCountingPolicy);

    return *this;
}

// Getters {

World::index_t World::row() const { return _x; }
World::index_t World::col() const { return _y; }
const World::matrix_t &World::matrix() const { return *_current_world_ptr; }
const World::point_vector_t &World::lastStapCellsChanged() const { return _lastStepCellsChanged; }
bool World::isSaveLastStepChanges() const { return _saveLastStepChages; }
World::NeighborCountingPolicy World::neighborCountingPolicy() const { return _neighborCountingPolicy; }

// } Getters

void World::setNeighborCountingPolicy(NeighborCountingPolicy policy) {

    _neighborCountingPolicy = policy;

    switch (_neighborCountingPolicy) {
        case WITH_BORDER: {
            _numberOfLivingAround = &World::numberOfLivingAroundWithBorder;
            return;
        }
        case WITHOUT_BORDER: {
            _numberOfLivingAround = &World::numberOfLivingAroundWithoutBorder;
            return;
        }
        default: {
            return;
        }
    }
}

void World::setSavingLastStepChages(bool value) {
    _saveLastStepChages = value;
    _lastStepCellsChanged.clear();
}

bool World::step() {

    auto was_changed{ false };

    if(_saveLastStepChages) _lastStepCellsChanged.clear();    // Очищаем вектор изменяющихся точек


    for(auto i{ 0ul }; i < _x; ++i) {
        for(auto j{ 0ul }; j < _y; ++j) {
            if(isCellChange({i, j})) {
                (*_next_world_ptr)[i][j] = !(*_current_world_ptr)[i][j];
                if(_saveLastStepChages) _lastStepCellsChanged.push_back({i, j});  // Добавляем в вектор изменившуюся точку
                was_changed = true;
            } else {
                (*_next_world_ptr)[i][j] = (*_current_world_ptr)[i][j];
            }
        }
    }

    auto tmp{ std::move(_current_world_ptr) };
    _current_world_ptr = std::move(_next_world_ptr);
    _next_world_ptr = std::move(tmp);

    return was_changed;
}

void World::clear() {
    for(auto i{ 0ul }; i < _x; ++i) {
        for(auto j{0ul }; j < _y; ++j) {
            (*_current_world_ptr)[i][j] = false;
        }
    }
}

void World::set(point_t point) {
    auto &[x, y]{ point };

    if(!checkIndex(x, y)) {
        throw std::out_of_range("Index out of Range");
    }

    (*_current_world_ptr)[x][y] = true;
}

void World::drop(point_t point) {
    auto &[x, y]{ point };

    if(!checkIndex(x, y)) {
        throw std::out_of_range("Index out of Range");
    }

    (*_current_world_ptr)[x][y] = false;
}

void World::inverse(point_t point) {
    auto &[x, y]{ point };

    if(!checkIndex(x, y)) {
        throw std::out_of_range("Index out of Range");
    }

    (*_current_world_ptr)[x][y] = !(*_current_world_ptr)[x][y];
}

bool World::test(point_t point) const {
    auto &[x, y] = point;

    if(!checkIndex(x, y)) {
        throw std::out_of_range("Index out of Range");
    }

    return (*_current_world_ptr)[x][y];
}



bool World::isCellChange(point_t point) const {
    auto &[x, y]{ point };
    if(!checkIndex(x, y)) {
        throw std::out_of_range{ "Index out of Range" };
    }

    auto alive_around{ _numberOfLivingAround(this, point) };

    auto become_alive_condition{ !test(point) && alive_around == 3 };
    auto becode_dead_condition{ test(point) && (alive_around < 2 || alive_around > 3) };

    return becode_dead_condition || become_alive_condition;
}

int World::numberOfLivingAroundWithBorder(point_t point) const {
    auto &[x, y]{ point };
    int number_of_living{};

    if(x > 0) {
        if(y > 0) number_of_living += (*_current_world_ptr)[x - 1][y - 1];
        number_of_living += (*_current_world_ptr)[x - 1][y];
        if(y < _y - 1) number_of_living += (*_current_world_ptr)[x - 1][y + 1];
    }

    if(y > 0) number_of_living += (*_current_world_ptr)[x][y - 1];
    if(y < _y - 1) number_of_living += (*_current_world_ptr)[x][y + 1];

    if(x < _x - 1)  {
        if(y > 0) number_of_living += (*_current_world_ptr)[x + 1][y - 1];
        number_of_living += (*_current_world_ptr)[x + 1][y];
        if(y < _y - 1) number_of_living += (*_current_world_ptr)[x + 1][y + 1];
    }

    return number_of_living;
}

int World::numberOfLivingAroundWithoutBorder(point_t point) const {
    auto &[x, y]{ point };
    int number_of_living{};

    std::array<point_t, 8> neighbors{};

    neighbors[0] = { x > 0 ? x - 1 : _x - 1, y > 0 ? y - 1 : _y - 1 }; // top - left
    neighbors[1] = { x > 0 ? x - 1 : _x - 1, y }; // top - middle
    neighbors[2] = { x > 0 ? x - 1 : _x - 1, y < _y - 1 ? y + 1 : 0 }; // top - right

    neighbors[3] = { x, y > 0 ? y - 1 : _y - 1 }; // left
    neighbors[4] = { x, y < _y - 1 ? y + 1 : 0 }; // right

    neighbors[5] = { x < _x - 1 ? x + 1 : 0, y > 0 ? y - 1 : _y - 1 }; // bottom - left
    neighbors[6] = { x < _x - 1 ? x + 1 : 0, y }; // bottom - middle
    neighbors[7] = { x < _x - 1 ? x + 1 : 0, y < _y - 1 ? y + 1 : 0 }; // bottom - right

    for(const auto &cell : neighbors) {
        number_of_living += (*_current_world_ptr)[std::get<0>(cell)][std::get<1>(cell)];
    }

    return number_of_living;
}

std::unique_ptr<World::matrix_t> World::createEmptyMatrix(point_t size) {
    auto matrix_ptr{ std::make_unique<matrix_t>(std::get<0>(size)) };
    for(auto i{ 0ul }; i < std::get<0>(size); ++i) {
        (*matrix_ptr)[i] = std::vector<bool>(std::get<1>(size));
    }

    return matrix_ptr;
}

bool World::checkIndex(index_t x, index_t y) const {
    return  x < _x && y < _y;
}

std::ostream &operator<<(std::ostream &out, const World &world) {
    out << world.row() << ' ' << world.col() << std::endl;
    out << world.neighborCountingPolicy() << std::endl;
    out << world.isSaveLastStepChanges() << std::endl;

    for(auto i{ 0ul }; i < world.row(); ++i) {
        for(auto j{ 0ul }; j < world.col(); ++j) {
            if(world.test({i, j})) {
                out << i << ' ' << j << std::endl;      // Пока что так!!!!!
            }
        }
    }

    return out;
}

std::istream &operator>>(std::istream &in, World &world) {
    World::index_t row{}, col{};
    uint policy_tmp{};
    World::NeighborCountingPolicy policy{};
    bool isSaveChanged{};

    in >> row >> col;
    in >> policy_tmp >> isSaveChanged;
    policy = World::NeighborCountingPolicy(policy_tmp);

    world = World{ std::tuple{row, col}, isSaveChanged, policy };

    while(true) {
        in >> row >> col;
        if(in.eof()) break;
        world.set({row, col});
    }

    return in;
}









