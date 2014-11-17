#include "common.h"

#ifdef USE_CONSOLE
ConsoleBuffer consoleInst;

ConsoleBuffer::ConsoleBuffer() :
    m_hConsole(INVALID_HANDLE_VALUE),
    m_hWnd(0)
{
    ::AllocConsole();
    m_hConsole  = ::GetStdHandle(STD_OUTPUT_HANDLE);
    m_hWnd      = ::GetConsoleWindow();

    setp(m_buffer, m_buffer + BUFFER_SIZE);
    std::cout.rdbuf(this);

    // Color codes at http://msdn.microsoft.com/en-us/library/windows/desktop/ms682088(v=vs.85).aspx#_win32_character_attributes
    // SetConsoleTextAttribute(m_hConsole, i);
    // 7 - normal
    // 2 - slaba zelena     - Utils
    // 3 - slaba modra      - Renderer
    // 5 - slaba fialova    - Animation
    // 6 - slaba hneda      - Loaders
    // 9 - modra
    // 10 - zelena
    // 11 - tyrkysova
    // 12 - cervena         - Error
    // 14 - zlta
}

ConsoleBuffer::~ConsoleBuffer()
{
    if (m_hConsole != INVALID_HANDLE_VALUE)
    {
        flushAndRewind();
        m_hConsole = INVALID_HANDLE_VALUE;
        m_hWnd     = 0;
        ::FreeConsole();
    }
}

std::streambuf::int_type ConsoleBuffer::overflow(std::streambuf::int_type c)
{
    if (m_hConsole == INVALID_HANDLE_VALUE)
        return traits_type::eof();

    if (c == traits_type::eof())
        return traits_type::eof();

    (*pptr()) = c;
    flushAndRewind();
    return c;
}

int ConsoleBuffer::sync()
{
    if (m_hConsole == INVALID_HANDLE_VALUE)
        return -1;

    flushAndRewind();
    return 0;
}

void ConsoleBuffer::flushAndRewind()
{
    DWORD bytesToWrite = DWORD(pptr() - pbase());
    if (bytesToWrite == 0)
        return;
    assert(bytesToWrite <= BUFFER_SIZE);

    DWORD bytesWritten = 0;
    if (!WriteFile(m_hConsole, m_buffer, bytesToWrite, &bytesWritten, nullptr))
    {
        // This shouldn't happen.
        assert(false);
        return;
    }

    assert(bytesWritten == bytesToWrite);
    pbump(-int(bytesToWrite));
    assert(pptr() == m_buffer);
}

#endif
