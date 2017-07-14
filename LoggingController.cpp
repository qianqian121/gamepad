#include "LoggingController.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
#define DPRINT

void *threadLoop(void *ctx)
{
//    msg = context->messg;
    struct Context *context = (struct Context *)ctx;
    while (!context->exit_flag) {
        cout << "." << endl;
        sleep(1);
    }
    return(NULL);
//    pthread_exit(NULL);
}

LoggingController::LoggingController() : Controller() {
    isOn = false;
}

LoggingController::~LoggingController() {
    turnOFF();
}

void LoggingController::steering(int angle) {
#ifdef DPRINT
    std::cout << "Steering " << angle << std::endl;
#endif
}

void LoggingController::throttle(int precentage) {
#ifdef DPRINT
    std::cout << "Throttle " << precentage << std::endl;
#endif
}
void LoggingController::braking(int precentage) {
#ifdef DPRINT
    std::cout << "Braking " << precentage << std::endl;
#endif
}

int LoggingController::turnON() {
    if (!isOn) {
        isOn = true;
        context.exit_flag = false;
        std::cout << "turning on" << std::endl;

        pthread_create(&threadID, NULL, threadLoop, (void *)&context);
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
        isOn = false;
        cout << "Main: completed thread, exiting with status :" << status << endl;
    }
}