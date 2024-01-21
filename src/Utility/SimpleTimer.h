#include <main_header.h>

#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#define SIMPLETIMER_CALLBACK std::function<void()> callback


class SimpleTimer
{
private:
    unsigned long lastTime;
    unsigned long interval;
    bool isRunning;
    void (*callback)();
    void (*callbackWithArg)(void *);
    bool isEnable;
    SIMPLETIMER_CALLBACK;

public:
    SimpleTimer(/* args */);
    void setInterval(unsigned long interval);
    void setCallback(SIMPLETIMER_CALLBACK);
    // void setCallback(void (*callback)(void *));
    void setEnable(bool isEnable);
    bool isEnable();
    void loop();
    ~SimpleTimer();
};

SimpleTimer::SimpleTimer(/* args */)
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
    return isEnable;
}

void SimpleTimer::setInterval(unsigned long interval)
{
    this->interval = interval;
}

void SimpleTimer::setCallback(SIMPLETIMER_CALLBACK)
{
    this->callback = callback;
}

// void SimpleTimer::setCallback(void (*callback)(void *))
// {
//     this->callbackWithArg = callback;
// }

void SimpleTimer::setEnable(bool isEnable)
{
    this->isEnable = isEnable;
}

void SimpleTimer::loop()
{
    if (isEnable)
    {
        if (millis() - lastTime >= interval)
        {
            if (callback != NULL)
            {
                callback();
            }
            else if (callbackWithArg != NULL)
            {
                callbackWithArg(NULL);
            }
            lastTime = millis();
        }
    }
}

#endif