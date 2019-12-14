#ifndef PLAYER_H
#define PLAYER_H

#if defined(PLAYER_LIBRARY)
#  define PLAYERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLAYERSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>
#include <QPushButton>
#include <QPoint>
#include <QLabel>
#include "dhplaycore.h"
#include "hkplaycore.h"

namespace Ui {
class Player;
}

class PLAYERSHARED_EXPORT Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();
    void play(const QString fileName,const int mold = 0);
    void reSet();
    void setMold(int m);
protected:
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event);
private slots:
    void on_playPauseButton_clicked();

    void on_nextFrameButton_clicked();

    void on_playSpeedButton_clicked(int speed);

    void on_closeButton_clicked();

    void on_voiceSwitchButton_clicked();
public slots:
    void haveError(int e,const QString describle);

private:
    Ui::Player *ui;
    DhPlayCore *dhCore;
    HkPlayCore *hkCore;
    PlayCore *core;
    void setButtonStyle(QPushButton *button, QString imgsrc, int CutSec);
    QPoint mousePos;
    bool mouseIsPress;
    int voiceState;
    QLabel *label;
    int mold;
};

#endif // PLAYER_H
