/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/25/2016 01:03:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Z.Shang (), shangzhanlin@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <ecl/ecl.h>

#define EVDEV "/dev/input/event16"

int main(int argc, char** argv){
    cl_boot(argc, argv);

    extern void init_lib_OPENMT(cl_object);
    ecl_init_module(NULL, init_lib_OPENMT);
    cl_eval(c_string_to_object("(test)"));

    cl_shutdown();

    struct input_event ev;
    int fd;
    char name[256] = "Unknown";

    fd = open(EVDEV, O_RDONLY);
    if(fd == -1){
        printf("Failed to open device\n");
        return EXIT_FAILURE;
    }
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    printf("Reading from: %s\n", name);

    for(;;){
        const size_t es = sizeof(struct input_event);
        ssize_t size;

        size = read(fd, &ev, es);
        if(size < es){
            printf("Size err\n");
            close(fd);
            return EXIT_FAILURE;
        }
        if(ev.type == EV_ABS){
            switch(ev.code){
                case ABS_MT_SLOT:
                    printf("Slot: %d\n", ev.value);
                    break;
                case ABS_MT_POSITION_X:
                case ABS_MT_POSITION_Y:
                    printf("Position %s: %d\n", ev.code == ABS_MT_POSITION_X ? "X" : "Y", ev.value);
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}
