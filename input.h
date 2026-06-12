#ifndef INPUT_H
#define INPUT_H


enum device_type { IGNORED, KEYBOARD, MOUSE };
enum key {
    KEY_ESCAPE,
    KEY_ALT,
    KEY_F,
    KEY_E,
    KEY_S,
    KEY_V,
    KEY_O,
    KEY_H
};


struct keyboard {
    enum key key;
    int key_is_pressed;
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
