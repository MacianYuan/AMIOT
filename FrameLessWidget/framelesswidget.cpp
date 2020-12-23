#include <QWidget>
#include "framelesswidget.h"
#include "ui_framelesswidget.h"
#include <QGraphicsDropShadowEffect>

FramelessWidget::FramelessWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FramelessWidget)
{
    ui->setupUi(this);

//    setWindowFlags (Qt::FramelessWindowHint);

}

FramelessWidget::~FramelessWidget()
{
    delete ui;
}


void FramelessWidget::setContent(QWidget *w)
{
    QHBoxLayout *pHlayout = new QHBoxLayout;
    pHlayout->addWidget(w);
    pHlayout->setMargin(0);

}



