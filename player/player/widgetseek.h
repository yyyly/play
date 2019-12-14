#ifndef WIDGETSEEK_H
#define WIDGETSEEK_H

#include <QWidget>
#include "playcore.h"

class QAbstractSlider;
class QLabel;
class QProgressBar;
class QTimer;

class DhPlayCore;



class WidgetSeek : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetSeek(QWidget *parent = nullptr);
    explicit WidgetSeek(PlayCore *core, QWidget *parent = nullptr);
    void setPlayCore(PlayCore *core);
    PlayCore* getPlayCore(){return _playCore;}
    void reSet();
signals:

public slots:
    void updateFullTime(int);
    void updateCurrentTime(int);
private:
    QAbstractSlider *_slider;
    QProgressBar *_progress;
    QLabel *_labelElapsed;
    QLabel *_labelTotal;
    PlayCore *_playCore;
    void initWidgetSeek(QWidget *slider);
};

#endif // WIDGETSEEK_H
