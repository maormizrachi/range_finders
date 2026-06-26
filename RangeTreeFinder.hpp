#ifndef RANGE_FINDERS_RANGE_TREE_FINDER_HPP
#define RANGE_FINDERS_RANGE_TREE_FINDER_HPP

#include <spatial_ds/BinaryTree.hpp>
#include <spatial_ds/RangeTree/RangeTree.hpp>
#include "utils/IndexedVector.hpp"
#include "RangeFinder.hpp"

#define RANGE_FINDERS_DIMENSIONS 3

template <typename PointT>
class RangeTreeFinder : public RangeFinder<PointT>
{
    using coord_type = typename PointT::coord_type;
    using IVec = IndexedVector<PointT>;

public:
    template<typename RandomAccessIterator>
    RangeTreeFinder(RandomAccessIterator first, RandomAccessIterator last);

    inline RangeTreeFinder(std::vector<PointT> &myPoints): RangeTreeFinder(myPoints.begin(), myPoints.end()){}

    inline ~RangeTreeFinder() override{delete this->rangeTree;}

    inline const PointT &getPoint(size_t index) const override{return this->myPoints[index];}

    inline std::vector<size_t> range(const PointT &center, coord_type radius, size_t N, const std::unordered_set<size_t> &ignore) const override
    {
        std::vector<size_t> toReturn;
        for(const IVec &vec : this->rangeTree->circularRange(center, radius, N, [&ignore](const IVec &vec){return ignore.find(vec.getIndex()) == ignore.cend();}))
        {
            toReturn.push_back(vec.index);
        }
        return toReturn;
    }

    std::vector<size_t> closestPointInSphere(const PointT &, coord_type, const PointT &, const std::unordered_set<size_t> &) const override
    {
        return std::vector<size_t>();
    }

    inline size_t size() const override{return this->rangeTree->size();}

private:
    std::vector<PointT> myPoints;
    RangeTree<IVec> *rangeTree;
};

template <typename PointT>
template<typename RandomAccessIterator>
inline RangeTreeFinder<PointT>::RangeTreeFinder(RandomAccessIterator first, RandomAccessIterator last)
{
    std::vector<IVec> data;
    size_t index = 0;

    myPoints.reserve(last - first);
    for(RandomAccessIterator it = first; it != last; it++)
    {
        const PointT &vec = *it;
        this->myPoints.push_back(vec);
        IVec idx_vec = IVec(vec.x, vec.y, vec.z, index);
        data.push_back(idx_vec);
        index++;
    }
    this->rangeTree = new RangeTree<IVec>(RANGE_FINDERS_DIMENSIONS);
    this->rangeTree->build(data.begin(), data.end());
}

#endif // RANGE_FINDERS_RANGE_TREE_FINDER_HPP
