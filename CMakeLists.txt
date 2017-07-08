cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)
pkg_check_modules(LIBEVDEV libevdev)
pkg_check_modules(GLIB2 glib-2.0)

message(warning $(LIBEVEDV))

PROJECT(gamepad)
add_executable(${PROJECT_NAME} gamepad.cpp)

include_directories(
        ${LIBEVDEV_INCLUDE_DIRS}
        ${GLIB2_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        ${LIBEVDEV_LIBRARIES}
        ${GLIB2_LIBRARIES}
)