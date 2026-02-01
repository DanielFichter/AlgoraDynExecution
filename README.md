# AlgoraDyn Execution
This CLI-tool enables users to execute fully dynamic single-source reachability algorithms from the AlgoraDyn library.

## Setup
Make sure you `ALGORACORE_ROOT_DIR` and `ALGORADYN_ROOT_DIR` to the root directory of your AlgoraCore and AlgoraDyn library, respectively. You can do this e.g. in your CMakeUserPresets.
Simply compile with CMake.

## Usage
To see all parameters, execute the binary with the `-h` parameter.
See the script file for an example

### Execution Modes
There are different execution modes. The default is `measurePerformance`, where the operation times are measured. `testCorrectness` compares the qeury results with the original `SimpleESTree` and `analyzeGraphs` outputs some statistics about the graphs.

### Random Graphs
To instantiate a random graph, you can pass `"random(n, m, o, a, r, f)"`, where n is the number of vertices, m is the number of arcs at timestamp 1, a is the proportion of arc additions, r is the proportion of arc removals and f is a multiplier for the other numbers.
