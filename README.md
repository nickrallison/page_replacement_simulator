# Page Replacement Simulator

## Overview
This project is a simulation of various page replacement algorithms used in operating systems. It implements and tests algorithms such as FIFO (First-In-First-Out), LRU (Least Recently Used), OPT (Optimal), and CLK (Clock). The simulator is designed to evaluate the performance of these algorithms by measuring page faults and write-backs under different scenarios.

## Features
- **Page Replacement Algorithms**: Supports FIFO, LRU, OPT, and CLK algorithms.
- **Simulation Metrics**: Tracks page faults and write-backs to evaluate algorithm performance.
- **Customizable Parameters**: Allows configuration of cache capacity, interrupt interval, and clock register size.
- **Test Cases**: Includes multiple test cases to validate the correctness of the implemented algorithms.

## Project Structure
The project is organized into the following directories:
- **src/scheduler**: Contains the implementation of the scheduling algorithms.
- **src/page**: Manages page records and related operations.
- **src/simulator**: Implements the core simulation logic.
- **src/executable**: Contains the main executable code.
- **src/test**: Includes test cases for validating the algorithms.
- **src/graphing**: (Optional) For generating performance graphs.

## Building the Project
To build the project, ensure you have CMake installed (version 3.15 or higher). Follow these steps:

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd CPSC457A2
   ```

2. Create a build directory and compile the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the executable:
   ```bash
   ./CPSC457A2
   ```

## Running Tests
The project includes a suite of test cases to validate the correctness of the implemented algorithms. To run the tests:

1. Navigate to the `build` directory:
   ```bash
   cd build
   ```

2. Run the test executable:
   ```bash
   ./test/CPSC457A2_test
   ```

## Usage
The simulator can be run with different scheduling algorithms by specifying the algorithm as a command-line argument. For example:

```bash
./CPSC457A2 FIFO < inputfile.csv
```

Supported algorithms:
- `FIFO`: First-In-First-Out
- `LRU`: Least Recently Used
- `OPT`: Optimal
- `CLK`: Clock

## Input Format
The input file should be in CSV format with the following structure:
```
Page#,dirty?
6,1
10,1
5,1
...
```
- `Page#`: The page number.
- `dirty?`: Indicates whether the page is dirty (1) or clean (0).

## Output
The simulator outputs the number of page faults and write-backs for the given input and algorithm. Example output:
```
Page Faults: 9
Write Backs: 8
```

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments
- This project was developed as part of a course assignment for CPSC 457.
- Special thanks to the teaching staff for their guidance and support.
