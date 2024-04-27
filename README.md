# Keylogger
A Key Logger in Linux (developed in C).
The Keyboard driver file is a character device file.
This program reads the keyboard driver files under `/dev/input` and parses the contents of each key press & outputs it to `/root/keylog.out`.
The program runs in the background as a demonized process.

# Usage
1. Clone the repository `git clone https://github.com/heeth16/keylogger.git && cd keylogger`

2. Make the executable `make`

3. Run the executable as root `sudo ./keylogger`

4. Keys for any user on the system gets logger under `/root/keylog.out`