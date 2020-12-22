#include "world.h"

#include <array>
#include <stdexcept>

World::World(point_t size) {
    std::tie(_x, _y) = size;
    _current_world_ptr = createEmptyMatrix(size);
    _next_world_ptr = createEmptyMatrix(size);

    numberOfLivingAround = &World::numberOfLivingAroundWithBorder;
}

World::World(World &&temporaryWorld) {
    _current_world_ptr = std::move(temporaryWorld._current_world_ptr);
    _next_world_ptr = std::move(temporaryWorld._next_world_ptr);

    _x = temporaryWorld._x;
    _y = temporaryWorld._y;

    numberOfLivingAround = &World::numberOfLivingAroundWithBorder;
}

bool World::step() {

    auto was_changed{ false };

    for(auto i{ 0ul }; i < _x; ++i) {
        for(auto j{ 0ul }; j < _y; ++j) {
            if(isCellChange({i, j})) {
                (*_next_world_ptr)[i][j] = !(*_current_world_ptr)[i][j];
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

void World::set(point_t point) {
    auto &[x, y] = point;

    if(!checkIndex(x, y)) {
        throw std::out_of_range("Index out of Range");
    }

    (*_current_world_ptr)[x][y] = true;
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

    auto alive_around{ numberOfLivingAround(this, point) };

    auto become_alive_condition{ !test(point) && alive_around == 3 };
    auto becode_dead_condition{ test(point) && (alive_around < 2 || alive_around > 3) };

    return becode_dead_condition || become_alive_condition;
}

//#include <iostream>
int World::numberOfLivingAroundWithBorder(point_t point) const {

    auto &[x, y]{ point };
    int number_of_living{};

    if(x > 0) {
        if(y > 0) number_of_living += (*_current_world_ptr)[x - 1][y - 1];
        number_of_living += (*_current_world_ptr)[x - 1][y];
        if(y < _y - 1) number_of_living += (*_current_world_ptr)[x - 1][y + 1];
    }

    if(y > 0) number_of_living += (*_current_world_ptr)[x][y - 1];
    if(y < _x - 1) number_of_living += (*_current_world_ptr)[x][y + 1];

    if(x < _x - 1)  {
        if(y > 0) number_of_living += (*_current_world_ptr)[x + 1][y - 1];
        number_of_living += (*_current_world_ptr)[x + 1][y];
        if(y < _y - 1) number_of_living += (*_current_world_ptr)[x + 1][y + 1];
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
    return  x < _x || y < _y;
}
