#ifndef LOOPBUTTON_H
#define LOOPBUTTON_H
#include <QPushButton>
#include <QVector>
#include <QMouseEvent>

class loopButton : public QPushButton
{
    Q_OBJECT
 public:
    loopButton(QWidget *parent);
    int getNum() {return num;}
    void setNum(int n);
private:
    QVector<int> numVector;
    int num;
    int suration;
protected:
    void mousePressEvent(QMouseEvent *e);
signals:
    void numChanged(int n);
};

#endif // LOOPBUTTON_H
