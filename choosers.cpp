#include "choosers.h"
#include <QApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QScreen>
#include "card.h"

/* ************************************************************************* */
// JsuitChooser Implementation

JsuitChooser::JsuitChooser(QString suit, QWidget* parent)
    : suit_{suit}
{
    setSuitname(suit_);
    loadImage();

    connect(this, &QPushButton::clicked, this, &JsuitChooser::toggle);
}

// Getters
QString JsuitChooser::suit()
{
    return suit_;
}

QString JsuitChooser::suitname()
{
    return suitname_;
}

QString JsuitChooser::str()
{
    return str_;
}

// Methods
void JsuitChooser::toggle()
{
    int index = suits.indexOf(suit_);
    if (index != -1) {
        index = (index + 1) % suits.size();
        suit_ = suits[index];
        suitname_ = suitnames[index];
        loadImage();
    } else {
        qDebug() << "Current suit not found in the list.";
    }
}

void JsuitChooser::toggle_to(const QString& target_suit)
{
    int toggle_count = 0;
    const int max_toggles = 4;

    while (suit_ != target_suit && toggle_count <= max_toggles) {
        toggle();
        toggle_count++;
    }
}

// Setters
void JsuitChooser::setSuit(const QString& suit)
{
    suit_ = suit;
}

void JsuitChooser::setSuitname(const QString& suit)
{
    suitname_ = suitnames[suits.indexOf(suit)];
}

void JsuitChooser::setStr()
{
    str_ = suit_ + suit_;
}

void JsuitChooser::loadImage()
{
    setStr();

    QString imagePath = QString(":res/choosers/jsuit_of_%1.png").arg(suitname_);
    QPixmap pixmap(imagePath); // Load the image as a QPixmap
    if (!pixmap.isNull() and not forAndroid) {
        // Fetch the size of the application's primary screen
        QSize screenSize = QApplication::primaryScreen()->size(); // Get the size of the primary screen
        QSize maxSize;

        // Calculate max size as a percentage of the screen size
        int height = screenSize.height() * 0.15; // 15% of the screen height
        int width = height * 0.5;                // 50% of height for the width
        maxSize = QSize(width, height);

        // Scale the image to fit within the maximum size while keeping aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the icon and icon size to the scaled pixmap size
        QIcon icon(scaledPixmap);
        setIcon(icon);
        setIconSize(scaledPixmap.size());

        // Resize the widget to fit the scaled image size
        this->resize(scaledPixmap.size());

        // Optional: Remove padding and margins
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
    } else {
        this->setText(str());
    }
}

// Slots
void JsuitChooser::onCardAddedToStack(const QSharedPointer<Card>& card)
{
    hide();
    setEnabled(false);
}

/* ************************************************************************* */
// EightsChooser Implementation

EightsChooser::EightsChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    setStr();
    loadImage();

    connect(this, &QPushButton::clicked, this, &EightsChooser::toggle);
}

// Getters
QString EightsChooser::decision()
{
    return decision_;
}

void EightsChooser::setStr()
{
    if (decision_ == "a") {
        str_ = "ALL";
    } else if (decision_ == "n") {
        str_ = "NEXT";
    } else {
        str_ = "Unknown";
    }
}

// Methods
void EightsChooser::toggle()
{
    decision_ = (decision_ == "a") ? "n" : "a";
    loadImage();
}

QString EightsChooser::str()
{
    return str_;
}

void EightsChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
}

void EightsChooser::toggleRandom(const QString& dec1, const QString& dec2)
{
    decision_ = (QRandomGenerator::global()->bounded(2) == 0) ? dec1 : dec2;
    loadImage();
}

void EightsChooser::loadImage()
{
    setStr();

    QString imagePath = QString(":res/choosers/chooser_eights_%1.png").arg(decision_);
    QPixmap pixmap(imagePath); // Load the image as a QPixmap
    if (!pixmap.isNull() and not forAndroid) {
        // Fetch the size of the application's primary screen
        QSize screenSize = QApplication::primaryScreen()->size(); // Get the size of the primary screen
        QSize maxSize;

        // Calculate max size as a percentage of the screen size
        int height = screenSize.height() * 0.15; // 15% of the screen height
        int width = height * 0.5;                // 50% of height for the width
        maxSize = QSize(width, height);

        // Scale the image to fit within the maximum size while keeping aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the icon and icon size to the scaled pixmap size
        QIcon icon(scaledPixmap);
        setIcon(icon);
        setIconSize(scaledPixmap.size());

        // Resize the widget to fit the scaled image size
        this->resize(scaledPixmap.size());

        // Optional: Remove padding and margins
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
    } else {
        this->setText(str());
    }
}

/* ************************************************************************* */
// QuteChooser Implementation

QuteChooser::QuteChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, &QuteChooser::toggle);
}

// Getters
QString QuteChooser::str()
{
    return str_;
}

QString QuteChooser::decision()
{
    return decision_;
}

// Methods
void QuteChooser::toggle()
{
    decision_ = (decision_ == "y") ? "n" : "y";
    emit quteDecisionChanged(decision_);
    loadImage();
}

void QuteChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
}

void QuteChooser::toggleRandom(const QString& dec1, const QString& dec2)
{
    decision_ = (QRandomGenerator::global()->bounded(2) == 0) ? dec1 : dec2;
    loadImage();
}

// Setters

void QuteChooser::setStr()
{
    if (decision_ == "y") {
        str_ = "QUTE";
    } else if (decision_ == "n") {
        str_ = "CONTINUE";
    } else {
        str_ = "";
    }
}

void QuteChooser::loadImage()
{
    setStr();

    QString imagePath = QString(":res/choosers/chooser_qute_%1.png").arg(decision_);
    QPixmap pixmap(imagePath); // Load the image as a QPixmap
    if (!pixmap.isNull() and not forAndroid) {
        // Set a maximum size for the icon (adjust as needed)
        const QSize maxSize(80, 160); // Maximum width and height (adjust these values as needed)

        // Scale the image to fit within the maximum size while keeping the aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the icon and icon size to the scaled pixmap size
        QIcon icon(scaledPixmap);
        setIcon(icon);
        setIconSize(scaledPixmap.size());

        // Resize the widget to fit the scaled image size
        this->resize(scaledPixmap.size());

        // Optional: Remove padding and margins
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
    } else {
        this->setText(str());
    }
}

/* ************************************************************************* */
// JpointsChooser Implementation

JpointsChooser::JpointsChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, &JpointsChooser::toggle);
}

// Getters
QString JpointsChooser::str()
{
    return str_;
}

QString JpointsChooser::decision()
{
    return decision_;
}

void JpointsChooser::setStr()
{
    if (decision_ == "m") {
        str_ = "MINUS";
    } else if (decision_ == "p") {
        str_ = "PLUS";
    } else {
        str_ = "";
    }
}
// Methods
void JpointsChooser::toggle()
{
    decision_ = (decision_ == "m") ? "p" : "m";
    loadImage();
}

void JpointsChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
}

void JpointsChooser::toggleRandom(const QString& dec1, const QString& dec2)
{
    decision_ = (QRandomGenerator::global()->bounded(2) == 0) ? dec1 : dec2;
    loadImage();
}

void JpointsChooser::loadImage()
{
    setStr();

    QString imagePath = QString(":res/choosers/chooser_jpoints_%1.png").arg(decision_);
    QPixmap pixmap(imagePath); // Load the image as a QPixmap
    if (!pixmap.isNull() and not forAndroid) {
        // Fetch the size of the application's primary screen
        QSize screenSize = QApplication::primaryScreen()->size(); // Get the size of the primary screen
        QSize maxSize;

        // Calculate max size as a percentage of the screen size
        int height = screenSize.height() * 0.15; // 15% of the screen height
        int width = height * 0.5;                // 50% of height for the width
        maxSize = QSize(width, height);

        // Scale the image to fit within the maximum size while keeping aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the icon and icon size to the scaled pixmap size
        QIcon icon(scaledPixmap);
        setIcon(icon);
        setIconSize(scaledPixmap.size());

        // Resize the widget to fit the scaled image size
        this->resize(scaledPixmap.size());

        // Optional: Remove padding and margins
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
    } else {
        this->setText(str());
    }
}

// Slots
void JpointsChooser::onQuteDecisionChanged(const QString& dec)
{
    if (dec == "y") {
        setEnabled(true);
        show();
    } else {
        hide();
        setEnabled(false);
    }
    loadImage();
}

/* ************************************************************************* */
// RoundChooser Implementation

RoundChooser::RoundChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, [this]() {
        if (decision_ == "f") {
            emit finishRound();
            setDecision("r");
        } else if (decision_ == "r") {
            emit newRound();
        } else if (decision_ == "g") {
            emit newGame();
        }
    });
}

// Getters
QString RoundChooser::str()
{
    return str_;
}

QString RoundChooser::decision()
{
    return decision_;
}

void RoundChooser::setStr()
{
    if (decision_ == "f") {
        str_ = "FINISH";
    } else if (decision_ == "r") {
        str_ = "NEW ROUND";
    } else if (decision_ == "g") {
        str_ = "NEW GAME";
    } else {
        str_ = "";
    }
}
// Setters
void RoundChooser::setDecision(const QString& target_decision)
{
    decision_ = target_decision;
    loadImage();
}

// Methods
void RoundChooser::loadImage()
{
    setStr();

    QString imagePath = QString(":res/choosers/chooser_new_%1.png").arg(decision_);
    QPixmap pixmap(imagePath); // Load the image as a QPixmap
    if (!pixmap.isNull() and not forAndroid) {
        // Fetch the size of the application's primary screen
        QSize screenSize = QApplication::primaryScreen()->size(); // Get the size of the primary screen
        QSize maxSize;

        // Calculate max size as a percentage of the screen size
        int height = screenSize.height() * 0.15; // 15% of the screen height
        int width = height * 0.5;                // 50% of height for the width
        maxSize = QSize(width, height);

        // Scale the image to fit within the maximum size while keeping aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the icon and icon size to the scaled pixmap size
        QIcon icon(scaledPixmap);
        setIcon(icon);
        setIconSize(scaledPixmap.size());

        // Resize the widget to fit the scaled image size
        this->resize(scaledPixmap.size());

        // Optional: Remove padding and margins
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
    } else {
        this->setText(str());
    }
}

// Slots
void RoundChooser::onQuteDecisionChanged(const QString& dec)
{
    if (dec == "y") {
        setEnabled(true);
        show();
    } else {
        hide();
        setEnabled(false);
    }
}

/* ************************************************************************* */
