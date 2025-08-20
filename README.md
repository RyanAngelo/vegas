# Vegas Dice Game

A command-line C++ implementation of the board game "Las Vegas".

## Game Overview

Players compete to win the most money by placing dice on different casino boards.

### Components
- 6 Casino boards (numbered 1-6)
- Dice sets in different colors (one set per player)
- Money bills in various denominations ($10,000 - $90,000)

### Rules

1. **Setup**
   - Each player receives a set of dice
   - Money is randomly distributed among the six casino boards
   - Each board represents a different casino with its own prize pool

2. **Gameplay**
   - Players take turns rolling their available dice
   - After rolling, the player must place all dice of one number on the corresponding casino board
   - Once dice are placed, they cannot be moved
   - Players continue taking turns until all dice are placed

3. **Scoring**
   - After all dice are placed, money is awarded at each casino
   - The player with the most dice at a casino wins the highest value bill
   - In case of a tie, those players receive nothing
   - The player with the most money after all rounds wins the game

## Building and Running

### Prerequisites
- C++ compiler with C++17 support
- Google Test (for running tests)
- CMake 3.16 or higher (optional, for CMake builds)

### Installing Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential libgtest-dev cmake
```

#### macOS
```bash
brew install googletest cmake
```

#### Other Systems
Build Google Test from source: https://github.com/google/googletest

### Project Structure
```
vegas/
├── main.cc           # Main game entry point
├── Die.h/cc         # Die class
├── Board.h/cc       # Board class  
├── Player.h/cc      # Player class
├── tests/           # Unit tests
├── build/           # Build output directory
├── Makefile         # Portable Makefile
└── CMakeLists.txt   # CMake configuration
```

### Building with Make (Recommended)
The Makefile automatically detects Google Test installation:

```bash
# Build and run tests
make test

# Build main game
make all

# Run the game
./build/vegas

# Show Google Test configuration
make gtest-info

# Clean build files
make clean
```

### Building with CMake
```bash
# Create build directory
mkdir -p build
cd build

# Configure and build
cmake ..
make

# Run tests
ctest

# Run the game
./bin/vegas
```

### Manual Build (if auto-detection fails)
```bash
# Create build directory
mkdir -p build

# Compile the project
g++ -std=c++17 Board.cc Die.cc Player.cc main.cc -o build/vegas

# For tests (adjust paths as needed)
g++ -std=c++17 -I/path/to/gtest/include Board.cc Die.cc Player.cc tests/*.cpp -L/path/to/gtest/lib -lgtest -lgtest_main -pthread -o build/vegas_tests
```

## Testing

### Running Tests
```bash
# Build tests
cd build
make vegas_tests

# Run tests
./bin/vegas_tests

# Run tests with verbose output
./bin/vegas_tests --gtest_verbose
```

## License

This project is open source and available under the MIT License.
