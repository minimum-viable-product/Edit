#include "input.h"
#include "wconsole.c"
#include "menus.c"
#include "handlers.c"


int main(void)
{
    struct input input;

    initialize_console(25, 80);
    /*create_bars();*/
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
