#include "playcore.h"

PlayCore::PlayCore(QObject *parent)
    :QObject(parent),state(PLAYER_PAUSE),speedLeve(1),oneByoneState(OUT_OneByne)
{

}

PlayCore::~PlayCore()
{

}
