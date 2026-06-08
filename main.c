/*#include <stdio.h>*/

#include "input.h"
#include "wconsole.c"
#include "handlers.c"


void create_bars(void)
{
    g_menubar.row = 0;
    g_menubar.item_count = 6;
    g_menubar.items = malloc(
            (size_t)g_menubar.item_count * sizeof(struct bar_item)
    );
    if (g_menubar.items != NULL) {
        g_menubar.items[0].text = "File";
        g_menubar.items[0].hint = "Commands for manipulating files";
        g_menubar.items[0].col.first = 2;
        g_menubar.items[0].col.last = 7;
        g_menubar.items[0].command = NULL;

        g_menubar.items[1].text = "Edit";
        g_menubar.items[1].hint = "Commands for editing files";
        g_menubar.items[1].col.first = 8;
        g_menubar.items[1].col.last = 13;
        g_menubar.items[1].command = NULL;

        g_menubar.items[2].text = "Search";
        g_menubar.items[2].hint = "Commands for searching and replacing text";
        g_menubar.items[2].col.first = 14;
        g_menubar.items[2].col.last = 21;
        g_menubar.items[2].command = NULL;

        g_menubar.items[3].text = "View";
        g_menubar.items[3].hint = "List of currently-loaded files";
        g_menubar.items[3].col.first = 22;
        g_menubar.items[3].col.last = 27;
        g_menubar.items[3].command = NULL;

        g_menubar.items[4].text = "Options";
        g_menubar.items[4].hint = "Commands for setting editor options";
        g_menubar.items[4].col.first = 28;
        g_menubar.items[4].col.last = 36;
        g_menubar.items[4].command = NULL;

        g_menubar.items[5].text = "Help";
        g_menubar.items[5].hint = "Help on Edit";
        g_menubar.items[5].col.first = 37;
        g_menubar.items[5].col.last = 42;
        g_menubar.items[5].command = NULL;
    } else {
        console_log("\n`g_menubar.items` is NULL\n");
        exit(1);
    }

    g_statusbar.row = 24;
    g_statusbar.item_count = 1;
    g_statusbar.items = malloc(
            (size_t) g_statusbar.item_count * sizeof(struct bar_item)
    );
    if (g_statusbar.items != NULL) {
        g_statusbar.items[0].text = "F1=Help";
        g_statusbar.items[0].hint = "";
        g_statusbar.items[0].col.first = 0;
        g_statusbar.items[0].col.last = 8;
        g_statusbar.items[0].command = NULL;
    } else {
        error_log("\n`g_statusbar.items` is NULL\n");
        exit(1);
    }
}


int main(void)
{
    struct input input;

    initialize_console(25, 80);
    create_bars();
    draw(&g_menubar);
    draw(&g_statusbar);
    focus_editor();

    while(1) {
        get_console_input(&input);
        switch(input.type) {
            case KEYBOARD:
                handle_keyboard(&input.device.keyboard);
                break;
            case MOUSE:
                handle_mouse(&input.device.mouse);
                break;
            case IGNORED:  /* fall through */
            default: break;
        }
    }

    return 0;
}
