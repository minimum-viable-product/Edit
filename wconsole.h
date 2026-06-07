#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef _WIN32
    #ifndef __GNUC__
        #pragma comment(lib, "user32")
        /*#pragma warning(disable : 4996)*/  /* _CRT_SECURE_NO_WARNINGS */
    #endif
#endif


enum colors {
    BLACK_FG,
    BLUE_FG,
    GREEN_FG,
    AQUA_FG,
    RED_FG,
    PURPLE_FG,
    YELLOW_FG,
    WHITE_FG,
    GRAY_FG,
    LIGHT_BLUE_FG,
    LIGHT_GREEN_FG,
    LIGHT_AQUA_FG,
    LIGHT_RED_FG,
    LIGHT_PURPLE_FG,
    LIGHT_YELLOW_FG,
    BRIGHT_WHITE_FG,

    BLACK_BG=0,
    BLUE_BG=16,
    GREEN_BG=32,
    AQUA_BG=48,
    RED_BG=64,
    PURPLE_BG=80,
    YELLOW_BG=96,
    WHITE_BG=112,
    GRAY_BG=128,
    LIGHT_BLUE_BG=144,
    LIGHT_GREEN_BG=160,
    LIGHT_AQUA_BG=176,
    LIGHT_RED_BG=192,
    LIGHT_PURPLE_BG=208,
    LIGHT_YELLOW_BG=224,
    BRIGHT_WHITE_BG=240
};


#endif  /* CONSOLE_H */
