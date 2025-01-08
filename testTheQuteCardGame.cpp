#include "testTheQuteCardGame.h"
#include "card.h"
#include "game.h"

void MyTest::testCardValues()
{
    Card card = Card("♦", "Q");
    qDebug() << "Testing card Q value should be 10:" << card.value();
    QCOMPARE(card.value(), 10);

    card = Card("♦", "6");
    qDebug() << "Testing card 6 value should be 0:" << card.value();
    QCOMPARE(card.value(), 0);
}

void MyTest::testCardsInGame()
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

    qDebug() << "Total number of Card should be 36 and is:" << numCards;
    QCOMPARE(numCards, 36);
}

// Run the tests
// QTEST_MAIN(MyTest)
#include "testTheQuteCardGame.moc"
