#ifndef RANGE_FINDERS_RANKED_INDEXED_VECTOR_HPP
#define RANGE_FINDERS_RANKED_INDEXED_VECTOR_HPP

#include <iostream>
#include <cmath>
#include <limits>
#include <string>

#ifdef RICH_MPI
    #include <mpi_utils/serialize/Serializer.hpp>
    #include <mpi_utils/types.h>
#endif // RICH_MPI

#ifdef RICH_MPI

template <typename PointT>
struct RankedIndexedVector : public Serializable
{
    using coord_type = typename PointT::coord_type;
    using Raw_type = PointT;

    static constexpr size_t ILLEGAL_INDEX = std::numeric_limits<size_t>::max();
    static constexpr rank_t ILLEGAL_RANK = -1;

    coord_type values[3];
    size_t index;
    rank_t rank;

    inline RankedIndexedVector(const coord_type *values, size_t index, rank_t rank): index(index), rank(rank){this->values[0] = values[0]; this->values[1] = values[1]; this->values[2] = values[2];}
    inline RankedIndexedVector(coord_type x, coord_type y, coord_type z, size_t index, rank_t rank): index(index), rank(rank){this->values[0] = x; this->values[1] = y; this->values[2] = z;}
    inline RankedIndexedVector(coord_type x = 0, coord_type y = 0, coord_type z = 0): RankedIndexedVector(x, y, z, ILLEGAL_INDEX, ILLEGAL_RANK){}
    inline RankedIndexedVector(const PointT &other): RankedIndexedVector(other.x, other.y, other.z){}
    inline RankedIndexedVector(const RankedIndexedVector &other): RankedIndexedVector(other.values[0], other.values[1], other.values[2], other.index, other.rank){}
    inline RankedIndexedVector(const PointT &point, size_t index, rank_t rank): RankedIndexedVector(point.x, point.y, point.z, index, rank){}

    inline RankedIndexedVector &operator=(const RankedIndexedVector &other)
    {
        this->values[0] = other.values[0];
        this->values[1] = other.values[1];
        this->values[2] = other.values[2];
        this->index = other.index;
        this->rank = other.rank;
        return *this;
    }

    inline bool operator==(const RankedIndexedVector &other) const
    {
        constexpr coord_type eps = static_cast<coord_type>(1e-12);
        return (std::abs(this->values[0] - other.values[0]) < eps) &&
               (std::abs(this->values[1] - other.values[1]) < eps) &&
               (std::abs(this->values[2] - other.values[2]) < eps);
    }

    inline bool operator<=(const RankedIndexedVector &other) const
    {
        if(this->values[0] < other.values[0]) return true;
        if(this->values[0] == other.values[0])
        {
            if(this->values[1] < other.values[1]) return true;
            if(this->values[1] == other.values[1]) return (this->values[2] <= other.values[2]);
        }
        return false;
    }

    inline bool operator<(const RankedIndexedVector &other) const{return (*this) <= other;}
    inline coord_type &operator[](size_t idx){return this->values[idx];}
    inline const coord_type &operator[](size_t idx) const{return this->values[idx];}
    inline RankedIndexedVector operator+(const RankedIndexedVector &other) const{return RankedIndexedVector(this->values[0] + other.values[0], this->values[1] + other.values[1], this->values[2] + other.values[2], ILLEGAL_INDEX, ILLEGAL_RANK);}
    inline RankedIndexedVector operator*(coord_type scalar) const{return RankedIndexedVector(this->values[0] * scalar, this->values[1] * scalar, this->values[2] * scalar, ILLEGAL_INDEX, ILLEGAL_RANK);}
    inline RankedIndexedVector operator/(coord_type scalar) const{return this->operator*(1/scalar);}

    friend inline std::ostream &operator<<(std::ostream &stream, const RankedIndexedVector &vec)
    {
        stream << "(" << vec.values[0] << ", " << vec.values[1] << ", " << vec.values[2] << ")";
        return stream;
    }

    friend inline std::istream &operator>>(std::istream &stream, RankedIndexedVector &point)
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
    inline rank_t getRank() const{return this->rank;}
    inline PointT getVector() const{return PointT(values[0], values[1], values[2]);}

    force_inline size_t dump(Serializer *serializer) const override
    {
        size_t bytes = 0;
        bytes += serializer->insert(this->values[0]);
        bytes += serializer->insert(this->values[1]);
        bytes += serializer->insert(this->values[2]);
        bytes += serializer->insert(this->index);
        bytes += serializer->insert(this->rank);
        return bytes;
    }

    force_inline size_t load(const Serializer *serializer, size_t byteOffset) override
    {
        size_t bytesRead = 0;
        bytesRead += serializer->extract(this->values[0], byteOffset);
        bytesRead += serializer->extract(this->values[1], byteOffset + bytesRead);
        bytesRead += serializer->extract(this->values[2], byteOffset + bytesRead);
        bytesRead += serializer->extract(this->index, byteOffset + bytesRead);
        bytesRead += serializer->extract(this->rank, byteOffset + bytesRead);
        return bytesRead;
    }
};

#endif // RICH_MPI

#endif // RANGE_FINDERS_RANKED_INDEXED_VECTOR_HPP
