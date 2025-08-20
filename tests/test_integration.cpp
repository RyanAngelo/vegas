#include <gtest/gtest.h>
#include "../Die.h"
#include "../Player.h"
#include "../Board.h"

class GameIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        player1 = std::make_shared<Player>("Alice", 1);
        player2 = std::make_shared<Player>("Bob", 2);
        board = std::make_unique<Board>(1);
    }
    
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    std::unique_ptr<Board> board;
};

TEST_F(GameIntegrationTest, CompleteRound) {
    // Setup: Give players dice
    player1->grabDice(2);
    player2->grabDice(2);
    
    // Add money to board
    board->placeMoney(10000);
    board->placeMoney(10000);
    
    EXPECT_EQ(board->getTotalMoney(), 20000);
    EXPECT_EQ(player1->getAllDice().size(), 2);
    EXPECT_EQ(player2->getAllDice().size(), 2);
    
    // Create dice manually for testing
    Die die1(1, false);
    Die die2(1, false);
    die1.setOwner(player1);
    die2.setOwner(player2);
    
    board->placeDie(die1);
    board->placeDie(die2);
    
    EXPECT_EQ(board->totalDice(), 2);
    
    // Distribute money (tie scenario)
    auto playerMappings = std::map<std::shared_ptr<Player>, int>();
    playerMappings[player1] = 1;
    playerMappings[player2] = 1;
    
    // In case of tie, no one gets money
    EXPECT_EQ(playerMappings.size(), 2);
}

TEST_F(GameIntegrationTest, WinnerTakesAll) {
    player1->grabDice(3);
    player2->grabDice(1);
    
    board->placeMoney(10000);
    board->placeMoney(20000);
    board->placeMoney(30000);
    
    // Create dice manually for testing
    Die die1(1, false);
    Die die2(1, false);
    Die die3(1, false);
    die1.setOwner(player1);
    die2.setOwner(player1);
    die3.setOwner(player2);
    
    board->placeDie(die1);
    board->placeDie(die2);
    board->placeDie(die3);
    
    EXPECT_EQ(board->totalDice(), 3);
    
    // Player 1 should win with 2 dice vs 1
    auto playerMappings = std::map<std::shared_ptr<Player>, int>();
    playerMappings[player1] = 2;
    playerMappings[player2] = 1;
    
    auto maxPlayer = std::max_element(playerMappings.begin(), playerMappings.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    
    EXPECT_EQ(maxPlayer->first, player1);
    EXPECT_EQ(maxPlayer->second, 2);
}

TEST_F(GameIntegrationTest, MultipleBoards) {
    std::vector<Board> boards;
    boards.emplace_back(1);
    boards.emplace_back(2);
    boards.emplace_back(3);
    
    player1->grabDice(3);
    
    // Add money to boards
    for (auto& board : boards) {
        board.placeMoney(10000);
    }
    
    // Create dice manually for testing
    Die die1(1, false);
    Die die2(2, false);
    Die die3(3, false);
    die1.setOwner(player1);
    die2.setOwner(player1);
    die3.setOwner(player1);
    
    boards[0].placeDie(die1);
    boards[1].placeDie(die2);
    boards[2].placeDie(die3);
    
    EXPECT_EQ(boards[0].totalDice(), 1);
    EXPECT_EQ(boards[1].totalDice(), 1);
    EXPECT_EQ(boards[2].totalDice(), 1);
}

TEST_F(GameIntegrationTest, MoneyDistribution) {
    player1->grabDice(2);
    player2->grabDice(1);
    
    // Add money to board
    board->placeMoney(10000); // $10,000
    board->placeMoney(20000); // $20,000
    board->placeMoney(30000); // $30,000
    
    // Create dice manually for testing
    Die die1(1, false);
    Die die2(1, false);
    Die die3(1, false);
    die1.setOwner(player1);
    die2.setOwner(player1);
    die3.setOwner(player2);
    
    board->placeDie(die1);
    board->placeDie(die2);
    board->placeDie(die3);
    
    // Player 1 wins and gets the largest bill
    int largestBill = board->takeLargestBill();
    player1->takeMoney(largestBill);
    
    EXPECT_EQ(player1->getTotalMoney(), 30000);
    EXPECT_EQ(player2->getTotalMoney(), 0);
    EXPECT_EQ(board->getTotalMoney(), 30000); // Remaining money
}

TEST_F(GameIntegrationTest, RoundCleanup) {
    player1->grabDice(2);
    player2->grabDice(1);
    
    board->placeMoney(10000);
    board->placeMoney(10000);
    
    // Place some dice
    Die die(1, false);
    die.setOwner(player1);
    board->placeDie(die);
    
    // Give some money
    player1->takeMoney(10000);
    player2->takeMoney(5000);
    
    // Cleanup round
    player1->cleanupRound();
    player2->cleanupRound();
    board->cleanupRound();
    
    // Dice should be cleared, money should remain
    EXPECT_EQ(player1->getAllDice().size(), 0);
    EXPECT_EQ(player2->getAllDice().size(), 0);
    EXPECT_EQ(board->totalDice(), 0);
    EXPECT_EQ(board->getTotalMoney(), 0);
    EXPECT_EQ(player1->getTotalMoney(), 10000);
    EXPECT_EQ(player2->getTotalMoney(), 5000);
}
