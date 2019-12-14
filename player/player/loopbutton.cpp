#include "loopbutton.h"

loopButton::loopButton(QWidget *parent)
    :QPushButton(parent)
{
    numVector.reserve(4);
    numVector << 1 << 2 << 4 << 8;
    suration = 0;
    num = numVector.at(suration);
}

void loopButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    if(++suration >= 4)
    {
        suration = 0;
    }
    setText(QString("%1X").arg(numVector.at(suration)));
    emit numChanged(numVector.at(suration));
}

void loopButton::setNum(int n)
{
    switch (n) {
    case 1:
    {
        suration = 0;
        setText(QString("%1X").arg(numVector.at(suration)));
        break;
    }
    case 2:
    {
        suration = 1;
        setText(QString("%1X").arg(numVector.at(suration)));
        break;
    }
    case 4:
    {
        suration = 2;
        setText(QString("%1X").arg(numVector.at(suration)));
        break;
    }
    case 8:
    {
        suration =3;
        setText(QString("%1X").arg(numVector.at(suration)));
        break;
    }
    default:
    break;

    }
}


