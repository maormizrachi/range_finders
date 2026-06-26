#ifndef RANGE_FINDER_HPP
#define RANGE_FINDER_HPP

#include <vector>
#include <limits>
#include <unordered_set>

template <typename PointT>
class RangeFinder
{
public:
    using coord_type = typename PointT::coord_type;

    template<typename T>
    using _set = std::unordered_set<T>;

    virtual ~RangeFinder() = default;

    virtual std::vector<size_t> range(const PointT &center, coord_type radius, size_t N = std::numeric_limits<size_t>::max(), const _set<size_t> &ignore = _set<size_t>()) const = 0;

    virtual std::vector<size_t> closestPointInSphere(const PointT &center, coord_type radius, const PointT &point, const _set<size_t> &ignore) const = 0;

    virtual const PointT &getPoint(size_t index) const = 0;

    virtual size_t size() const = 0;
};

#endif // RANGE_FINDER_HPP
