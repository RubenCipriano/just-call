#pragma once

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <pthread.h>

#include <unistd.h>

#include <arpa/inet.h>

#define MAX_CLIENTS 100

#define MAX_CHANNELS 100

#define BUFFER_SIZE 2048


enum MessageType {
    Message = 1,
    Audio = 2,
    Joined = 4,
    Leaved = 8
};

typedef struct Message {
    enum MessageType type;
    char value[1024];
};

typedef struct {
    int socket;
    char username[32];
} client_t;

int num_clients[MAX_CHANNELS];

client_t clients[MAX_CHANNELS][MAX_CLIENTS];

pthread_mutex_t mutex;