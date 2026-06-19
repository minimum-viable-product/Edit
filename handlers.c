#include <limits.h>

#include "input.h"
#include "wconsole.h"
#include "menus.h"
#include "handlers.h"


short g_text_cursor_row = 1;
short g_text_cursor_col = 0;


void draw(const struct bar * bar)
{
    unsigned short attributes[80];
    char characters[80];
    short i;

    for (i=0; i < 80; ++i) {
        attributes[i] = WHITE_BG | BLACK_FG;
    }
    memset(characters, ' ', 80);

    for (i=0; i < bar->item_count; ++i) {
        memcpy(
                &characters[bar->items[i].col.first + 1],
                bar->items[i].text,
                bar->items[i].col.length - 2
        );
    }

    write_characters_at(bar->row, 0, characters, 80);
    write_attributes_at(bar->row, 0, attributes, 80);
}


void show_statusbar_hint(const char * string)
{
    char characters[80] = { ' ' };
    size_t length = strlen(string);

    memcpy(&characters[1], string, length);
    memset(&characters[length + 1], ' ', 80 - length - 1);
    write_characters_at(g_statusbar.row, 0, characters, 80);
}


static void highlight_menu_title(const struct bar_item * title)
{
    unsigned short attributes[9];
    unsigned int i;

    read_color_at(0, title->col.first, attributes, title->col.length);
    for (i=0; i < title->col.length; ++i) {
        attributes[i] |= 0x77;
        attributes[i] ^= 0x70;
    }
    write_attributes_at(0, title->col.first, attributes, title->col.length);
}


void menu_focused(struct keyboard * keyboard)
{
    switch (keyboard->key) {
        case KEY_ESCAPE:
            if (keyboard->key_is_pressed) {
                /* unfocus menu */
                /*unpaint_menu*/
                draw(&g_menubar);
                focus_editor();
            }
            break;
        default: break;
    }
}


static void focus_menu(enum menubar_items menu)
{
    char character_buffer[512];

    memset(character_buffer, ' ', 512);
    build_menu(menu, character_buffer);
    draw_window_at(
            1,  /* row */
            g_menubar.items[menu].col.first - 1,  /* col */
            character_buffer,
            g_menubar.items[menu].menu.command_count + 2,  /* row count */
            g_menubar.items[menu].menu.width  /* column count */
    );
    handle_keyboard = menu_focused;
}


static void menubar_focused(struct keyboard * keyboard)
{
    switch (keyboard->key) {
        case KEY_ALT:
            if ( ! keyboard->key_is_pressed) {
                /* unfocus menubar */
                draw(&g_menubar);
                focus_editor();
            }
            break;
        case KEY_F:
            focus_menu(FILE_MENU);
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
    highlight_menu_title(&g_menubar.items[FILE_MENU]);
    show_statusbar_hint(g_menubar.items[FILE_MENU].hint);
}


static void menubar_prefocused(struct keyboard * keyboard)
{
    switch (keyboard->key) {
        case KEY_ALT:
            if ( ! keyboard->key_is_pressed) { focus_menubar(); }
            break;
        case KEY_F:
            focus_menu(FILE_MENU);
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
        default: break;
    }
}


static void highlight_menubar_hotkeys(void)  /* TODO: Optimize */
{
    unsigned short foreground = BRIGHT_WHITE_FG | WHITE_BG;
    int i;

    for (i=0; i < g_menubar.item_count; ++i) {
        write_attributes_at(
                0,
                g_menubar.items[i].col.first + 1,
                &foreground,
                1
        );
    }
}


static void editor_focused(struct keyboard * keyboard)
{
    switch (keyboard->key) {
        case KEY_ESCAPE:
            if (keyboard->key_is_pressed) { exit(0); }
            break;
        case KEY_ALT:
            if (keyboard->key_is_pressed) { highlight_menubar_hotkeys(); }
            handle_keyboard = menubar_prefocused;
            break;
        default: break;
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
