/*#define _CRT_SECURE_NO_WARNINGS*/
#include <windows.h>
#include "wconsole.h"
#include "input.h"


HANDLE g_original_output_handle;
HANDLE g_console_output_handle;
HANDLE g_console_input_handle;


/***********************************************************************
    OUTPUT
***********************************************************************/

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

    MessageBox(NULL, (LPCSTR) lpMsgBuf, title, MB_OK | MB_ICONINFORMATION);
    LocalFree(lpMsgBuf);
}


void write_console(HANDLE handle, const char * string)
{
    DWORD characters_written_count;

    if ( ! WriteConsole(
            handle,
            string,
            (DWORD) lstrlen(string),
            &characters_written_count,
            NULL))
    {
        display_error("WriteConsole");
    }
}


void console_log(const char * string)
{
    write_console(g_original_output_handle, string);
}


void error_log(const char * string)
{
    write_console(GetStdHandle(STD_ERROR_HANDLE), string);
}


void set_text_cursor_visibility(int is_visible)
{
    CONSOLE_CURSOR_INFO cursor_info;

    GetConsoleCursorInfo(g_console_output_handle, &cursor_info);
    cursor_info.bVisible = is_visible;
    SetConsoleCursorInfo(g_console_output_handle, &cursor_info);
}


void set_bg_color(enum colors color)
{
    if ( ! SetConsoleTextAttribute(g_console_output_handle, (WORD) color)) {
        display_error("SetConsoleTextAttribute");
    }
}


void set_cursor_position(const short row, const short col)
{
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if ( ! SetConsoleCursorPosition(g_console_output_handle, coord)) {
        display_error("SetConsoleCursorPosition");
    }
}


void write_color_at(const short row,
                    const short col,
                    unsigned short attribute_buffer[],
                    const unsigned long length)
{
    DWORD cells_written_count;
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if ( ! WriteConsoleOutputAttribute(
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
    set_cursor_position(row, col);
    write_console(g_console_output_handle, string);
}


/***********************************************************************
    RESIZE
***********************************************************************/

void resize_console_buffer(const COORD buffer_size)
{
    if ( ! SetConsoleScreenBufferSize(g_console_output_handle, buffer_size)) {
        display_error("SetConsoleScreenBufferSize");
    }
}

void resize_console_window(SMALL_RECT * window_rect)
{
    if ( ! SetConsoleWindowInfo(g_console_output_handle, TRUE, window_rect)) {
        display_error("SetConsoleWindowInfo");
    }
}


void resize_console(const short row_count, const short col_count)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT window_rect;
    COORD buffer_size;

    if ( ! GetConsoleScreenBufferInfo(
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


/**********************************************************************
    INIT
**********************************************************************/

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


void initialize_console(const short row_count, const short col_count)
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

    resize_console(row_count, col_count);

    /* Make screen buffer active / visible */
    if ( ! SetConsoleActiveScreenBuffer(g_console_output_handle)) {
        display_error("SetConsoleActiveScreenBuffer(g_console_output_handle)");
    }

    if ((g_console_input_handle = GetStdHandle(STD_INPUT_HANDLE))
            == INVALID_HANDLE_VALUE)
    {
        display_error("GetStdHandle(STD_INPUT_HANDLE)");
    }

    if ( ! FlushConsoleInputBuffer(g_console_input_handle)) {
        display_error("FlushConsoleInputBuffer");
    }

    if ( ! SetConsoleMode(
            g_console_input_handle,
            ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
    {
        display_error("SetConsoleMode");
    }

    if ( ! SetConsoleCtrlHandler((PHANDLER_ROUTINE) ctrl_handler, TRUE)) {
        display_error("SetConsoleCtrlHandler");
    }
}


/**********************************************************************
    INPUT
**********************************************************************/

void read_color_at(const short row,
                   const short col,
                   unsigned short attribute_buffer[],
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


void flush_input(void)
{
    if ( ! FlushConsoleInputBuffer(g_console_input_handle)) {
        display_error("FlushConsoleInputBuffer");
    }
}


void get_console_input(struct input * p_input)
{
    static INPUT_RECORD input_record;
    KEY_EVENT_RECORD * p_key_event;
    MOUSE_EVENT_RECORD * p_mouse_event;
    DWORD records_read_count;

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
            p_key_event = &input_record.Event.KeyEvent;
            p_input->type = KEYBOARD;
            p_input->device.keyboard.key_is_pressed = p_key_event->bKeyDown;
            switch(p_key_event->wVirtualKeyCode) {
                case VK_ESCAPE:
                    p_input->device.keyboard.key = KEY_ESCAPE;
                    break;
                case VK_MENU:
                    p_input->device.keyboard.key = KEY_ALT;
                    break;
                case 0x46:  /* F */
                    if (p_key_event->bKeyDown) {
                        if (p_key_event->dwControlKeyState
                                == LEFT_ALT_PRESSED
                            || p_key_event->dwControlKeyState
                                == RIGHT_ALT_PRESSED)
                        {
                            p_input->device.keyboard.key = KEY_ALT_F;
                        } else {
                            p_input->device.keyboard.key = KEY_F;
                        }
                    }
                    break;
                default:
                    p_input->type = IGNORED;
            }
            break;  /* KEY_EVENT */
        case MOUSE_EVENT:
            p_mouse_event = &input_record.Event.MouseEvent;
            p_input->type = MOUSE;
            p_input->device.mouse.row = p_mouse_event->dwMousePosition.Y;
            p_input->device.mouse.col = p_mouse_event->dwMousePosition.X;
            p_input->device.mouse.button = p_mouse_event->dwButtonState;
            break;
        default:
            p_input->type = IGNORED;
    }
}
