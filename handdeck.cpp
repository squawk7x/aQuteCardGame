#include "handdeck.h"
#include <QGroupBox>

Handdeck::Handdeck(QWidget* parent)
    : CardVec(parent)
{}

void Handdeck::addCard(QSharedPointer<Card> card)
{
    if (card) {
        connect(card.data(), &Card::cardClicked, this, [this, card]() {
            this->onCardClicked(card);
        });
        card->setParent(this);

        QGroupBox* handdeck = qobject_cast<QGroupBox*>(parent());

        if (handdeck && handdeck->objectName() == "gbPlayer1") {
            card->loadImage(true);
        } else {
            card->loadImage(isCardFaceVisible_);
        }

        cards_.append(card);
        layout_->addWidget(card.data());
        card->show();
    }
}

void Handdeck::removeCard(QSharedPointer<Card> card)
{
    if (card && cards_.contains(card)) {
        disconnect(card.data(), &Card::cardClicked, this, nullptr);
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
        cards_.removeOne(card);
    }
}

void Handdeck::permuteSixes(QString stackSuit)
{
    // KI permute sixes

    int sixes = countCardsOfRank("6");
    qDebug() << "number of 6s:" << sixes;

    int count = 0;
    if (sixes > 2 && cards()[0]->rank() == "6") {
        // assure first '6' suit matches stackcard suit
        while (cards()[0]->suit() != stackSuit && count < std::tgamma(sixes + 1)) {
            std::next_permutation(cards().begin(), cards().begin() + sixes - 1);
            count++;
            qDebug() << count << "swap for first 6:" << cardsAsString();
        }

        count = 0;
        bool found = false;
        // Permute remaining '6's until next card suit is matched
        if (sixes > 2 && cards().size() > sixes) { // not only '6' in handdeck

            for (size_t i = sixes; i < cards().size(); ++i) {
                while (count < std::tgamma(sixes)) {
                    std::next_permutation(cards().begin() + 1, cards().begin() + sixes);

                    if (cards()[i]->suit() == cards()[sixes - 1]->suit()) {
                        qDebug() << count << "swap for last 6:" << cardsAsString();
                        qDebug() << "last 6:" << cards()[sixes - 1]->str();
                        qDebug() << "found card:" << cards()[i]->str();
                        found = true;
                        break;
                    } // end if loop
                    count++;
                } // end while loop
                if (found)
                    break;
            } // end for loop

        } // end if
    }
    // end KI permute sixes
}

// void Handdeck::permuteAces(QString stackSuit)
// {
//     // KI permute aces

//     int aces = countCardsOfRank("A");
//     qDebug() << "number of aces:" << aces;

//     auto firstAce = std::find_if(cards().begin(),
//                                  cards().end(),
//                                  [](const QSharedPointer<Card>& card) {
//                                      return card->rank() == "A";
//                                  });

//     // Find the last element with rank "A"
//     auto lastAce = std::find_if(cards().rbegin(),
//                                 cards().rend(),
//                                 [](const QSharedPointer<Card>& card) {
//                                     return card->rank() == "A";
//                                 });

//     int firstIndex = 0;
//     if (firstAce != cards().end()) {
//         firstIndex = std::distance(cards().begin(), firstAce);
//         qDebug() << "First element found at index: " << firstIndex;
//     } else {
//         qDebug() << "Element not found in the vector.";
//     }

//     int lastIndex = 0;
//     if (lastAce != cards().rend()) {
//         lastIndex = std::distance(cards().begin(), lastAce.base()) - 1;
//         qDebug() << "Last element found at index: " << lastIndex;
//     } else {
//         qDebug() << "Element not found in the vector.";
//     }

//     int count = 0;
//     if (aces > 2) {
//         // assure first 'A' suit matches stackcard suit
//         while (cards()[firstIndex]->suit() != stackSuit && count < std::tgamma(aces + 1)) {
//             std::next_permutation(cards().begin() + firstIndex, cards().begin() + lastIndex);
//             count++;
//             qDebug() << count << "swap for first A:" << cardsAsString();
//         }

//         count = 0;
//         bool found = false;
//         // Permute remaining 'A's until next card suit is matched
//         if (aces > 2 && cards().size() > lastIndex) { // more cards in handdeck

//             for (size_t i = lastIndex + 1; i < cards().size(); ++i) {
//                 while (count < std::tgamma(aces)) {
//                     std::next_permutation(cards().begin() + firstIndex + 1,
//                                           cards().begin() + lastIndex);

//                     if (cards()[i]->suit() == cards()[lastIndex]->suit()) {
//                         qDebug() << count << "swap for last A:" << cardsAsString();
//                         qDebug() << "last A" << cards()[lastIndex]->str();
//                         qDebug() << "found card:" << cards()[i]->str();
//                         found = true;
//                         break;
//                     } // end if
//                     count++;
//                 } // end while loop
//                 if (found)
//                     break;
//             } // end for loop

//         } // end if
//     }
//     // end KI permute aces
// }

void Handdeck::permuteRanks(QString rank, const QSharedPointer<Card>& stackCard, QString stackSuit)
{
    // KI permute ranks

    int numRanks = countCardsOfRank(rank);
    qDebug() << "number cards of rank" << rank << "=" << numRanks;

    auto firstRank = std::find_if(cards().begin(),
                                  cards().end(),
                                  [&](const QSharedPointer<Card>& card) {
                                      return card->rank() == rank;
                                  });

    // Find the last element with given rank
    auto lastRank = std::find_if(cards().rbegin(),
                                 cards().rend(),
                                 [&](const QSharedPointer<Card>& card) {
                                     return card->rank() == rank;
                                 });

    int firstIndex = 0;
    if (firstRank != cards().end()) {
        firstIndex = std::distance(cards().begin(), firstRank);
        qDebug() << "First" << rank << " found at index: " << firstIndex;
    } else {
        qDebug() << "Element not found in the vector.";
    }

    int lastIndex = 0;
    if (lastRank != cards().rend()) {
        lastIndex = std::distance(cards().begin(), lastRank.base()) - 1;
        qDebug() << "Last" << rank << "found at index:" << lastIndex;
    } else {
        qDebug() << "Element not found in the vector.";
    }

    int count = 0;
    if (numRanks >= 2) {
        // assure first rank->suit matches stackcard suit
        while (cards()[firstIndex]->suit() != stackSuit && count < std::tgamma(numRanks + 1)) {
            std::next_permutation(cards().begin() + firstIndex, cards().begin() + lastIndex);
            count++;
            qDebug() << count << "permute cards to stackcard suit:" << stackSuit << cardsAsString();
        }

        count = 0;
        bool found = false;
        // Permute remaining ranks until another suit in handdeck is matched
        if (numRanks >= 2 && cards().size() > lastIndex) { // more cards in handdeck

            int inc = 1;
            if (rank == "A" && stackCard->rank() == "A")
                inc = 0;

            for (size_t i = lastIndex + 1; i < cards().size(); ++i) {
                qDebug() << "in for loop ...";
                while (count < std::tgamma(numRanks)) {
                    qDebug() << "in while loop ...";
                    std::next_permutation(cards().begin() + firstIndex + inc,
                                          cards().begin() + lastIndex + 1);

                    if (cards()[i]->suit() == cards()[lastIndex]->suit()) {
                        qDebug() << "in if ...";

                        qDebug() << count << "permute for last" << rank << cardsAsString();
                        qDebug() << "found card that fits to" << cards()[lastIndex]->str() << ":"
                                 << cards()[i]->str();
                        found = true;
                        break;
                    } // end if
                    count++;
                } // end while loop
                if (found)
                    break;
            } // end for loop

        } // end if
    }
    // end KI permute ranks
}

void Handdeck::sortCardsBy(SortOption option)
{
    if (option == SortOption::Suit) {
        std::sort(cards_.begin(),
                  cards_.end(),
                  [](const QSharedPointer<Card>& a, const QSharedPointer<Card>& b) {
                      if (a->suit() == b->suit()) {
                          return ranks.indexOf(a->rank()) < ranks.indexOf(b->rank());
                      }
                      return suits.indexOf(a->suit()) < suits.indexOf(b->suit());
                  });
    } else if (option == SortOption::Rank) {
        std::sort(cards_.begin(),
                  cards_.end(),
                  [](const QSharedPointer<Card>& a, const QSharedPointer<Card>& b) {
                      if (a->rank() == b->rank()) {
                          return suits.indexOf(a->suit()) < suits.indexOf(b->suit());
                      }
                      return ranks.indexOf(a->rank()) < ranks.indexOf(b->rank());
                  });
    }

    updateLayout();
}

int Handdeck::pointsOnHand()
{
    int points = 0;
    for (const auto& card : cards_)
        points += card->value();
    return points;
}

void Handdeck::onCardClicked(const QSharedPointer<Card>& card)
{
    if (cards().contains(card)) {
        emit handCardClicked(card);
    }
}

void Handdeck::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = isVisible;

    QGroupBox* handdeck = qobject_cast<QGroupBox*>(parent());

    if (!handdeck) {
        return; // Safeguard against invalid parent casting
    }

    for (const auto& card : std::as_const(cards_)) { // Using as_const() to prevent detachment
        if (handdeck && handdeck->objectName() == "gbPlayer1") {
            card->loadImage(true);
        } else {
            card->loadImage(isCardFaceVisible_);
        }
    }
}
