#include "handdeck.h"
#include <QGroupBox>
#include <QMap>
#include <QPair>
#include <algorithm>
#include <cmath>

Handdeck::Handdeck(QWidget* parent)
    : CardVec(parent)
{
    // cardFace_ different for player1 and other players
    // cardFace_ = CardFace::Open;
    // cardFace_ = CardFace::Closed;
}

void Handdeck::addCard(QSharedPointer<Card> card)
{
    if (card) {
        connect(card.data(), &Card::cardClicked, this, [this, card]() {
            this->onCardClicked(card);
        });
        card->setParent(this);
        card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

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

void Handdeck::playThisCard(const Card& cardToPlay)
{
    for (const auto& card : std::as_const(cards_)) {
        if (cardToPlay.str() == card->str()) {
            qDebug() << "playing card" << card->str();
            card->click();
            break;
        }
    }
}

QVector<QString> Handdeck::patternByRankPoints()
{
    QMap<QString, int> rankPoints;

    for (const auto& card : std::as_const(cards_)) {
        rankPoints[card->rank()] += card->value();
    }

    QVector<QString> pattern = {"6", "A", "K", "Q", "10", "8", "7", "9", "J"};

    for (const auto& rank : pattern) {
        if (!rankPoints.contains(rank)) {
            rankPoints[rank] = 0;
        }
    }

    std::sort(pattern.begin(), pattern.end(), [&](const QString& a, const QString& b) {
        return rankPoints[a] > rankPoints[b];
    });

    return pattern;
}

void Handdeck::prependRank(QVector<QString>& pattern, const QString& rank)
{
    auto it = std::find(pattern.begin(), pattern.end(), rank);
    if (it != pattern.end()) {
        pattern.erase(it);
        pattern.prepend(rank);
    }
}

void Handdeck::appendRank(QVector<QString>& pattern, const QString& rank)
{
    auto it = std::find(pattern.begin(), pattern.end(), rank);
    if (it != pattern.end()) {
        pattern.erase(it);
        pattern.append(rank);
    }
}

void Handdeck::permuteRanks(const QString& rank,
                            const QSharedPointer<Card>& stackCard,
                            const QString& stackSuit)
{
    int numRanks = countCardsOfRank(rank);

    if (numRanks >= 2) {
        auto firstRank = std::find_if(cards().begin(),
                                      cards().end(),
                                      [&](const QSharedPointer<Card>& card) {
                                          return card->rank() == rank;
                                      });

        auto lastRank = std::find_if(cards().rbegin(),
                                     cards().rend(),
                                     [&](const QSharedPointer<Card>& card) {
                                         return card->rank() == rank;
                                     });

        int firstIndex = 0;
        if (firstRank != cards().end()) {
            firstIndex = std::distance(cards().begin(), firstRank);
        } else {
            qDebug() << "Element not found in the vector.";
        }

        int lastIndex = 0;
        if (lastRank != cards().rend()) {
            lastIndex = std::distance(cards().begin(), lastRank.base()) - 1;
        } else {
            qDebug() << "Element not found in the vector.";
        }

        int count = 0;

        while (cards()[firstIndex]->suit() != stackSuit && count < std::tgamma(numRanks + 1)) {
            std::next_permutation(cards().begin() + firstIndex, cards().begin() + lastIndex + 1);
            count++;
        }

        count = 0;
        bool found = false;

        int inc = 1;
        if (rank == stackCard->rank()) {
            inc = 0;
        }

        for (int i = 0; i < cards().size(); i++) {
            while (count < std::tgamma(numRanks + 1)) {
                std::next_permutation(cards().begin() + firstIndex + inc,
                                      cards().begin() + lastIndex + 1);

                if (cards()[lastIndex]->rank() != cards()[i]->rank()
                    && cards()[lastIndex]->suit() == cards()[i]->suit()) {
                    found = true;
                    break;
                }
                count++;
            }
            count = 0;
            if (found)
                break;
        }
    }
}

int Handdeck::pointsOnHand()
{
    int points = 0;
    for (const auto& card : std::as_const(cards_)) {
        points += card->value();
    }
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
        return;
    }

    for (const auto& card : std::as_const(cards_)) {
        if (handdeck && handdeck->objectName() == "gbPlayer1") {
            card->loadImage(true);
        } else {
            card->loadImage(isCardFaceVisible_);
        }
    }
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
