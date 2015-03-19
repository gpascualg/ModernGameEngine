#pragma once

#include "core.h"
#include "Updater.h"

class Timer : public Core::Updater
{
public:
    Timer();

    virtual int update() override;

signals:
    void tick() SIGNAL;
};
