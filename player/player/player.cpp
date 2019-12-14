#include "player.h"
#include "ui_player.h"
#include "error.h"

#include <QStyleOption>
#include <QPainter>

Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player),
    dhCore(new DhPlayCore(this)),
    hkCore(new HkPlayCore(this)),
    core(nullptr),
    voiceState(0),
    label(new QLabel(this))

{
    ui->setupUi(this); 
    ui->titleWidget->installEventFilter(this);
    this->setAttribute(Qt::WA_Mapped);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPlay.png"),4);
    setButtonStyle(ui->nextFrameButton,QString(":/images/ToolbarPlayback.png"),4);
    setButtonStyle(ui->voiceSwitchButton,QString(":/images/ToolbarSilence.png"),4);
    setButtonStyle(ui->closeButton,QString(":/images/close.png"),4);
    ui->nextFrameButton->setToolTip("下一帧");
    ui->playSpeedButton->setToolTip("播放速度");
    dhCore->setWidget(ui->playWidget);
    hkCore->setWidget(ui->playWidget);
    this->setStyleSheet("QWidget{background-color: rgb(80,80,80);color:white;}");
    //Error *error= Error::getInstance();
    //connect(error,SIGNAL(errorSended(DWORD,const QString)),this,SLOT(haveError(DWORD,const QString)));
    setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPause.png"),4);

    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    label->setStyleSheet("QLabel{border:none;color:red;font:14px}");
    label->raise();
    label->hide();
    ui->playPauseButton->setDefault(true);
    connect(ui->playSpeedButton,SIGNAL(numChanged(int)),this,SLOT(on_playSpeedButton_clicked(int)));
}

Player::~Player()
{
    delete ui;
}


void Player::setMold(int m)
{

}
void Player::reSet()
{
    ui->seekWidget->reSet();
    ui->playSpeedButton->setNum(1);
    setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPlay.png"),4);
}

void Player::play(const QString fileName, const int mold)
{
    if(core)
    {
        core->stop();
        core->closeFile();
        core->free();
    }
    if(mold == 0)
    {
        core = hkCore;
    }
    else
    {
        core = dhCore;
    }
    if(ui->seekWidget->getPlayCore() != core)
    {
        ui->seekWidget->setPlayCore(core);
    }
    label->hide();
    if(core->play(fileName))
    {
       setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPause.png"),4);
    }
}



bool Player::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->titleWidget)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            switch(mouseEvent->type())
            {
            case QEvent::MouseButtonPress:
                {
                    if(mouseEvent->button() == Qt::LeftButton)
                    {
                        mouseIsPress = true;
                        mousePos = mouseEvent->pos();
                    }
                }
                    break;
            case QEvent::MouseMove:
                {
                    move(mouseEvent->pos() - mousePos  + pos());
                }
                    break;
            case QEvent::MouseButtonRelease:
                {
                    mouseIsPress = false;
                }
                    break;

            }
           return false;
        }
        return false;
}

void Player::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    this->setAttribute(Qt::WA_Mapped);
    if(label->isVisible())
    {
        label->move((this->width() - label->width()) / 2,(this->height() - label->height()) / 2);
    }
}

void Player::on_playPauseButton_clicked()
{
    PlayState state = core->getState();
    if(state == PLAYER_PLAY)
    {
        core->pause();
        setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPlay.png"),4);
        ui->playPauseButton->setToolTip("播放");
    }
    else {

        core->rePlay();
        setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPause.png"),4);
        ui->playPauseButton->setToolTip("暂停");
    }
}

void Player::on_nextFrameButton_clicked()
{
    core->oneByOne();
    setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPlay.png"),4);
    //ui->playSpeedButton->setNum(1);
}

void Player::on_playSpeedButton_clicked(int speed)
{
    setButtonStyle(ui->playPauseButton,QString(":/images/ToolbarPause.png"),4);
    core->setPlaySpeed(speed);
    if(speed != 1)
    {
        setButtonStyle(ui->voiceSwitchButton,QString(":/images/ToolbarSilence.png"),4);
    }
    else if(speed == 1 && voiceState == 1)
    {
        setButtonStyle(ui->voiceSwitchButton,":/images/ToolbarVoicechange.png",4);
    }
    else {
        setButtonStyle(ui->voiceSwitchButton,QString(":/images/ToolbarSilence.png"),4);
    }
}

void Player::setButtonStyle(QPushButton *button, QString imgsrc, int CutSec)
{
    int img_w=QPixmap(imgsrc).width();
        int img_h=QPixmap(imgsrc).height();
        int PicWidth = img_w/CutSec;
        button->setFixedSize(PicWidth,img_h);
        button->setStyleSheet(QString("QPushButton{border-width: 41px; border-image: url(%1) 0 0 0 %2 repeat repeat;border-width: 0px; border-radius: 0px;}")
        .append("QPushButton::hover{border-image: url(%1) 0 0 0 %3 repeat repeat;}")
        .append("QPushButton::pressed{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
        .append("QPushButton::checked{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
        .append("QPushButton::disabled{border-image: url(%1) 0 0 0 %5 repeat repeat;}")
        .arg(imgsrc).arg(0).arg(PicWidth*1).arg(PicWidth*2).arg(PicWidth*3));
}

void Player::on_closeButton_clicked()
{
    core->stop();
    core->closeFile();
    core->free();
    this->close();
}

void Player::on_voiceSwitchButton_clicked()
{
    if(voiceState == 0)
    {
        voiceState = 1;
        setButtonStyle(ui->voiceSwitchButton,":/images/ToolbarVoicechange.png",4);
        core->playSound();
    }
    else {
        voiceState = 0;
        setButtonStyle(ui->voiceSwitchButton,QString(":/images/ToolbarSilence.png"),4);
        core->stopSound();
    }
}

void Player::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Player::haveError(int e, const QString describle)
{
    switch (e) {
    case 7:
    {
        label->setText(describle);
        label->adjustSize();
        label->show();
    }
        break;
    default:
        break;
    }
}
