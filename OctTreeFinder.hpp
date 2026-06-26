#ifndef RANGE_FINDERS_OCT_TREE_FINDER_HPP
#define RANGE_FINDERS_OCT_TREE_FINDER_HPP

#include <spatial_ds/OctTree/OctTree.hpp>
#include "utils/IndexedVector.hpp"
#include "RangeFinder.hpp"

template <typename PointT>
class OctTreeFinder : public RangeFinder<PointT>
{
    using coord_type = typename PointT::coord_type;
    using IVec = IndexedVector<PointT>;

public:
    template<typename RandomAccessIterator>
    OctTreeFinder(RandomAccessIterator first, RandomAccessIterator last, const PointT &ll, const PointT &ur);

    inline OctTreeFinder(std::vector<PointT> &myPoints, const PointT &ll, const PointT &ur): OctTreeFinder(myPoints.begin(), myPoints.end(), ll, ur){}

    inline OctTreeFinder(OctTree<IVec> *tree, const std::vector<PointT> &myPoints): octTree(tree), myPoints(myPoints), givenOctTree(true){}

    inline ~OctTreeFinder() override
    {
        if(!this->givenOctTree)
            delete this->octTree;
    }

    std::vector<size_t> closestPointInSphere(const PointT &center, coord_type radius, const PointT &point, const std::unordered_set<size_t> &ignore) const override
    {
        std::pair<IVec, coord_type> closestPointPair = this->octTree->getClosestPointInSphere(Sphere<PointT>(center, radius), point,
                                                                                              [&ignore](const IVec &vec){return ignore.find(vec.getIndex()) == ignore.cend();});
        const IVec &closestPoint = closestPointPair.first;
        const coord_type &closestDistance = closestPointPair.second;

        if(closestDistance != std::numeric_limits<coord_type>::max())
            return std::vector<size_t>({closestPoint.index});
        return std::vector<size_t>();
    }

    inline std::vector<size_t> range(const PointT &center, coord_type radius, size_t N, const std::unordered_set<size_t> &ignore) const override
    {
        constexpr coord_type eps = static_cast<coord_type>(1e-12);
        std::vector<size_t> toReturn;
        for(const IVec &vec : this->octTree->range(Sphere<IVec>(IVec(center.x, center.y, center.z, ILLEGAL_IDX), radius + eps), N,
                                                   [&ignore](const IVec &vec){return ignore.find(vec.getIndex()) == ignore.cend();}))
        {
            toReturn.push_back(vec.index);
        }
        return toReturn;
    }

    inline size_t size() const override{return this->octTree->getSize();}

    inline const PointT &getPoint(size_t index) const override{return this->myPoints[index];}

private:
    std::vector<PointT> myPoints;
    OctTree<IVec> *octTree;
    bool givenOctTree = false;
};

template <typename PointT>
template<typename RandomAccessIterator>
inline OctTreeFinder<PointT>::OctTreeFinder(RandomAccessIterator first, RandomAccessIterator last, const PointT &ll, const PointT &ur)
{
    this->octTree = new OctTree<IVec>(ll, ur);
    this->givenOctTree = false;
    this->myPoints.reserve(last - first);
    size_t index = 0;
    for(RandomAccessIterator it = first; it != last; it++)
    {
        const PointT &vec = *it;
        this->myPoints.push_back(vec);
        IVec idx_vec = IVec(vec.x, vec.y, vec.z, index);
        this->octTree->insert(idx_vec);
        index++;
    }
}

#endif // RANGE_FINDERS_OCT_TREE_FINDER_HPP
