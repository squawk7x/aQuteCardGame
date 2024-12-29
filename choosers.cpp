#include "choosers.h"
#include <QDebug>
#include <random>

// -----------------------------------------------------------------------
int getRandomNumber(int max)
{
    std::random_device rd;                              // Seed generator
    std::mt19937 gen(rd());                             // Random number engine
    std::uniform_int_distribution<int> distrib(0, max); // Range [0, max]

    return distrib(gen);
}

// -----------------------------------------------------------------------
// Constructor
BaseChooser::BaseChooser(QVector<QString> decs, QObject *parent)
    : decs_(decs)
{
    if (!decs_.isEmpty()) {
        setData();
    }
    connect(this, &QPushButton::clicked, this, &BaseChooser::toggle);
}

// Methods
void BaseChooser::toggle()
{
    if (!decs().isEmpty()) {
        auto firstElement = decs().front(); // Access the first element
        decs().erase(decs().begin());       // Remove the first element
        decs().push_back(firstElement);
        setData();
        if (isAndroidVersion) {
            emit chooserToggled();
            qDebug() << this << "chooserToggled emitted";
        }
        qDebug() << this << decision();
    }
}

void BaseChooser::toggle_to(const QString &target_decision)
{
    while (decs_.at(0) != target_decision) {
        toggle();
        qDebug() << this << decision();
    }
}

void BaseChooser::toggleRandom()
{
    for (int i = 0; i < getRandomNumber(decs().size()); i++) {
        toggle();
    }
}

// Getters
QVector<QString> &BaseChooser::decs()
{
    return decs_;
}

QString BaseChooser::decision()
{
    return decision_;
}

QString BaseChooser::str()
{
    return str_;
}

QString BaseChooser::name()
{
    return name_;
}

// Setters
void BaseChooser::setDecision()
{
    decision_ = decs_.at(0);
}

void BaseChooser::setStr()
{
    str_ = decs_.at(0);
}

void BaseChooser::setName()
{
    name_ = decs_.at(0);
}

void BaseChooser::setData()
{
    setDecision();
    setStr();
    setName();
    loadImage();
}

void BaseChooser::loadImage()
{
    setStr();
    qDebug() << str();
    this->setText(str());
}

// -----------------------------------------------------------------------

JsuitChooser::JsuitChooser(QVector<QString> decs, QObject *parent)
    : BaseChooser(decs)
{}

QString JsuitChooser::suit()
{
    return str();
}

void JsuitChooser::onCardAddedToStack(const QSharedPointer<Card> &card)
{
    hide();
    setEnabled(false);
}

// -----------------------------------------------------------------------
QuteChooser::QuteChooser(QVector<QString> decs, QObject *parent)
    : BaseChooser(decs)
{}

void QuteChooser::toggle()
{
    if (!decs().isEmpty()) {
        auto firstElement = decs().front(); // Access the first element
        decs().erase(decs().begin());       // Remove the first element
        decs().push_back(firstElement);
        setData();

        emit quteDecisionChanged(decision());

        if (isAndroidVersion) {
            emit chooserToggled();
            qDebug() << this << "chooserToggled emitted";
        }

        qDebug() << this << decision();
    }
}

// -----------------------------------------------------------------------
EightsChooser::EightsChooser(QVector<QString> decs, QObject *parent)
    : BaseChooser(decs)
{}
// -----------------------------------------------------------------------
JpointsChooser::JpointsChooser(QVector<QString> decs, QObject *parent)
    : BaseChooser(decs)
{}

void JpointsChooser::onQuteDecisionChanged(const QString &quteDec)
{
    qDebug() << this << "has received signal " << quteDec;
    if (quteDec == QString("QUTE")) {
        setEnabled(true);
        show();
    } else {
        hide();
        setEnabled(false);
    }
    loadImage();
}
// -----------------------------------------------------------------------
RoundChooser::RoundChooser(QVector<QString> decs, QObject *parent)
    : BaseChooser(decs)
{
    connect(this, &QPushButton::clicked, this, [this]() {
        if (decision() == QString("FINISH")) {
            qDebug() << this << decision();
            toggle_to(QString("NEW"));
            qDebug() << this << decision();
            emit finishRound();
        } else if (decision() == QString("NEW")) {
            qDebug() << this << decision();
            emit newRound();
        } else if (decision() == QString("GAME")) {
            qDebug() << this << decision();
            emit newGame();
        }
    });
}

void RoundChooser::onQuteDecisionChanged(const QString &quteDec)

{
    qDebug() << this << "has received signal " << quteDec;
    if (quteDec == QString("QUTE")) {
        toggle_to(QString("FINISH"));
        setEnabled(true);
        show();
    } else {
        hide();
        setEnabled(false);
    }
    loadImage();
}
// -----------------------------------------------------------------------
