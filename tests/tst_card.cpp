// #include <gmock/gmock-matchers.h>
// #include <gtest/gtest.h>

// #include <QApplication>

// #include "../aQuteCardGame/card.h"

// using namespace testing;

// // Test fixture to handle QApplication setup
// class CardTest : public ::testing::Test {
//  protected:
//   // SetUp() will initialize the QApplication
//   void SetUp() override {
//     int argc = 1;
//     char* argv[] = {};
//     app = new QApplication(argc, argv);  // Create QApplication object
//   }

//   // TearDown() will delete the QApplication
//   void TearDown() override { delete app; }

//   QApplication* app;  // QApplication pointer
// };

// // Test the value() function of the Card class
// TEST_F(
//     CardTest, ValueTest) {
//   // Create cards for each rank and check their value
//   Card card6("♦", "6");    // Expected value is 0
//   Card card7("♦", "7");    // Expected value is 0
//   Card card8("♦", "8");    // Expected value is 0
//   Card card9("♦", "9");    // Expected value is 0
//   Card card10("♦", "10");  // Expected value is 10
//   Card cardJ("♦", "J");    // Expected value is 20
//   Card cardQ("♦", "Q");    // Expected value is 10
//   Card cardK("♦", "K");    // Expected value is 10
//   Card cardA("♦", "A");    // Expected value is 15

//   // Test for different card ranks
//   EXPECT_EQ(card6.value(), 0);    // Should be 0 for rank 6
//   EXPECT_EQ(card7.value(), 0);    // Should be 0 for rank 7
//   EXPECT_EQ(card8.value(), 0);    // Should be 0 for rank 8
//   EXPECT_EQ(card9.value(), 0);    // Should be 0 for rank 9
//   EXPECT_EQ(card10.value(), 10);  // Should be 10 for rank 10
//   EXPECT_EQ(cardJ.value(), 20);   // Should be 20 for Jack
//   EXPECT_EQ(cardQ.value(), 10);   // Should be 10 for Queen
//   EXPECT_EQ(cardK.value(), 10);   // Should be 10 for King
//   EXPECT_EQ(cardA.value(), 15);   // Should be 15 for Ace
// }

// // Test invalid card rank (if applicable)
// TEST_F(
//     CardTest, InvalidCardTest) {
//   // Create a card with an invalid rank
//   Card invalidCard("♦", "2");  // Rank "2" is not handled by setValue

//   // The value should default to 0 since the rank "2" is not defined in
//   setValue EXPECT_EQ(invalidCard.value(), 0);
// }

// // Test value for all suits (since the value does not depend on suit, only
// rank) TEST_F(
//     CardTest, SuitIndependenceTest) {
//   // Create cards for each suit with the same rank "A" (Ace)
//   Card cardDiamond("♦", "A");
//   Card cardSpade("♠", "A");
//   Card cardHeart("♥", "A");
//   Card cardClub("♣", "A");

//   // The value should be the same for all suits, as it's based on rank alone
//   EXPECT_EQ(cardDiamond.value(), 15);
//   EXPECT_EQ(cardSpade.value(), 15);
//   EXPECT_EQ(cardHeart.value(), 15);
//   EXPECT_EQ(cardClub.value(), 15);
// }

// int main(
//     int argc, char** argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }
