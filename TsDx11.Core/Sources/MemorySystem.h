#pragma once

namespace TS
{
    /**
    * \brief
    */
    struct DebugInfo
    {
        int line;
        const char* filename;
        const char* function;

        DebugInfo(int __line__ = 0, const char * __filename__ = "", const char * __functionname__ = "")
        {
            line = __line__;
            filename = __filename__;
            function = __functionname__;
        }
    };

    /**
    * \brief メモリを確保したときのヘッダデータ
    */
    struct MemoryMetaData
    {
        size_t      objectSize;     //! 確保したオブジェクトの1つあたりサイズ
        size_t      arrayCount;     //! 確保したオブジェクトの数

        MemoryMetaData * pPrevBlock;
        MemoryMetaData * pNextBlock;

        int         line;           //! ソースコード内での行番号
        const char* fileName;       //! ソースコードのファイル名
        const char* functionName;   //! 関数名
        const char* typeData;       //! タイプ情報
    };
    
    /**
    * \brief メモリのメタデータを取得します。
    * \param pointer メモリを開放するポインタ
    */
    inline MemoryMetaData& GetMemoryMetaDeta(void* pointer)
    {
        return *reinterpret_cast<MemoryMetaData*>(static_cast<char*>(pointer) - sizeof(MemoryMetaData));
    }

    class MemorySystem
    {
    public:
        MemorySystem(IAllocator * pAllocator, bool _isMemoryLeakCheck);

        /**
        * \brief メモリリークチェックが有効か取得する
        * \return
        */
        bool IsEnableMemoryLeak() const;

        /**
        * \brief デフォルトアロケータを取得する
        * \return
        */
        IAllocator* GetAllocator()const;

        /**
        * \brief メモリを確保した際の情報を登録する
        * \param pMetaData
        */
        void RegisterMemoryMetaData(MemoryMetaData* pMetaData);

        /**
        * \brief メモリを開放する際に登録したメタデータを削除する
        * \param pMetaData
        */
        void RemoveMemoryMetaData(MemoryMetaData * pMetaData);

        /**
        * \brief デバッグ情報を設定する、 Alloc の前に呼び出す
        * \param debugInfo
        */
        MemorySystem& SetDebugInfo(const DebugInfo& debugInfo);

        template<typename T, typename... Params>
        T* Alloc(Params ... params);

        /**
        * \brief 配列のメモリを確保する
        */
        template<typename T>
        T* AllocArray(size_t itemCount = 0);

        /**
        * \brief メモリを開放する
        * \param ptr メモリを開放するポインタ
        */
        template <typename T>
        void Delete(T*& ptr);

        /**
         * \brief 現在確保されているメモリチャンクの数を取得する
         * \return 
         */
        unsigned GetAllocatedChunkCount()const;
    private:
        bool _isLeakChek;
        DebugInfo   _debugInfo;
        IAllocator* _pAllocator;
        MemoryMetaData* _pHeadMetaData;
        MemoryMetaData* _pCurrentMetaData;
    };
}

#include "MemorySystem.hpp"
