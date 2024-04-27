TARGET := keylogger

CC=gcc
CFLAGS=-c -Wall

SRC_DIR := src
INCLUDE_DIR := $(SRC_DIR)/include

SRCS := $(wildcard $(SRC_DIR)/*.c)
SRCS += $(wildcard $(INCLUDE_DIR)/*.c)
OBJS := $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	@$(CC) -o $(TARGET) $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJS)
