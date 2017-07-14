//
// Created by Mike on 7/7/17.
//
#include "PlexusController.hpp"
#include <pthread.h>

int mycallback (void* messageTypeStruct)
{
    receivedSensorMessage = *(static_cast<CLIENT_DATA*>(messageTypeStruct));
    switch (receivedSensorMessage.updatedSensor)
    {
        case MessageType::VEHICLE:
            //  cout << "Vehicle velocity: " << VehicleController::getInstance().getVelocity(Unit::Velocity_Unit_KPH).velocity() <<endl;
            break;
        case MessageType::STEERING:
            //  cout << "Steering position: " << VehicleController::getInstance().Steering()->getHandWheelAngle().angle()  << endl;
            break;
        case MessageType::GEAR:
            //  cout << "Vehicle Gear position: " << VehicleController::getInstance().Gears()->getGearPosition().position() << endl;
            break;
        case MessageType::BRAKING:
            //  cout << "Vehicle brake position: " << VehicleController::getInstance().Brakes()->getPedalForce().percent() << " %" << endl;
            break;
        case MessageType::THROTTLE:
            //  cout << "Vehicle Throttle position: " << VehicleController::getInstance().Throttle()->getPedalPosition().percent() << " %" << endl;
            break;
        case MessageType::BENCH:
            // cout << "Bench Throttle position: " << VehicleController::getInstance().Bench()->getPedalPosition().percent() << " %" << endl;
            break;
        default:
            break;
    }
    return 0;
}

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

PlexusController::PlexusController() : Controller() {
    vctrl = &(VehicleController::getInstance());
    vctrl->registerCallback(mycallback);
    vctrl.waitSeconds(1);
}

PlexusController::~PlexusController() {
    turnOFF();
}

void PlexusController::steering(int angle) {
    if (vctrl->isMoving()) {
//    Direction direction;
//            direction.set_direction(Direction::CW);
        vctrl->Steering()->setAngle(angle, direction);
    }
    std::cout << "Steering " << angle << std::endl;
}

void PlexusController::throttle(int percentage) {

}
void PlexusController::braking(int percentage) {

}

int PlexusController::turnON() {
    if (!isOn) {
        if (VehicleController::getInstance().turnON() == ClientMessages::Fault::ERROR) {
            std::cout << "Error setting VehicleController ON state " << std::endl;
            return -1;
        }
        isOn = true;
        context.exit_flag = false;
        std::cout << "turning on" << std::endl;

        pthread_create(&threadID, NULL, threadLoop, (void *)&context);
    }
    return 0;
}

int setVelocity(int velocity) {
    this->PID(velocity);
}

void PlexusController::turnOFF() {
    if (isOn) {
        int rc = 0;
        void *status;

        vctrl->turnOFF();
        std::cout << "Turning everying off complete" << std::endl;
        vctrl->waitSeconds(1);

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