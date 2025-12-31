CC     := gcc
CFLAGS := -Wall -Wextra -O2
TARGET := mrtn

SRCS   := $(shell find . -type f -name "*.c")
OBJS   := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
