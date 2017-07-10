//
// Created by Mike on 7/7/17.
//
#include "controller.hpp"

PlexusController::PlexusController() : Controller() {
    vctrl = &(VehicleController::getInstance());
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

int PlexusController::turnON() {
    if (VehicleController::getInstance().turnON() == ClientMessages::Fault::ERROR)
    {
        cout << "Error setting VehicleController ON state " << endl;
        return -1;
    }
    cout << "turning on" << endl;
    return 0;
}

void PlexusController::turnOFF() {
    vctrl->turnOFF();
    cout << "Turning everying off complete" << endl;
    vctrl->waitSeconds(1);
}