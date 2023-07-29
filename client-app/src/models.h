#pragma once

enum MessageType {
    Message = 1,
    Audio = 2,
    Joined = 4,
    Leaved = 8,
    ClientList = 16,
};

typedef struct Message {
    MessageType type;
    char value[1024];
};

typedef struct Client {
    int socket;
    char username[32] = "";
};