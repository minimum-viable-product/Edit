#ifndef INPUT_H
#define INPUT_H


enum device_type { KEYBOARD, MOUSE };
enum key { KEY_ESCAPE, KEY_ALT };


struct keyboard {
    enum key key;
    int is_pressed;
};


struct mouse {
    short row;
    short col;
    unsigned int button;
};


struct input {
    enum device_type type;
    union {
        struct keyboard keyboard;
        struct mouse mouse;
    } device;
};


#endif  /* INPUT_H */
