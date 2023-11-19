#include "doctest.h"
#include "../include/lttb.hpp"
#include <algorithm>
#include <vector>

template<typename T>
struct EmptySample
{
    T a;
    T b;
};

template<typename T>
using PointLttb = LargestTriangleThreeBuckets<EmptySample<T>, T, &EmptySample<T>::a, &EmptySample<T>::b>;

TEST_CASE("Downsample does not copy anything to empty destination containers (array, float)") {
    EmptySample<float> source[7501];
    EmptySample<float> actual[500];
    std::fill_n(source, 7501, EmptySample<float> { 3.1459, 9.876543 } );
    std::fill_n(actual, 500, EmptySample<float> { 42, 1.234567 } );

    PointLttb<float>::Downsample(source, 7501, actual, 0);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not copy anything to empty destination containers (array, double)") {
    EmptySample<double> source[7501];
    EmptySample<double> actual[500];
    std::fill_n(source, 7501, EmptySample<double> { 3.1459, 9.876543 } );
    std::fill_n(actual, 500, EmptySample<double> { 42, 1.234567 } );

    PointLttb<double>::Downsample(source, 7501, actual, 0);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not copy anything to empty destination containers (array, long double)") {
    EmptySample<long double> source[7501];
    EmptySample<long double> actual[500];
    std::fill_n(source, 7501, EmptySample<long double> { 3.1459, 9.876543 } );
    std::fill_n(actual, 500, EmptySample<long double> { 42, 1.234567 } );

    PointLttb<long double>::Downsample(source, 7501, actual, 0);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not copy anything to empty destination containers (std::vector, float)") {
    std::vector<EmptySample<float>> source(7501, EmptySample<float> { 3.1459, 9.876543 });
    std::vector<EmptySample<float>> actual;

    PointLttb<float>::Downsample(source.begin(), source.size(), std::back_inserter(actual), 0);
    REQUIRE(actual.size() == 0);
}

TEST_CASE("Downsample does not copy anything to empty destination containers (std::vector, double)") {
    std::vector<EmptySample<double>> source(7501, EmptySample<double> { 3.1459, 9.876543 });
    std::vector<EmptySample<double>> actual;

    PointLttb<double>::Downsample(source.begin(), source.size(), std::back_inserter(actual), 0);
    REQUIRE(actual.size() == 0);
}

TEST_CASE("Downsample does not copy anything to empty destination containers (std::vector, long double)") {
    std::vector<EmptySample<long double>> source(7501, EmptySample<long double> { 3.1459, 9.876543 });
    std::vector<EmptySample<long double>> actual;

    PointLttb<long double>::Downsample(source.begin(), source.size(), std::back_inserter(actual), 0);
    REQUIRE(actual.size() == 0);
}

TEST_CASE("Downsample does not do anything if source container is empty (array, float)") {
    EmptySample<float> source[7501];
    EmptySample<float> actual[500];
    std::fill_n(source, 7501, EmptySample<float> { 3.1459, 9.876543 } );
    std::fill_n(actual, 500, EmptySample<float> { 42, 1.234567 } );

    PointLttb<float>::Downsample(source, 0, actual, 500);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not do anything if source container is empty (array, double)") {
    EmptySample<double> source[7501];
    EmptySample<double> actual[500];
    std::fill_n(source, 7501, EmptySample<double> { 3.1459, 9.876543 } );
    std::fill_n(actual, 500, EmptySample<double> { 42, 1.234567 } );

    PointLttb<double>::Downsample(source, 0, actual, 500);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not do anything if source container is empty (array, long double)") {
    EmptySample<long double> source[7501];
    EmptySample<long double> actual[500];
    std::fill_n(source, 7501, EmptySample<long double> { 3.1459, 9.876543 } );
    std::fill_n(actual, 500, EmptySample<long double> { 42, 1.234567 } );

    PointLttb<long double>::Downsample(source, 0, actual, 500);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not do anything if source container is empty (std::vector, float)") {
    std::vector<EmptySample<float>> source;
    std::vector<EmptySample<float>> actual(500, EmptySample<float> { 42, 1.234567 });

    PointLttb<float>::Downsample(source.begin(), 0, std::back_inserter(actual), 500);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not do anything if source container is empty (std::vector, double)") {
    std::vector<EmptySample<double>> source;
    std::vector<EmptySample<double>> actual(500, EmptySample<double> { 42, 1.234567 });

    PointLttb<double>::Downsample(source.begin(), 0, std::back_inserter(actual), 500);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}

TEST_CASE("Downsample does not do anything if source container is empty (std::vector, long double)") {
    std::vector<EmptySample<long double>> source;
    std::vector<EmptySample<long double>> actual(500, EmptySample<long double> { 42, 1.234567 });

    PointLttb<long double>::Downsample(source.begin(), 0, std::back_inserter(actual), 500);
    for (int i = 0; i < 500; ++i) {
        REQUIRE(actual[i].a == doctest::Approx(42));
        REQUIRE(actual[i].b == doctest::Approx(1.234567));
    }
}