//----------------------------------------------------------------------------------------
/**
* \file       ConsoleBuffer.h
* \author     Jakub Bokšanský
* \date       2011/27/10
* \brief      Console buffer class
*
*  Provides functionality for text output to dedicated console window. Can be
*  supplied to C++ streams (attaches to std::cout on construction);
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <iostream>

#ifdef USE_CONSOLE
    #define TRACE(X) std::cout << X;
    #define TRACE_RENDERER(X) consoleInst.setColor(3); std::cout << "Renderer: " << X; consoleInst.setColor(7);
    #define TRACE_UTILS(X) consoleInst.setColor(2); std::cout << "Utils: " << X; consoleInst.setColor(7);
    #define TRACE_ANIMATION(X) consoleInst.setColor(5); std::cout << "Animation: " << X; consoleInst.setColor(7);
    #define TRACE_LOADER(X) consoleInst.setColor(6); std::cout << "Loader: " << X; consoleInst.setColor(7);
    #define TRACE_ERROR(X) {consoleInst.setColor(12); std::cout << X; consoleInst.setColor(7);}
    #define TRACE_WARNING(X) {consoleInst.setColor(11); std::cout << X; consoleInst.setColor(7);}
#else
    #define TRACE(X) ;
    #define TRACE_RENDERER(X) ;
    #define TRACE_UTILS(X) ;
    #define TRACE_ANIMATION(X) ;
    #define TRACE_LOADER(X) ;
    #define TRACE_ERROR(X) ;
    #define TRACE_WARNING(X) ;
#endif


class ConsoleBuffer : public std::streambuf
{
public:
    explicit ConsoleBuffer();

    virtual ~ConsoleBuffer();

    void setColor(WORD color)
    {
        flushAndRewind();
        SetConsoleTextAttribute(m_hConsole, color);
    }

    void move(int x, int y, int width, int height)
    {
        MoveWindow(m_hWnd, x, y, width, height, true);
    }

protected:
    virtual int_type overflow(int_type c);
    virtual int sync();

private:
    void flushAndRewind();

    static const int BUFFER_SIZE = 256;

    HANDLE m_hConsole;
    HWND m_hWnd;
    char m_buffer[BUFFER_SIZE+1];

    ConsoleBuffer(const ConsoleBuffer &);
    ConsoleBuffer &operator= (const ConsoleBuffer &);
};

extern ConsoleBuffer consoleInst;
