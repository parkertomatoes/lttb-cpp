# C++ Implementation of the Largest Triangle Three Buckets (LTTB) Downsampling Algorithm

This is a straightforward C++ port of the [reference implementation](https://github.com/sveinn-steinarsson/flot-downsample) of the Largest Triangle Three Buckets (LTTB) downsampling algorithm described in the paper ["Downsampling time series for visual representation" by Sveinn Steinarsson](http://hdl.handle.net/1946/15343). It is a single header, with a single class template that allows using different structures and data types.

# How To Install
Simply add the ```lttb.hpp``` header to your project. There are no binaries to install, and no dependencies outside the standard library.

## How To Use
Create a typedef to specify your time series datapoint type
```c++
#include "lttb.hpp"

struct ExamplePoint
    float x;
    float y;
};

using PointLttb = LargestTriangleThreeBuckets<ExamplePoint, float, &ExamplePoint::x, &ExamplePoint::y>
```

Then use the static method ```Downsample``` in the class. It can be used with iterators
```c++
std::vector<ExamplePoint> in = GetYourInputsFromSomewhere();
std::vector<ExamplePoint> out;
result.reserve(50);
PointLttb::Downsample(in.begin(), in.size(), std::back_insert_iterator<std::vector<ExamplePoint>>(out), 50);
return result;
```

...or pointers:
```c++
ExamplePoint in[500];
GetYourInputsFromSomewhere(in);
ExamplePoint out[50];
PointLttb::Downsample(in, 500, out, 50);
```
