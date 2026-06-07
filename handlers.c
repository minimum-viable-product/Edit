#include <limits.h>
#include "input.h"


struct command {
    const char * text;
    const char * hint;
    int    hotkey_position;
    int    (*command)(void);
};


struct bar_item {
    const char * text;
    const char * hint;
    struct { short first; short last; } col;
    struct command * command;
};


struct bar {
    short row;
    short item_count;
    struct bar_item * items;
} g_menubar, g_statusbar;


void highlight_menubar_hotkeys(void)
{
    unsigned short foreground = BRIGHT_WHITE_FG | WHITE_BG;
    int i;

    for (i=0; i < g_menubar.item_count; ++i) {
        write_color_at(0, g_menubar.items[i].col.first + 1, &foreground, 1);
    }
}


void alt_command(struct keyboard * keyboard)
{
    static int is_menu_active = 0;
    static int are_menubar_hotkeys_bright = 0;

    if (keyboard->is_pressed
            && ! is_menu_active
            && ! are_menubar_hotkeys_bright)
    {
        highlight_menubar_hotkeys();
        are_menubar_hotkeys_bright = 1;
    }
}


void invert_color(unsigned short attributes[], unsigned long length)
{
    unsigned long i;
    for (i=0; i < length; ++i) {
        attributes[i] ^= 127;
    }
}


void handle_keyboard(struct keyboard * keyboard)
{
    if (keyboard->key == KEY_ESCAPE) {
        exit(0);
    } else if (keyboard->key == KEY_ALT) {
        alt_command(keyboard);
    }
}


void handle_mouse(struct mouse * p_mouse)
{
    /*static short last_row = SHRT_MAX;
    static short last_col = SHRT_MAX;
    unsigned short attributes[2];

    if (p_mouse->row != last_row || p_mouse->col != last_col)
    {
        read_color_at(last_row, last_col, attributes, 2);
        invert_color(attributes, 1);
        write_color_at(last_row, last_col, attributes, 2);

        read_color_at(p_mouse->row, p_mouse->col, attributes, 2);
        invert_color(attributes, 1);
        write_color_at(p_mouse->row, p_mouse->col, attributes, 2);

        last_row = p_mouse->row;
        last_col = p_mouse->col;
    }*/

        (void) p_mouse;
}
