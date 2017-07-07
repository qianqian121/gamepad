#ifndef H_CONTROLLER
#define H_CONTROLLER

#ifdef __cplusplus
extern "C" {
#endif

class Controller {
public:
    Controller();
    virtual ~Controller();
    virtual void steering() = 0;
    virtual void throttle() = 0;
    virtual void braking() = 0;
};

class Logging_Controller: public Controller {
    Logging_Controller() : Controller() {

    }

    void steering(int angle) {
        std::cout << "Steering " << angle << std::endl;
    }
};

#ifdef __cplusplus
}
#endif

#endif  // H_CONTROLLER