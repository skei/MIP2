
class MIP_Tree;
class MIP_TreeNode;
typedef MIP_Array<MIP_TreeNode*> MIP_TreeNodes;

//----------------------------------------------------------------------

class MIP_TreeNode {
private:
  MIP_Tree*     MOwner;
  MIP_TreeNode* MParent;
  MIP_TreeNodes MChildren;
};

//----------------------------------------------------------------------

class MIP_Tree {
private:
  MIP_TreeNode MRoot;
};

//----------------------------------------------------------------------

