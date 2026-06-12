struct command {
    const char * text;
    const char * hint;
    int    hotkey_position;
    int    (*command)(void);
};


struct menu {
    int width;
    struct command * commands;
};


struct bar_item {
    const char * text;
    const char * hint;
    struct { short first; unsigned long length; } col;
    struct menu menu;
};


struct bar {
    short row;
    short item_count;
    struct bar_item items[6];
} g_menubar = {
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
                0,    /* width */
                NULL  /* commands */
            },
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
                NULL  /* commands */
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
                NULL  /* commands */
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
                NULL  /* commands */
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
                NULL
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
                NULL
            },
        }
    }
}, g_statusbar = {
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
            {  /* menu */
                0,    /* width */
                NULL  /* commands */
            }
        }
    }
};
