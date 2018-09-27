#pragma once

#include "pch.h"

//--------------------------------------------
//! 共通定義
//--------------------------------------------

namespace TS
{
    //-------------------------------------------------------
    //! 文字コード
#ifdef UNICODE
    using TsChar = wchar_t;
#ifndef _T
#define _T(__char__string__) L#__char__string__
#endif
#else
    using TsChar = char;
#ifndef _T
#define _T(__char__string__) __char__string__
#endif
#endif

    //-------------------------------------------------------
    //! メモリ解放
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( ptr )\
        {\
            if( (ptr) != nullptr )\
            {\
                (ptr)->Release();\
                (ptr) = nullptr;\
            }\
        }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE( ptr )\
        {\
            if( ptr != nullptr )\
            {\
                TS_DELETE(ptr);\
                ptr = nullptr;\
            }\
        }
#endif   

//! スマートポインタ用のカスタムデリータ―
    template<typename T>
    class ReleaseDeleter
    {
    public:
        void operator()(T*& _ptr)
        {
            if (_ptr)
                _ptr->Release();
            _ptr = nullptr;
        }
    };

    //-------------------------------------------------------
    //! 開発支援

#define TS_CLASS_DISABLE_COPY(name) name(const name&) = delete;name& operator =(const name&) = delete
#define TS_CLASS_DISABLE_MOVE(name) name(const name&&) = delete;name& operator =(const name&&) = delete

    enum class ErrorLevel : unsigned
    {
        Unknown,
        Success,
        Develop,
        Warning,
        Error,
    };

    /**
    * \brief ログを出力するためのメタデータを定義します
    */
    struct LogMetaData
    {
        int line;                   //! ソースコード内での行番号
        const char* fileName;       //! ソースコードのファイル名
        const char* functionName;   //! 関数名
        ErrorLevel logLevel;          //! 重大性
    };

    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void Log(const char* message, LogMetaData errorLevel) = 0;
        static ILogger* GetDefault();

        static const char* Format(const char * format, ...)
        {
            static char logBuffer[4096] = "\0";

            char* ap;
            va_start(ap, format);
            vsprintf_s(logBuffer, format, ap);
            return logBuffer;
        }
    protected:
        virtual void PreLog(LogMetaData& logMetaData) = 0;
        virtual void EndLog(LogMetaData& logMetaData) = 0;
    };

    struct ErrorResult
    {
        friend class Error;
    public:

        bool IsError() const;

        const TsChar* GetErrorMessage() const;

        ErrorLevel GetErrorLevel() const;

        ErrorResult Log() const;

        bool DoError(bool isThrow = true) const;

        ErrorResult Do()const;

    private:
        ErrorResult(const TS::TsChar* message = _T(""), const ErrorLevel error_level = ErrorLevel::Error, ILogger* logger = nullptr);

    private:
        const TS::TsChar * _message;
        TS::ErrorLevel _errorLevel;
        ILogger* _logger;
    };

    //! ログを出力
#define TS_LOG(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = __LINE__; \
        metadata.fileName = __FILE__; \
        metadata.functionName = __FUNCTION__; \
        metadata.logLevel = TS::ErrorLevel::Develop; \
        TS::ILogger::GetDefault()->Log(TS::ILogger::Format(__VA_ARGS__),metadata);\
    }

    //! デバッグログを出力
#ifdef _DEBUG
#define TS_LOG_DEBUG(...)\
        {\
            TS::LogMetaData metadata; \
            metadata.line = __LINE__; \
            metadata.fileName = __FILE__; \
            metadata.functionName = __FUNCTION__; \
            metadata.logLevel = TS::ErrorLevel::Develop; \
            TS::ILogger::GetDefault()->Log(TS::ILogger::Format(__VA_ARGS__),metadata);\
        }
#else
#define TS_LOG_DEBUG(...);
#endif


    //! ワーニングログ
#define TS_LOG_WARNING(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = __LINE__; \
        metadata.fileName = __FILE__; \
        metadata.functionName = __FUNCTION__; \
        metadata.logLevel = TS::ErrorLevel::Warning; \
        TS::ILogger::GetDefault()->Log(TS::ILogger::Format(__VA_ARGS__),metadata);\
    }

    //! エラーログを出力
#define TS_LOG_ERROR(...)\
    {\
        TS::LogMetaData metadata; \
        metadata.line = __LINE__; \
        metadata.fileName = __FILE__; \
        metadata.functionName = __FUNCTION__; \
        metadata.logLevel = TS::ErrorLevel::Error; \
        TS::ILogger::GetDefault()->Log(TS::ILogger::Format(__VA_ARGS__),metadata);\
    }

    class Error
    {
    public:
        static ErrorResult Success;
        static ErrorResult Make(HRESULT hResult);
        static ErrorResult Make(const TS::TsChar* message = _T(""), const ErrorLevel error_level = ErrorLevel::Error);
        static ErrorResult Assert(bool _true, const TS::TsChar* message = _T("Assert に失敗しました"));
        static void SetLogger(ILogger* logger);

    private:
        static ILogger* _loger;
    };

    namespace ExceptionMessage
    {
        static const char * IndexOfOutrange = "配列のアクセスに無効な添え字が使われました。";
        static const char * NotFound        = "指定されたデータが見つかりませんでした。";
        static const char * InvalidArgs     = "無効な引数が指定されました。";
        static const char * AllocFailed     = "メモリ確保に失敗しました。";
    }


    //-------------------------------------------------------
    //! 基本型

    #define TsSharedPtr  std::shared_ptr
    #define TsWeakptr    std::weak_ptr   
    #define TsMakeShared std::make_shared

    using HashCode = unsigned long long;
}