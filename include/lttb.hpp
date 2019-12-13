#ifndef __LTTP_HPP__
#define __LTTP_HPP__

#include <cstddef>
#include <cmath>
#include <algorithm>

template <typename TPoint, typename TData, TData TPoint::*x, TData TPoint::*y>
struct LargestTriangleThreeBuckets
{
    template <typename InputIt, typename OutputIt>
    static void Downsample(InputIt source, size_t sourceSize, OutputIt destination, size_t destinationSize)
    {
        if (destinationSize == 0)
        {
            return;
        }

        if (destinationSize >= sourceSize)
        {
            std::copy_n(source, sourceSize, destination);
        }

        *destination = *source;
        ++destination;
        
        TData every = static_cast<TData>(sourceSize - 2) / (destinationSize - 2);
        size_t aIndex = 0;

        for (size_t i = 0; i < destinationSize - 2; ++i)
        {
            size_t avgRangeStart = static_cast<size_t>((i + 1) * every) + 1;
            size_t avgRangeEnd = static_cast<size_t>((i + 2) * every) + 1;
            if (avgRangeEnd > sourceSize)
            {
                avgRangeEnd = sourceSize;
            }

            TData avgRangeLength = avgRangeEnd - avgRangeStart;
            TData avgX = 0;
            TData avgY = 0;
            for (; avgRangeStart < avgRangeEnd; ++avgRangeStart)
            {
                avgX += source[avgRangeStart].*x;
                avgY += source[avgRangeStart].*y;
            }
            avgX /= avgRangeLength;
            avgY /= avgRangeLength;

            size_t rangeOffs = static_cast<size_t>(i * every) + 1;
            size_t rangeTo = static_cast<size_t>((i + 1) * every) + 1;
            TData pointAX = source[aIndex].*x;
            TData pointAY = source[aIndex].*y;

            TData maxArea = -1;
            size_t nextAIndex = 0;
            for (; rangeOffs < rangeTo; ++rangeOffs)
            {
                TData area = std::abs(
                    (pointAX - avgX) * (source[rangeOffs].*y - pointAY) -
                    (pointAX - source[rangeOffs].*x) * (avgY - pointAY)) / 2;
                if (area > maxArea) 
                {
                    maxArea = area;
                    nextAIndex = rangeOffs;
                }
            }

            *destination = source[nextAIndex];
            ++destination;
            aIndex = nextAIndex;
        }

        *destination = source[sourceSize-1];
        ++destination;
    }
};

#endif
