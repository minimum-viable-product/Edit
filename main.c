#include <stdio.h>

#include "console.c"


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
    short length;
    struct bar_item * items;
} g_menubar, g_statusbar;


void create_bars(void)
{
    g_menubar.row = 0;
    g_menubar.length = 6;
    g_menubar.items = malloc(
            (size_t)g_menubar.length * sizeof(struct bar_item)
    );

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

    g_statusbar.row = 24;
    g_statusbar.length = 1;
    g_statusbar.items = malloc(
            (size_t) g_statusbar.length * sizeof(struct bar_item)
    );

    g_statusbar.items[0].text = "F1=Help";
    g_statusbar.items[0].hint = "";
    g_statusbar.items[0].col.first = 0;
    g_statusbar.items[0].col.last = 8;
    g_statusbar.items[0].command = NULL;
}


void draw(struct bar * bar)
{
    unsigned short bg_color = WHITE_BG;
    short i;

    for (i=0; i < bar->length; ++i) {
        write_at(bar->row, bar->items[i].col.first + 1, bar->items[i].text);
    }

    for (i=0; i < 80; ++i) {
        write_color_at(bar->row, i, &bg_color, 1);
    }
}


int main(void)
{
    initialize_console();
    create_bars();

    /* Paint Screen */
    draw(&g_menubar);
    draw(&g_statusbar);

    set_cursor_position(1, 0);  /* TODO: Set focus in editor window */
    loop_over_console_input();
    return 0;
}
