#include "player.h"

Player::Player(int id,
               const QString &name,
               bool isRobot,
               int score,
               QSharedPointer<Handdeck> handdeck,
               QObject *parent) // QObject constructor
    : QObject(parent)           // QObject constructor
    , id_(id)
    , name_(name)
    , isRobot_(isRobot)
    , score_(score)
    , jpoints_(0)
{
    handdeck_ = QSharedPointer<Handdeck>(new Handdeck());
}

// Operators
bool operator<(const Player &lhs, const Player &rhs)
{
    return lhs.score_ < rhs.score_;
}

bool operator>(const Player &lhs, const Player &rhs)
{
    return lhs.score_ > rhs.score_;
}

bool operator==(const Player &lhs, const Player &rhs)
{
    return lhs.score_ == rhs.score_;
}

// Getters
int Player::id() const
{
    return id_;
}

QString Player::name() const
{
    return name_;
}

bool Player::isRobot() const
{
    return isRobot_;
}

int Player::jpoints() const
{
    return jpoints_;
}

int Player::score() const
{
    return score_;
}

QSharedPointer<Handdeck> Player::handdeck() const
{
    return handdeck_;
}

// Methods
int Player::pointsOnHand()
{
    int pointsOnHand = 0;
    for (const auto &card : std::as_const(handdeck_->cards())) {
        pointsOnHand += card->value();
    }
    return pointsOnHand;
}

// Setters
void Player::setName(const QString &name)
{
    name_ = name;
}

void Player::setIsRobot(bool isRobot)
{
    isRobot_ = isRobot;
}

void Player::setJpoints(int jpoints)
{
    jpoints_ = jpoints;
}

void Player::setHanddeck(QSharedPointer<Handdeck> handdeck)
{
    if (handdeck_ != handdeck) {
        handdeck_ = handdeck;
    }
}

// Slots
void Player::onCountPoints(int shuffles)
{
    if (shuffles == 0) {
        score_ = 0;
        jpoints_ = 0;
    } else {
        score_ += pointsOnHand() * shuffles;
        score_ += jpoints_ * shuffles;

        if (score_ == 125)
            score_ = 0;
    }
}
