#ifndef mip_vm_compiler_included
#define mip_vm_compiler_included
//----------------------------------------------------------------------

/*
  TODO:
    - 2-pass, 2nd to resolve adresses to labels and words
    - macro, insert opcodes directly, instead of func call
    - do some of this in parsing...
    - jit (?)
    - comments: { }

    label & -> @
    var   % -> #
*/

#include "base/types/mip_stack.h"
//#include "src/h_String.h"
#include "base/utils/mip_vm.h"

#define MIP_VM_COMPILER_NUM_OPCODES     65536
#define MIP_VM_COMPILER_MAX_CONDSTACK   32

//----------------------------------------------------------------------

struct MIP_VMSrcWord {
  char    name[16];
  int     pos;
};

struct MIP_VMSrcLabel {
  char    name[16];
  int     pos;
};

struct MIP_VMSrcVar {
  char    name[16];
  int     pos;
};

typedef MIP_Array<MIP_VMSrcWord>                      MIP_VMSrcWords;
typedef MIP_Array<MIP_VMSrcLabel>                     MIP_VMSrcLabels;
typedef MIP_Array<MIP_VMSrcVar>                       MIP_VMSrcVars;
typedef MIP_Stack<int,MIP_VM_COMPILER_MAX_CONDSTACK>  MIP_VMCondStack;
typedef MIP_Array<const char*>                        MIP_VMTokens;

//----------------------------------------------------------------------

class MIP_VMCompiler {

//------------------------------
private:
//------------------------------

  char*           m_Sourcecode      = nullptr;
  int             m_SourcecodeSize  = 0;
  MIP_VMOpcode*   m_Opcodes         = nullptr;
  int             m_NumOpcodes      = 0;
  MIP_VMTokens    m_Tokens          = {};
  MIP_VMSrcWords  m_Words           = {};
  MIP_VMSrcLabels m_Labels          = {};
  MIP_VMSrcVars   m_Vars            = {};
  MIP_VMCondStack m_CondStack       = {};
  int             m_Pos             = 0;
  bool            m_NewWord         = false;
  bool            m_NewLabel        = false;
  bool            m_NewVar          = false;

//------------------------------
public:
//------------------------------

  MIP_VMCompiler() {
    m_Opcodes = new MIP_VMOpcode[MIP_VM_COMPILER_NUM_OPCODES];
  }

  //----------

  ~MIP_VMCompiler() {
    if (m_Sourcecode) free(m_Sourcecode);
    if (m_Opcodes) delete[] m_Opcodes;
    // delete words
    // delete labels
  }

//------------------------------
//
//------------------------------

  #define WRITE_op(t,v) m_Opcodes[m_Pos].type = t;                \
                        m_Opcodes[m_Pos].value = (MIP_VMValue)v;  \
                        m_Pos++

  #define PUSH_cond     m_CondStack.push(m_Pos)
  #define POP_cond      m_CondStack.pop()


//------------------------------
public:
//------------------------------

  void parse(char* a_Source, int a_Size) {
    m_SourcecodeSize = a_Size;
    m_Sourcecode = (char*)malloc(a_Size + 1);
    memcpy(m_Sourcecode,a_Source,a_Size);
    m_Sourcecode[a_Size] = 0;
    int pos = 0;
    char prev = 0;
    while (pos < a_Size) {
      unsigned char ch = m_Sourcecode[pos];
      if (ch <= 32) {
        ch = 0;
        m_Sourcecode[pos] = 0;
      }
      else if (prev == 0) {
        const char* token = &m_Sourcecode[pos];
        m_Tokens.append(token);
      }
      prev = ch;
      pos++;
    }
  }

  //----------

  //void preprocess() {
  //}

  //----------

  void compile() {
    m_Pos = 0;
    m_NewWord = false;
    uint32_t tok = 0;
    while (tok < m_Tokens.size()) {
      //char* token = NEXT_t;
      const char* token = m_Tokens[tok++];
      // try them all, until one reports 'true'
      if (!encode_newword(token))     // ()
      if (!encode_newlabel(token))    // &
      if (!encode_newvar(token))      // %
      if (!encode_conditional(token)) // ?:;
      if (!encode_word(token))
      if (!encode_label(token))
      if (!encode_var(token))
      if (!encode_internal(token))
      encode_number(token);
    } // while tok
    WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_EXIT);
    m_NumOpcodes = m_Pos;
  }

  //----------

  int getNumOpcodes() {
    return m_NumOpcodes;
  }

  //----------

  MIP_VMOpcode* getOpcodes() {
    return m_Opcodes;
  }

  //----------

  uint32_t getNumTokens() {
    return m_Tokens.size();
  }

  const char* getToken(uint32_t AIndex) {
    return m_Tokens[AIndex];
  }

  //----------

  int findWord(char* token) {
    for (uint32_t i=0; i<m_Words.size(); i++) {
      if (strcmp(m_Words[i].name,token)==0) return m_Words[i].pos;
      }
    return -1;
  }

  //----------

  int findLabel(char* token) {
    for (uint32_t i=0; i<m_Labels.size(); i++) {
      if (strcmp(m_Labels[i].name,token)==0) return m_Labels[i].pos;
    }
    return -1;
  }

  //----------

  int findVar(char* token) {
    for (uint32_t i=0; i<m_Vars.size(); i++) {
      if (strcmp(m_Vars[i].name,token)==0) return m_Vars[i].pos;
    }
    return -1;
  }

//------------------------------
private:
//------------------------------

  int _find_opcode(const char* token) {
    for (int i=0; i<MIP_VM_NUM_OPCODES; i++) {
      if (strcmp(MIP_VMOpcodeNames[i],token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_word(const char* token) {
    for (uint32_t i=0; i<m_Words.size(); i++) {
      if (strcmp(m_Words[i].name,token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_label(const char* token) {
    for (uint32_t i=0; i<m_Labels.size(); i++) {
      if (strcmp(m_Labels[i].name,token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_var(const char* token) {
    for (uint32_t i=0; i<m_Vars.size(); i++) {
      if (strcmp(m_Vars[i].name,token) == 0) return i;
    }
    return -1;
  }

//------------------------------
private:
//------------------------------

  // ( call, ) return

  bool encode_newword(const char* token) {
    if ((strcmp(token,"(") == 0)) {
      m_NewWord = true;
      return true;
    }
    else if (strcmp(token,")") == 0) {
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_RET);
      m_NewWord = false;
      return true;
    }
    else if (m_NewWord) {
      MIP_VMSrcWord word;
      word.pos = m_Pos;
      strcpy(word.name,token);
      m_Words.append(word);
      m_NewWord = false;
      return true;
    }
    return false;
  }

  //----------

  // &

  bool encode_newlabel(const char* token) {
    if ((strcmp(token,"&") == 0)) {
      m_NewLabel = true;
      return true;
    }
    else if (m_NewLabel) {
      MIP_VMSrcLabel label;
      label.pos = m_Pos;
      strcpy(label.name,token);
      m_Labels.append(label);
      m_NewLabel = false;
      return true;
    }
    return false;
  }

  //----------

  // %

  bool encode_newvar(const char* token) {
    if ((strcmp(token,"%") == 0)) {
      m_NewVar = true;
      return true;
    }
    else if (m_NewVar) {
      MIP_VMSrcVar var;
      var.pos = m_Vars.size();
      strcpy(var.name,token);
      m_Vars.append(var);
      m_NewVar = false;
      return true;
    }
    return false;
  }

  //----------

  // ? : ;

  bool encode_conditional(const char* token) {
    if ((strcmp(token,"?") == 0) ) { // if
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_JZ);
      PUSH_cond;
      WRITE_op(MIP_VM_OP_TYPE_CODEPTR,0); // temp
      return true;
    }
    else if ((strcmp(token,":") == 0)) {// else
      int pos = POP_cond;
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_JMP);
      PUSH_cond;
      WRITE_op(MIP_VM_OP_TYPE_CODEPTR,0); // temp
      m_Opcodes[pos].value = (MIP_VMValue)m_Pos;
      return true;
    }
    else if ((strcmp(token,";") == 0)) {// endif
      int pos = POP_cond;
      m_Opcodes[pos].value = (MIP_VMValue)m_Pos;
      return true;
    }
    return false;
  }

  //----------

  bool encode_word(const char* token) {
    int word = _find_word(token);
    if (word >= 0) {
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_CALL);
      WRITE_op(MIP_VM_OP_TYPE_CODEPTR, m_Words[word].pos);
      return true;
    }
    return false;
  }

  //----------

  bool encode_label(const char* token) {
    int label = _find_label(token);
    if (label >= 0) {
      //WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_jmp);
      WRITE_op(MIP_VM_OP_TYPE_CODEPTR, m_Labels[label].pos);
      return true;
    }
    return false;
  }

  //----------

  // <- push
  // -> pop

  bool encode_var(const char* token) {
    if ((strcmp(token,"->") == 0)) {
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_POPD);
      return true;
    }
    else if ((strcmp(token,"<-") == 0)) {
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_PUSHD);
      return true;
    }
    //else
    int var = _find_var(token);
    if (var >= 0) {
      WRITE_op(MIP_VM_OP_TYPE_DATAPTR, m_Vars[var].pos);
      return true;
    }
    return false;
  }

  //----------

  bool encode_internal(const char* token) {
    int internal = _find_opcode(token);
    if (internal >= 0) {
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,internal);
      return true;
    }
    return false;
  }

  //----------

  bool encode_number(const char* token) {
    if (strchr(token,'.') != nullptr) { // float
      float f = atof(token);
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_PUSH);
      WRITE_op(MIP_VM_OP_TYPE_FLOAT, *(MIP_VMValue*)&f );   // warning: dereferencing type-punned pointer will break strict-aliasing rules...
      return true;                                          // TODO: the _may_alias 'trick ?
    }
    else { // int
      int i = atoi(token);
      WRITE_op(MIP_VM_OP_TYPE_INTERNAL,MIP_OP_PUSH);
      WRITE_op(MIP_VM_OP_TYPE_INT, i );
      return true;
    }
    return false;
  }

  //------------------------------

  #undef WRITE_op
  #undef PUSH_cond
  #undef POP_cond

};

//----------------------------------------------------------------------
#endif
