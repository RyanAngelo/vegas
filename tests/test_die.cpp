#include <gtest/gtest.h>
#include "../Die.h"
#include "../Player.h"

class DieTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_shared<Player>("TestPlayer", 1);
    }
    
    std::shared_ptr<Player> player;
};

TEST_F(DieTest, DefaultConstructor) {
    Die die;
    EXPECT_EQ(die.getValue(), 0);
    EXPECT_FALSE(die.isPlayed());
    EXPECT_EQ(die.getOwner(), nullptr);
}

TEST_F(DieTest, ParameterizedConstructor) {
    Die die(5, true);
    EXPECT_EQ(die.getValue(), 5);
    EXPECT_TRUE(die.isPlayed());
    EXPECT_EQ(die.getOwner(), nullptr);
}

TEST_F(DieTest, RollDie) {
    Die die;
    int value = die.rollIt();
    EXPECT_GE(value, 1);
    EXPECT_LE(value, 6);
    EXPECT_EQ(die.getValue(), value);
}

TEST_F(DieTest, SetAndGetPlayed) {
    Die die;
    EXPECT_FALSE(die.isPlayed());
    
    die.setPlayed(true);
    EXPECT_TRUE(die.isPlayed());
    
    die.setPlayed(false);
    EXPECT_FALSE(die.isPlayed());
}

TEST_F(DieTest, SetAndGetOwner) {
    Die die;
    EXPECT_EQ(die.getOwner(), nullptr);
    
    die.setOwner(player);
    EXPECT_EQ(die.getOwner(), player);
}

TEST_F(DieTest, CopyConstructor) {
    Die original(3, true);
    original.setOwner(player);
    
    Die copy(original);
    EXPECT_EQ(copy.getValue(), 3);
    EXPECT_TRUE(copy.isPlayed());
    EXPECT_EQ(copy.getOwner(), player);
}

TEST_F(DieTest, AssignmentOperator) {
    Die original(4, false);
    original.setOwner(player);
    
    Die assigned;
    assigned = original;
    EXPECT_EQ(assigned.getValue(), 4);
    EXPECT_FALSE(assigned.isPlayed());
    EXPECT_EQ(assigned.getOwner(), player);
}
