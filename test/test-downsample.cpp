#include "doctest.h"
#include "../include/lttb.hpp"
#include <stdio.h>
#include <vector>

template<typename T>
struct Sample
{
    T a;
    T b;
};

template<typename T>
using PointLttb = LargestTriangleThreeBuckets<Sample<T>, T, &Sample<T>::a, &Sample<T>::b>;

template <typename T> const char* DataFileTemplate();
template<> const char* DataFileTemplate<float>() { return "%f,%f\n"; }
template<> const char* DataFileTemplate<double>() { return "%lf,%lf\n"; }
template<> const char* DataFileTemplate<long double>() { return "%Lf,%Lf\n"; }

template<typename T>
void LoadDataFile(const char* path, Sample<T>* output, size_t count)
{
    FILE* file = fopen(path, "r");
    if (file == nullptr)
        throw std::runtime_error("unable to open file");
    std::unique_ptr<FILE, int(*)(FILE*)> fguard(file, fclose);

    for (int i = 0; i < count; ++i)
    {
        fscanf(file, DataFileTemplate<T>(), &output[i].a, &output[i].b);
        if (ferror(file))
            throw std::runtime_error("error reading file");
    }
}

constexpr size_t sourceSize = 7501;
constexpr size_t sampleSize = 500;

TEST_CASE("Downsample output matches javascript implementation (array, float)") {
    Sample<float> source[sourceSize] = { 0 };
    Sample<float> expected[sampleSize] = { 0 };
    Sample<float> actual[sampleSize] = { 0 };
    LoadDataFile("data/source.csv", source, sourceSize);
    LoadDataFile("data/sample.csv", expected, sampleSize);
    PointLttb<float>::Downsample(source, sourceSize, actual, sampleSize);

    for (int i = 0; i < sampleSize; ++i) {
        REQUIRE(expected[i].a == doctest::Approx(actual[i].a));
        REQUIRE(expected[i].b == doctest::Approx(actual[i].b));
    }
}

TEST_CASE("Downsample output matches javascript implementation (array, double)") {
    Sample<double> source[sourceSize] = { 0 };
    Sample<double> expected[sampleSize] = { 0 };
    Sample<double> actual[sampleSize] = { 0 };
    LoadDataFile("data/source.csv", source, sourceSize);
    LoadDataFile("data/sample.csv", expected, sampleSize);
    PointLttb<double>::Downsample(source, sourceSize, actual, sampleSize);

    for (int i = 0; i < sampleSize; ++i) {
        REQUIRE(expected[i].a == doctest::Approx(actual[i].a));
        REQUIRE(expected[i].b == doctest::Approx(actual[i].b));
    }
}

TEST_CASE("Downsample output matches javascript implementation (array, long double)") {
    Sample<long double> source[sourceSize] = { 0 };
    Sample<long double> expected[sampleSize] = { 0 };
    Sample<long double> actual[sampleSize] = { 0 };
    LoadDataFile("data/source.csv", source, sourceSize);
    LoadDataFile("data/sample.csv", expected, sampleSize);
    PointLttb<long double>::Downsample(source, sourceSize, actual, sampleSize);

    for (int i = 0; i < sampleSize; ++i) {
        REQUIRE(expected[i].a == doctest::Approx(actual[i].a));
        REQUIRE(expected[i].b == doctest::Approx(actual[i].b));
    }
}

TEST_CASE("Downsample output matches javascript implementation (std::vector, float)") {
    Sample<float> sourceArray[sourceSize] = { 0 };
    Sample<float> expectedArray[sampleSize] = { 0 };
    LoadDataFile("data/source.csv", sourceArray, sourceSize);
    LoadDataFile("data/sample.csv", expectedArray, sampleSize);

    std::vector<Sample<float>> source(sourceArray, sourceArray + sourceSize);
    std::vector<Sample<float>> expected(expectedArray, expectedArray + sampleSize);
    std::vector<Sample<float>> actual;
    PointLttb<float>::Downsample(
        source.begin(), 
        sourceSize, 
        std::back_inserter(actual), 
        sampleSize);

    REQUIRE(actual.size() == sampleSize);
    for (int i = 0; i < sampleSize; ++i) {
        REQUIRE(expected[i].a == doctest::Approx(actual[i].a));
        REQUIRE(expected[i].b == doctest::Approx(actual[i].b));
    }
}

TEST_CASE("Downsample output matches javascript implementation (std::vector, double)") {
    Sample<double> sourceArray[sourceSize] = { 0 };
    Sample<double> expectedArray[sampleSize] = { 0 };
    LoadDataFile("data/source.csv", sourceArray, sourceSize);
    LoadDataFile("data/sample.csv", expectedArray, sampleSize);

    std::vector<Sample<double>> source(sourceArray, sourceArray + sourceSize);
    std::vector<Sample<double>> expected(expectedArray, expectedArray + sampleSize);
    std::vector<Sample<double>> actual;
    PointLttb<double>::Downsample(
        source.begin(), 
        sourceSize, 
        std::back_inserter(actual), 
        sampleSize);

    REQUIRE(actual.size() == sampleSize);
    for (int i = 0; i < sampleSize; ++i) {
        REQUIRE(expected[i].a == doctest::Approx(actual[i].a));
        REQUIRE(expected[i].b == doctest::Approx(actual[i].b));
    }
}

TEST_CASE("Downsample output matches javascript implementation (std::vector, long double)") {
    Sample<long double> sourceArray[sourceSize] = { 0 };
    Sample<long double> expectedArray[sampleSize] = { 0 };
    LoadDataFile("data/source.csv", sourceArray, sourceSize);
    LoadDataFile("data/sample.csv", expectedArray, sampleSize);

    std::vector<Sample<long double>> source(sourceArray, sourceArray + sourceSize);
    std::vector<Sample<long double>> expected(expectedArray, expectedArray + sampleSize);
    std::vector<Sample<long double>> actual;
    PointLttb<long double>::Downsample(
        source.begin(), 
        sourceSize, 
        std::back_inserter(actual), 
        sampleSize);

    REQUIRE(actual.size() == sampleSize);
    for (int i = 0; i < sampleSize; ++i) {
        REQUIRE(expected[i].a == doctest::Approx(actual[i].a));
        REQUIRE(expected[i].b == doctest::Approx(actual[i].b));
    }
}