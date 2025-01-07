#define XO_MAIN user_main
#include "xo.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CHAN_WRITE 0
#define CHAN_DEAD 1
#define CHAN_READ 2

struct {
    int value;
    int state;
} channel = {0};

int chan_read(void) {
    switch (channel.state) {
    case CHAN_WRITE: {
        xo_yield();
        return chan_read();
    }
    case CHAN_READ: {
        channel.state = CHAN_WRITE;
        return channel.value;
    }
    case CHAN_DEAD: {
        fprintf(stderr, "PANIC: attempting to read from dead channel\n");
        exit(1);
    }
    default: {
        fprintf(stderr, "PANIC: corrupted channel state\n");
        exit(1);
    }
    }
}

void chan_write(int value) {
    switch (channel.state) {
    case CHAN_WRITE: {
        channel.state = CHAN_READ;
        channel.value = value;
    } break;
    case CHAN_READ: {
        xo_yield();
        chan_write(value);
    } break;
    case CHAN_DEAD: {
        fprintf(stderr, "PANIC: attempting to write to a dead channel\n");
        exit(1);
    } break;
    default: {
        fprintf(stderr, "PANIC: corrupted file state\n");
        exit(1);
    } break;
    }
}

void chan_close(void) {
    switch (channel.state) {
    case CHAN_WRITE: {
        channel.state = CHAN_DEAD;
    } break;
    case CHAN_READ: {
        xo_yield();
        chan_close();
    } break;
    case CHAN_DEAD: {
    } break;
    default: {
        fprintf(stderr, "PANIC: corrupted file state\n");
        exit(1);
    } break;
    }
}

bool chan_is_open(void) {
    switch (channel.state) {
    case CHAN_WRITE: {
        xo_yield();
        return chan_is_open();
    } break;
    case CHAN_READ: {
        return true;
    } break;
    case CHAN_DEAD: {
        return false;
    } break;
    default: {
        fprintf(stderr, "PANIC: corrupted file state\n");
        exit(1);
    } break;
    }
}

void producer(void* a) {
    (void)a;
    for (int i = 0; i < 10; i += 1) {
        printf("Producing: %i\n", i);
        chan_write(i);
    }
    chan_close();
}

int user_main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    printf("Hello World\n");

    while (chan_is_open()) {
        int value = chan_read();
        printf("Consumer: %d\n", value);
    }

    xo_routine_t routine = {
        .run = producer,
    };

    xo_routine_push(routine);

    return 0;
}
