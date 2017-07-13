#ifndef H_CONTROLLER
#define H_CONTROLLER

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

class Controller {
public:
    Controller() {

    }
    virtual ~Controller() {

    }
    virtual void steering(int) = 0;
    virtual void throttle(int) = 0;
    virtual void braking(int) = 0;
};

struct Context {
    bool exitflag;
};

class LoggingController: public Controller {
public:
    LoggingController() : Controller() {

    }

    virtual ~LoggingController() {

    }

    void steering(int angle);

    void throttle(int precentage);
    void braking(int precentage);
private:
    bool isOn;
    struct Context context;
};

class PlexusController: public Controller {
public:
    PlexusController();

    virtual ~PlexusController();

    void steering(int angle);

    void throttle(int percentage) {

    }
    void braking(int percentage) {

    }
    int turnON();
    void turnOFF();
private:
    VehicleController *vctrl;
};

#ifdef __cplusplus
}
#endif

#endif  // H_CONTROLLER