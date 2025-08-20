#include <gtest/gtest.h>
#include "../Board.h"
#include "../Die.h"
#include "../Player.h"

class BoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        board = std::make_unique<Board>(1);
        player = std::make_shared<Player>("TestPlayer", 1);
    }
    
    std::unique_ptr<Board> board;
    std::shared_ptr<Player> player;
};

TEST_F(BoardTest, DefaultConstructor) {
    Board defaultBoard;
    EXPECT_EQ(defaultBoard.getBoardId(), 0);
    EXPECT_EQ(defaultBoard.totalDice(), 0);
    EXPECT_EQ(defaultBoard.getTotalMoney(), 0);
}

TEST_F(BoardTest, ParameterizedConstructor) {
    Board testBoard(5);
    EXPECT_EQ(testBoard.getBoardId(), 5);
    EXPECT_EQ(testBoard.totalDice(), 0);
    EXPECT_EQ(testBoard.getTotalMoney(), 0);
}

TEST_F(BoardTest, PlaceDie) {
    Die die(3, false);
    die.setOwner(player);
    
    int diceCount = board->placeDie(die);
    EXPECT_EQ(diceCount, 1);
    EXPECT_EQ(board->totalDice(), 1);
    EXPECT_TRUE(die.isPlayed());
    
    const auto& dice = board->getDice();
    EXPECT_EQ(dice.size(), 1);
    EXPECT_EQ(dice[0].getValue(), 3);
    EXPECT_EQ(dice[0].getOwner(), player);
}

TEST_F(BoardTest, PlaceMoney) {
    board->placeMoney(10000);
    EXPECT_EQ(board->getTotalMoney(), 10000);
    
    board->placeMoney(10000);
    EXPECT_EQ(board->getTotalMoney(), 20000);
}

TEST_F(BoardTest, GetTotalMoney) {
    EXPECT_EQ(board->getTotalMoney(), 0);
    
    board->placeMoney(10000);
    board->placeMoney(10000);
    board->placeMoney(10000);
    
    EXPECT_EQ(board->getTotalMoney(), 30000);
}

TEST_F(BoardTest, TakeLargestBill) {
    board->placeMoney(10000); // $10,000
    board->placeMoney(20000); // $20,000
    board->placeMoney(30000); // $30,000
    
    int largest = board->takeLargestBill();
    EXPECT_EQ(largest, 30000);
    EXPECT_EQ(board->getTotalMoney(), 30000); // 10k + 20k
    
    largest = board->takeLargestBill();
    EXPECT_EQ(largest, 20000);
    EXPECT_EQ(board->getTotalMoney(), 10000);
}

TEST_F(BoardTest, TakeLargestBillEmpty) {
    EXPECT_EQ(board->takeLargestBill(), 0);
}

TEST_F(BoardTest, CleanupRound) {
    // Add some dice and money
    Die die(4, false);
    board->placeDie(die);
    board->placeMoney(10000);
    board->placeMoney(10000);
    
    EXPECT_EQ(board->totalDice(), 1);
    EXPECT_EQ(board->getTotalMoney(), 20000);
    
    board->cleanupRound();
    
    EXPECT_EQ(board->totalDice(), 0);
    EXPECT_EQ(board->getTotalMoney(), 0);
}

TEST_F(BoardTest, CopyConstructor) {
    board->placeMoney(10000);
    Die die(2, false);
    board->placeDie(die);
    
    Board copy(*board);
    EXPECT_EQ(copy.getBoardId(), board->getBoardId());
    EXPECT_EQ(copy.totalDice(), board->totalDice());
    EXPECT_EQ(copy.getTotalMoney(), board->getTotalMoney());
}

TEST_F(BoardTest, AssignmentOperator) {
    board->placeMoney(10000);
    Die die(2, false);
    board->placeDie(die);
    
    Board assigned;
    assigned = *board;
    EXPECT_EQ(assigned.getBoardId(), board->getBoardId());
    EXPECT_EQ(assigned.totalDice(), board->totalDice());
    EXPECT_EQ(assigned.getTotalMoney(), board->getTotalMoney());
}

TEST_F(BoardTest, DiceOwnership) {
    // Create two players
    auto player1 = std::make_shared<Player>("Player1", 1);
    auto player2 = std::make_shared<Player>("Player2", 2);
    
    // Place dice from different players
    Die die1(3, false);
    Die die2(3, false);
    Die die3(4, false);
    
    die1.setOwner(player1);
    die2.setOwner(player1);
    die3.setOwner(player2);
    
    board->placeDie(die1);
    board->placeDie(die2);
    board->placeDie(die3);
    
    EXPECT_EQ(board->totalDice(), 3);
    
    // Test dice ownership mapping
    auto playerMappings = std::map<std::shared_ptr<Player>, int>();
    const auto& dice = board->getDice();
    for (const auto& die : dice) {
        auto owner = die.getOwner();
        playerMappings[owner]++;
    }
    
    EXPECT_EQ(playerMappings[player1], 2);
    EXPECT_EQ(playerMappings[player2], 1);
    EXPECT_EQ(playerMappings.size(), 2);
}
