#ifndef mip_assoc_array_included
#define mip_assoc_array_included
//----------------------------------------------------------------------

/*
  based on:
    http://www.codeproject.com/Articles/149879/Associative-Array-in-C
    by Arnav Mukhopadhyay
*/

//----------------------------------------------------------------------

#include "mip_array.h"

//----------

template <class _T>
class MIP_AssocArray {

  private:

    struct MIP_AssocArrayItem {
      const char* name;
      _T value;
    };

    MIP_Array<MIP_AssocArrayItem> MItems;

  //------------------------------
  //
  //------------------------------

  public:

    uint32_t size() {
      return MItems.size();
    }

    //----------

    bool hasItem(const char* name) {
      for (uint32_t i=0; i<size(); i++) {
        if (MItems[i].name == name) return true; // strcmp
      }
      return false;
    }

    //----------

    int32_t getItemIndex(const char* name) {
      for (uint32_t i=0; i<size(); i++) {
        if (MItems[i].name == name) return i; // strcmp
      }
      return -1;
    }

    //----------

    const char* getItemName(uint32_t index) {
      return MItems[index].name;
    }

    //----------

    _T& operator [] (uint32_t index) {
      return MItems[index].value;
    }

    //----------

    _T& operator [] (const char* name) {
      for (uint32_t i=0; i<size(); i++) {
        if (MItems[i].name == name) return MItems[i].value; // strcmp
      }
      uint32_t idx = size();
      MIP_AssocArrayItem d;
      d.name = name;
      d.value = 0;  // ???
      MItems.append(d);
      return MItems[idx].value;
    }

    //----------

    bool addItem(const char* name, _T value) {
      if (hasItem(name)) return false;
      MIP_AssocArrayItem d;
      d.name = name;
      d.value = value;
      MItems.append(d);
      return true;
    }

    //----------

    bool removeItem(uint32_t index) {
      MItems.remove(index);
      return true;
    }

    //----------

    bool removeItem(const char* name) {
      for (uint32_t i=0; i<size(); i++) {
        if (MItems[i].name == name) { // strcmp
          MItems.remove(i);
          return true;
        }
      }
      return false;
    }

    //----------

};

//----------------------------------------------------------------------
#endif
