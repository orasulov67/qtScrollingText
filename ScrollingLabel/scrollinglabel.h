#ifndef SCROLLINGLABEL_H
#define SCROLLINGLABEL_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QColor>
#include <QFrame>

class ScrollingLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollingLabel(QWidget *parent = nullptr);
    ScrollingLabel(const QString& str, QWidget *parent = nullptr);
    ~ScrollingLabel();

    enum Mode {
        SingleContinuous = 0,
        SingleFullText = 1,
        MultiLineContinuous = 3,
        MultiLineFullText = 4,
    };

    QString text() const;
    void setText(const QString &text);


    QColor textColor() const;
    void setTextColor(const QColor &textColor);

    quint32 period() const;
    void setPeriod(quint32 period);

    quint32 offset() const;
    void setOffset(quint32 offset);

    int frameStyle() const;
    void setFrameStyle(int style);

    Mode mode() const;
    void setMode(Mode mode);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    bool isStarted() const;

    QFont font() const;
    void setFont(const QFont &font);

signals:
    void textChanged();

public slots:
    void start(quint32 period, quint32 offset);
    void start();
    void stop();

private:
    int mTimerId = -1;
    QLabel *mLabel = nullptr;
    QString mLabelText = "";
    QColor mTextColor;
    QColor mBackgroundColor;
    bool mStarted = false;
    quint32 mPeriodMs = 0;
    quint32 mOffset = 0;
    Mode mMode = Mode::SingleContinuous;

    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void updatePixMap();

};

#endif // SCROLLINGLABEL_H
