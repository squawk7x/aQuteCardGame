#include "handdeck.h"
#include <QGroupBox>
#include <QMap>
#include <QPair>

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

QVector<QString> Handdeck::patternByRankPoints()
{
    // Step 1: Create a map to store the sum of points for each rank
    QMap<QString, int> rankPoints;

    // Step 2: Iterate through each card in the hand
    for (const auto &card : std::as_const(cards_)) {
        // Add the card's value to the corresponding rank in the map
        rankPoints[card->rank()] += card->value();
    }

    // Define all possible ranks in the desired order
    QVector<QString> pattern = {"6", "A", "K", "Q", "10", "8", "7", "9", "J"};

    // Ensure all ranks are included in the map, even if they have 0 points
    for (const auto& rank : pattern) {
        if (!rankPoints.contains(rank)) {
            rankPoints[rank] = 0;
        }
    }

    // Step 3: Sort allRanks based on the points in rankPoints map
    std::sort(pattern.begin(), pattern.end(), [&](const QString& a, const QString& b) {
        return rankPoints[a] > rankPoints[b];
    });

    // Step 4: Return the sorted ranks
    return pattern;
}

void Handdeck::prependRank(QVector<QString>& pattern, const QString& rank)
{
    auto it = std::find(pattern.begin(), pattern.end(), rank);
    if (it != pattern.end()) {
        pattern.erase(it);     // Remove the rank from its current position
        pattern.prepend(rank); // Push the rank to the front
    }
}

void Handdeck::appendRank(QVector<QString>& pattern, const QString& rank)
{
    auto it = std::find(pattern.begin(), pattern.end(), rank);
    if (it != pattern.end()) {
        pattern.erase(it);    // Remove the rank from its current position
        pattern.append(rank); // Append the rank at the end
    }
}

void Handdeck::permuteRanks(const QString& rank,
                            const QSharedPointer<Card>& stackCard,
                            const QString& stackSuit)
{
    // KI permute ranks

    int numRanks = countCardsOfRank(rank);

    // permute makes sense when 2 or more cards of rank
    if (numRanks >= 2) {
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
        } else {
            qDebug() << "Element not found in the vector.";
        }

        int lastIndex = 0;
        if (lastRank != cards().rend()) {
            lastIndex = std::distance(cards().begin(), lastRank.base()) - 1;
        } else {
            qDebug() << "Element not found in the vector.";
        }
        // end find indeces of rank

        int count = 0; // counter to avoid endless looping if no permutation matches

        // assure first cards's suit matches stackcard suit    // n! == tgamma(n+1)
        while (cards()[firstIndex]->suit() != stackSuit && count < std::tgamma(numRanks + 1)) {
            std::next_permutation(cards().begin() + firstIndex,
                                  cards().begin() + lastIndex + 1); // range [first, last)
            count++;
        }

        count = 0;
        bool found = false;

        // over how many cards can be permuted
        int inc = 1; // permute over numRanks - 1   // first card must fit to stackCard
        if (rank == stackCard->rank()) {
            inc = 0; // permute over numRanks       // all cards used to permute
        }

        // Permute ranks until another suit of next rank(s) is matched
        for (int i = 0; i < cards().size(); i++) {
            while (count < std::tgamma(numRanks + 1)) {
                std::next_permutation(cards().begin() + firstIndex + inc,
                                      cards().begin() + lastIndex + 1); // range [first, last)

                // check all cards in handdeck in order of pattern if a rank fits
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
// end KI permute ranks

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
    for (const auto &card : std::as_const(cards_))
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
