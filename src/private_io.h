#ifndef PRIVATE_IO_H
#define PRIVATE_IO_H
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))

typedef struct AC483_Ctrl_Msg {
    uint8_t ctrl_h;
    uint8_t ctrl_l;
    uint8_t msg_h;
    uint8_t msg_l;
} ac483_ctrl_msg_t;

#endif
