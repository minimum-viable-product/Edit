#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "wconsole.h"
#include "input.h"

#define SHOW_ERROR() display_error(__LINE__)


HANDLE g_original_output_handle;
DWORD  g_original_console_mode;
TCHAR  g_original_console_title[256];
HANDLE g_console_output_handle;
HANDLE g_console_input_handle;
struct {
    CHAR_INFO buffer[4096];  /* 80 x 50 */
    COORD col_row_size;
    COORD upper_left_cell;
    SMALL_RECT region;
} g_original_screen_contents;


/***********************************************************************
    OUTPUT
***********************************************************************/


void display_error(const short line_number)
{
    LPVOID lpMsgBuf;
    TCHAR  title[32];

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL);

    wsprintf(title, "Line Number: %d", line_number);
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
        SHOW_ERROR();
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


void set_color(enum colors color)
{
    if ( ! SetConsoleTextAttribute(g_console_output_handle, (WORD) color)) {
        SHOW_ERROR();
    }
}


void set_cursor_position(const short row, const short col)
{
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if ( ! SetConsoleCursorPosition(g_console_output_handle, coord)) {
        SHOW_ERROR();
    }
}


void write_attributes_at(const short row,
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
        SHOW_ERROR();
    }
}


void write_characters_at(const short row,
                         const short col,
                         char character_buffer[],
                         const unsigned long length)
{
    DWORD cells_written_count;
    COORD coord;
    coord.X = col;
    coord.Y = row;

    if ( ! WriteConsoleOutputCharacter(
            g_console_output_handle,
            character_buffer,
            length,
            coord,
            &cells_written_count))
    {
        SHOW_ERROR();
    }
}


void write_at(const short row, const short col, const char * string)
{
    set_cursor_position(row, col);
    write_console(g_console_output_handle, string);
}

#if 0
    void save_screen_at(short row, short col, struct menu * menu)
    {
        TCHAR characters[26];
        int i, j;

        g_original_screen_contents.col_row_size.X = 80;
        g_original_screen_contents.col_row_size.Y = 50;
        g_original_screen_contents.left_upper_cell.X = 0;
        g_original_screen_contents.left_upper_cell.Y = 0;
        g_original_screen_contents.region = {
            col,
            row,
            menu->width,
            menu->command_count
        };

        ReadConsoleOutput(
                g_console_output_handle,
                &g_original_screen_contents.buffer,
                g_original_screen_contents.col_row_size,
                g_original_screen_contents.left_upper_cell,
                &g_original_screen_contents.region
        );

        /* Print menu's top line */
        if (menu->command_count > 0) {
            characters[0] = '\xDA';
            for (j=1; j < menu->width - 1; ++j) {
                characters[j] = '\xC4';
            }
            characters[] = '\xBF';
        }

        for (i=0; i < menu->command_count; ++i) {

        }

        WriteConsoleOutput(
                g_console_output_handle,
                &char_info,
                destination_buffer_size,
                destination_buffer_upper_left,
                &region
        );
    }
#endif

void draw_window_at(const short row,
                     const short col,
                     TCHAR * character_buffer,
                     const short row_count,
                     const short col_count)
{
    CHAR_INFO char_info[4096];
    COORD buffer_col_row_size;
    COORD buffer_left_upper_cell = { 0, 0 };
    SMALL_RECT region;
    int i;

    buffer_col_row_size.X = col_count;
    buffer_col_row_size.Y = row_count;

    region.Left = row;
    region.Top  = col;
    region.Right  = col_count;
    region.Bottom = row_count;

    for (i=0; i < row_count * col_count; ++i) {
        char_info[i].Char.AsciiChar = character_buffer[i];
        char_info[i].Attributes = WHITE_BG | BLACK_FG;
    }

    WriteConsoleOutput(
            g_console_output_handle,
            char_info,
            buffer_col_row_size,
            buffer_left_upper_cell,
            &region
    );
}


/*void erase_window_at(short row, short col, struct menu * menu)
{
    g_original_screen_contents.col_row_size = { 80, 50 };
    g_original_screen_contents.left_upper_cell = { 0, 0 };
    g_original_screen_contents.region = {
        col,
        row,
        menu->width,
        menu->command_count
    };

    WriteConsoleOutput(
            g_console_output_handle,
            &char_info,
            destination_buffer_size,
            destination_buffer_upper_left,
            &region
    );
}*/


/***********************************************************************
    RESIZE
***********************************************************************/


void resize_console_buffer(const COORD buffer_size)
{
    if ( ! SetConsoleScreenBufferSize(g_console_output_handle, buffer_size)) {
        SHOW_ERROR();
    }
}

void resize_console_window(SMALL_RECT * window_rect)
{
    if ( ! SetConsoleWindowInfo(g_console_output_handle, TRUE, window_rect)) {
        SHOW_ERROR();
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
        SHOW_ERROR();
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

    if ( ! ReadConsoleOutputAttribute(
            g_console_output_handle,
            attribute_buffer,
            length,
            coord,
            &cells_read_count))
    {
        SHOW_ERROR();
    }
}


void flush_input(void)
{
    if ( ! FlushConsoleInputBuffer(g_console_input_handle)) {
        SHOW_ERROR();
    }
}


/**
 * Read input from the console.
 */
void get_console_input(struct input * p_input)
{
    static INPUT_RECORD input_record;
    KEY_EVENT_RECORD * p_key_event;
    MOUSE_EVENT_RECORD * p_mouse_event;
    DWORD records_read_count;

    if ( ! ReadConsoleInput(
            g_console_input_handle,
            &input_record,
            1,
            &records_read_count))
    {
        SHOW_ERROR();
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
                case 0x46:  /* F */
                    if (p_key_event->bKeyDown) {
                        p_input->device.keyboard.key = KEY_F;
                    }
                    break;
                case VK_MENU:
                    p_input->device.keyboard.key = KEY_ALT;
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


void restore_original_console(void)
{
    if ( ! SetConsoleCtrlHandler((PHANDLER_ROUTINE) ctrl_handler, FALSE)) {
        SHOW_ERROR();
    }

    if ( ! SetConsoleActiveScreenBuffer(g_original_output_handle)) {
        SHOW_ERROR();
    }

    if ( ! SetConsoleMode(g_original_output_handle, g_original_console_mode)) {
        SHOW_ERROR();
    }

    if ( ! SetConsoleTitle(g_original_console_title)) {
        SHOW_ERROR();
    }

    if ( ! FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE))) {
        SHOW_ERROR();
    }
}


void initialize_console(const short row_count, const short col_count)
{
    if ( ! GetConsoleTitle(g_original_console_title, 256)) {
        SHOW_ERROR();
    }

    if ( ! SetConsoleTitle("NT-CMD Editor")) {
        SHOW_ERROR();
    }

    if ((g_original_output_handle = GetStdHandle(STD_OUTPUT_HANDLE))
            == INVALID_HANDLE_VALUE)
    {
        SHOW_ERROR();
    }

    if ( ! GetConsoleMode(g_original_output_handle, &g_original_console_mode)) {
        SHOW_ERROR();
    }

    if ((g_console_output_handle = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL)) == INVALID_HANDLE_VALUE)
    {
        SHOW_ERROR();
    }

    resize_console(row_count, col_count);

    /* Make screen buffer active / visible */
    if ( ! SetConsoleActiveScreenBuffer(g_console_output_handle)) {
        SHOW_ERROR();
    }

    if ((g_console_input_handle = GetStdHandle(STD_INPUT_HANDLE))
            == INVALID_HANDLE_VALUE)
    {
        SHOW_ERROR();
    }

    if ( ! FlushConsoleInputBuffer(g_console_input_handle)) {
        SHOW_ERROR();
    }

    if ( ! SetConsoleMode(
            g_console_input_handle,
            ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
    {
        SHOW_ERROR();
    }

    if ( ! SetConsoleCtrlHandler((PHANDLER_ROUTINE) ctrl_handler, TRUE)) {
        SHOW_ERROR();
    }

    atexit(restore_original_console);
}
