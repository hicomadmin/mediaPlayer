#ifndef LOG_H
#define LOG_H

#ifndef USE_LIBARY_WLOG
    #define USE_LIBARY_WLOG 1
#endif

#if defined(USE_LIBARY_WLOG) && (USE_LIBARY_WLOG==1)
    #include <WLog/WLogInterface.h>
#else //your own define.... like,,,,
    #include <cassert>
    #include <cstdio>
    #define logonlytrace
    #define logtrace printf
    #define loginfo printf
    #define logdebug printf
    #define logwarning printf
    #define lognotice printf
    #define logerror printf
    #define logassert(condition) assert(condition)
    #define logfatal printf
    #define logoutput printf
#endif

enum ErrorType
{
    NO_ERROR,
    MT_FILE_NOT_EXIST,
    MT_READ_ERROR,
    MT_WRITE_ERROR,
    UB_RELOAD_SKIN_ERROR,
    UB_COMPONENTATTR_NOT_FOUND,
    UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,
    UB_WIDGET_POINTER_IS_NULL,
    SL_CANT_CONVERT_TO_ELE,
    SL_NODE_NAME_NOT_EQ,
    SL_CANT_OPEN_FILE,
    SL_WHEN_PARSE_MEET_ERROR,
    SL_EXP_IS_FALSE,
    LL_FOUND_A_NOT_NULL_POINTER,
};
#endif // LOG_H
