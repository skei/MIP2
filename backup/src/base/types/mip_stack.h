#ifndef mip_stack_included
#define mip_stack_included
//----------------------------------------------------------------------

/*
  - primitive
  - no error checking (yet)
*/

//----------------------------------------------------------------------

template<class _T,int SIZE>
class MIP_Stack
{

//------------------------------
private:
//------------------------------

  _T  MBuffer[SIZE];
  int MIndex;

//------------------------------
public:
//------------------------------

  MIP_Stack() {
    reset();
  }

  //----------

  ~MIP_Stack() {
  }

//------------------------------
public:
//------------------------------

  void reset(void) {
    MIndex = 0;
  }

  //----------

  int getSize() {
    return SIZE;
  }

  //----------

  int getNumItems() {
    return MIndex;
  }

  //----------

  bool isEmpty() {
    return (MIndex > 0);
  }

  //----------

  _T* getTop(void) {
    return &MBuffer[MIndex-1];
  }

  //----------

  // c  ->  a b -  ->  c a b -

  void push(_T AItem) {
    MBuffer[MIndex] = AItem;
    MIndex++;
  }

  //----------

  // a b -  ->  b - -  -> [a]

  _T pop(void) {
    MIndex--;
    return MBuffer[MIndex];
  }

  //----------

  // a b -  ->  a a b -

  void dup(void) {
    MBuffer[MIndex] = MBuffer[MIndex-1];
    MIndex++;
  }

  //----------

  // a b -  ->  b a b -

  void dup2(void) {
    MBuffer[MIndex] = MBuffer[MIndex-2];
    MIndex++;
  }

  //----------

  // a b -  ->  b - -  ->

  void drop(void) {
    MIndex--;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
