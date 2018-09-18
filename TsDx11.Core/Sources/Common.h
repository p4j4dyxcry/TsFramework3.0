#pragma once
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
		        delete ptr;\
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

    class ILogger
    {
    public:
        virtual ~ILogger() = default;
        virtual void Log(const TsChar* message, ErrorLevel errorLevel) = 0;
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


    //-------------------------------------------------------
    //! 基本型

#define TsSharedPtr  std::shared_ptr
#define TsWeakptr    std::weak_ptr   
#define TsMakeShared std::make_shared
#define TsList       std::list
#define TsMap        std::unordered_map

    using HashCode = unsigned long long;
}