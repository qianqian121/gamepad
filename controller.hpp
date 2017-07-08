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
    virtual void throttle() = 0;
    virtual void braking() = 0;
};

class LoggingController: public Controller {
public:
    LoggingController() : Controller() {

    }

    virtual ~LoggingController() {

    }

    void steering(int angle) {
        std::cout << "Steering " << angle << std::endl;
    }

    void throttle() {

    }
    void braking() {

    }
};

class PlexusController: public Controller {
public:
    PlexusController();

    virtual ~PlexusController();

    void steering(int angle);

    void throttle() {

    }
    void braking() {

    }
};

#ifdef __cplusplus
}
#endif

#endif  // H_CONTROLLER