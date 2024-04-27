#ifndef KEY_UTIL_H_
#define KEY_UTIL_H_

#include <stdbool.h>
#include <stdint.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define UNKNOWN_KEY "\0"

#define IS_SHIFT(code) (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) ? 1 : 0

void check_root_user();
char *get_keyboard_device_file();
FILE *open_log_file(const char *file);
FILE *open_keyboard_device_file(char *deviceFile);
const char *get_key_stroke(uint16_t code, uint8_t shift_pressed);

#endif