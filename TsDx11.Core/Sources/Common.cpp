#include "pch.h"
#include "TsDx11Core.Internal.h"

//------------------------------------------------------------------------------------------------------------------------
//! ErrorResult

bool TS::ErrorResult::IsError() const
{
    return _errorLevel != ErrorLevel::Success;
}

const TS::TsChar* TS::ErrorResult::GetErrorMessage() const
{
    return _message;
}

TS::ErrorLevel TS::ErrorResult::GetErrorLevel() const
{
    return _errorLevel;
}


class PrintfLogger : TS::ILogger
{
public:
    void Log(const char* message, TS::LogMetaData errorLevel)
    {
        printf(message);
    };
    static ILogger* Get()
    {
        static PrintfLogger logger;
        return &logger;
    }

    void PreLog(TS::LogMetaData& logMetaData)override
    {
        const HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD attr = 0;
        attr |= FOREGROUND_INTENSITY;

        switch (logMetaData.logLevel)
        {
            case TS::ErrorLevel::Error:	    attr |= (FOREGROUND_RED); break;
            case TS::ErrorLevel::Success:   attr |= (FOREGROUND_GREEN); break;
            case TS::ErrorLevel::Warning:	attr |= (FOREGROUND_RED | FOREGROUND_GREEN); break;
        default:
            attr |= (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN); break;
        }

        SetConsoleTextAttribute(hCons, attr);
    }
    void EndLog(TS::LogMetaData& logMetaData)override
    {
        const HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
        const WORD attr = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
        SetConsoleTextAttribute(hCons, attr);
    }

};

TS::ILogger* TS::ILogger::GetDefault()
{
    return PrintfLogger::Get();
}


TS::ErrorResult::ErrorResult(const TS::TsChar* message, const ErrorLevel error_level, ILogger* logger)
{
    _errorLevel = error_level;
    _message = message;
    _logger = logger != nullptr ? logger : PrintfLogger::Get();
}

TS::ErrorResult TS::ErrorResult::Log() const
{
//    if(_logger != nullptr)
//        _logger->Log(_message);
    return *this;
}

bool TS::ErrorResult::DoError(bool isThrow) const
{
    if (IsError())
    {
        Log();
        if (isThrow) 
            throw _message;
        return true;
    }
    return false;
}

TS::ErrorResult TS::ErrorResult::Do() const
{
    if (IsError())
    {
        Log();
        throw _message;
    }
    return *this;
}

//------------------------------------------------------------------------------------------------------------------------
//! Error

TS::ErrorResult TS::Error::Success = TS::ErrorResult(_T(""), TS::ErrorLevel::Success, nullptr);
TS::ILogger* TS::Error::_loger = nullptr;

TS::ErrorResult TS::Error::Make(HRESULT hResult)
{
    if(FAILED(hResult))
    {
        static TsChar message[1024];
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hResult,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            message, 0, nullptr);

        return ErrorResult(message);
    }
    return Success;
}

TS::ErrorResult TS::Error::Make(const TS::TsChar* message, const ErrorLevel error_level)
{
    return ErrorResult(message, error_level, _loger);
}

TS::ErrorResult TS::Error::Assert(bool _true, const TS::TsChar* message)
{
#ifdef _DEBUG
    return _true ? Success : Make(message, ErrorLevel::Develop).Do();
#else
    return Success;
#endif
}

void TS::Error::SetLogger(ILogger* logger)
{
    _loger = logger;
}
