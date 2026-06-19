#ifndef MENUS_H
#define MENUS_H


#define NULL (void*)0


enum menubar_items {
    FILE_MENU,
    EDIT_MENU,
    SEARCH_MENU,
    VIEW_MENU,
    OPTIONS_MENU,
    HELP_MENU
};


struct command {
    const char * text;
    const char * hint;
    const int    hotkey_position;
    int (*command)(void);
};


struct menu {
    const short width;
    const short command_count;
    /*const struct command * commands;*/
    const struct command commands[1];
};


struct bar_item {
    const char * text;
    const char * hint;
    const struct { const short first; const unsigned long length; } col;
    const struct menu menu;
};


struct bar {
    const short row;
    const short item_count;
    const struct bar_item items[6];
};


void build_menu(const enum menubar_items m, char * cb);


const struct bar g_menubar = {
    0,  /* row */
    6,  /* item_count */
    {   /* items */
        {
            "File",
            "Commands for manipulating files",
            {  /* col */
                2,  /* first  */
                6,  /* length */
            },
            {  /* menu */
                18,  /* width */
                1,   /* command_count */
                {  /* commands */
                    {
                        "New",
                        "Creates a new file",
                        0,
                        NULL
                    }
                }
            }
        },
        {
            "Edit",
            "Commands for editing files",
            {  /* col */
                8,  /* first  */
                6,  /* length */
            },
            {  /* menu */
                0,    /* width */
                0,
                { { NULL, NULL, 0, NULL } }
            },
        },
        {
            "Search",
            "Commands for searching and replacing text",
            {  /* col */
                14,  /* first  */
                8,   /* length */
            },
            {  /* menu */
                0,    /* width */
                0,
                { { NULL, NULL, 0, NULL } }
            },
        },
        {
            "View",
            "List of currently-loaded files",
            {  /* col */
                22,  /* first  */
                6,   /* length */
            },
            {  /* menu */
                0,    /* width */
                0,
                { { NULL, NULL, 0, NULL } }
            },
        },
        {
            "Options",
            "Commands for setting editor options",
            {
                28,
                9,
            },
            {
                0,
                0,
                { { NULL, NULL, 0, NULL } }
            },
        },
        {
            "Help",
            "Help on Edit",
            {
                37,
                6,
            },
            {
                0,
                0,
                { { NULL, NULL, 0, NULL } }
            },
        }
    }
};


const struct bar g_statusbar = {
    24,  /* row */
    1,   /* item_count */
    {    /* items */
        {
            "F1=Help",
            "",
            {  /* col */
                0,  /* first  */
                9,  /* length */
            },
            {
                0,
                0,
                { { NULL, NULL, 0, NULL } }
            }
        }
    }
};


#endif  /* MENUS_H */
