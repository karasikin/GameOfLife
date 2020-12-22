#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include <functional>


class World {

public:

    using index_t = unsigned long;
    using point_t = std::tuple<index_t, index_t>;
    using matrix_t = std::vector<std::vector<bool>>;


public:

    explicit World(point_t size = {10, 10});
    World(World &&temporaryWorld);


    index_t row() const { return _x; }
    index_t col() const { return _y; }
    const matrix_t &matrix() const { return *_current_world_ptr; }

    bool step();
    void set(point_t point);
    bool test(point_t point) const;

private:

    bool isCellChange(point_t point) const;
    int numberOfLivingAroundWithBorder(point_t index) const;

    std::unique_ptr<matrix_t> createEmptyMatrix(point_t size);
    bool checkIndex(index_t x, index_t y) const;

private:

    std::unique_ptr<matrix_t> _current_world_ptr;
    std::unique_ptr<matrix_t> _next_world_ptr;

    std::function<int(const World *, point_t)> numberOfLivingAround;

    index_t _x;
    index_t _y;

};

#endif // WORLD_H
