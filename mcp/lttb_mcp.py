#!/usr/bin/env python3
"""Dependency-free MCP server for the vcpkg lttb C++ package."""

from __future__ import annotations

import json
import re
import sys
from typing import Any


SERVER_NAME = "lttb-helper"
SERVER_VERSION = "0.1.0"
PROTOCOL_VERSION = "2024-11-05"


API_REFERENCE = """# lttb API reference

Package: lttb
Header: <lttb.hpp>
CMake package: lttb
CMake imported target: lttb::lttb

The installed vcpkg package is header-only. Its public C++ API is one class
template:

template <typename TPoint, typename TData, TData TPoint::*x, TData TPoint::*y>
struct LargestTriangleThreeBuckets

The template parameters are:
- TPoint: the point struct/class stored in the input and output ranges.
- TData: the numeric coordinate type, normally float, double, or long double.
- x: pointer to the x/time member on TPoint.
- y: pointer to the y/value member on TPoint.

The struct exposes one public function:

template <typename InputIt, typename OutputIt>
static void Downsample(InputIt source,
                       size_t sourceSize,
                       OutputIt destination,
                       size_t destinationSize);

InputIt must support random access with source[index]. OutputIt can be a raw
pointer, a pre-sized vector iterator, or an inserter such as std::back_inserter.
The function writes points to destination and does not return a container.
"""


EDGE_CASES = """# lttb Downsample behavior

Downsample(source, sourceSize, destination, destinationSize) has these edge
behaviors in the installed header and upstream tests:

- destinationSize == 0: writes nothing.
- sourceSize == 0: writes nothing.
- destinationSize >= sourceSize: copies all source points.
- destinationSize == 1: writes only source[0].
- destinationSize == 2: writes source[0] and source[sourceSize - 1].
- destinationSize > 2 and destinationSize < sourceSize: runs the LTTB bucket
  algorithm and always preserves the first and last source points.

Practical constraints:

- The source iterator must be random-access because the implementation uses
  source[index].
- The output range must have capacity for destinationSize writes unless an
  output inserter is used.
- TData is used for coordinate arithmetic and bucket averages, so prefer
  floating-point types for time-series visualization.
"""


CMAKE_USAGE = """# vcpkg/CMake usage

vcpkg manifest dependency:

{
  "dependencies": [
    "lttb"
  ]
}

CMake:

find_package(lttb CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE lttb::lttb)

Include:

#include <lttb.hpp>
"""


RESOURCES = {
    "lttb://api": {
        "name": "lttb API reference",
        "mimeType": "text/markdown",
        "text": API_REFERENCE,
    },
    "lttb://cmake": {
        "name": "lttb vcpkg CMake usage",
        "mimeType": "text/markdown",
        "text": CMAKE_USAGE,
    },
    "lttb://edge-cases": {
        "name": "lttb Downsample edge cases",
        "mimeType": "text/markdown",
        "text": EDGE_CASES,
    },
}


def text_result(text: str, is_error: bool = False) -> dict[str, Any]:
    return {"content": [{"type": "text", "text": text}], "isError": is_error}


def section_text(section: str) -> str:
    sections = {
        "overview": API_REFERENCE,
        "cmake": CMAKE_USAGE,
        "downsample": API_REFERENCE,
        "edge_cases": EDGE_CASES,
        "all": "\n\n".join([API_REFERENCE, CMAKE_USAGE, EDGE_CASES]),
    }
    return sections.get(section, sections["all"])


def identifier(value: str, fallback: str) -> str:
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", value or ""):
        return value
    return fallback


def cmake_target_name(value: str, fallback: str) -> str:
    if re.fullmatch(r"[A-Za-z0-9_.:+-]+", value or ""):
        return value
    return fallback


def positive_int(value: Any, fallback: int) -> int:
    try:
        parsed = int(value)
    except (TypeError, ValueError):
        return fallback
    return parsed if parsed > 0 else fallback


def cmake_snippet(target_name: str, include_manifest: bool) -> str:
    target = cmake_target_name(target_name, "your_target")
    pieces = []
    if include_manifest:
        pieces.append(
            """vcpkg.json:

```json
{
  "dependencies": [
    "lttb"
  ]
}
```"""
        )
    pieces.append(
        f"""CMakeLists.txt:

```cmake
find_package(lttb CONFIG REQUIRED)
target_link_libraries({target} PRIVATE lttb::lttb)
```"""
    )
    return "\n\n".join(pieces)


def cpp_snippet(args: dict[str, Any]) -> str:
    point_type = identifier(str(args.get("point_type", "Point")), "Point")
    numeric_type = str(args.get("numeric_type", "double") or "double")
    x_member = identifier(str(args.get("x_member", "x")), "x")
    y_member = identifier(str(args.get("y_member", "y")), "y")
    source_name = identifier(str(args.get("source_name", "data")), "data")
    output_name = identifier(str(args.get("output_name", "downsampled")), "downsampled")
    target_size = positive_int(args.get("target_size", 10), 10)
    strategy = str(args.get("destination_strategy", "preallocated_vector"))

    using_line = (
        f"using PointLttb = LargestTriangleThreeBuckets<{point_type}, "
        f"{numeric_type}, &{point_type}::{x_member}, &{point_type}::{y_member}>;"
    )

    if strategy == "array":
        destination = (
            f"std::array<{point_type}, {target_size}> {output_name}{{}};\n"
            f"PointLttb::Downsample({source_name}.data(), {source_name}.size(), "
            f"{output_name}.data(), {output_name}.size());"
        )
    elif strategy == "back_inserter":
        destination = (
            f"std::vector<{point_type}> {output_name};\n"
            f"{output_name}.reserve({target_size});\n"
            f"PointLttb::Downsample({source_name}.begin(), {source_name}.size(), "
            f"std::back_inserter({output_name}), {target_size});"
        )
    else:
        destination = (
            f"std::vector<{point_type}> {output_name}({target_size});\n"
            f"PointLttb::Downsample({source_name}.begin(), {source_name}.size(), "
            f"{output_name}.begin(), {output_name}.size());"
        )

    return f"""```cpp
#include <algorithm>
#include <array>
#include <iterator>
#include <vector>

#include <lttb.hpp>

struct {point_type} {{
    {numeric_type} {x_member};
    {numeric_type} {y_member};
}};

{using_line}

// {source_name} must be a random-access container of {point_type}.
{destination}
```"""


def review_usage(code: str) -> str:
    findings: list[str] = []

    if "<lttb.hpp>" not in code and '"lttb.hpp"' not in code:
        findings.append("Missing `#include <lttb.hpp>`.")
    if "LargestTriangleThreeBuckets" not in code:
        findings.append("No `LargestTriangleThreeBuckets<...>` alias or use found.")
    if "::Downsample" not in code:
        findings.append("No call to `Downsample(...)` found.")
    if "find_package(lttb CONFIG REQUIRED)" not in code and "lttb::lttb" not in code:
        findings.append("CMake should use `find_package(lttb CONFIG REQUIRED)` and link `lttb::lttb`.")
    if "std::list" in code:
        findings.append("Avoid `std::list` for the source range; lttb indexes with `source[index]`.")
    if re.search(r"Downsample\([^,]+,\s*[^,]+,\s*\w+\.begin\(\),\s*\d+\s*\)", code):
        findings.append("If writing to `.begin()`, make sure the destination container is pre-sized.")

    if not findings:
        return "No obvious lttb usage issues found. Verify that the source iterator is random-access and the destination can receive `destinationSize` points."

    return "Potential lttb usage issues:\n\n" + "\n".join(f"- {finding}" for finding in findings)


TOOLS = [
    {
        "name": "lttb_api_reference",
        "description": "Return notes about the API exposed by vcpkg's header-only lttb package.",
        "inputSchema": {
            "type": "object",
            "properties": {
                "section": {
                    "type": "string",
                    "enum": ["overview", "cmake", "downsample", "edge_cases", "all"],
                    "default": "all",
                }
            },
            "additionalProperties": False,
        },
    },
    {
        "name": "lttb_generate_cmake",
        "description": "Generate vcpkg manifest and CMake snippets for linking lttb.",
        "inputSchema": {
            "type": "object",
            "properties": {
                "target_name": {"type": "string", "default": "your_target"},
                "include_manifest": {"type": "boolean", "default": True},
            },
            "additionalProperties": False,
        },
    },
    {
        "name": "lttb_generate_cpp",
        "description": "Generate a C++ snippet that instantiates LargestTriangleThreeBuckets and calls Downsample.",
        "inputSchema": {
            "type": "object",
            "properties": {
                "point_type": {"type": "string", "default": "Point"},
                "numeric_type": {"type": "string", "default": "double"},
                "x_member": {"type": "string", "default": "x"},
                "y_member": {"type": "string", "default": "y"},
                "source_name": {"type": "string", "default": "data"},
                "output_name": {"type": "string", "default": "downsampled"},
                "target_size": {"type": "integer", "default": 10, "minimum": 1},
                "destination_strategy": {
                    "type": "string",
                    "enum": ["preallocated_vector", "back_inserter", "array"],
                    "default": "preallocated_vector",
                },
            },
            "additionalProperties": False,
        },
    },
    {
        "name": "lttb_review_usage",
        "description": "Check a C++/CMake snippet for common lttb integration mistakes.",
        "inputSchema": {
            "type": "object",
            "properties": {"code": {"type": "string"}},
            "required": ["code"],
            "additionalProperties": False,
        },
    },
]


PROMPTS = [
    {
        "name": "lttb_integration_checklist",
        "description": "Prompt for reviewing or adding lttb downsampling to a C++ target.",
        "arguments": [
            {
                "name": "target_name",
                "description": "CMake target name to integrate with lttb.",
                "required": False,
            }
        ],
    }
]


def handle_request(message: dict[str, Any]) -> dict[str, Any] | None:
    method = message.get("method")
    request_id = message.get("id")
    params = message.get("params") or {}

    if method == "initialize":
        result = {
            "protocolVersion": PROTOCOL_VERSION,
            "capabilities": {"tools": {}, "resources": {}, "prompts": {}},
            "serverInfo": {"name": SERVER_NAME, "version": SERVER_VERSION},
        }
    elif method == "tools/list":
        result = {"tools": TOOLS}
    elif method == "tools/call":
        name = params.get("name")
        args = params.get("arguments") or {}
        if name == "lttb_api_reference":
            result = text_result(section_text(str(args.get("section", "all"))))
        elif name == "lttb_generate_cmake":
            result = text_result(
                cmake_snippet(
                    str(args.get("target_name", "your_target")),
                    bool(args.get("include_manifest", True)),
                )
            )
        elif name == "lttb_generate_cpp":
            result = text_result(cpp_snippet(args))
        elif name == "lttb_review_usage":
            result = text_result(review_usage(str(args.get("code", ""))))
        else:
            return error_response(request_id, -32602, f"Unknown tool: {name}")
    elif method == "resources/list":
        result = {
            "resources": [
                {"uri": uri, "name": data["name"], "mimeType": data["mimeType"]}
                for uri, data in RESOURCES.items()
            ]
        }
    elif method == "resources/read":
        uri = params.get("uri")
        resource = RESOURCES.get(uri)
        if resource is None:
            return error_response(request_id, -32602, f"Unknown resource: {uri}")
        result = {
            "contents": [
                {
                    "uri": uri,
                    "mimeType": resource["mimeType"],
                    "text": resource["text"],
                }
            ]
        }
    elif method == "prompts/list":
        result = {"prompts": PROMPTS}
    elif method == "prompts/get":
        name = params.get("name")
        args = params.get("arguments") or {}
        if name != "lttb_integration_checklist":
            return error_response(request_id, -32602, f"Unknown prompt: {name}")
        target = identifier(str(args.get("target_name", "the target")), "the_target")
        result = {
            "description": "Checklist for lttb integration",
            "messages": [
                {
                    "role": "user",
                    "content": {
                        "type": "text",
                        "text": (
                            f"Review or add lttb integration for `{target}`. Confirm the vcpkg "
                            "manifest contains `lttb`, CMake links `lttb::lttb`, the source includes "
                            "`<lttb.hpp>`, the point type has numeric x/y members, the source range is "
                            "random-access, and the destination can receive the requested sample count."
                        ),
                    },
                }
            ],
        }
    elif method and method.startswith("notifications/"):
        return None
    else:
        return error_response(request_id, -32601, f"Method not found: {method}")

    if request_id is None:
        return None
    return {"jsonrpc": "2.0", "id": request_id, "result": result}


def error_response(request_id: Any, code: int, message: str) -> dict[str, Any]:
    return {"jsonrpc": "2.0", "id": request_id, "error": {"code": code, "message": message}}


def write_json_line(response: dict[str, Any]) -> None:
    sys.stdout.write(json.dumps(response, separators=(",", ":")) + "\n")
    sys.stdout.flush()


def write_framed_json(response: dict[str, Any]) -> None:
    body = json.dumps(response, separators=(",", ":")).encode("utf-8")
    sys.stdout.buffer.write(f"Content-Length: {len(body)}\r\n\r\n".encode("ascii"))
    sys.stdout.buffer.write(body)
    sys.stdout.buffer.flush()


def handle_message_bytes(body: bytes) -> dict[str, Any] | None:
    try:
        message = json.loads(body.decode("utf-8"))
        return handle_request(message)
    except Exception as exc:  # Keep server errors visible to MCP clients.
        return error_response(None, -32603, str(exc))


def run_framed_stdio(first_header: bytes) -> int:
    header = first_header
    while header:
        if not header.strip():
            header = sys.stdin.buffer.readline()
            continue

        content_length: int | None = None
        while header.strip():
            name, _, value = header.decode("ascii", errors="replace").partition(":")
            if name.lower() == "content-length":
                try:
                    content_length = int(value.strip())
                except ValueError:
                    response = error_response(None, -32700, "Invalid Content-Length header")
                    write_framed_json(response)
                    return 1
            header = sys.stdin.buffer.readline()

        if content_length is None:
            response = error_response(None, -32700, "Missing Content-Length header")
            write_framed_json(response)
            return 1

        body = sys.stdin.buffer.read(content_length)
        if len(body) != content_length:
            response = error_response(None, -32700, "Incomplete message body")
            write_framed_json(response)
            return 1

        response = handle_message_bytes(body)
        if response is not None:
            write_framed_json(response)

        header = sys.stdin.buffer.readline()
    return 0


def main() -> int:
    first_line = sys.stdin.buffer.readline()
    if first_line.lower().startswith(b"content-length:"):
        return run_framed_stdio(first_line)

    line = first_line
    while line:
        if line.strip():
            response = handle_message_bytes(line)
            if response is not None:
                write_json_line(response)
        line = sys.stdin.buffer.readline()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
