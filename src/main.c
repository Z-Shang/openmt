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
#include <string.h>
#include <linux/input.h>
#include <ecl/ecl.h>

#include "openmt.h"

int main(int argc, char** argv){
    cl_boot(argc, argv);

    extern void init_lib_OPENMT(cl_object);
    ecl_init_module(NULL, init_lib_OPENMT);

    //Check Config File
    cl_object devname = cl_eval(c_string_to_object("(look-for-config-file)"));
    if(ecl_t_of(devname) != t_string){
        fprintf(stderr, "Invalid Config File, Please Check ~/openmt.conf\n");
        cl_shutdown();
        return EXIT_FAILURE;
    }
    char evname[devname->string.fillp];
    for(int i = 0; i < devname->string.fillp; i++){
        evname[i] = devname->string.self[i];
    }


    struct input_event ev;
    int fd;
    char name[256] = "Unknown";
    int evcount = 0;
    int errcount = 0;
    char evdev[256];
    do{
        sprintf(evdev, "/dev/input/event%d", evcount++);
        printf("Trying %s\n", evdev);
        fd = open(evdev, O_RDONLY);
        ioctl(fd, EVIOCGNAME(sizeof(name)), name);
        if(strcmp(name, evname) == 0){
            break;
        }
        if(fd == -1){
            if(errcount++ >= 1){
                printf("Failed to open device\n");
                return EXIT_FAILURE;
            }
        }
    }while(1);

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

    cl_shutdown();
    return EXIT_SUCCESS;
}
