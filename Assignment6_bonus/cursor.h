#include <cassert>
#include <windows.h>

/*
    Move the cursor to the given location in which column is the number
    of columns and line the number of lines (both starting at 0, from the left-top of
    the console window.)
*/
void set_cursor_position ( int column, int line )
{// pre-condition:
    assert (column >= 0 && line >= 0) ;
//  post-condition: the cursor has moved to (column,line)
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}

/*
    Wipe the console window 'clean' with space characters and move the cursor to (0,0) - left top.

    Source for the code below: http://www.cplusplus.com/forum/beginner/1988/3/
*/
void cls ()
{// pre-condition:
    assert (true) ;
//  post-condition: the console window contains only space characters and the cursor is at (0,0).

    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE ) ;

    // Get the number of character cells in the current buffer.
    if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
        return;
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten ))
        return;

    // Get the current text attribute.
    if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
        return;

    // Set the buffer's attributes accordingly.
    if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten ))
        return;

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition( hConsole, coordScreen );
}
