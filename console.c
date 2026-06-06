#include "console.h"
#include "handlers.c"


HANDLE g_original_output_handle;
HANDLE g_console_output_handle;
HANDLE g_console_input_handle;


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
            return TRUE;
        case CTRL_CLOSE_EVENT:
            return TRUE;
        default:
            return FALSE;
    }
}


void initialize_console(void)
{
    if ((g_original_output_handle = GetStdHandle(STD_OUTPUT_HANDLE))
            == INVALID_HANDLE_VALUE)
    {
        display_error("GetStdHandle(STD_OUTPUT_HANDLE)");
    }

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
        display_error("SetConsoleActiveScreenBuffer(g_console_output_handle)");
    }

    if ((g_console_input_handle = GetStdHandle(STD_INPUT_HANDLE))
            == INVALID_HANDLE_VALUE)
    {
        display_error("GetStdHandle(STD_INPUT_HANDLE)");
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


void set_bg_color(enum colors color)
{
    if (! SetConsoleTextAttribute(g_console_output_handle, (WORD) color)) {
        display_error("SetConsoleTextAttribute");
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


void read_color_at(const short row,
                    const short col,
                    unsigned short * attribute_buffer,
                    const unsigned long length)
{
    DWORD cells_read_count;
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if (! ReadConsoleOutputAttribute(
            g_console_output_handle,
            attribute_buffer,
            length,
            coord,
            &cells_read_count))
    {
        display_error("ReadConsoleOutputAttribute");
    }
}


void write_color_at(const short row,
                    const short col,
                    unsigned short * attribute_buffer,
                    const unsigned long length)
{
    DWORD cells_written_count;
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if (! WriteConsoleOutputAttribute(
            g_console_output_handle,
            attribute_buffer,
            length,
            coord,
            &cells_written_count))
    {
        display_error("WriteConsoleOutputAttribute");
    }
}


void write_at(const short row, const short col, const char * string)
{
    DWORD characters_written_count;

    set_cursor_position(row, col);
    if (! WriteConsole(
            g_console_output_handle,
            string,
            (DWORD) lstrlen(string),
            &characters_written_count,
            NULL))
    {
        display_error("write_at() WriteConsole");
    }
}


void console_log(const char * string)
{
    DWORD characters_written_count;

    if (! WriteConsole(
            g_original_output_handle,
            string,
            (DWORD) lstrlen(string),
            &characters_written_count,
            NULL))
    {
        display_error("console_log() WriteConsole");
    }
}


int handle_key_event(const KEY_EVENT_RECORD * key_event)
{
    if (key_event->wVirtualKeyCode == VK_ESCAPE) {
        return handle_key(KEY_ESCAPE);
    }
    else {
        return 0;
    }
}


int handle_mouse_event(const MOUSE_EVENT_RECORD * mouse_event)
{
    /*WORD attribute_buffer;

    read_color_at(
            mouse_event->dwMousePosition.Y,
            mouse_event->dwMousePosition.X,
            &attribute_buffer,
            1
    );
    attribute_buffer ^= 255;
    write_color_at(
            mouse_event->dwMousePosition.Y,
            mouse_event->dwMousePosition.X,
            &attribute_buffer,
            1
    );*/

    return handle_mouse();
}


void loop_over_console_input(void)
{
    INPUT_RECORD input_record;
    DWORD records_read_count;
    int should_exit = 0;

    while (! should_exit) {
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
                should_exit = handle_key_event(&input_record.Event.KeyEvent);
                break;
            case MOUSE_EVENT:
                should_exit = handle_mouse_event(&input_record.Event.MouseEvent);
                break;
        }
    }
}
