#pragma once

/**
* \brief メモリを確保したときのヘッダデータ
*/
struct MemoryMetaData
{
    size_t      objectSize;     //! 確保したオブジェクトの1つあたりサイズ
    unsigned    arrayCount;     //! 確保したオブジェクトの数

    MemoryMetaData * pPrevBlock;
    MemoryMetaData * pNextBlock;

    int         line;           //! ソースコード内での行番号
    const char* fileName;       //! ソースコードのファイル名
    const char* functionName;   //! 関数名
    const char* typeData;       //! タイプ情報

    void*		pointer;		//! 先頭ポインタ情報
    IAllocator* pAllocator;		//! アロケータ情報

};

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
    * \param metadata
    */
    void RegisterMemoryMetaData(MemoryMetaData* metadata);
    
    /**
    * \brief メモリを開放する際に登録したメタデータを削除する
    * \param pointer
    */
    void RemoveMemoryMetaData(MemoryMetaData * metadata);
    
    unsigned GetAllocMemoryCount()const;

    void DumpLog();

private:
    bool _isLeakChek;
    IAllocator* _pAllocator;
    MemoryMetaData* _pHeadMetaData;
    MemoryMetaData* _pCurrentMetaData;

};