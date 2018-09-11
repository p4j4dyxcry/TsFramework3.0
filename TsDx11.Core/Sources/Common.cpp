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

TS::ErrorResult::ErrorResult(const TS::TsChar* message, const ErrorLevel error_level, ILogger* logger)
{
    _errorLevel = error_level;
    _message = message;
    _logger = logger;
}

TS::ErrorResult TS::ErrorResult::Log() const
{
    if(_logger != nullptr)
        _logger->Log(_message, _errorLevel);
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
