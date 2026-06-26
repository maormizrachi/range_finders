#ifndef RANGE_FINDERS_INDEXED_VECTOR_HPP
#define RANGE_FINDERS_INDEXED_VECTOR_HPP

#include <iostream>
#include <cmath>
#include <limits>
#include <string>

#define ILLEGAL_IDX std::numeric_limits<size_t>::max()

template <typename PointT>
struct IndexedVector
{
    using coord_type = typename PointT::coord_type;
    using Raw_type = PointT;

    coord_type values[3];
    size_t index;

    inline IndexedVector(const coord_type *values, size_t index): index(index){this->values[0] = values[0]; this->values[1] = values[1]; this->values[2] = values[2];}
    inline IndexedVector(coord_type x, coord_type y, coord_type z, size_t index): index(index){this->values[0] = x; this->values[1] = y; this->values[2] = z;}
    inline IndexedVector(coord_type x, coord_type y, coord_type z): IndexedVector(x, y, z, 0){}
    inline explicit IndexedVector(): IndexedVector(0, 0, 0){}
    inline IndexedVector(const PointT &other): IndexedVector(other.x, other.y, other.z){}
    inline IndexedVector(const IndexedVector &other): IndexedVector(&other.values[0], other.index){}
    inline IndexedVector(const PointT &point, size_t index): IndexedVector(point.x, point.y, point.z, index){}

    inline IndexedVector &operator=(const IndexedVector &other){this->values[0] = other.values[0]; this->values[1] = other.values[1]; this->values[2] = other.values[2]; this->index = other.index; return *this;}

    inline bool operator==(const IndexedVector &other) const
    {
        constexpr coord_type eps = static_cast<coord_type>(1e-12);
        return (std::abs(this->values[0] - other.values[0]) < eps) &&
               (std::abs(this->values[1] - other.values[1]) < eps) &&
               (std::abs(this->values[2] - other.values[2]) < eps);
    }

    inline bool operator<=(const IndexedVector &other) const
    {
        if(this->values[0] < other.values[0]) return true;
        if(this->values[0] == other.values[0])
        {
            if(this->values[1] < other.values[1]) return true;
            if(this->values[1] == other.values[1]) return (this->values[2] <= other.values[2]);
        }
        return false;
    }

    inline bool operator<(const IndexedVector &other) const{return (*this) <= other;}
    inline coord_type &operator[](size_t idx){return this->values[idx];}
    inline const coord_type &operator[](size_t idx) const{return this->values[idx];}
    inline IndexedVector operator+(const IndexedVector &other) const{return IndexedVector(this->values[0] + other.values[0], this->values[1] + other.values[1], this->values[2] + other.values[2], ILLEGAL_IDX);}
    inline IndexedVector operator*(coord_type scalar) const{return IndexedVector(this->values[0] * scalar, this->values[1] * scalar, this->values[2] * scalar, ILLEGAL_IDX);}
    inline IndexedVector operator/(coord_type scalar) const{return this->operator*(1/scalar);}

    friend inline std::ostream &operator<<(std::ostream &stream, const IndexedVector &vec)
    {
        stream << "(" << vec.values[0] << ", " << vec.values[1] << ", " << vec.values[2] << ")";
        return stream;
    }

    friend inline std::istream &operator>>(std::istream &stream, IndexedVector &point)
    {
        std::string str;
        std::getline(stream, str, '(');
        std::getline(stream, str, ',');
        point.values[0] = static_cast<coord_type>(std::stod(str));
        std::getline(stream, str, ',');
        point.values[1] = static_cast<coord_type>(std::stod(str));
        std::getline(stream, str, ')');
        point.values[2] = static_cast<coord_type>(std::stod(str));
        return stream;
    }

    inline size_t getIndex() const{return this->index;}
    inline PointT getVector() const{return PointT(values[0], values[1], values[2]);}
};

#endif // RANGE_FINDERS_INDEXED_VECTOR_HPP
