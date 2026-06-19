#include <string.h>
#include "menus.h"


void build_menu(const enum menubar_items menu, char * character_buffer)
{
    int menu_width = g_menubar.items[menu].menu.width;
    int i, command, line;

    /* Menu top line */
    character_buffer[0] = '\xDA';
    for (i=1; i < menu_width - 1; ++i) {
        character_buffer[i] = '\xC4';
    }
    character_buffer[menu_width - 1] = '\xBF';

    /* Menu commands */
    for (command=0, line=1;
         command < g_menubar.items[menu].menu.command_count;
         ++command, ++line)
    {
        strncpy(&character_buffer[menu_width * line], "\xB3 ", 2);
        strncpy(
                &character_buffer[menu_width * line + 2],
                g_menubar.items[menu].menu.commands[command].text,
                strlen(g_menubar.items[menu].menu.commands[command].text)
        );
        strncpy(
                &character_buffer[menu_width * line + menu_width - 1],
                "\xB3",
                1
        );
    }

    /* Menu bottom line */
    character_buffer[menu_width * line] = '\xC0';
    for (i=1; i < menu_width - 1; ++i) {
        character_buffer[menu_width * line + i] = '\xC4';
    }
    character_buffer[menu_width * line + i] = '\xD9';
}
