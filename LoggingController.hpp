#ifndef H_LOGGING_CONTROLLER
#define H_LOGGING_CONTROLLER

#ifdef __cplusplus
extern "C" {
#endif

#include "controller.hpp"
#include <pthread.h>

class LoggingController: public Controller {
public:
    LoggingController();

    virtual ~LoggingController();

    void steering(int angle);

    void throttle(int precentage);
    void braking(int precentage);

    int turnON();
    void turnOFF();
private:
    bool isOn;
    pthread_t threadID;
    struct Context context;
};

#ifdef __cplusplus
}
#endif

#endif  // H_LOGGING_CONTROLLER