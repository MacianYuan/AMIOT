#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLayout>

namespace Ui {
class FramelessWidget;
}

class FramelessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWidget(QWidget *parent = 0);
    virtual ~FramelessWidget();
    void setContent(QWidget *w);
private slots:

protected:

private:
    Ui::FramelessWidget *ui;
};


#endif // FRAMELESSWIDGET_H
