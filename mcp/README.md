# lttb Helper MCP

This is a dependency-free stdio MCP server for the vcpkg `lttb` package. It exposes the package's small public API, edge-case behavior, CMake usage, and snippet generators for future C++ integrations.

Run it directly:

```sh
python3 /Users/josephantoon/Stuff/test-lttb/mcp/lttb_mcp.py
```

Register it with Codex:

```sh
codex mcp add lttb-helper -- python3 /Users/josephantoon/Stuff/test-lttb/mcp/lttb_mcp.py
```

Example MCP client configuration:

```json
{
  "mcpServers": {
    "lttb-helper": {
      "command": "python3",
      "args": [
        "/Users/josephantoon/Stuff/test-lttb/mcp/lttb_mcp.py"
      ]
    }
  }
}
```

Tools:

- `lttb_api_reference`: returns API, CMake, and edge-case notes.
- `lttb_generate_cmake`: returns manifest and CMake snippets for `lttb::lttb`.
- `lttb_generate_cpp`: returns C++ snippets for `LargestTriangleThreeBuckets<...>::Downsample`.
- `lttb_review_usage`: checks snippets for common lttb integration mistakes.

Resources:

- `lttb://api`
- `lttb://cmake`
- `lttb://edge-cases`

Prompt:

- `lttb_integration_checklist`
