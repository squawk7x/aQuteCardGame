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

// KI

QSharedPointer<CardVec> Handdeck::bestPermutation(const QSharedPointer<Card>& card,
                                                  const QString& jsuit = "",
                                                  int numPlayers = 3)
{
    QString lastRank = card->rank();
    QString lastSuit = card->suit();
    if (jsuit != "")
        lastSuit = jsuit;

    QSharedPointer<QVector<QSharedPointer<CardVec>>> permutStartWith6 = filteredPermutationsFor("6");

    for (const auto& cardVec : *permutStartWith6) {
        if (cardVec->cards().front()->suit() == lastSuit)
            qDebug() << cardVec->cardsAsString();
    }

    return nullptr; // Adjust as needed for actual return
}

QSharedPointer<QVector<QSharedPointer<CardVec>>> Handdeck::filteredPermutationsFor(QString rank)
{
    auto allPermutations = QSharedPointer<QVector<QSharedPointer<CardVec>>>::create();
    QVector<QSharedPointer<Card>> cardsCopy = cards(); // Copy the cards

    std::sort(cardsCopy.begin(),
              cardsCopy.end(),
              [](const QSharedPointer<Card>& a, const QSharedPointer<Card>& b) { return *a < *b; });

    do {
        if (cardsCopy.front()->rank() == rank) {
            QSharedPointer<CardVec> permutation = QSharedPointer<CardVec>::create(nullptr,
                                                                                  cardsCopy);
            allPermutations->append(permutation);
            qDebug() << permutation;
        }
    } while (std::next_permutation(cardsCopy.begin(),
                                   cardsCopy.end(),
                                   [](const QSharedPointer<Card>& a,
                                      const QSharedPointer<Card>& b) { return *a < *b; }));

    return allPermutations;
}

// end KI

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
