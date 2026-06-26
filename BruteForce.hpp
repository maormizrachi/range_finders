#ifndef RANGE_FINDERS_BRUTE_FORCE_HPP
#define RANGE_FINDERS_BRUTE_FORCE_HPP

#include "RangeFinder.hpp"

template <typename PointT>
class BruteForceFinder : public RangeFinder<PointT>
{
    using coord_type = typename PointT::coord_type;

public:
    template<typename RandomAccessIterator>
    BruteForceFinder(const RandomAccessIterator &first, const RandomAccessIterator &last):
                points(std::vector<PointT>()){this->points.insert(this->points.begin(), first, last); this->pointsSize = this->points.size();}

    BruteForceFinder(const std::vector<PointT> &points): BruteForceFinder(points.begin(), points.end()){}

    inline ~BruteForceFinder() = default;

    std::vector<size_t> closestPointInSphere(const PointT &center, coord_type radius, const PointT &point, const std::unordered_set<size_t> &ignore) const override
    {
        size_t closestSoFarIndex = std::numeric_limits<size_t>::max();
        coord_type closestSoFar = std::numeric_limits<coord_type>::max();
        constexpr coord_type eps = static_cast<coord_type>(1e-12);
        const PointT *_points = this->points.data();
        for(size_t i = 0; i < this->pointsSize; i++)
        {
            if((ignore.find(i) != ignore.cend()))
                continue;
            const PointT &_point = _points[i];
            coord_type sphere_dx = _point.x - center.x;
            coord_type sphere_dy = _point.y - center.y;
            coord_type sphere_dz = _point.z - center.z;
            coord_type dx = point.x - center.x;
            coord_type dy = point.y - center.y;
            coord_type dz = point.z - center.z;
            sphere_dx *= sphere_dx;
            sphere_dy *= sphere_dy;
            sphere_dz *= sphere_dz;
            dx *= dx;
            dy *= dy;
            dz *= dz;
            coord_type distanceToPoint = dx + dy + dz;
            coord_type distanceToSphere = sphere_dx + sphere_dy + sphere_dz;
            if((distanceToPoint < closestSoFar) && (distanceToSphere <= ((radius * radius) + eps)))
            {
                closestSoFar = distanceToPoint;
                closestSoFarIndex = i;
            }
        }

        std::vector<size_t> result;
        result.reserve(1);
        if(closestSoFarIndex != std::numeric_limits<size_t>::max())
            result.push_back(closestSoFarIndex);
        return result;
    }

    inline const PointT &getPoint(size_t index) const override{return this->points[index];}

    inline std::vector<size_t> range(const PointT &center, coord_type radius, size_t N, const std::unordered_set<size_t> &ignore) const override
    {
        std::vector<size_t> result;
        constexpr coord_type eps = static_cast<coord_type>(1e-12);
        const PointT *_points = this->points.data();
        for(size_t i = 0; i < this->pointsSize; i++)
        {
            if(result.size() >= N)
                break;
            if((ignore.find(i) != ignore.cend()))
                continue;
            const PointT &point = _points[i];
            coord_type dx = point.x - center.x;
            coord_type dy = point.y - center.y;
            coord_type dz = point.z - center.z;
            dx *= dx;
            dy *= dy;
            dz *= dz;
            if((dx + dy + dz) <= ((radius * radius) + eps))
                result.push_back(i);
        }
        return result;
    }

    inline size_t size() const override{return this->pointsSize;}

private:
    std::vector<PointT> points;
    size_t pointsSize;
};

#endif // RANGE_FINDERS_BRUTE_FORCE_HPP
