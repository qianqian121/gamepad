#ifndef H_CONTROLLER
#define H_CONTROLLER

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
    virtual int turnON() = 0;
    virtual void turnOFF() = 0;
};

struct Context {
    bool exit_flag;
};

#ifdef __cplusplus
}
#endif

#endif  // H_CONTROLLER