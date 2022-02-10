#include "include/AKL/log.hpp"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    AK::Log::Logger log("[%l %d %t]: %s\n", L"[%l %d %t]: %s\n", AK::Log::WarningLevel::LEVEL_TRACE);
    
    log.logTrace("trace test %c", 0x3C0);
    log.logDebug("debug test %c", 0x3C0);
    log.logInfo("info test %c", 0x3C0);
    log.logWarning("warning test %c", 0x3C0);
    log.logError("error test %c", 0x3C0);
    log.logFatal("fatal test %c", 0x3C0);

    log.logTraceW(L"wide trace test %c", 0x3C0);
    log.logDebugW(L"wide debug test %c", 0x3C0);
    log.logInfoW(L"wide info test %c", 0x3C0);
    log.logWarningW(L"wide warning test %c", 0x3C0);
    log.logErrorW(L"wide error test %c", 0x3C0);
    log.logFatalW(L"wide fatal test %c", 0x3C0);

    LOG_INFO_ARGS("%c", 0x3C0); // prints the ASCII interpretation of the bits of the utf-16 PI symbol
    LOG_INFO_ARGS_WIDE(L"%c", 0x3C0); // prints the utf-16 PI symbol

    LOG_ASSERT(0, "Assert prints propperly!"); // prints the file with line in the format: 'file:line' then prints the passed in message
    LOG_ASSERT(1, "Assert prints propperly!"); // prints nothing
}
