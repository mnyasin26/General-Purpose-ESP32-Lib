#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
}

SimpleTimer::~SimpleTimer()
{
}

void SimpleTimer::setInterval(unsigned long interval)
{
    this->interval = interval;
}

bool SimpleTimer::isEnable()
{
    return isEnabled;
}

void SimpleTimer::setCallback(SIMPLETIMER_CALLBACK aCallback)
{
    this->timerCallback = aCallback;
}

void SimpleTimer::setEnable(bool isEnabled)
{
    this->isEnabled = isEnabled;
}

void SimpleTimer::loop()
{
    if (isEnabled)
    {
        if (millis() - lastTime >= interval)
        {
            if (timerCallback != NULL)
            {
                timerCallback();
            }
            lastTime = millis();
        }
    }
}