## Problem Description

Given a square map and multiple airplane flight paths, this program:
- Models each flight path as a rectangle with fixed width (50 units)
- Checks if these rectangles completely cover the entire map area
- Reports the location of any coverage gaps

## Algorithm Overview

The program uses a **scan-line algorithm** with **interval merging**:

1. **Rectangle Generation**: Converts each flight path (line segment) into a rectangle by extending 50 units perpendicular to both sides of the path
2. **Horizontal Scanning**: For each integer y-coordinate in the map, determines which x-intervals are covered by intersecting rectangles
3. **Coverage Verification**: Uses a greedy interval-merging algorithm to check if the x-intervals provide complete coverage from 0 to map_size
4. **Gap Detection**: Reports the first gap found, or confirms complete coverage

## Input Format

### File: `INPUT.txt` (or command line argument)
```
<map_size>
<number_of_planes>
<x0> <y0> <x1> <y1>
<x0> <y0> <x1> <y1>
...
```

**Parameters:**
- `map_size`: Integer (1-1000) - Side length of square map
- `number_of_planes`: Integer (1-100) - Number of flight paths
- `x0, y0, x1, y1`: Float coordinates defining flight path from (x0,y0) to (x1,y1)

### Example Input:
```
100
3
0.0 0.0 100.0 50.0
0.0 50.0 100.0 100.0
0.0 25.0 50.0 75.0
```

## Output Format

### File: `OUTPUT.txt`

**Complete Coverage:**
```
OK
```

**Gap Found:**
```
<gap_x> <gap_y>
```

Where `gap_x` and `gap_y` are the coordinates of the first uncovered point found.

**Error Cases:**
```
ERROR
```
(Invalid map size or number of planes)

## Compilation and Usage

This project includes a comprehensive Makefile for easy compilation and testing.

### Quick Start:
```bash
# Build the executable
make

# Run all tests
make test

# Get help with available commands
make help
```

### Manual Compilation:
```bash
gcc -Wall -Wextra -std=c99 task.c -o task -lm
```

### Available Make Targets:

| Target | Description |
|--------|-------------|
| `make` or `make all` | Build the executable |
| `make clean` | Remove compiled files and output |
| `make test` | Run program on all `INPUT_*.txt` files in `test/` directory |
| `make test-file FILE=path` | Test a specific input file |
| `make test-verbose` | Run tests with detailed input/output display |
| `make list-tests` | Show all available test files |
| `make setup-tests` | Create test directory structure |
| `make rebuild` | Clean and rebuild |
| `make help` | Show help message with all available targets |

### Usage Examples:

```bash
# Build and run all tests
make
make test

# Test a specific file
make test-file FILE=test/INPUT_1.txt

# Run with verbose output (shows rectangle calculations)
make test-verbose

# Set up testing environment
make setup-tests

# Manual execution
./task                    # Uses default INPUT.txt
./task custom_input.txt   # Uses specified file
```

### Test Organization:

The Makefile expects test files in the `test/` directory with naming pattern `INPUT_*.txt`:
```
test/
├── INPUT_1.txt
├── INPUT_2.txt
├── INPUT_coverage_test.txt
└── INPUT_edge_case.txt
```

Create the test directory structure:
```bash
make setup-tests
```

## Technical Details

### Data Structures

- **`plane_limits`**: Stores flight path endpoints
- **`rectangle`**: Represents coverage area with 4 corner points
- **`interval`**: Represents x-coverage range for scan-line algorithm
- **`point`**: 2D coordinate structure

### Key Functions

| Function | Purpose |
|----------|---------|
| `get_rectangle_from_line()` | Converts flight path to coverage rectangle |
| `calculate_x_interval_at_y()` | Finds x-coverage of rectangle at given y |
| `intervals_cover_range_with_gap()` | Checks if intervals cover [0, map_size] |
| `check_full_coverage_optimized()` | Main coverage verification algorithm |

### Algorithm Complexity

- **Time Complexity**: O(map_size × planes)
- **Space Complexity**: O(planes)

## Rectangle Generation Logic

Each flight path creates a rectangle as follows:

1. **Calculate direction vector**: `(dx, dy) = (x1-x0, y1-y0)`
2. **Normalize**: `(nx, ny) = (dx, dy) / length`
3. **Get perpendicular**: `(px, py) = (-ny, nx)`
4. **Create corners**:
   - A: `(x0 + px*50, y0 + py*50)`
   - B: `(x1 + px*50, y1 + py*50)`
   - C: `(x1 - px*50, y1 - py*50)`
   - D: `(x0 - px*50, y0 - py*50)`

## Coverage Detection Method

For each horizontal line at y-coordinate:

1. **Find intersecting rectangles**: Check which rectangles intersect this y-line
2. **Calculate x-intervals**: For each intersecting rectangle, compute the x-range it covers
3. **Sort intervals**: Order intervals by start position for greedy processing
4. **Check coverage**: Use greedy algorithm to verify continuous coverage from 0 to map_size
5. **Report gaps**: If any y-line has incomplete coverage, report the gap location

## Edge Cases Handled

- **Boundary coverage**: Gaps at map edges (reports `map_size + 0.001`)
- **Overlapping rectangles**: Correctly merges overlapping coverage areas
- **Horizontal flight paths**: Handles degenerate cases where paths are horizontal
- **Precision issues**: Uses epsilon values (1e-6) for floating-point comparisons

## Limitations

- Fixed rectangle width (50 units)
- Integer y-coordinate scanning (may miss sub-pixel gaps)
- Maximum 100 airplanes, 1000×1000 map size
- Assumes rectangular coverage areas (no curved paths)

## Example Scenarios

### Complete Coverage:
```
Map: 100×100
Planes: 
- (0,0) to (100,0)   → Rectangle covers y: [-50, 50]
- (0,50) to (100,50) → Rectangle covers y: [0, 100]
Result: OK (overlapping coverage ensures full map coverage)
```

### Gap Found:
```
Map: 100×100  
Planes:
- (0,0) to (50,0)    → Rectangle covers x: [0,50], y: [-50, 50]
- (60,0) to (100,0)  → Rectangle covers x: [60,100], y: [-50, 50]
Result: 50.000 25.000 (gap between x=50 and x=60)
```

## Project Structure

```
project/
├── task.c              # Main source code
├── Makefile           # Build and test automation
├── INPUT.txt          # Default input file (optional)
├── OUTPUT.txt         # Generated output file
├── test/              # Test directory
│   ├── INPUT_1.txt    # Test case 1
│   ├── INPUT_2.txt    # Test case 2
│   └── ...            # Additional test cases
└── README.md          # This file
```

## Dependencies

- Standard C libraries: `stdio.h`, `stdlib.h`, `math.h`, `stdbool.h`
- Math library: Automatically linked with `-lm` flag via Makefile
- Make utility for build automation

## Extra Notes
### GCC version
```bash
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1/lto-wrapper
Target: x86_64-pc-linux-gnu
Configured with: /tmp/pkg/src/gcc/configure --enable-languages=ada,c,c++,d,fortran,go,lto,m2,objc,obj-c++,rust,cobol --enable-bootstrap --prefix=/usr --libdir=/usr/lib --li
bexecdir=/usr/lib --mandir=/usr/share/man --infodir=/usr/share/info --with-bugurl=https://github.com/CachyOS/CachyOS-PKGBUILDS/issues --with-build-config=bootstrap-lto --wi
th-linker-hash-style=gnu --with-system-zlib --enable-__cxa_atexit --enable-cet=auto --enable-checking=release --enable-clocale=gnu --enable-default-pie --enable-default-ssp
 --enable-gnu-indirect-function --enable-gnu-unique-object --enable-libstdcxx-backtrace --enable-link-serialization=1 --enable-linker-build-id --enable-lto --enable-multili
b --enable-plugin --enable-shared --enable-threads=posix --disable-libssp --disable-libstdcxx-pch --disable-werror
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 15.1.1 20250425 (GCC) 
```
### OS
Arch linux
