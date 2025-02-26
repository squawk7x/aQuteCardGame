#include <gtest/gtest.h>

#include <QApplication>

#include "../card.h"
#include "../game.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv); // Add QApplication

    ::testing::InitGoogleTest(&argc, argv); // Initialize Google Test
    return RUN_ALL_TESTS();                 // Run all tests
}

TEST(MyGoogleTest, gTestCardValues)
{
    Card card = Card("♦", "Q");
    qDebug() << "Testing card Q value should be 10:" << card.value();
    EXPECT_EQ(card.value(), 10);

    card = Card("♦", "6");
    qDebug() << "Testing card 6 value should be 0:" << card.value();
    ASSERT_EQ(card.value(), 0);
}

TEST(MyGoogleTest, testCardsInGame)
{
    Game game(3);

    int numCards = 0;

    qDebug() << "Player_1 has :" << game.player1_->handdeck()->cards().count();
    numCards += game.player1_->handdeck()->cards().count();
    qDebug() << "Player_2 has :" << game.player2_->handdeck()->cards().count();
    numCards += game.player2_->handdeck()->cards().count();
    qDebug() << "Player_3 has :" << game.player3_->handdeck()->cards().count();
    numCards += game.player3_->handdeck()->cards().count();
    qDebug() << "Blind has :" << game.blind_->cards().count();
    numCards += game.blind_->cards().count();
    qDebug() << "Stack has :" << game.stack_->cards().count();
    numCards += game.stack_->cards().count();

    qDebug() << "Total number of Cards should be 36 and is:" << numCards;
    ASSERT_EQ(numCards, 36);
}
