#include <gtest/gtest.h>
#include "../GameScene.h"

// Mock function for OnGameOver callback
void mockGameOverCallback(unsigned) {}

class GameSceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a new GameScene before each test
        gameScene = std::make_unique<GameScene>(mockGameOverCallback);
    }

    void TearDown() override {
        // Clean up after each test
        gameScene.reset();
    }

    std::unique_ptr<GameScene> gameScene;
};

// Test coordinate conversion functions
TEST_F(GameSceneTest, CoordinateConversion) {
    // Test fromCartesian
    EXPECT_EQ(gameScene->fromCartesian(0, 0), 0);
    EXPECT_EQ(gameScene->fromCartesian(1, 0), GameScene::W);
    EXPECT_EQ(gameScene->fromCartesian(0, 1), 1);
    EXPECT_EQ(gameScene->fromCartesian(5, 7), 5 * GameScene::W + 7);

    // Test toCartesian
    auto [i, j] = gameScene->toCartesian(0);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 0);

    std::tie(i, j) = gameScene->toCartesian(1);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 1);

    std::tie(i, j) = gameScene->toCartesian(GameScene::W);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 0);

    std::tie(i, j) = gameScene->toCartesian(5 * GameScene::W + 7);
    EXPECT_EQ(i, 5);
    EXPECT_EQ(j, 7);
}

// Test posColor function
TEST_F(GameSceneTest, PosColor) {
    // We can't test all cells because some will be occupied by the current flying tetromino
    // Instead, we'll test that the function returns a valid color (one of the defined enum values)
    for (unsigned i = 0; i < GameScene::W; ++i) {
        for (unsigned j = 0; j < GameScene::H; ++j) {
            CellColor color = gameScene->posColor(gameScene->fromCartesian(i, j));
            // Check that the color is one of the valid enum values
            EXPECT_GE(static_cast<int>(color), static_cast<int>(CellColor::EmptyCell));
            EXPECT_LE(static_cast<int>(color), static_cast<int>(CellColor::ZColor));
        }
    }
}

// Test collision detection
TEST_F(GameSceneTest, Collision) {
    // Test collision with walls
    // This is a bit tricky to test directly since the flying position is private
    // We would need to expose it or add a method to set it for testing

    // For now, we can test that hardDropPos returns a valid position
    EXPECT_GE(gameScene->hardDropPos(), 0);
    EXPECT_LT(gameScene->hardDropPos(), GameScene::W * GameScene::H);
}

// Test level up logic
TEST_F(GameSceneTest, LevelUp) {
    // Test linesToLevelUp
    EXPECT_EQ(gameScene->linesToLevelUp(), 10); // At level 0, need 10 lines to level up
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
