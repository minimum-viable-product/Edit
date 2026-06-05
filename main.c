#include <stdio.h>

#include "console.c"


struct command {
    char * name;
    char * hint;
    int    hotkey_position;
    int    (*command)(void);
};

struct menu {
    char * name;
    char * hint;
    struct command * commands;
};

struct {
    struct menu menus[6];
} g_menubar;


void create_menus(void)
{
    g_menubar.menus[0].name = "File";
    g_menubar.menus[0].hint = "Commands for manipulating files";
    g_menubar.menus[0].commands = NULL;

    g_menubar.menus[1].name = "Edit";
    g_menubar.menus[1].hint = "Commands for editing files";
    g_menubar.menus[1].commands = NULL;

    g_menubar.menus[2].name = "Search";
    g_menubar.menus[2].hint = "Commands for searching and replacing text";
    g_menubar.menus[2].commands = NULL;

    g_menubar.menus[3].name = "View";
    g_menubar.menus[3].hint = "List of currently-loaded files";
    g_menubar.menus[3].commands = NULL;

    g_menubar.menus[4].name = "Options";
    g_menubar.menus[4].hint = "Commands for setting editor options";
    g_menubar.menus[4].commands = NULL;

    g_menubar.menus[5].name = "Help";
    g_menubar.menus[5].hint = "Help on Edit";
    g_menubar.menus[5].commands = NULL;
}


void display_menubar(void)
{
    unsigned short attributes[80];
    char * char_buffer = "File  Edit  Search  View  Options  Help";
    int i;

    for (i=0; i < 80; ++i) {
        attributes[i] = WHITE_BG;
    }

    /* Write menu names in menubar */
    set_cursor_position(0, 3);
    write_console(char_buffer);
    write_console_color(attributes, 0, 0, 80);

    /*SetConsoleTextAttribute(g_console_output_handle, WHITE_FG | BLACK_BG);*/
}


int main(void)
{
    initialize_console();
    create_menus();
    display_menubar();

    set_cursor_position(1, 0);
    loop_over_console_input();
    return 0;
}
