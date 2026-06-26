#ifndef RANGE_FINDERS_GROUP_RANGE_TREE_FINDER_HPP
#define RANGE_FINDERS_GROUP_RANGE_TREE_FINDER_HPP

#include <spatial_ds/GroupRangeTree/GroupRangeTree.hpp>
#include "utils/IndexedVector.hpp"
#include "RangeFinder.hpp"

#define RANGE_FINDERS_DIMENSIONS 3

template <typename PointT, int GroupSize>
class GroupRangeTreeFinder : public RangeFinder<PointT>
{
    using coord_type = typename PointT::coord_type;
    using IVec = IndexedVector<PointT>;

public:
    template<typename RandomAccessIterator>
    GroupRangeTreeFinder(RandomAccessIterator first, RandomAccessIterator last);

    inline GroupRangeTreeFinder(std::vector<PointT> &myPoints): GroupRangeTreeFinder(myPoints.begin(), myPoints.end()){}

    inline ~GroupRangeTreeFinder() override{delete this->groupRangeTree;}

    inline const PointT &getPoint(size_t index) const override{return this->myPoints[index];}

    inline std::vector<size_t> range(const PointT &center, coord_type radius, size_t N, const std::unordered_set<size_t> &ignore) const override
    {
        std::vector<size_t> toReturn;
        for(const IVec &vec : this->groupRangeTree->circularRange(center, radius, N, [&ignore](const IVec &vec){return ignore.find(vec.getIndex()) == ignore.cend();}))
        {
            toReturn.push_back(vec.index);
        }
        return toReturn;
    }

    std::vector<size_t> closestPointInSphere(const PointT &, coord_type, const PointT &, const std::unordered_set<size_t> &) const override
    {
        return std::vector<size_t>();
    }

    inline size_t size() const override{return this->myPoints.size();}

private:
    std::vector<PointT> myPoints;
    GroupRangeTree<IVec, GroupSize> *groupRangeTree;
};

template <typename PointT, int GroupSize>
template<typename RandomAccessIterator>
inline GroupRangeTreeFinder<PointT, GroupSize>::GroupRangeTreeFinder(RandomAccessIterator first, RandomAccessIterator last)
{
    size_t index = 0;
    std::vector<IVec> data;
    for(RandomAccessIterator it = first; it != last; it++)
    {
        const PointT &vec = *it;
        this->myPoints.push_back(vec);
        IVec idx_vec = IVec(vec.x, vec.y, vec.z, index);
        data.push_back(idx_vec);
        index++;
    }
    this->groupRangeTree = new GroupRangeTree<IVec, GroupSize>(RANGE_FINDERS_DIMENSIONS);
    this->groupRangeTree->build(data.begin(), data.end());
}

#endif // RANGE_FINDERS_GROUP_RANGE_TREE_FINDER_HPP
