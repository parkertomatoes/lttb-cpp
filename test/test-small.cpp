#include "doctest.h"
#include "../include/lttb.hpp"
#include <algorithm>
#include <vector>

template<typename T>
struct SmallSample
{
    T a;
    T b;
};

template<typename T>
using PointLttb = LargestTriangleThreeBuckets<SmallSample<T>, T, &SmallSample<T>::a, &SmallSample<T>::b>;

TEST_CASE("Downsample copies first and last point into a size-two destination (array, float)") {
    SmallSample<float> source[6] = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    SmallSample<float> actual[2];

    PointLttb<float>::Downsample(source, 6, actual, 2);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
    REQUIRE(actual[1].a == doctest::Approx(5));
    REQUIRE(actual[1].b == doctest::Approx(12.0));
}

TEST_CASE("Downsample copies first and last point into a size-two destination (array, double)") {
    SmallSample<double> source[6] = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    SmallSample<double> actual[2];

    PointLttb<double>::Downsample(source, 6, actual, 2);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
    REQUIRE(actual[1].a == doctest::Approx(5));
    REQUIRE(actual[1].b == doctest::Approx(12.0));
}

TEST_CASE("Downsample copies first and last point into a size-two destination (array, long double)") {
    SmallSample<long double> source[6] = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    SmallSample<long double> actual[2];

    PointLttb<long double>::Downsample(source, 6, actual, 2);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
    REQUIRE(actual[1].a == doctest::Approx(5));
    REQUIRE(actual[1].b == doctest::Approx(12.0));
}

TEST_CASE("Downsample copies first and last point into a size-two destination (std::vector, float)") {
    std::vector<SmallSample<float>> source = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    std::vector<SmallSample<float>> actual;

    PointLttb<float>::Downsample(source.begin(), source.size(), std::back_inserter(actual), 2);
    REQUIRE(actual.size() == 2);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
    REQUIRE(actual[1].a == doctest::Approx(5));
    REQUIRE(actual[1].b == doctest::Approx(12.0));
}

TEST_CASE("Downsample copies first and last point into a size-two destination (std::vector, double)") {
    std::vector<SmallSample<double>> source = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    std::vector<SmallSample<double>> actual;

    PointLttb<double>::Downsample(source.begin(), source.size(), std::back_inserter(actual), 2);
    REQUIRE(actual.size() == 2);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
    REQUIRE(actual[1].a == doctest::Approx(5));
    REQUIRE(actual[1].b == doctest::Approx(12.0));
}

TEST_CASE("Downsample copies first and last point into a size-two destination (std::vector, long double)") {
    std::vector<SmallSample<long double>> source = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    std::vector<SmallSample<long double>> actual;

    PointLttb<long double>::Downsample(source.begin(), source.size(), std::back_inserter(actual), 2);
    REQUIRE(actual.size() == 2);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
    REQUIRE(actual[1].a == doctest::Approx(5));
    REQUIRE(actual[1].b == doctest::Approx(12.0));
}

TEST_CASE("Downsample copies only the first point into a size-one destination (array, float)") {
    SmallSample<float> source[6] = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    SmallSample<float> actual[1];

    PointLttb<float>::Downsample(source, 6, actual, 1);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
}

TEST_CASE("Downsample copies only the first point into a size-one destination (array, double)") {
    SmallSample<double> source[6] = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    SmallSample<double> actual[1];

    PointLttb<double>::Downsample(source, 6, actual, 1);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
}

TEST_CASE("Downsample copies only the first point into a size-one destination (array, long double)") {
    SmallSample<long double> source[6] = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    SmallSample<long double> actual[1];

    PointLttb<long double>::Downsample(source, 6, actual, 1);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
}

TEST_CASE("Downsample copies only the first point into a size-one destination (std::vector, float)") {
    std::vector<SmallSample<float>> source = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    std::vector<SmallSample<float>> actual;

    PointLttb<float>::Downsample(source.begin(), 6, std::back_inserter(actual), 1);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
}

TEST_CASE("Downsample copies only the first point into a size-one destination (std::vector, double)") {
    std::vector<SmallSample<double>> source = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    std::vector<SmallSample<double>> actual;

    PointLttb<double>::Downsample(source.begin(), 6, std::back_inserter(actual), 1);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
}

TEST_CASE("Downsample copies only the first point into a size-one destination (std::vector, long double)") {
    std::vector<SmallSample<long double>> source = {
        { 0, 82.0 },
        { 1, 1082.0 },
        { 2, 2085.0 },
        { 3, 8192.0 },
        { 4, 16386.0 },
        { 5, 12.0 }
    };
    std::vector<SmallSample<long double>> actual;

    PointLttb<long double>::Downsample(source.begin(), 6, std::back_inserter(actual), 1);
    REQUIRE(actual[0].a == doctest::Approx(0));
    REQUIRE(actual[0].b == doctest::Approx(82.0));
}


