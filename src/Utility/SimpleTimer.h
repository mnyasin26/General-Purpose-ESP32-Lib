#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include <main_header.h>

#ifndef SIMPLETIMER_CALLBACK
#include <functional>
#define SIMPLETIMER_CALLBACK std::function<void()>
#endif

class SimpleTimer
{
private:
    unsigned long lastTime;
    unsigned long interval;
    bool isRunning;
    bool isEnabled;
    SIMPLETIMER_CALLBACK timerCallback;

public:
    SimpleTimer();
    void setInterval(unsigned long interval);
    void setCallback(SIMPLETIMER_CALLBACK aCallback);
    void setEnable(bool isEnable);
    bool isEnable();
    void loop();
    ~SimpleTimer();
};



#endif