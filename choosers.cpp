#include "choosers.h"
#include <QDebug>
#include <qstyle.h>
#include <random>

// -----------------------------------------------------------------------

// used for random toggle
int getRandomNumber(int max)
{
    std::random_device rd;                              // Seed generator
    std::mt19937 gen(rd());                             // Random number engine
    std::uniform_int_distribution<int> distrib(0, max); // Range [0, max]

    return distrib(gen);
}

// -----------------------------------------------------------------------

// Constructor
Chooser::Chooser(QVector<QString> decs, QObject *parent)
    : decs_(decs)
{
    if (!decs_.isEmpty()) {
        setData();
    }

    this->setProperty("control", true); // Add the custom property
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    connect(this, &QPushButton::clicked, this, &Chooser::toggle);
}

// Methods
void Chooser::toggle()
{
    if (!decs().isEmpty()) {
        auto firstElement = decs().front(); // Access the first element
        decs().erase(decs().begin());       // Remove the first element
        decs().push_back(firstElement);
        setData();
        loadImage();
        // emit chooserToggled();
    }
}

void Chooser::toggle_to(const QString &target_decision)
{
    while (decs_.at(0) != target_decision) {
        toggle();
        // qDebug() << this << decision();
    }
}

void Chooser::toggleRandom()
{
    for (int i = 0; i < getRandomNumber(decs().size()); i++) {
        toggle();
    }
}

// Getters
QVector<QString> &Chooser::decs()
{
    return decs_;
}

QString Chooser::decision()
{
    return decision_;
}

QString Chooser::str()
{
    return str_;
}

QString Chooser::name()
{
    return name_;
}

// Setters
void Chooser::setDecision()
{
    decision_ = decs_.at(0);
}

void Chooser::setStr()
{
    str_ = decs_.at(0);
}

void Chooser::setData()
{
    setDecision();
    setStr();
    loadImage();
}

void Chooser::setCardType(CardType type)
{
    cardType_ = type;
    setProperty("card-type", type == CardType::Small ? "Small" : "Normal");
}

void Chooser::loadImage()
{
    setStr();
    this->setText(str());
}

// -----------------------------------------------------------------------

QuteChooser::QuteChooser(QVector<QString> decs, QObject *parent)
    : Chooser(decs)
{}

void QuteChooser::toggle()
{
    if (!decs().isEmpty()) {
        auto firstElement = decs().front(); // Access the first element
        decs().erase(decs().begin());       // Remove the first element
        decs().push_back(firstElement);
        setData();

        emit quteDecisionChanged(decision());
        // emit chooserToggled();
    }
}

// -----------------------------------------------------------------------

EightsChooser::EightsChooser(QVector<QString> decs, QObject *parent)
    : Chooser(decs)
{}
// -----------------------------------------------------------------------

JpointsChooser::JpointsChooser(QVector<QString> decs, QObject *parent)
    : Chooser(decs)
{}

void JpointsChooser::toggle()
{
    if (!decs().isEmpty()) {
        auto firstElement = decs().front(); // Access the first element
        decs().erase(decs().begin());       // Remove the first element
        decs().push_back(firstElement);
        setData();
        if (decision() == "") {
            auto firstElement = decs().front(); // Access the first element
            decs().erase(decs().begin());       // Remove the first element
            decs().push_back(firstElement);
            setData();
        }
        // emit chooserToggled();
    }
}

void JpointsChooser::onQuteDecisionChanged(const QString &quteDec)
{
    if (decision() != "" && quteDec == QString("QUTE")) {
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
    : Chooser(decs)
{
    disconnect(this, &QPushButton::clicked, this, &Chooser::toggle);

    connect(this, &QPushButton::clicked, this, [&]() {
        if (decision() == QString("FINISH")) {
            toggle_to(QString("NEW"));
            emit finishRound();
        } else if (decision() == QString("NEW")) {
            emit newRound();
        } else if (decision() == QString("GAME")) {
            emit newGame();
        }
    });
}

void RoundChooser::onQuteDecisionChanged(const QString &quteDec)

{
    if (quteDec == QString("QUTE")) {
        toggle_to(QString("FINISH"));
        setEnabled(true);
        show();
    } else {
        toggle_to(QString(""));
        hide();
        setEnabled(false);
    }
    loadImage();
}

// -----------------------------------------------------------------------

JsuitChooser::JsuitChooser(QVector<QString> decs, QObject *parent)
    : Chooser(decs)
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

void JsuitChooser::loadImage()
{
    setStr();

    QString imagePath;

    // Determine the image path based on visibility

    imagePath = QString(":res/choosers/jsuit_of_%1.png").arg(suitnames.at(suits.indexOf(str())));

    setProperty("card-type", cardType_ == CardType::Small ? "Small" : "Normal");

    QPixmap pixmap(imagePath); // Load the image as a QPixmap

    if (!pixmap.isNull() && cardType_ == CardType::Normal) {
        setText("");            // Remove any text
        setIcon(QIcon(pixmap)); // Set the icon

        QSize buttonSize = QSize(500, 726) * 0.2;
        setIconSize(buttonSize);
    } else {
        setIcon(QIcon());
        this->setText(str());
    }
    // Notify Qt to reapply the stylesheet
    style()->unpolish(this);
    style()->polish(this);
}

// -----------------------------------------------------------------------
