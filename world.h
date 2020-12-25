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
    using point_vector_t = std::vector<point_t>;

    enum NeighborCountingPolicy{ WITH_BORDER, WITHOUT_BORDER };

public:

    explicit World(point_t size = {10, 10},
                   bool saveLastStepChanges = false,
                   NeighborCountingPolicy neighborCountingPolicy = WITH_BORDER);
    World(World &&temporaryWorld);
    World(const World &other);

    World &operator=(World &&other);
    World &operator=(World &other);

    index_t row() const;
    index_t col() const;
    const matrix_t &matrix() const;
    const point_vector_t &lastStapCellsChanged() const;
    bool isSaveLastStepChanges() const;
    NeighborCountingPolicy neighborCountingPolicy() const;

    void setNeighborCountingPolicy(NeighborCountingPolicy policy);
    void setSavingLastStepChages(bool value);

    bool step();
    void clear();

    void set(point_t point);
    void drop(point_t point);
    void inverse(point_t point);
    bool test(point_t point) const;

private:

    bool isCellChange(point_t point) const;
    int numberOfLivingAroundWithBorder(point_t index) const;
    int numberOfLivingAroundWithoutBorder(point_t index) const;

    std::unique_ptr<matrix_t> createEmptyMatrix(point_t size);
    bool checkIndex(index_t x, index_t y) const;

private:

    std::unique_ptr<matrix_t> _current_world_ptr;
    std::unique_ptr<matrix_t> _next_world_ptr;

    bool _saveLastStepChages;
    point_vector_t _lastStepCellsChanged;

    index_t _x;
    index_t _y;

    NeighborCountingPolicy _neighborCountingPolicy;
    std::function<int(const World *, point_t)> _numberOfLivingAround;

};

std::ostream &operator<<(std::ostream &out, const World &world);
std::istream &operator>>(std::istream &in, World &world);

#endif // WORLD_H
