#ifndef H_PLEXUS_CONTROLLER
#define H_PLEXUS_CONTROLLER

#ifdef __cplusplus
extern "C" {
#endif

#include "controller.hpp"
#include <pthread.h>

class PlexusController: public Controller {
public:
    PlexusController();

    virtual ~PlexusController();

    void steering(int angle);

    void throttle(int percentage);
    void braking(int percentage);

    int turnON();
    void turnOFF();
private:
    VehicleController *vctrl;

    bool isOn;
    pthread_t threadID;
    struct Context context;
};

#ifdef __cplusplus
}
#endif

#endif  // H_PLEXUS_CONTROLLER