#include "scrollinglabel.h"

#include <QBoxLayout>
#include <QTimer>
#include <QPainter>
#include <QPalette>

#include <QDebug>

ScrollingLabel::ScrollingLabel(QWidget *parent)
    : QWidget{parent}
{

    mTextColor = this->palette().color(QPalette::WindowText);
    mBackgroundColor = this->palette().color(QPalette::Window);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    mLabel = new QLabel(this);
    mLabel->setFont(font());
    mLabel->setPalette(palette());

    mLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mLabel->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    layout->addWidget(mLabel);

    mStarted = false;
}

ScrollingLabel::ScrollingLabel(const QString &str, QWidget *parent)
    : ScrollingLabel(parent)
{
    mLabelText = str;
    updatePixMap();
}

ScrollingLabel::~ScrollingLabel()
{
    stop();
}

QString ScrollingLabel::text() const
{
    return mLabelText;
}

void ScrollingLabel::setText(const QString &text)
{
    mLabelText = text;
    updatePixMap();
    emit textChanged();
}

QColor ScrollingLabel::textColor() const
{
    return mTextColor;
}

void ScrollingLabel::setTextColor(const QColor &textColor)
{
    mTextColor = textColor;
    updatePixMap();
}

quint32 ScrollingLabel::period() const
{
    return mPeriodMs;
}

void ScrollingLabel::setPeriod(quint32 period)
{
    mPeriodMs = period;
    if(mStarted) {
        stop();
        mTimerId = startTimer(mPeriodMs);
        mStarted = true;
    }
}

quint32 ScrollingLabel::offset() const
{
    return mOffset;
}

void ScrollingLabel::setOffset(quint32 offset)
{
    mOffset = offset;
}

int ScrollingLabel::frameStyle() const
{
    return mLabel->frameStyle();
}

void ScrollingLabel::setFrameStyle(int style)
{
    mLabel->setFrameStyle(style);
}

void ScrollingLabel::start(quint32 period, quint32 offset)
{
    mPeriodMs = period;
    mOffset = offset;
    start();
}

void ScrollingLabel::start()
{
    if(mPeriodMs > 0) {
        if(mStarted) {
            stop();
        }
        mTimerId = startTimer(mPeriodMs);
        mStarted = true;
    }
}

void ScrollingLabel::stop()
{
    if(mTimerId > 0) {
        killTimer(mTimerId);
        mTimerId = -1;
    }
    mStarted = false;
}

QColor ScrollingLabel::backgroundColor() const
{
    return mBackgroundColor;
}

void ScrollingLabel::setBackgroundColor(const QColor &color)
{
    mBackgroundColor = color;
}

bool ScrollingLabel::isStarted() const
{
    return mStarted;
}

QFont ScrollingLabel::font() const
{
    return mLabel->font();
}

void ScrollingLabel::setFont(const QFont &font)
{
    mLabel->setFont(font);
    updatePixMap();
}

ScrollingLabel::Mode ScrollingLabel::mode() const
{
    return mMode;
}

void ScrollingLabel::setMode(ScrollingLabel::Mode mode)
{
    mMode = mode;
    updatePixMap();
}

void ScrollingLabel::timerEvent(QTimerEvent *event)
{
    if(!mStarted || mPeriodMs == 0 || mOffset == 0) {
        return;
    }
    if(mTimerId <= 0 || mTimerId != event->timerId()) {
        return;
    }
    if(mLabel == nullptr || mLabelText.isEmpty()) {
        return;
    }

    QPixmap&& originalPixmap = mLabel->pixmap();
    int width = originalPixmap.width();
    int height = originalPixmap.height();

    int splitPosition = static_cast<int>(mOffset);

    QRect leftRect(0, 0, splitPosition, height);
    QPixmap leftPixmap = originalPixmap.copy(leftRect);

    QRect rightRect(splitPosition, 0, width - splitPosition, height);
    QPixmap rightPixmap = originalPixmap.copy(rightRect);

    QPixmap combinedPixmap(originalPixmap.size());
    combinedPixmap.fill(Qt::transparent);
    QPainter painter(&combinedPixmap);

    painter.drawPixmap(0, 0, rightPixmap);
    painter.drawPixmap(width - splitPosition, 0, leftPixmap);
    painter.end();

    mLabel->setPixmap(combinedPixmap);
}

void ScrollingLabel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    mLabel->resize(size());
    updatePixMap();
}

void ScrollingLabel::updatePixMap()
{
    int width;
    int height = mLabel->height();
    QFont font(mLabel->font());
    if(mLabelText.isEmpty()) {
        width = mLabel->width();
    } else {
        QFontMetrics metrics(font);
        QRect&& textRect = metrics.boundingRect(mLabelText);
        switch (mMode) {
        case SingleContinuous:
            width = textRect.width() < mLabel->width() ? mLabel->width() : textRect.width() + mLabel->width() / 2;
            break;
        case SingleFullText:
            width = textRect.width() + mLabel->width();
            break;
        default: // other modes are not yet implemented
            width = mLabel->width();
            break;
        }
    }

    QPixmap pixMap(width, height);
    pixMap.fill(mBackgroundColor);
    if(!mLabelText.isEmpty()) {
        QPainter painter(&pixMap);
        painter.setFont(font);
        painter.setPen(mTextColor);
        painter.drawText(pixMap.rect(), Qt::AlignLeft | Qt::AlignVCenter, mLabelText);
        painter.end();
    }
    mLabel->setPixmap(pixMap);
}

