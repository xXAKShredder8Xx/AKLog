#ifndef AK_LOGGER_H
#define AK_LOGGER_H

#include <stdint.h>
#include <stdarg.h>
#include <wchar.h>

#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#endif

#if defined(PLATFORM_WINDOWS)
#include <Windows.h>
#else
#include <linux.h>
#endif

namespace AK 
{
    namespace Log 
    {
        enum WarningLevel 
        {
            LEVEL_TRACE,
            LEVEL_DEBUG,
            LEVEL_INFO,
            LEVEL_WARNING,
            LEVEL_ERROR,
            LEVEL_FATAL
        };

        class Logger 
        {
        public:
            Logger();
            Logger(const char* fmt, const wchar_t* fmtW);
            Logger(const char* fmt, const wchar_t* fmtW, WarningLevel _level);
            ~Logger();

            void log(const char* text, va_list args);
            void log(WarningLevel _level, const char* text, va_list args);
            void logMsg(const char* text, ...);
            void logMsg(WarningLevel _level, const char* text, ...);
            void logTrace(const char* text, ...);
            void logDebug(const char* text, ...);
            void logInfo(const char* text, ...);
            void logWarning(const char* text, ...);
            void logError(const char* text, ...);
            void logFatal(const char* text, ...);
            void setLevel(WarningLevel _level);
            void printFmt(const char* fmt, const char* text, ...);
            void printFmtArgs(const char* fmt, const char* text, va_list args);

            void logW(const wchar_t* text, va_list args);
            void logW(WarningLevel _level, const wchar_t* text, va_list args);
            void logMsgW(const wchar_t* text, ...);
            void logMsgW(WarningLevel _level, const wchar_t* text, ...);
            void logTraceW(const wchar_t* text, ...);
            void logDebugW(const wchar_t* text, ...);
            void logInfoW(const wchar_t* text, ...);
            void logWarningW(const wchar_t* text, ...);
            void logErrorW(const wchar_t* text, ...);
            void logFatalW(const wchar_t* text, ...);
            void setLevelW(WarningLevel _level);
            void printFmtW(const wchar_t* fmt, const wchar_t* text, ...);
            void printFmtArgsW(const wchar_t* fmt, const wchar_t* text, va_list args);
        private:
            void printLevel();
            void printLevelColor();
            void printColor(int color);
            void clearLevel();
            char readChar(const char* fmt, size_t length, int i);
            char* getCurrentTime(char* str);
            char* getCurrentDate(char* str);

            void printLevelW();
            wchar_t readCharW(const wchar_t* fmt, size_t length, int i);
            wchar_t* getCurrentTimeW(wchar_t* str);
            wchar_t* getCurrentDateW(wchar_t* str);
            void ConvertWs(const char* src, wchar_t* dest);

            const char* fmt;
            const wchar_t* fmtW;
            WarningLevel level;
            char currentTime[10];
            char currentDate[11];
            wchar_t currentTimeW[10];
            wchar_t currentDateW[11];

            #if defined(PLATFORM_WINDOWS)

            HANDLE handle;

            static int FORMAT_COLOR_BLACK;
            static int FORMAT_COLOR_RED;
            static int FORMAT_COLOR_GREEN;
            static int FORMAT_COLOR_YELLOW;
            static int FORMAT_COLOR_BLUE;
            static int FORMAT_COLOR_MAGENTA;
            static int FORMAT_COLOR_CYAN;
            static int FORMAT_COLOR_WHITE;
            static int FORMAT_COLOR_RESET;

            #else

            static const char* FORMAT_COLOR_BLACK;
            static const char* FORMAT_COLOR_RED;
            static const char* FORMAT_COLOR_GREEN;
            static const char* FORMAT_COLOR_YELLOW;
            static const char* FORMAT_COLOR_BLUE;
            static const char* FORMAT_COLOR_MAGENTA;
            static const char* FORMAT_COLOR_CYAN;
            static const char* FORMAT_COLOR_WHITE;
            static const char* FORMAT_COLOR_RESET;

            #endif
        };
    }
}

#endif // AK_LOGGER_H
