#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TsDx11.Core/Include/TsDx11Core.h"
#pragma comment(lib,"winmm.lib")
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		

    void Log(const char* pcFormat, ...)
    {
        va_list valist;

        va_start(valist, pcFormat);
        static char message[65535]{};

        vsprintf_s(message, 65535, pcFormat, valist);
        va_end(valist);
        Logger::WriteMessage(message);
    }
    TEST_CLASS(UnitTest1)
    {
    public:
        
        TEST_METHOD(allocator_benchmark)
        {
            // アロケータ速度テスト
            auto t = timeGetTime();
            for( int i=0; i < 10000000 ; ++i )
            {
                void* p = malloc(sizeof(int));
                free(p);
            }
            Log("malloc - %d \n", timeGetTime() - t);

            //! カスタムアロケータ
            auto allocator = TS::Allocator(1024);
            t = timeGetTime();
            for (int i = 0; i < 10000000; ++i)
            {
                void* p = allocator.Alloc(sizeof(int));
                allocator.Free(p);
            }
            Log("ts::allocator - %d \n", timeGetTime() - t);
        }
        
        TEST_METHOD(string_replace)
        {
            //! 文字列置換テスト
            TS::StringA string = "abc012abc012abc012";
            const TS::StringA success = "fbcdefgfbcdefgfbcdefg";
            string = string.Replace("012", "defg");
            string = string.Replace('a', 'f');
            Log("%s \n", static_cast<char*>(string));
            Assert::IsTrue(string == success);            
        }


    };
}