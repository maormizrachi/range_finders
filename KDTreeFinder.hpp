#ifndef RANGE_FINDERS_KD_TREE_FINDER_HPP
#define RANGE_FINDERS_KD_TREE_FINDER_HPP

#include <spatial_ds/KDTree/KDTree.hpp>
#include "utils/IndexedVector.hpp"
#include "RangeFinder.hpp"

#define RANGE_FINDERS_DIMENSIONS 3

template <typename PointT>
class KDTreeFinder : public RangeFinder<PointT>
{
    using coord_type = typename PointT::coord_type;
    using IVec = IndexedVector<PointT>;

public:
    template<typename RandomAccessIterator>
    KDTreeFinder(RandomAccessIterator first, RandomAccessIterator last, const PointT &ll, const PointT &ur);

    inline KDTreeFinder(std::vector<PointT> &myPoints, const PointT &ll, const PointT &ur): KDTreeFinder(myPoints.begin(), myPoints.end(), ll, ur){}

    inline ~KDTreeFinder() override{delete this->kdTree;}

    std::vector<size_t> closestPointInSphere(const PointT &center, coord_type radius, const PointT &point, const std::unordered_set<size_t> &ignore) const override
    {
        std::pair<IVec, coord_type> closestPointPair = this->kdTree->getClosestPointInSphere(Sphere<PointT>(center, radius), point,
                                                                                              [&ignore](const IVec &vec){return ignore.find(vec.getIndex()) == ignore.cend();});
        const IVec &closestPoint = closestPointPair.first;
        const coord_type &closestDistance = closestPointPair.second;

        if(closestDistance != std::numeric_limits<coord_type>::max())
            return std::vector<size_t>({closestPoint.index});
        return std::vector<size_t>();
    }

    inline const PointT &getPoint(size_t index) const override{return this->myPoints[index];}

    inline std::vector<size_t> range(const PointT &center, coord_type radius, size_t N, const std::unordered_set<size_t> &ignore) const override
    {
        std::vector<size_t> toReturn;
        for(const IVec &vec : this->kdTree->range(Sphere<PointT>(center, radius), N,
                                                  [&ignore](const IVec &vec){return ignore.find(vec.getIndex()) == ignore.cend();}))
        {
            toReturn.push_back(vec.index);
        }
        return toReturn;
    }

    inline size_t size() const override{return this->kdTree->getSize();}

private:
    std::vector<PointT> myPoints;
    KDTree<IVec, RANGE_FINDERS_DIMENSIONS> *kdTree;
};

template <typename PointT>
template<typename RandomAccessIterator>
inline KDTreeFinder<PointT>::KDTreeFinder(RandomAccessIterator first, RandomAccessIterator last, const PointT &ll, const PointT &ur)
{
    size_t index = 0;
    this->kdTree = new KDTree<IVec, RANGE_FINDERS_DIMENSIONS>(ll, ur);

    myPoints.reserve(last - first);
    for(RandomAccessIterator it = first; it != last; it++)
    {
        const PointT &vec = *it;
        IVec idx_vec = IVec(vec.x, vec.y, vec.z, index);
        this->myPoints.push_back(vec);
        this->kdTree->insert(idx_vec);
        index++;
    }
}

#endif // RANGE_FINDERS_KD_TREE_FINDER_HPP
