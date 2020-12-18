#include "world.h"

#include <array>

World::World(const linematrix_t &alive, point_t size)    /// продумать
    : _alive_ptr(std::make_unique<linematrix_t>(alive))
{
    std::tie(_x, _y) = size;
    if(_x * _y < _alive_ptr->size()) _x = _y = 0;
}

World::World(linematrix_t &&alive, point_t size)
    : _alive_ptr(std::make_unique<linematrix_t>(alive))
{
    std::tie(_x, _y) = size;
    if(_x * _y < _alive_ptr->size()) _x = _y = 0;
}

bool World::step() {
    for( auto i{ 0ul }; i < _alive_ptr->size(); ++i) {
        if(isCellChange(i, &World::numberOfLivingAroundWithBorder )) {
            _alive_ptr->at(i) = !_alive_ptr->at(i);
        }
    }


    //////   Проверка на конец игры !!!!!!!!!!!!!!!!!!!!!!
    return true;
}

bool World::isCellChange(index_t index, std::function<int(const World *, index_t)> numberOfLivingAround) const {
    auto alive_around{ numberOfLivingAround(this, index) };

    auto become_alive_condition{ !_alive_ptr->at(index) && alive_around == 3 };
    auto becode_dead_condition{ _alive_ptr->at(index) && (alive_around < 2 || alive_around > 3) };

    return becode_dead_condition || become_alive_condition;
}

int World::numberOfLivingAroundWithBorder(index_t index) const {

    auto x{ index / _y };
    auto y{ index % _x };
    int number_of_living{};

    if(x > 0) {
        if(y > 0) number_of_living += _alive_ptr->at(index - _x - 1);
        number_of_living += _alive_ptr->at(index - _x);
        if(y < _y - 1) number_of_living += _alive_ptr->at(index - _x + 1);
    }

    if(y > 0) number_of_living += _alive_ptr->at(index - 1);
    if(y < _y - 1) number_of_living += _alive_ptr->at(index + 1);

    if(x < _x - 1) {
        if(y > 0) number_of_living += _alive_ptr->at(index + _x - 1);
        number_of_living += _alive_ptr->at(index + _x);
        if(y < _y - 1) number_of_living += _alive_ptr->at(index + _x + 1);
    }


//    if(x > 0) {
//        if(y > 0) { number_of_living += _alive_ptr->at(index - _x - 1); _alive_ptr->at(index - _x -1) = true; }
//        { number_of_living += _alive_ptr->at(index - _x); _alive_ptr->at(index - _x) = true; }
//        if(y < _y - 1) { number_of_living += _alive_ptr->at(index - _x + 1); _alive_ptr->at(index - _x +1) = true; }
//    }

//    if(y > 0) { number_of_living += _alive_ptr->at(index - 1); _alive_ptr->at(index - 1) = true; }
//    if(y < _y - 1) { number_of_living += _alive_ptr->at(index + 1); _alive_ptr->at(index + 1) = true; }

//    if(x < _x - 1) {
//        if(y > 0) { number_of_living += _alive_ptr->at(index + _x - 1); _alive_ptr->at(index + _x -1) = true; }
//        { number_of_living += _alive_ptr->at(index + _x); _alive_ptr->at(index + _x) = true; }
//        if(y < _y - 1) { number_of_living += _alive_ptr->at(index + _x + 1); _alive_ptr->at(index + _x +1) = true; }
//    }


//    for(auto i {0ul}; i < _alive_ptr->size(); ++i) {
//        std::cout << _alive_ptr->at(i);
//        if((i+1) % 10 == 0) {
//            std::cout << std::endl;
//        }
//    }

    return number_of_living;
}
