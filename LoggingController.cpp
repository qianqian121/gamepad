#include "controller.hpp"
#include <pthread.h>

using namespace std;

void *threadLoop(void *sz)
{
    msg = context->messg;
    while (!context->exit_flag) {

    }
    pthread_exit(NULL);
}

LoggingController::LoggingController() : Controller() {
    isOn = false;
}

context;

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
        context.exitflag = false;
        std::cout << "turning on" << std::endl;
        pthread_t threadID;
        pthread_create(&threadID, NULL, threadLoop, (void *)context);
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
        std::cout << "Turning everying off complete" << std::endl;

        context->exit_flag = 1;
        rc = pthread_join(threads[i], &status);

        if (rc){
            cout << "Error:unable to join," << rc << endl;
            exit(-1);
        }

        cout << "Main: completed thread id :" << i ;
        cout << "  exiting with status :" << status << endl;
        pthread_exit(NULL);
        isOn = false;
    }
}