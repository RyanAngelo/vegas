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
- Standard library support

### Compilation
```bash
g++ -std=c++17 *.cc -o vegas
```

### Running the Game
```bash
./vegas
```

## License

This project is open source and available under the MIT License.
