#include <windows.h>

#ifdef _WIN32
    #ifndef __GNUC__
        #pragma comment(lib, "user32")
    #endif
#endif


HANDLE g_console_output_handle;
HANDLE g_console_input_handle;


enum {
    BLACK_FG,
    BLUE_FG,
    GREEN_FG,
    AQUA_FG,
    RED_FG,
    PURPLE_FG,
    YELLOW_FG,
    WHITE_FG,
    GRAY_FG,
    LIGHT_BLUE_FG,
    LIGHT_GREEN_FG,
    LIGHT_AQUA_FG,
    LIGHT_RED_FG,
    LIGHT_PURPLE_FG,
    LIGHT_YELLOW_FG,
    BRIGHT_WHITE_FG,

    BLACK_BG=0,
    BLUE_BG=16,
    GREEN_BG=32,
    AQUA_BG=48,
    RED_BG=64,
    PURPLE_BG=80,
    YELLOW_BG=96,
    WHITE_BG=112,
    GRAY_BG=128,
    LIGHT_BLUE_BG=144,
    LIGHT_GREEN_BG=160,
    LIGHT_AQUA_BG=176,
    LIGHT_RED_BG=192,
    LIGHT_PURPLE_BG=208,
    LIGHT_YELLOW_BG=224,
    BRIGHT_WHITE_BG=240
};


void display_error(char * title)
{
    LPVOID lpMsgBuf;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL);

    MessageBox(NULL, lpMsgBuf, title, MB_OK | MB_ICONINFORMATION);
    LocalFree(lpMsgBuf);
}


void resize_console_buffer(const COORD buffer_size)
{
    if (! SetConsoleScreenBufferSize(g_console_output_handle, buffer_size)) {
        display_error("SetConsoleScreenBufferSize");
    }
}


void resize_console_window(SMALL_RECT * window_rect)
{
    if (! SetConsoleWindowInfo(g_console_output_handle, TRUE, window_rect)) {
        display_error("SetConsoleWindowInfo");
    }
}


void resize_console(const short row_count, const short col_count)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT window_rect;
    COORD buffer_size;

    if (! GetConsoleScreenBufferInfo(
            g_console_output_handle,
            &csbi))
    {
        display_error("GetConsoleScreenBufferInfo");
    }

    buffer_size = csbi.dwSize;
    window_rect = csbi.srWindow;
    window_rect.Right = window_rect.Right - window_rect.Left;
    window_rect.Bottom = window_rect.Bottom - window_rect.Top;
    window_rect.Left = 0;
    window_rect.Top = 0;

    /* 1. If new window is smaller in either dimension... */
    if (row_count < window_rect.Bottom + 1) {
        /* Shrink window height */
        window_rect.Bottom = row_count - 1;
        resize_console_window(&window_rect);
    }
    if (col_count < window_rect.Right + 1) {
        /* Shrink window width */
        window_rect.Right = col_count - 1;
        resize_console_window(&window_rect);
    }

    /* 2. Adjust screen buffer dimensions as necessary... */
    if (row_count != buffer_size.Y) {
        /* Adjust buffer height */
        buffer_size.Y = row_count;
        resize_console_buffer(buffer_size);
    }
    if (col_count != buffer_size.X) {
        /* Adjust buffer width */
        buffer_size.X = col_count;
        resize_console_buffer(buffer_size);
    }

    /* 3. If new window is larger in either dimension... */
    if (row_count > window_rect.Bottom + 1) {
        /* Grow window height */
        window_rect.Bottom = row_count - 1;
        resize_console_window(&window_rect);
    }
    if (col_count > window_rect.Right + 1) {
        /* Grow window width */
        window_rect.Right = col_count - 1;
        resize_console_window(&window_rect);
    }
}


BOOL WINAPI ctrl_handler(DWORD ctrl_type)
{
    switch (ctrl_type) {
        case CTRL_C_EVENT:
            Beep(1000, 1000);
            return TRUE;
        case CTRL_CLOSE_EVENT:
            return TRUE;
        default:
            return FALSE;
    }
}


void initialize_console(void)
{
    if ((g_console_output_handle = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL))  == INVALID_HANDLE_VALUE)
    {
        display_error("CreateConsoleScreenBuffer");
    }

    resize_console(25, 80);

    /* Make screen buffer active / visible */
    if (! SetConsoleActiveScreenBuffer(g_console_output_handle)) {
        display_error("SetConsoleActiveScreenBuffer");
    }

    if ((g_console_input_handle = GetStdHandle(STD_INPUT_HANDLE))
            == INVALID_HANDLE_VALUE)
    {
        display_error("GetStdHandle");
    }

    if (! FlushConsoleInputBuffer(g_console_input_handle)) {
        display_error("FlushConsoleInputBuffer");
    }

    if (! SetConsoleMode(
            g_console_input_handle,
            ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
    {
        display_error("SetConsoleMode");
    }

    if (! SetConsoleCtrlHandler((PHANDLER_ROUTINE) ctrl_handler, TRUE)) {
        display_error("SetConsoleCtrlHandler");
    }
}


void set_cursor_position(const short row, const short col)
{
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if (! SetConsoleCursorPosition(g_console_output_handle, coord)) {
        display_error("SetConsoleCursorPosition");
    }
}


void write_console_color(unsigned short * attributes,
                         const short row,
                         const short col,
                         const unsigned long length)
{
    DWORD cells_written_count;
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if (! WriteConsoleOutputAttribute(
            g_console_output_handle,
            attributes,
            length,
            coord,
            &cells_written_count))
    {
        display_error("WriteConsoleOutputAttribute");
    }
}


void write_console(char * string)
{
    DWORD characters_written_count;

    if (! WriteConsole(
            g_console_output_handle,
            string,
            (DWORD) lstrlen(string),
            &characters_written_count,
            NULL))
    {
        display_error("WriteConsole");
    }
}


void handle_key_event(const KEY_EVENT_RECORD * key_event)
{
    (void) key_event;
    write_console("\nkey event\n");
}


void handle_mouse_event(const MOUSE_EVENT_RECORD * mouse_event)
{
    (void) mouse_event;
    write_console("\nmouse event\n");
}


void loop_over_console_input(void)
{
    INPUT_RECORD input_record;
    DWORD records_read_count;

    while (TRUE) {
        if (! ReadConsoleInput(
                g_console_input_handle,
                &input_record,
                1,
                &records_read_count))
        {
            display_error("ReadConsoleInput");
        }

        switch(input_record.EventType) {
            case KEY_EVENT:
                handle_key_event(&input_record.Event.KeyEvent);
                break;
            case MOUSE_EVENT:
                handle_mouse_event(&input_record.Event.MouseEvent);
                break;
        }
    }
}
