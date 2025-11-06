#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowTitle("Scrolling Text Demo");

    // ================ Static UI settings ==============================
    ui->scrollingText_1->setText("Long text that is longer than the width of the label: SingleFullText mode");
    ui->scrollingText_2->setText("Long text that is longer than the width of the label: SingleContinuous mode");
    ui->scrollingText_3->setText("Short text: SingleFullText mode");
    ui->scrollingText_4->setText("Short text: SingleContinuous mode");

    ui->scrollingText_1->setTextColor(QColor(0xff, 0x00, 0x00));
    ui->scrollingText_2->setTextColor(QColor(0x00, 0xff, 0x00));
    ui->scrollingText_3->setTextColor(QColor(0x00, 0x27, 0x61));
    ui->scrollingText_4->setTextColor(QColor(0xaa, 0x44, 0x13));

    ui->scrollingText_3->setBackgroundColor(QColor(0x8f, 0x59, 0x02));

    ui->scrollingText_1->setFrameStyle(QFrame::Panel | QFrame::Raised);
    ui->scrollingText_2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    ui->scrollingText_3->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui->scrollingText_4->setFrameStyle(QFrame::Box | QFrame::Raised);

    ui->scrollingText_1->setMode(ScrollingLabel::SingleFullText);
    ui->scrollingText_2->setMode(ScrollingLabel::SingleContinuous);
    ui->scrollingText_3->setMode(ScrollingLabel::SingleFullText);
    ui->scrollingText_4->setMode(ScrollingLabel::SingleContinuous);

    ui->scrollingText_1->start(80, 2);
    ui->scrollingText_2->start(80, 2);
    ui->scrollingText_3->start(35, 1);
    ui->scrollingText_4->start(20, 1);

    ui->pushButton_1->setText("Stop");
    ui->pushButton_2->setText("Stop");
    ui->pushButton_3->setText("Stop");
    ui->pushButton_4->setText("Stop");

    connect(ui->pushButton_1, &QPushButton::clicked, this, [=]() {
        switchStartStop(ui->pushButton_1, ui->scrollingText_1);
    });
    connect(ui->pushButton_2, &QPushButton::clicked, this, [=]() {
        switchStartStop(ui->pushButton_2, ui->scrollingText_2);
    });
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
        switchStartStop(ui->pushButton_3, ui->scrollingText_3);
    });
    connect(ui->pushButton_4, &QPushButton::clicked, this, [=]() {
        switchStartStop(ui->pushButton_4, ui->scrollingText_4);
    });

    // ================ Adding dynamically ==============================
    ScrollingLabel *scrollingText_5 = new ScrollingLabel("Dynamic added label", this);
    scrollingText_5->setGeometry(50, 300, 570, 50);
    scrollingText_5->setBackgroundColor(QColor(0x30, 0x30, 0x30));
    scrollingText_5->setTextColor(QColor(0xa0, 0xa0, 0xa0));
    scrollingText_5->start(30, 1);
    scrollingText_5->setFont(QFont("Arial", 14, QFont::Normal));

    QPushButton *pushButton_5 = new QPushButton("Stop", this);
    pushButton_5->setGeometry(650, 300, 60, 50);

    connect(pushButton_5, &QPushButton::clicked, this, [=]() {
        switchStartStop(pushButton_5, scrollingText_5);
    });
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::switchStartStop(QPushButton *button, ScrollingLabel *label)
{
    if(button == nullptr || label == nullptr)
        return;
    if(label->isStarted()) {
        label->stop();
        button->setText("Start");
    } else {
        label->start();
        button->setText("Stop");
    }
}







