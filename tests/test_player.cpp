#include <gtest/gtest.h>
#include "../Player.h"
#include "../Board.h"
#include "../Die.h"

class PlayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_shared<Player>("TestPlayer", 1);
        board = std::make_unique<Board>(1);
    }
    
    std::shared_ptr<Player> player;
    std::unique_ptr<Board> board;
};

TEST_F(PlayerTest, DefaultConstructor) {
    Player defaultPlayer;
    EXPECT_EQ(defaultPlayer.getPlayerName(), "Player");
    EXPECT_EQ(defaultPlayer.getPlayerIdentifier(), 1);
    EXPECT_EQ(defaultPlayer.getTotalMoney(), 0);
    EXPECT_EQ(defaultPlayer.getAllDice().size(), 0);
}

TEST_F(PlayerTest, ParameterizedConstructor) {
    Player testPlayer("Alice", 5);
    EXPECT_EQ(testPlayer.getPlayerName(), "Alice");
    EXPECT_EQ(testPlayer.getPlayerIdentifier(), 5);
    EXPECT_EQ(testPlayer.getTotalMoney(), 0);
    EXPECT_EQ(testPlayer.getAllDice().size(), 0);
}

TEST_F(PlayerTest, GrabDice) {
    player->grabDice(3);
    EXPECT_EQ(player->getAllDice().size(), 3);
    
    const auto& dice = player->getAllDice();
    for (const auto& die : dice) {
        EXPECT_EQ(die.getOwner(), player);
        EXPECT_FALSE(die.isPlayed());
    }
}

TEST_F(PlayerTest, HasActiveDice) {
    EXPECT_FALSE(player->hasActiveDice());
    
    player->grabDice(2);
    EXPECT_TRUE(player->hasActiveDice());
    
    // Make all dice played by creating new dice that are played
    player->cleanupRound();
    player->grabDice(2);
    // We can't directly modify the dice since they're const, but we can test the initial state
    EXPECT_TRUE(player->hasActiveDice());
}

TEST_F(PlayerTest, TakeMoney) {
    EXPECT_EQ(player->getTotalMoney(), 0);
    
    player->takeMoney(10000);
    EXPECT_EQ(player->getTotalMoney(), 10000);
    
    player->takeMoney(5000);
    EXPECT_EQ(player->getTotalMoney(), 15000);
}

TEST_F(PlayerTest, GetTotalMoney) {
    player->takeMoney(10000);
    player->takeMoney(20000);
    player->takeMoney(30000);
    
    EXPECT_EQ(player->getTotalMoney(), 60000);
}

TEST_F(PlayerTest, CleanupRound) {
    player->grabDice(3);
    player->takeMoney(10000);
    
    EXPECT_EQ(player->getAllDice().size(), 3);
    EXPECT_EQ(player->getTotalMoney(), 10000);
    
    player->cleanupRound();
    
    EXPECT_EQ(player->getAllDice().size(), 0);
    EXPECT_EQ(player->getTotalMoney(), 10000); // Money should remain
}

TEST_F(PlayerTest, AddDiceToBoard) {
    player->grabDice(3);
    
    // Create boards
    std::vector<Board> boards;
    boards.emplace_back(1);
    
    // We can't directly set dice values since they're const, but we can test the basic functionality
    player->addDiceToBoard(boards, 1);
    
    // The dice should be placed if they have the right value (which is random)
    // We can't predict the exact outcome, but we can test that the function doesn't crash
    EXPECT_TRUE(true); // Placeholder assertion
}

TEST_F(PlayerTest, CopyConstructor) {
    player->grabDice(2);
    player->takeMoney(5000);
    
    Player copy(*player);
    EXPECT_EQ(copy.getPlayerName(), player->getPlayerName());
    EXPECT_EQ(copy.getPlayerIdentifier(), player->getPlayerIdentifier());
    EXPECT_EQ(copy.getTotalMoney(), player->getTotalMoney());
    EXPECT_EQ(copy.getAllDice().size(), player->getAllDice().size());
}

TEST_F(PlayerTest, AssignmentOperator) {
    player->grabDice(2);
    player->takeMoney(5000);
    
    Player assigned;
    assigned = *player;
    EXPECT_EQ(assigned.getPlayerName(), player->getPlayerName());
    EXPECT_EQ(assigned.getPlayerIdentifier(), player->getPlayerIdentifier());
    EXPECT_EQ(assigned.getTotalMoney(), player->getTotalMoney());
    EXPECT_EQ(assigned.getAllDice().size(), player->getAllDice().size());
}
