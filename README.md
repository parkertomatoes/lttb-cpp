# C++ Implementation of the Largest Triangle Three Buckets (LTTB) Downsampling Algorithm

This is a straightforward C++ port of the [reference implementation](https://github.com/sveinn-steinarsson/flot-downsample) of the Largest Triangle Three Buckets (LTTB) downsampling algorithm described in the paper ["Downsampling time series for visual representation" by Sveinn Steinarsson](http://hdl.handle.net/1946/15343). It is a single header, with a single class template that allows using different structures and data types.

## How To Install

This is a single-header library with no dependencies outside the C++ standard library, so there are many ways to incorporate it into your project:

### Installing Without A Tool 

Copy `include/lttb.hpp` into your project, make sure it's available from your compiler's include path, and include it from your source:

```c++
#include "lttb.hpp"
```

### Installing With vcpkg

In classic mode, install the `lttb` package:

```sh
vcpkg install lttb
```

In manifest mode, add `lttb` to your project's `vcpkg.json`:

```sh
vcpkg add port lttb
```

Then use the CMake package from your project in either mode:

```cmake
find_package(lttb CONFIG REQUIRED)
target_link_libraries(your-target PRIVATE lttb::lttb)
```

Include the header from your source:

```c++
#include "lttb.hpp"
```

### Installing With CMake

Clone this repository into your source tree, for example as a subrepo under `external/lttb-cpp`, then add it from your top-level `CMakeLists.txt`:

```cmake
add_subdirectory(external/lttb-cpp)
target_link_libraries(your-target PRIVATE lttb::lttb)
```

The `lttb::lttb` target adds the library's `include` directory and requires C++11.

Include the header from your source:

```c++
#include "lttb.hpp"
```

## How To Use With LLMs

This repository includes an optional MCP server that helps AI coding tools generate and review `lttb` integrations. The server is dependency-free and runs over stdio.

Clone the upstream repository if you do not already have it locally:

```sh
git clone https://github.com/parkertomatoes/lttb-cpp.git
```

### Registering with Codex:

```sh
codex mcp add lttb-helper -- python3 /path/to/lttb-cpp/mcp/lttb_mcp.py
```

### Registering with Claude Code:

```sh
claude mcp add --transport stdio lttb-helper -- python3 /path/to/lttb-cpp/mcp/lttb_mcp.py
```

To share the Claude Code MCP configuration with other contributors, add it with project scope from the repository root:

```sh
claude mcp add --transport stdio --scope project lttb-helper -- python3 ./mcp/lttb_mcp.py
```

### Other MCP Clients

Add a stdio server entry like this:

```json
{
  "mcpServers": {
    "lttb-helper": {
      "command": "python3",
      "args": [
        "/path/to/lttb-cpp/mcp/lttb_mcp.py"
      ]
    }
  }
}
```

The MCP exposes helpers for API reference, CMake snippets, C++ usage snippets, and common integration checks.

## How To Use
Create a typedef to specify your time series datapoint type
```c++
#include "lttb.hpp"

struct ExamplePoint {
    float x;
    float y;
};

using PointLttb = LargestTriangleThreeBuckets<ExamplePoint, float, &ExamplePoint::x, &ExamplePoint::y>
```

Then use the static method ```Downsample``` in the class. It can be used with iterators
```c++
std::vector<ExamplePoint> in = GetYourInputsFromSomewhere();
std::vector<ExamplePoint> out;
PointLttb::Downsample(in.begin(), in.size(), std::back_inserter(out), 50);
```

...or pointers:
```c++
ExamplePoint in[500];
GetYourInputsFromSomewhere(in);
ExamplePoint out[50];
PointLttb::Downsample(in, 500, out, 50);
```
