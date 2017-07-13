#include "controller.hpp"
#include <pthread.h>

using namespace std;

void *threadLoop(void *ctx)
{
//    msg = context->messg;
    struct Context *context = (struct Context *)ctx;
    while (!context->exit_flag) {

    }
    pthread_exit(NULL);
}

LoggingController::LoggingController() : Controller() {
    isOn = false;
}

LoggingController::~LoggingController() {
    turnOFF();
}

void LoggingController::steering(int angle) {
    std::cout << "Steering " << angle << std::endl;
}

void LoggingController::throttle(int precentage) {
    std::cout << "Throttle " << precentage << std::endl;
}
void LoggingController::braking(int precentage) {
    std::cout << "Braking " << precentage << std::endl;
}

int LoggingController::turnON() {
    if (!isOn) {
        context.exit_flag = false;
        std::cout << "turning on" << std::endl;

        pthread_create(&threadID, NULL, threadLoop, (void *)&context);
        isOn = true;
    }
    return 0;
}
//
//int setVelocity(int velocity) {
//    this->PID(velocity);
//}

void LoggingController::turnOFF() {
    if (isOn) {
        int rc = 0;
        void *status;
        std::cout << "Turning everying off complete" << std::endl;

        context.exit_flag = 1;
        rc = pthread_join(threadID, &status);

        if (rc){
            cout << "Error:unable to join," << rc << endl;
            return;
        }

        cout << "Main: completed thread id :" << endl ;
        cout << "  exiting with status :" << status << endl;
        pthread_exit(NULL);
        isOn = false;
    }
}