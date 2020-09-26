/***************************************************************************/ /**
 * 
 * Source File	: 	sonos.Test.Unit.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200925
 * Version		:	0.1.0
 * 
 * Description	:	Unit Tests for src/Sonos.cpp
 * 
 * 
 ******************************************************************************/

//#include "../src/sonos.hpp"
#include <gtest/gtest.h>

/*
// Args for _uut constructor
std::string _FakeEvent = "waffleiron_sonos";
std::string[3] _FakeStates = {"heating", "ready", "baking"};

// Create Test Fixture
class SonoscontrolTest : public ::testing : Test
{
protected:
    // Common Arrange
    Sonoscontrol _uut(_FakeEvent, _FakeStates);

    void SetUp() override
    {
    }

    //void TerDown() override {}

}

// Use Test Fixture to Test UUT
TEST_F(SonoscontrolTest, StateIsKnown)
{
    // Arrange

    // Act

    // Assert
    EXPECT_STREQ(2, 2);
}

*/

TEST(SonoscontrolTest, StateIsKnow)
{
    ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}