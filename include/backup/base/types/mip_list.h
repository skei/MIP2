#ifndef mip_list_included
#define mip_list_included
//----------------------------------------------------------------------

/*

doubly-linked 'invasive' list..

  a templates (ala array)data-container)
  b multiple inheritance (ala widgetListener)
  * non-invasive list.. nodes = next, prev, ptr

*/

//----------------------------------------------------------------------

class MIP_ListNode {
private:
  MIP_ListNode*  MPrev = nullptr;
  MIP_ListNode*  MNext = nullptr;
public:
  MIP_ListNode() {}
  virtual ~MIP_ListNode() {}
public:
  MIP_ListNode*  prev() { return MPrev; }
  MIP_ListNode*  next() { return MNext; }
  void prev(MIP_ListNode* ANode) { MPrev = ANode; }
  void next(MIP_ListNode* ANode) { MNext = ANode; }
};

//----------------------------------------------------------------------

class MIP_List {

//------------------------------
private:
//------------------------------

  MIP_ListNode*  MHead = nullptr;
  MIP_ListNode*  MTail = nullptr;
  uint32_t       MSize = 0;

//------------------------------
public:
//------------------------------

  MIP_List() {}
  virtual ~MIP_List() {}

//------------------------------
public:
//------------------------------

public:

  MIP_ListNode*  head() { return MHead; }
  MIP_ListNode*  tail() { return MTail; }

  void head(MIP_ListNode* ANode) { MHead = ANode; }
  void tail(MIP_ListNode* ANode) { MTail = ANode; }

  //----------

  void clear() {
    MIP_ListNode* node = MHead;
    while (node) {
      MIP_ListNode* next = node->next();
      delete node;
      node = next;
    }
  }

  //----------

  bool isEmpty() {
    return (MHead == nullptr);
  }

  //----------

  bool isNotEmpty() {
    return (MHead != nullptr);
  }

  //----------

  bool hasNode(MIP_ListNode* ANode) {
    MIP_ListNode* node = MHead;
    while (node) {
      if (node == ANode) return true;
      node = node->next();
    }
    return false;
  }

  //----------

  void insertHead(MIP_ListNode* ANode) {
    MSize += 1;
    if (MHead) {
      // -H -> -*H
      MHead->prev(ANode);
      ANode->next(MHead);
      ANode->prev(nullptr);
      MHead = ANode;
    }
    else {
      // - -> -*-
      ANode->next(nullptr);
      ANode->prev(nullptr);
      MTail = ANode;
      MHead = ANode;
    }
  }

  //----------

  void insertTail(MIP_ListNode* ANode) {
    MSize += 1;
    if (MTail) {
      // T- -> T*-
      ANode->next(nullptr);
      ANode->prev(MTail);
      MTail->next(ANode);
      MTail = ANode;
    }
    else {
      // empty list
      // - -> -*-
      ANode->next(nullptr);
      ANode->prev(nullptr);
      MTail = ANode;
      MHead = ANode;
    }
  }

  //----------

  void removeHead() {
    MSize -= 1;
    if (MHead) {
      // -H?
      if (MHead==MTail) {
        // -H- -> -
        MHead = nullptr;
        MTail = nullptr;
      }
      else {
        // -Hn -> n-
        MHead->next()->prev(nullptr);
        MHead = MHead->next();
      }
    }
  }

  //----------

  void removeTail() {
    MSize -= 1;
    if (MTail) {
      // ?T-
      if (MTail==MHead) {
        // -T- -> -
        MHead = nullptr;
        MTail = nullptr;
      }
      else {
        // pT- -> p-
        MTail->prev()->next(nullptr);
        MTail = MTail->prev();
      }
    }
  }

  //----------

  void insertBefore(MIP_ListNode* ANode, MIP_ListNode* ANewNode) {
    MSize += 1;
    if (ANode->prev()) {
      // pN?
      //if (ANode->next()) {
        // pNn -> p*Nn
        ANode->prev(ANewNode);
        ANewNode->next(ANode);
        ANewNode->prev(ANode->prev());
        ANode->prev()->next(ANewNode);
      //}
      //else {
      //  // pN- -> p*N-
      //  // (similar to the above)
      //  ANode->prev(ANewNode);
      //  ANewNode->next(ANode);
      //  ANewNode->prev(ANode->prev());
      //  ANode->prev()->next(ANewNode);
      //}
    }
    else {
      // -N?
      //if (ANode->next()) {
        // -Nn -> -*Nn
        ANode->prev(ANewNode);
        ANewNode->next(ANode);
        ANewNode->prev(nullptr);
        MHead = ANewNode;
      //}
      //else {
      //  // -N- -> -*N-
      //  // (similar to the above)
      //  ANode->prev(ANewNode);
      //  ANewNode->next(ANode);
      //  ANewNode->prev(nullptr);
      //  MHead = ANewNode;
      //}
    }
  }

  //----------

  void insertAfter(MIP_ListNode* ANode, MIP_ListNode* ANewNode) {
    MSize += 1;
    if (ANode->next()) {
      // ?Nn
      //if (ANode->prev()) {
        // pNn -> pN*n
        ANode->next()->prev(ANewNode);
        ANewNode->next(ANode->next());
        ANewNode->prev(ANode);
        ANode->next(ANewNode);
      //}
      //else {
      //  // -Nn -> -N*n
      //  // (similar to the above)
      //  ANode->next()->prev(ANewNode);
      //  ANewNode->next(ANode->next());
      //  ANewNode->prev(ANode);
      //  ANode->next(ANewNode);
      //}
    }
    else {
      // ?N-
      //if (ANode->prev()) {
        // pN- -> pN*-
        ANewNode->next(nullptr);
        ANewNode->prev(ANode);
        ANode->next(ANewNode);
        MTail = ANode;
      //}
      //else {
      //  // -N- -> -N*-
      //  // (similar to the above)
      //  ANewNode->next(nullptr);
      //  ANewNode->prev(ANode);
      //  ANode->next(ANewNode);
      //  MTail = ANode;
      //}
    }
  }

  //----------

  void removeNode(MIP_ListNode* ANode) {
    MSize -= 1;
    if (ANode->next()) {
      // ?Nn
      if (ANode->prev()) {
        // pNn -> pn
        ANode->next()->prev(ANode->prev());
        ANode->prev()->next(ANode->next());
      }
      else {
        // -Nn -> -n
        ANode->next()->prev(nullptr);
        MHead = ANode->next();
      }
    }
    else {
      // ?N-
      if (ANode->prev()) {
        // pN- -> p-
        ANode->prev()->next(nullptr);
        MTail = ANode->prev();
      }
      else {
        // -N- -> -
        MHead = nullptr;
        MTail = nullptr;
      }
    }
  }

  //----------

  //void sortNodes() {
  //}

  //void findNode() {
  //}

  //void swapNodes(MIP_ListNode* ANode1, MIP_ListNode* ANode2) {
  //}

  //void moveNodeToHead(MIP_ListNode* ANode) {
  //}

  //void moveNodeToTail(MIP_ListNode* ANode) {
  //}

  //----------

};

//----------------------------------------------------------------------
#endif
