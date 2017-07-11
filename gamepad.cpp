#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libevdev/libevdev.h>
#include <glib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "controller.hpp"

// A lot of the following code has been adapted from SDL

// Dependencies are glib and libevdev so compile with:
// gcc `pkg-config --cflags --libs libevdev glib-2.0` gamepad-libevdev-playground.c

void get_guid(struct libevdev *dev, guint16 * guid) {
    guid[0] = GINT16_TO_LE(libevdev_get_id_bustype(dev));
    guid[1] = 0;
    guid[2] = GINT16_TO_LE(libevdev_get_id_vendor(dev));
    guid[3] = 0;
    guid[4] = GINT16_TO_LE(libevdev_get_id_product(dev));
    guid[5] = 0;
    guid[6] = GINT16_TO_LE(libevdev_get_id_version(dev));
    guid[7] = 0;
}

void guid_to_string(guint16 * guid, char * guidstr) {
    static const char k_rgchHexToASCII[] = "0123456789abcdef";
    int i;
    for (i = 0; i < 8; i++) {
        unsigned char c = guid[i];

        *guidstr++ = k_rgchHexToASCII[c >> 4];
        *guidstr++ = k_rgchHexToASCII[c & 0x0F];

        c = guid[i] >> 8;
        *guidstr++ = k_rgchHexToASCII[c >> 4];
        *guidstr++ = k_rgchHexToASCII[c & 0x0F];
    }
    *guidstr = '\0';
}

int main () {
    int i;
    struct libevdev *dev = NULL;
    int fd;
    int rc = 1;

    // Detect the first joystick event file
    GDir* dir = g_dir_open("/dev/input/by-path/", 0, NULL);
    const gchar * fname;
    while ((fname = g_dir_read_name (dir))) {
        if (g_str_has_suffix (fname, "event-joystick")) break;
    }
    printf("Opening event file /dev/input/by-path/%s\n", fname);

    fd = open(g_strconcat("/dev/input/by-path/", fname, NULL), O_RDONLY|O_NONBLOCK);
    rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0) {
        fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
        exit(1);
    }
    printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
    printf("Input device ID: bus %#x vendor %#x product %#x\n",
            libevdev_get_id_bustype(dev),
            libevdev_get_id_vendor(dev),
            libevdev_get_id_product(dev));

    // GUID stuff
    guint16 guid[8];
    get_guid(dev, guid);

    char guidstr[33];
    guid_to_string(guid, guidstr);

    printf("Input device GUID %s\n", guidstr);

    // printf("KEY_MAX : %d\tBTN_JOYSTICK : %d\tBTN_MISC : %d\n", KEY_MAX, BTN_JOYSTICK, BTN_MISC);

    printf("\n\n");

    // Get info about buttons
    int nbuttons = 0;
    guint8 key_map[KEY_MAX];
    for (i = BTN_JOYSTICK; i < KEY_MAX; ++i) {
        if (libevdev_has_event_code(dev, EV_KEY, i)) {
            printf("%d - Joystick has button: 0x%x - %s\n", nbuttons, i,
                    libevdev_event_code_get_name(EV_KEY, i));
            key_map[i - BTN_MISC] = nbuttons;
            ++nbuttons;
        }
    }
    for (i = BTN_MISC; i < BTN_JOYSTICK; ++i) {
        if (libevdev_has_event_code(dev, EV_KEY, i)) {
            printf("%d - Joystick has button: 0x%x - %s\n", nbuttons, i,
                    libevdev_event_code_get_name(EV_KEY, i));
            key_map[i - BTN_MISC] = nbuttons;
            ++nbuttons;
        }
    }

    int nleds = 0;
    for (i = LED_NUML; i < LED_MAX; ++i) {
        if (libevdev_has_event_code(dev, EV_LED, i)) {
            printf("%d - Joystick has led: 0x%x - %s\n", nleds, i,
                   libevdev_event_code_get_name(EV_LED, i));
            ++nleds;
        }
    }

    printf("\n\n");

    // Get info about axes
    int naxes = 0;
    guint8 abs_map[ABS_MAX];
    for (i = 0; i < ABS_MAX; ++i) {
        /* Skip hats */
        if (i == ABS_HAT0X) {
            i = ABS_HAT3Y;
            continue;
        }
        if (libevdev_has_event_code(dev, EV_ABS, i)) {
            const struct input_absinfo *absinfo = libevdev_get_abs_info(dev, i);
            printf("Joystick has absolute axis: 0x%.2x - %s\n", i, libevdev_event_code_get_name(EV_ABS, i));
            printf("Values = { %d, %d, %d, %d, %d }\n",
                   absinfo->value, absinfo->minimum, absinfo->maximum,
                   absinfo->fuzz, absinfo->flat);
            abs_map[i] = naxes;
            ++naxes;
        }
    }

    printf("\n\n");

    // Get info about hats
    int nhats = 0;
    for (i = ABS_HAT0X; i <= ABS_HAT3Y; i += 2) {
        if (libevdev_has_event_code(dev, EV_ABS, i) || libevdev_has_event_code(dev, EV_ABS, i+1)) {
            const struct input_absinfo *absinfo = libevdev_get_abs_info(dev, i);
            if (absinfo == NULL) continue;

            printf("Joystick has hat %d\n", (i - ABS_HAT0X) / 2);
            printf("Values = { %d, %d, %d, %d, %d }\n",
                   absinfo->value, absinfo->minimum, absinfo->maximum,
                   absinfo->fuzz, absinfo->flat);
            ++nhats;
        }
    }

    printf("\n\n");

    struct timespec start_time;
    struct timespec end_time;
    long diffInNanos;
    clock_gettime(CLOCK_REALTIME, &start_time);
    Controller *ctrl = new LoggingController();
    int axis;
    int percentage;
    int angle;

    // Poll events
    do {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            clock_gettime(CLOCK_REALTIME, &end_time);
            diffInNanos = 1e6 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec);
//            printf("Diff time %ld\n", diffInNanos);
            switch (ev.type) {
            case EV_KEY:
                if (ev.code >= BTN_MISC) {
                    printf("Button %d\n", key_map[ev.code - BTN_MISC]);
                }
                break;
            case EV_ABS:
                switch (ev.code) {
                case ABS_HAT0X:
                case ABS_HAT0Y:
                case ABS_HAT1X:
                case ABS_HAT1Y:
                case ABS_HAT2X:
                case ABS_HAT2Y:
                case ABS_HAT3X:
                case ABS_HAT3Y:
                    ev.code -= ABS_HAT0X;
                    printf("Hat %d Axis %d Value %d\n", ev.code / 2, ev.code % 2, ev.value);
                    break;
                default:
                    printf("Axis %d Value %d\n", abs_map[ev.code], ev.value);
                    axis = abs_map[ev.code];
                    // Axis 132: Steering   F310    // 132: L2  0-16383
                    // Axis 136: Throttle  F310   // 133: L2    255-0
                    // Axis 137: Brake  F310    // 134: L2  255-0
                    switch (axis) {
                    case 0:
                        angle = (ev.value - 16384 / 2) * 900 / 16384;
                        ctrl->steering(angle);
                        break;
                    case 1:
                        percentage = 100 - ev.value * 100 / 255;
                        ctrl->throttle(percentage);
                        break;
                    case 2:
                        percentage = 100 - ev.value * 100 / 255;
                        ctrl->braking(percentage);
                        break;
                    default:
                        break;
                    }

                    break;
                }
                break;
            case EV_REL:
                switch (ev.code) {
                case REL_X:
                case REL_Y:
                    ev.code -= REL_X;
                    printf("Ball %d Axis %d Value %d\n", ev.code / 2, ev.code % 2, ev.value);
                    break;
                default:
                    break;
                }
                break;
            }
        }
    } while (rc == 1 || rc == 0 || rc == -EAGAIN);

    delete ctrl;
}