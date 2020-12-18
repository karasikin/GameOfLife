#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include <functional>


class World {

public:

    using index_t = unsigned long;
    using point_t = std::tuple<index_t, index_t>;
    using linematrix_t = std::vector<bool>;


public:

    World(const linematrix_t &alive, point_t size = {0, 0});
    World(linematrix_t &&alive, point_t size = {0, 0});

    index_t x() const { return _x; }
    index_t y() const { return _y; }

private:

     bool isCellChange(index_t index, std::function<int(index_t)> numberOfLivingAround) const;
public:     int numberOfLivingAroundWithBorder(index_t index) const;


private:

    std::unique_ptr<linematrix_t> _alive_ptr;
    index_t _x;
    index_t _y;

};

#endif // WORLD_H
