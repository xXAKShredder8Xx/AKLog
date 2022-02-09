#include "AKL/log.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

namespace AK 
{
    namespace Log 
    {
        #if defined(PLATFORM_WINDOWS)

        Logger::Logger() 
            : fmt("[%l %t]: %s\n"), fmtW(L"[%l %t]: %s\n"), level(WarningLevel::LEVEL_INFO), handle(GetStdHandle(STD_OUTPUT_HANDLE))
        {
            if (FORMAT_COLOR_RESET == -1) 
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                GetConsoleScreenBufferInfo(handle, &info);
                FORMAT_COLOR_RESET = info.wAttributes;
            }
        }

        Logger::Logger(const char* fmt, const wchar_t* fmtW)
            : fmt(fmt), fmtW(fmtW), level(WarningLevel::LEVEL_INFO), handle(GetStdHandle(STD_OUTPUT_HANDLE))
        {
            if (FORMAT_COLOR_RESET == -1) 
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                GetConsoleScreenBufferInfo(handle, &info);
                FORMAT_COLOR_RESET = info.wAttributes;
            }
        }

        Logger::Logger(const char* fmt, const wchar_t* fmtW, WarningLevel _level)
            : fmt(fmt), fmtW(fmtW), level(_level), handle(GetStdHandle(STD_OUTPUT_HANDLE))
        {
            if (FORMAT_COLOR_RESET == -1) 
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                GetConsoleScreenBufferInfo(handle, &info);
                FORMAT_COLOR_RESET = info.wAttributes;
            }
        }

        #else

        Logger::Logger() 
            : fmt("[%l %t]: %s\n"), fmtW(L"[%l %t]: %s\n"), level(WarningLevel::LEVEL_INFO)
        {
        }

        Logger::Logger(const char* fmt, const wchar_t* fmtW)
            : fmt(fmt), fmtW(fmtW), level(WarningLevel::LEVEL_INFO)
        {
        }

        Logger::Logger(const char* fmt, const wchar_t* fmtW, WarningLevel _level)
            : fmt(fmt), fmtW(fmtW), level(_level)
        {
        }

        #endif

        Logger::~Logger()
        {
        }

        void Logger::log(const char* text, va_list args)
        {
            _setmode(_fileno(stdout), _O_TEXT);
            printLevelColor();
            printFmtArgs(fmt, text, args);
            clearLevel();
        }
        
        void Logger::log(WarningLevel _level, const char* text, va_list args)
        {
            _setmode(_fileno(stdout), _O_TEXT);
            WarningLevel previousLevel = level;
            setLevel(_level);
            printLevelColor();
            log(text, args);
            level = previousLevel;
            clearLevel();
        }

        void Logger::logMsg(const char* text, ...) 
        {
            printLevelColor();
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            clearLevel();
        }

        void Logger::logMsg(WarningLevel _level, const char* text, ...) 
        {
            WarningLevel previousLevel = level;
            setLevel(_level);
            printLevelColor();
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
            clearLevel();
        }
        
        void Logger::logTrace(const char* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_TRACE);
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logDebug(const char* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_DEBUG);
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logInfo(const char* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_INFO);
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logWarning(const char* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_WARNING);
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logError(const char* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_ERROR);
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logFatal(const char* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_FATAL);
            va_list args;
            va_start(args, text);
            log(text, args);
            va_end(args);
            level = previousLevel;
        }

        void Logger::setLevel(WarningLevel _level) 
        {
            level = _level;
        }

        void Logger::printFmt(const char* fmt, const char* text, ...)
        {
            char c = 0;
            int i = 0;
            int currentObj = 0;
            int length = strlen(fmt);

            while ((c = readChar(fmt, length, i)) != '\0') 
            {
                switch (c) 
                {
                    case '%':
                    {
                        i++;
                        c = readChar(fmt, length, i);
                        switch (c)
                        {
                            case '0':
                                printColor(FORMAT_COLOR_BLACK);
                                break;
                            case '1':
                                printColor(FORMAT_COLOR_RED);
                                break;
                            case '2':
                                printColor(FORMAT_COLOR_GREEN);
                                break;
                            case '3':
                                printColor(FORMAT_COLOR_YELLOW);
                                break;
                            case '4':
                                printColor(FORMAT_COLOR_BLUE);
                                break;
                            case '5':
                                printColor(FORMAT_COLOR_MAGENTA);
                                break;
                            case '6':
                                printColor(FORMAT_COLOR_CYAN);
                                break;
                            case '7':
                                printColor(FORMAT_COLOR_WHITE);
                                break;
                            case '8':
                                printColor(FORMAT_COLOR_RESET);
                                break;
                            case 't':
                            {
                                getCurrentTime(currentTime);
                                printf("%s", currentTime);
                                break;
                            }
                            case 'd':
                            {
                                getCurrentDate(currentDate);
                                printf("%s", currentDate);
                                break;
                            }
                            case 'l':
                            {
                                printLevel();
                                break;
                            }
                            case 's':
                            {
                                va_list args;
                                va_start(args, text);
                                vprintf(text, args);
                                va_end(args);
                                break;
                            }
                        }
                        i++;
                        break;
                    default:
                        i++;
                        printf("%c", c);
                        break;
                    }
                }
            }
        }

        void Logger::printFmtArgs(const char* fmt, const char* text, va_list args)
        {
            char c = 0;
            int i = 0;
            int currentObj = 0;
            int length = strlen(fmt);

            while ((c = readChar(fmt, length, i)) != '\0') 
            {
                switch (c) 
                {
                    case '%':
                    {
                        i++;
                        c = readChar(fmt, length, i);
                        switch (c)
                        {
                            case '0':
                                printColor(FORMAT_COLOR_BLACK);
                                break;
                            case '1':
                                printColor(FORMAT_COLOR_RED);
                                break;
                            case '2':
                                printColor(FORMAT_COLOR_GREEN);
                                break;
                            case '3':
                                printColor(FORMAT_COLOR_YELLOW);
                                break;
                            case '4':
                                printColor(FORMAT_COLOR_BLUE);
                                break;
                            case '5':
                                printColor(FORMAT_COLOR_MAGENTA);
                                break;
                            case '6':
                                printColor(FORMAT_COLOR_CYAN);
                                break;
                            case '7':
                                printColor(FORMAT_COLOR_WHITE);
                                break;
                            case '8':
                                printColor(FORMAT_COLOR_RESET);
                                break;
                            case 't':
                            {
                                getCurrentTime(currentTime);
                                printf("%s", currentTime);
                                break;
                            }
                            case 'd':
                            {
                                getCurrentDate(currentDate);
                                printf("%s", currentDate);
                                break;
                            }
                            case 'l':
                            {
                                printLevel();
                                break;
                            }
                            case 's':
                            {
                                vprintf(text, args);
                                break;
                            }
                        }
                        i++;
                        break;
                    default:
                        i++;
                        printf("%c", c);
                        break;
                    }
                }
            }
        }

        void Logger::logW(const wchar_t* text, va_list args)
        {
            _setmode(_fileno(stdout), _O_U16TEXT);
            printLevelColor();
            printFmtArgsW(fmtW, text, args);
            clearLevel();
        }
        
        void Logger::logW(WarningLevel _level, const wchar_t* text, va_list args)
        {
            _setmode(_fileno(stdout), _O_U16TEXT);
            WarningLevel previousLevel = level;
            setLevel(_level);
            printLevelColor();
            logW(text, args);
            level = previousLevel;
            clearLevel();
        }

        void Logger::logMsgW(const wchar_t* text, ...) 
        {
            printLevelColor();
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            clearLevel();
        }

        void Logger::logMsgW(WarningLevel _level, const wchar_t* text, ...) 
        {
            WarningLevel previousLevel = level;
            setLevel(_level);
            printLevelColor();
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
            clearLevel();
        }
        
        void Logger::logTraceW(const wchar_t* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_TRACE);
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logDebugW(const wchar_t* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_DEBUG);
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logInfoW(const wchar_t* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_INFO);
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logWarningW(const wchar_t* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_WARNING);
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logErrorW(const wchar_t* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_ERROR);
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::logFatalW(const wchar_t* text, ...)
        {
            WarningLevel previousLevel = level;
            setLevel(WarningLevel::LEVEL_FATAL);
            va_list args;
            va_start(args, text);
            logW(text, args);
            va_end(args);
            level = previousLevel;
        }
        
        void Logger::printFmtW(const wchar_t* fmt, const wchar_t* text, ...)
        {
            wchar_t c = 0;
            int i = 0;
            int currentObj = 0;
            size_t length = wcslen(fmt);

            while ((c = readCharW(fmt, length, i)) != L'\0') 
            {
                switch (c) 
                {
                    case L'%':
                    {
                        i++;
                        c = readCharW(fmt, length, i);
                        switch (c)
                        {
                            case L'0':
                                printColor(FORMAT_COLOR_BLACK);
                                break;
                            case L'1':
                                printColor(FORMAT_COLOR_RED);
                                break;
                            case L'2':
                                printColor(FORMAT_COLOR_GREEN);
                                break;
                            case L'3':
                                printColor(FORMAT_COLOR_YELLOW);
                                break;
                            case L'4':
                                printColor(FORMAT_COLOR_BLUE);
                                break;
                            case L'5':
                                printColor(FORMAT_COLOR_MAGENTA);
                                break;
                            case L'6':
                                printColor(FORMAT_COLOR_CYAN);
                                break;
                            case L'7':
                                printColor(FORMAT_COLOR_WHITE);
                                break;
                            case L'8':
                                printColor(FORMAT_COLOR_RESET);
                                break;
                            case L't':
                            {
                                getCurrentTimeW(currentTimeW);
                                wprintf(L"%ls", currentTimeW);
                                break;
                            }
                            case L'd':
                            {
                                getCurrentDateW(currentDateW);
                                wprintf(L"%ls", currentDateW);
                                break;
                            }
                            case L'l':
                            {
                                printLevelW();
                                break;
                            }
                            case L's':
                            {
                                va_list args;
                                va_start(args, text);
                                vwprintf(text, args);
                                va_end(args);
                                break;
                            }
                        }
                        i++;
                        break;
                    default:
                        i++;
                        wprintf(L"%lc", c);
                        break;
                    }
                }
            }
        }
        
        void Logger::printFmtArgsW(const wchar_t* fmt, const wchar_t* text, va_list args)
        {
            wchar_t c = 0;
            int i = 0;
            int currentObj = 0;
            size_t length = wcslen(fmt);

            while ((c = readCharW(fmt, length, i)) != L'\0') 
            {
                switch (c) 
                {
                    case L'%':
                    {
                        i++;
                        c = readCharW(fmt, length, i);
                        switch (c)
                        {
                            case L'0':
                                printColor(FORMAT_COLOR_BLACK);
                                break;
                            case L'1':
                                printColor(FORMAT_COLOR_RED);
                                break;
                            case L'2':
                                printColor(FORMAT_COLOR_GREEN);
                                break;
                            case L'3':
                                printColor(FORMAT_COLOR_YELLOW);
                                break;
                            case L'4':
                                printColor(FORMAT_COLOR_BLUE);
                                break;
                            case L'5':
                                printColor(FORMAT_COLOR_MAGENTA);
                                break;
                            case L'6':
                                printColor(FORMAT_COLOR_CYAN);
                                break;
                            case L'7':
                                printColor(FORMAT_COLOR_WHITE);
                                break;
                            case L'8':
                                printColor(FORMAT_COLOR_RESET);
                                break;
                            case L't':
                            {
                                getCurrentTimeW(currentTimeW);
                                wprintf(L"%ls", currentTimeW);
                                break;
                            }
                            case L'd':
                            {
                                getCurrentDateW(currentDateW);
                                wprintf(L"%ls", currentDateW);
                                break;
                            }
                            case L'l':
                            {
                                printLevelW();
                                break;
                            }
                            case L's':
                            {
                                vwprintf(text, args);
                                break;
                            }
                        }
                        i++;
                        break;
                    default:
                        i++;
                        wprintf(L"%lc", c);
                        break;
                    }
                }
            }
        }
        
        void Logger::printLevel()
        {
            switch (level) 
            {
                case LEVEL_TRACE:
                    printf("TRACE");
                    break;
                case LEVEL_DEBUG:
                    printf("DEBUG");
                    break;
                case LEVEL_INFO:
                    printf("INFO");
                    break;
                case LEVEL_WARNING:
                    printf("WARNING");
                    break;
                case LEVEL_ERROR:
                    printf("ERROR");
                    break;
                case LEVEL_FATAL:
                    printf("FATAL");
                    break;
            }
        }

        void Logger::printLevelColor()
        {
            switch (level) 
            {
                case LEVEL_TRACE:
                case LEVEL_DEBUG:
                case LEVEL_INFO:
                    printColor(FORMAT_COLOR_GREEN);
                    break;
                case LEVEL_WARNING:
                    printColor(FORMAT_COLOR_YELLOW);
                    break;
                case LEVEL_ERROR:
                case LEVEL_FATAL:
                    printColor(FORMAT_COLOR_RED);
                    break;
            }
        }

        #if defined(PLATFORM_WINDOWS)

        void Logger::printColor(int color) 
        {
            SetConsoleTextAttribute(handle, color);
        }

        #else

        void Logger::printColor(const char* color) 
        {
            printf("%s", color);
        }

        #endif

        void Logger::clearLevel()
        {
            printColor(FORMAT_COLOR_RESET);
        }

        char Logger::readChar(const char* fmt, size_t length, int i)
        {
            if (i >= length) return '\0';
            return fmt[i];
        }

        char* Logger::getCurrentTime(char* str)
        {
            time_t t = time(NULL);
            struct tm* lt = localtime(&t);

            sprintf(str, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
            str[8] = '\0';
            return str;
        }

        char* Logger::getCurrentDate(char* str)
        {
            time_t t = time(NULL);
            struct tm* lt = localtime(&t);

            sprintf(str, "%04d/%02d/%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
            str[10] = '\0';
            return str;
        }

        void Logger::printLevelW()
        {
            switch (level) 
            {
                case LEVEL_TRACE:
                    wprintf(L"TRACE");
                    break;
                case LEVEL_DEBUG:
                    wprintf(L"DEBUG");
                    break;
                case LEVEL_INFO:
                    wprintf(L"INFO");
                    break;
                case LEVEL_WARNING:
                    wprintf(L"WARNING");
                    break;
                case LEVEL_ERROR:
                    wprintf(L"ERROR");
                    break;
                case LEVEL_FATAL:
                    wprintf(L"FATAL");
                    break;
            }
        }

        wchar_t Logger::readCharW(const wchar_t* fmt, size_t length, int i)
        {
            if (i >= length) return L'\0';
            return fmt[i];
        }

        wchar_t* Logger::getCurrentTimeW(wchar_t* str)
        {
            time_t t = time(NULL);
            struct tm* lt = localtime(&t);

            wsprintfW((wchar_t*)str, L"%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
            str[8] = L'\0';
            return str;
        }

        wchar_t* Logger::getCurrentDateW(wchar_t* str)
        {
            time_t t = time(NULL);
            struct tm* lt = localtime(&t);

            wsprintfW((wchar_t*)str, L"%04d/%02d/%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
            str[10] = L'\0';
            return str;
        }

        void Logger::ConvertWs(const char* src, wchar_t* dest) 
        {
            const size_t cSize = strlen(src);
            mbstowcs(dest, src, cSize);
        }

        #if defined(PLATFORM_WINDOWS) 
        int Logger::FORMAT_COLOR_BLACK =    0x0;
        int Logger::FORMAT_COLOR_BLUE =     0x1;
        int Logger::FORMAT_COLOR_GREEN =    0x2;
        int Logger::FORMAT_COLOR_CYAN =     0x3;
        int Logger::FORMAT_COLOR_RED =      0x4;
        int Logger::FORMAT_COLOR_MAGENTA =  0x5;
        int Logger::FORMAT_COLOR_YELLOW =   0x6;
        int Logger::FORMAT_COLOR_WHITE =    0x7;
        int Logger::FORMAT_COLOR_RESET =    -1;
        
        #else
        
        const char* Logger::FORMAT_COLOR_BLACK =    "\x1B[30m";
        const char* Logger::FORMAT_COLOR_RED =      "\x1B[31m";
        const char* Logger::FORMAT_COLOR_GREEN =    "\x1B[32m";
        const char* Logger::FORMAT_COLOR_YELLOW =   "\x1B[33m";
        const char* Logger::FORMAT_COLOR_BLUE =     "\x1B[34m";
        const char* Logger::FORMAT_COLOR_MAGENTA =  "\x1B[35m";
        const char* Logger::FORMAT_COLOR_CYAN =     "\x1B[36m";
        const char* Logger::FORMAT_COLOR_WHITE =    "\x1B[37m";
        const char* Logger::FORMAT_COLOR_RESET =    "\x1B[0m";
        #endif
    }
}
