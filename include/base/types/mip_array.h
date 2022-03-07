#ifndef mip_array_included
#define mip_array_included
//----------------------------------------------------------------------

#include "base/utils/mip_integer_math.h" // MIP_NextPowerOfTwo
//#include "base/mip_stdlib.h"

//----------------------------------------------------------------------

template<class _T>
class MIP_Array {

//------------------------------
private:
//------------------------------

  const uint32_t	MTypeSize   = sizeof(_T);
  _T*             MBuffer     = nullptr;
  uint32_t	      MBufferSize = 0;
  uint32_t	      MSize       = 0;

//------------------------------
public:
//------------------------------

  MIP_Array() {
    //MBuffer     = nullptr;
    //MBufferSize = 0;
    //MSize       = 0;
  }

  //----------

  MIP_Array(const MIP_Array& AArray) {
    MBuffer = (_T*)malloc(MTypeSize*AArray.MBufferSize);
    MBufferSize = AArray.MBufferSize;
    MSize = AArray.MSize;
    memcpy(MBuffer, AArray.MBuffer, MTypeSize*AArray.MBufferSize);
  }

  //----------

  ~MIP_Array() {
    if (MBuffer) {
      free(MBuffer);
    }
  }

//------------------------------
public:
//------------------------------

  uint32_t  size(void)                    { return MSize; }
  void*     buffer(void)                  { return MBuffer; }
  _T&       item(const uint32_t AIndex)   { return MBuffer[AIndex]; }
  uint32_t  bufferSize(void)              { return MBufferSize; }

  void      resize(const uint32_t ASize)  { resizeBuffer(ASize); }

//------------------------------
public:
//------------------------------

  _T& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  //----------

  MIP_Array<_T>& operator = (const MIP_Array<_T> &AArray) {
    if (this == &AArray) return *this;
//      if (AArray.MSize == 0) clear();
//      setSize(AArray.MSize);
    memcpy(MBuffer, AArray.MBuffer, MTypeSize*AArray.MSize);
    return *this;
  }

//------------------------------
private:
//------------------------------

  void freeItems(void) {
    for (uint32_t i=0; i<MSize; i++) free( MBuffer[i] );
  }

  //----------

  void deleteItems(void) {
    for (uint32_t i=0; i<MSize; i++) delete MBuffer[i];
  }

  //----------

  void clearBuffer(bool AErase=false) {
    if (AErase) {
      if (MBuffer) free(MBuffer);
      MBuffer = nullptr;
      MBufferSize = 0;
    }
    MSize = 0;
  }

  //----------

  void resizeBuffer(const uint32_t ABufferSize) {
    uint32_t size = MIP_NextPowerOfTwo(ABufferSize);
    if (size != MBufferSize) {
      MBuffer = (_T*)realloc(MBuffer, MTypeSize*size);
      MBufferSize = size;
      //MSize = size;
      //if (MSize > MBufferSize) MSize = MBufferSize;
    }
  }

  //----------

  void growBuffer(const uint32_t ANum) {
    uint32_t newsize = MSize + ANum;
    resizeBuffer(newsize);
  }

  //----------

  void shrinkBuffer(const uint32_t ANum) {
    uint32_t newsize = MSize - ANum;
    resizeBuffer(newsize);
  }

//------------------------------
public:
//------------------------------

  int32_t findItem(_T& AItem) {
    for (uint32_t i=0; i<MSize; i++) {
      if (MBuffer[i] == AItem) return i;
    }
    return -1;
  }

  void clear(bool AErase=false) {
    clearBuffer(AErase);
  }

  //----------

  void insert(const _T& AItem, uint32_t APos) {
    growBuffer(1);
    memmove( &MBuffer[APos+1], &MBuffer[APos], (MSize-APos) * MTypeSize);
    MBuffer[APos] = AItem;
    MSize++;
  }

  //----------

  void insert(MIP_Array<_T>& AArray, uint32_t APos) {
    uint32_t num = AArray.size();
    growBuffer(num);
    memmove( &MBuffer[APos+num], &MBuffer[APos], (MSize-APos) * MTypeSize);
    memcpy( &MBuffer[APos], AArray.buffer(), num * MTypeSize );
    MSize += num;
  }

  //----------

  void insertHead(const _T& AItem) {
    insert(AItem,0);
  }

  //----------

  void insertTail(const _T& AItem) {
    growBuffer(1);
    MBuffer[MSize] = AItem;
    MSize++;
  }

  //------------------------------

  void remove(const uint32_t APos) {
    memmove( &MBuffer[APos], &MBuffer[APos+1], (MSize-APos-1) * MTypeSize);
    shrinkBuffer(1);
    MSize--;
  }

  //----------

  void remove(const uint32_t APos, uint32_t ANum) {
    //moveBufferItems(APos+ANum,APos,ANum);
    memmove( &MBuffer[APos], &MBuffer[APos+ANum], (MSize-APos-ANum) * MTypeSize);
    shrinkBuffer(ANum);
    MSize -= ANum;
  }

  //----------

  void removeHead(void) {
    remove(0);
  }

  //----------

  void removeTail(void) {
    MSize--;
    shrinkBuffer(1);
  }

//------------------------------
public:
//------------------------------

  int32_t find(const _T& AItem) {
    for (uint32_t i=0; i<MSize; i++) {
      if (MBuffer[i] == AItem) return i;
    }
    return -1;
  }

  //----------

  void append(_T& AItem) {
    insertTail(AItem);
  }

  void push_back(_T& AItem) {
    insertTail(AItem);
  }

  //----------

  _T& head(void) {
    return MBuffer[0];
  }

  //----------

  _T& tail(void) {
    return MBuffer[MSize-1];
  }

  //----------

  void push(_T AItem) {
    insertTail(AItem);
  }

  //----------

  _T pop(void) {
    removeTail();
    return MBuffer[MSize];
  }


};

//----------------------------------------------------------------------

typedef MIP_Array<char>          MIP_CharArray;
typedef MIP_Array<char*>         MIP_CharPtrArray;
typedef MIP_Array<void*>         MIP_PtrArray;
typedef MIP_Array<void**>        MIP_PtrPtrArray;
typedef MIP_Array<float*>        MIP_FloatPtrArray;
typedef MIP_Array<float>         MIP_FloatArray;
typedef MIP_Array<bool>          MIP_BoolArray;
typedef MIP_Array<int32_t>       MIP_Int32Array;
typedef MIP_Array<uint32_t>      MIP_Uint32Array;

//typedef MIP_Array<MIP_String>   MIP_StringArray;
//typedef MIP_Array<MIP_Sample>   MIP_SampleArray;
//typedef MIP_Array<MIP_Sample*>  MIP_SamplePtrArray;

//----------------------------------------------------------------------
#endif














