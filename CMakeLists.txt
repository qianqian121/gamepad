cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)
pkg_check_modules(LIBEVDEV libevdev)
pkg_check_modules(GLIB2 glib-2.0)

PROJECT(gamepad)
add_executable(${PROJECT} gamepad.cpp)

TARGET_LINK_LIBRARIES(
        ${PROJECT}
)