#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TsAssetInterface/Include/TsAssetInterface.h"
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

    class TestTimer
    {
    public:
        TestTimer()
        {
            Restart();
        }     

        void PrintElapsedTime(const char * message) const
        {
            Log("%s - %d \n",message, timeGetTime() - _startTime);
        }

        void Restart()
        {
            _startTime = timeGetTime();
        }

    private:
        DWORD _startTime;
    };

    #define RUN_ACTION(...) Log("RUN_ACTION = [%s]\n",#__VA_ARGS__);__VA_ARGS__
    #define IS_TRUE(...) Log("%s\n",#__VA_ARGS__);Assert::IsTrue(__VA_ARGS__)

    TEST_CLASS(UnitTest1)
    {
    public:
        
        TEST_METHOD(allocator_benchmark)
        {
            // �A���P�[�^���x�e�X�g
            auto timer = TestTimer();
            for( int i=0; i < 10000000 ; ++i )
            {
                void* p = malloc(sizeof(int));
                free(p);
            }
            timer.PrintElapsedTime("malloc");

            //! �J�X�^���A���P�[�^
            auto allocator = TS::Allocator(1024);
            timer.Restart();
            for (int i = 0; i < 10000000; ++i)
            {
                void* p = allocator.Alloc(sizeof(int));
                allocator.Free(p);
            }
            timer.PrintElapsedTime("ts::allocator");
        }

        TEST_METHOD(collection_test)
        {
            TS::Collection<int> data;

            RUN_ACTION(data.Add(1));
            {
                IS_TRUE(data[0] == 1);
            }

            RUN_ACTION(for (auto& i : data) { i = 2; });
            {
                IS_TRUE(data[0] == 2);
            }

            RUN_ACTION(data = { 6,7,8 });
            {
                IS_TRUE(data[0] == 6);
                IS_TRUE(data[1] == 7);
                IS_TRUE(data[2] == 8);
            }


            RUN_ACTION(data.Remove(7));
            {
                IS_TRUE(data[0] == 6);
                IS_TRUE(data[1] == 8);
            }

            RUN_ACTION(data.RemoveAt(0));
            {
                IS_TRUE(data[0] == 8);
            }

            RUN_ACTION(data.Insert(0,10));
            {
                IS_TRUE(data.Length() == 2);
                IS_TRUE(data[0] == 10);
                IS_TRUE(data[1] == 8);
            }

            RUN_ACTION(data.RemoveRange(0, 2));
            {
                IS_TRUE(data.Length() == 0);
            }

            RUN_ACTION(data.AddRange({1,2}));
            {
                IS_TRUE(data[0] == 1);
                IS_TRUE(data[1] == 2);
            }

            RUN_ACTION(data.Swap(0,1));
            {
                IS_TRUE(data[0] == 2);
                IS_TRUE(data[1] == 1);
            }

            RUN_ACTION(data.Resize(0));
            {
                IS_TRUE(data.IsEmpty() == true);
            }

            RUN_ACTION(data.Resize(1024));
            {
                IS_TRUE(data.Length() == 1024);
            }

            auto a = data.Select<int>([](int& xa) {return 2; })
                         .Where([](int& x) {return x > 0; })
                         .Any();
        }

        TEST_METHOD(string_replace)
        {
            //! ������u���e�X�g
            TS::StringA string = "abc012abc012abc012";
            const TS::StringA success = "fbcdefgfbcdefgfbcdefg";
            RUN_ACTION(string = string.Replace("012", "defg"));
            RUN_ACTION(string = string.Replace('a', 'f'));
            Log("%s \n", string.Data());
            IS_TRUE(string == success);
        }

        TEST_METHOD(string_find)
        {
            TS::StringA string = "110306060";
            
            int index = 0;

            RUN_ACTION(index = string.Find("7"));
            IS_TRUE(index == -1);

            RUN_ACTION(index = string.Rfind('7'));
            IS_TRUE(index == -1);

            RUN_ACTION(index = string.Find("6"));
            IS_TRUE(index == 5);

            RUN_ACTION(index = string.Rfind('6'));
            IS_TRUE(index == 7);

            RUN_ACTION(index = string.Find("06"));
            IS_TRUE(index == 4);

            RUN_ACTION(index = string.Find("1"));
            IS_TRUE(index == 0);

            RUN_ACTION(index = string.Rfind("1"));
            IS_TRUE(index == 1);

            RUN_ACTION(index = string.Find("60"));
            IS_TRUE(index == 5);

            RUN_ACTION(index = string.Rfind("60"));
            IS_TRUE(index == 7);
        }

        TEST_METHOD(obj_loader)
        {
            TS::ObjParser parser;
            parser.Parse("../UnitTest/test_data/cube.obj");
            parser.SaveAs("../UnitTest/test_data/cube_edit.obj");

            parser.Parse("../UnitTest/test_data/cube_edit.obj");
            parser.SaveAs("../UnitTest/test_data/cube_edit_edit.obj");

            parser.Parse("../UnitTest/test_data/diamond.obj");
            parser.SaveAs("../UnitTest/test_data/diamond_edit.obj");

            parser.Parse("../UnitTest/test_data/diamond_edit.obj");
            parser.SaveAs("../UnitTest/test_data/diamond_edit_edit.obj");

            parser.Parse("../UnitTest/test_data/magnolia.obj");
            parser.SaveAs("../UnitTest/test_data/magnolia_edit.obj");

            parser.Parse("../UnitTest/test_data/magnolia_edit.obj");
            parser.SaveAs("../UnitTest/test_data/magnolia_edit_edit.obj");
        }

        TEST_METHOD(convert_obj_test)
        {
            auto model = TS::BasicModel();
            
            RUN_ACTION(model = TS::LoadModelFromFile("../UnitTest/test_data/cube.obj"));

            IS_TRUE(model.meshes.Length() == 1);
            IS_TRUE(model.meshes[0].postions.Length() == 8);
            IS_TRUE(model.meshes[0].normals.Length()  == 8);
            IS_TRUE(model.meshes[0].uvs.Length() == 0);
            IS_TRUE(model.meshes[0].indices.Length()     == 12);

            RUN_ACTION(model = TS::LoadModelFromFile("../UnitTest/test_data/diamond.obj"));

            IS_TRUE(model.meshes.Length() == 1);
            IS_TRUE(model.meshes[0].postions.Length() == 6);
            IS_TRUE(model.meshes[0].normals.Length() == 0);
            IS_TRUE(model.meshes[0].uvs.Length() == 0);
            IS_TRUE(model.meshes[0].indices.Length() == 9);
        }
    };
}