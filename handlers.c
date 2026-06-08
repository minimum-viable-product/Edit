#include <limits.h>
#include "input.h"
#include "wconsole.h"


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


void draw(struct bar * b);
void handle_mouse(struct mouse * m);
void focus_editor(void);


void (*handle_keyboard)(struct keyboard * k);
short g_text_cursor_row = 1;
short g_text_cursor_col = 0;


void draw(struct bar * bar)
{
    unsigned short bg_color = WHITE_BG;
    short i;

    for (i=0; i < bar->item_count; ++i) {
        write_at(bar->row, bar->items[i].col.first + 1, bar->items[i].text);
    }

    for (i=0; i < 80; ++i) {
        write_color_at(bar->row, i, &bg_color, 1);
    }
}


static void highlight_menu_title(struct bar_item * title)
{
    unsigned short attribute = WHITE_FG | BLACK_BG;
    unsigned short attribute_bright = BRIGHT_WHITE_FG | BLACK_BG;
    short i;

    for (i=title->col.first; i <= title->col.last; ++i) {
        write_color_at(0, i, &attribute, 1);
    }

    write_color_at(0, title->col.first + 1, &attribute_bright, 1);
}


static void menubar_focused(struct keyboard * keyboard)
{
    switch(keyboard->key) {
        case KEY_ALT:
            if ( ! keyboard->key_is_pressed) {
                /* unfocus menubar */
                draw(&g_menubar);
                focus_editor();
            }
            break;
        case KEY_ALT_F:
            break;
        case KEY_F:
            break;
        case KEY_E:
            break;
        case KEY_S:
            break;
        case KEY_V:
            break;
        case KEY_O:
            break;
        case KEY_H:
            break;
        case KEY_ESCAPE:
            if (keyboard->key_is_pressed) {
                /* unfocus menubar */
                draw(&g_menubar);
                focus_editor();
            }
            break;
    }
}


static void focus_menubar(void)
{
    handle_keyboard = menubar_focused;
    set_text_cursor_visibility(0);
    highlight_menu_title(&g_menubar.items[0]);
}


static void highlight_menubar_hotkeys(void)
{
    unsigned short foreground = BRIGHT_WHITE_FG | WHITE_BG;
    int i;

    for (i=0; i < g_menubar.item_count; ++i) {
        write_color_at(0, g_menubar.items[i].col.first + 1, &foreground, 1);
    }
}


/*static void invert_color(unsigned short attributes[], unsigned long length)
{
    unsigned long i;
    for (i=0; i < length; ++i) {
        attributes[i] ^= 127;
    }
}*/


static void editor_focused(struct keyboard * keyboard)
{
    switch(keyboard->key) {
        case KEY_ESCAPE:
            if (keyboard->key_is_pressed) { exit(0); }
            break;
        case KEY_ALT:
            if (keyboard->key_is_pressed) { highlight_menubar_hotkeys(); }
            else { focus_menubar(); }  /* alt released */
            break;
        case KEY_ALT_F:
            break;
        case KEY_F:
            break;
        case KEY_E:
            break;
        case KEY_S:
            break;
        case KEY_V:
            break;
        case KEY_O:
            break;
        case KEY_H:
            break;
    }
}


void focus_editor(void)
{
    flush_input();
    handle_keyboard = editor_focused;
    set_cursor_position(g_text_cursor_row, g_text_cursor_col);
    set_text_cursor_visibility(1);
}


void handle_mouse(struct mouse * p_mouse)
{
    (void) p_mouse;
}
