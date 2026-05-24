#pragma once

#include <windows.h>

struct cursor_pos {
    int x, y;
};

//#ifdef _WIN32

inline cursor_pos get_cursor_position() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
    }
    return { -1, -1 };
}

//#endif