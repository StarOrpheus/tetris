#include <gtest/gtest.h>
#include "../GameOverScene.h"

class GameOverSceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a new GameOverScene before each test with a score of 1000
        gameOverScene = std::make_unique<GameOverScene>(1000);
    }

    void TearDown() override {
        // Clean up after each test
        gameOverScene.reset();
    }

    std::unique_ptr<GameOverScene> gameOverScene;
};

// Test construction
TEST_F(GameOverSceneTest, Construction) {
    // Just verify that the object was constructed without errors
    EXPECT_NE(gameOverScene, nullptr);
    
    // Create another instance with a different score
    auto anotherScene = std::make_unique<GameOverScene>(2000);
    EXPECT_NE(anotherScene, nullptr);
}

// Test onKey method (which is empty but should not crash)
TEST_F(GameOverSceneTest, OnKey) {
    // Call onKey with various parameters to ensure it doesn't crash
    gameOverScene->onKey(0, 0);
    gameOverScene->onKey(1, 1);
    gameOverScene->onKey(-1, -1);
    
    // No assertions needed as we're just testing that it doesn't crash
    SUCCEED();
}

// Note: We can't easily test the draw method as it depends on ImGui
// and would require a more complex setup with a mock rendering context.
// In a real-world scenario, we might use a mocking framework to test this.