#pragma once

#include "core.hpp"
#include "Updater.hpp"

class Timer : public Core::Updater
{
public:
    Timer();

    virtual int update() override;

signals:
    SIGNAL(tick());
};
