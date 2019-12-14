#include "widgetseek.h"
#include "hkplaycore.h"
#include "dhplaycore.h"
#include <QSlider>
#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QTime>
#include <QPalette>
#include <QStyleOption>
#include <QPainter>

WidgetSeek::WidgetSeek(QWidget *parent)
    : QWidget(parent),
      _slider(nullptr),
      _progress(nullptr),
      _labelElapsed(nullptr),
      _labelTotal(nullptr),
      _playCore(nullptr)
{
    initWidgetSeek(nullptr);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background,Qt::gray);
    this->setPalette(palette);
    this->setStyleSheet("QWidget{background-color: rgb(80,80,80);color:white;}");
    _labelTotal->setStyleSheet("QLabel{background-color: rgb(80,80,80);color:white;}");
    _labelElapsed->setStyleSheet("QLabel{background-color: rgb(80,80,80);color:white;}");
}

WidgetSeek::WidgetSeek(PlayCore *core,QWidget *parent)
    :QWidget(parent),
      _slider(nullptr),
      _progress(nullptr),
      _labelElapsed(nullptr),
      _labelTotal(nullptr),
      _playCore(core)
{
    initWidgetSeek(nullptr);
}

void WidgetSeek::initWidgetSeek(QWidget *slider)
{
    if(slider == nullptr)
        slider = new QSlider();
    QAbstractSlider *sl = qobject_cast<QAbstractSlider *>(slider);
    _slider = sl;
    if(sl)
    {
        sl->setOrientation(Qt::Horizontal);
        sl->setMaximum(1);
        if(_playCore)
        {
            connect(sl,SIGNAL(valueChanged(int)),_playCore,SLOT(setPlayedTime(int)));
        }
    }
    QProgressBar *bar = qobject_cast<QProgressBar *>(slider);
        _progress = bar;
        if (bar) {
            bar->setOrientation(Qt::Horizontal);
            bar->setMaximum(1);
            bar->setTextVisible(false);
        }
    if(!_labelElapsed)
    {
        _labelElapsed = new QLabel(this);
    }
    _labelElapsed->setText("--:--");
    if(!_labelTotal)
    {
        _labelTotal = new QLabel(this);
    }
    _labelTotal->setText("--:--");
    delete layout();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(_labelElapsed);
    layout->addWidget(slider);
    layout->addWidget(_labelTotal);
    layout->setContentsMargins(6,0,6,0);
    setLayout(layout);
}

void WidgetSeek::reSet()
{
    if(_labelTotal != nullptr)
    {
        _labelTotal->setText("--:--");
    }
    if(_labelElapsed != nullptr)
    {
        _labelElapsed->setText("--:--");
    }
    if (_slider != nullptr) {
        _slider->blockSignals(true);
        _slider->setValue(0);
        _slider->blockSignals(false);
    }

}

void WidgetSeek::setPlayCore(PlayCore *core)
{
    if(core == nullptr)
        return;
    if(_playCore != nullptr)
    {
        disconnect(_playCore,SIGNAL(fileTimeChanged(int)),this,SLOT(updateFullTime(int)));
        disconnect(_playCore,SIGNAL(stampChanged(int)),this,SLOT(updateCurrentTime(int)));
        if(_slider != nullptr)
        {
            disconnect(_slider,SIGNAL(valueChanged(int)),_playCore,SLOT(setPlayedTime(int)));
        }
    }
    _playCore = core;
    connect(_playCore,SIGNAL(fileTimeChanged(int)),this,SLOT(updateFullTime(int)));
    connect(_playCore,SIGNAL(stampChanged(int)),this,SLOT(updateCurrentTime(int)));
    if(_slider)
    {
        connect(_slider,SIGNAL(valueChanged(int)),_playCore,SLOT(setPlayedTime(int)));
    }
}

void WidgetSeek::updateFullTime(int time)
{
    if (time == 0) {
            _labelTotal->setText("--:--");
        } else {
            QTime fullTime = QTime(0, 0);
            fullTime = fullTime.addMSecs(time * 1000);

            QString display = "mm:ss";
            if (fullTime.hour() > 0)
                display = "hh:mm:ss";

            _labelTotal->setText(fullTime.toString(display));
        }

        if (!time) {
            if (_slider != nullptr)
                _slider->setMaximum(1);
            else if (_progress)
                _progress->setMaximum(1);
            setVisible(true);
        } else {
            if (_slider != nullptr) {
                _slider->setMaximum(time * 1000);
                _slider->setSingleStep(1000);
                _slider->setPageStep(time / 100);
            }
            if (_progress)
                _progress->setMaximum(time);
            setVisible(true);
        }
}

void WidgetSeek::updateCurrentTime(int time)
{
    QTime currentTime = QTime(0, 0);
        currentTime = currentTime.addMSecs(time);

        QString display = "mm:ss";
        if (currentTime.hour() > 0)
            display = "hh:mm:ss";

        _labelElapsed->setText(currentTime.toString(display));
        if (_slider) {
            _slider->blockSignals(true);
            _slider->setValue(time);
            _slider->blockSignals(false);
        } else if (_progress) {
            _progress->setValue(time);
        }
}

