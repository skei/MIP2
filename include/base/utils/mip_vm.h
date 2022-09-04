#ifndef mip_gcc_vm_included
#define mip_gcc_vm_included
//----------------------------------------------------------------------

/*
  uses calculated gotos ('goto **ip++')..
  .mip_script // text -> parse -> bytecode
  .mip_bytecode

*/

//----------

#include <stdio.h>  // printf
#include "base/types/mip_stack.h"

//----------

#define MIP_VM_CALL_STACK_SIZE   1024
#define MIP_VM_DATA_STACK_SIZE   1024
#define MIP_VM_CODE_SEGMENT_SIZE 65536
#define MIP_VM_DATA_SEGMENT_SIZE 65536
#define MIP_VM_MAX_EXTERNALS     256
#define MIP_VM_NUM_REGISTERS     256

#define MIP_VM_OP_TYPE_NONE      0
#define MIP_VM_OP_TYPE_INTERNAL  1
#define MIP_VM_OP_TYPE_EXTERNAL  2
#define MIP_VM_OP_TYPE_WORD      3
#define MIP_VM_OP_TYPE_INT       4
#define MIP_VM_OP_TYPE_FLOAT     5
#define MIP_VM_OP_TYPE_PTR       6
#define MIP_VM_OP_TYPE_DATAPTR   7
#define MIP_VM_OP_TYPE_CODEPTR   8

enum MIP_EVMOpcodes {
  MIP_OP_NOOP = 0,  // "noop",  // 0
  MIP_OP_EXIT,      // "exit",   // 1
  MIP_OP_PUSH,      // "push",   // 2
  MIP_OP_DROP,      // "drop",   // 3
  MIP_OP_DUP,       // "dup",    // 4
  MIP_OP_DUP2,      // "dup2",   // 5
  MIP_OP_PUSHR,     // "pushr",  // 6
  MIP_OP_POPR,      // "popr",   // 7
  MIP_OP_PUSHD,     // "pushd",  // 8
  MIP_OP_POPD,      // "popd",   // 9
  MIP_OP_CALL,      // "call",   // 10
  MIP_OP_RET,       // "ret",    // 11
  MIP_OP_JMP,       // "jmp",    // 12
  MIP_OP_JZ,        // "jz",     // 13
  MIP_OP_JNZ,       // "jnz",    // 14
  MIP_OP_EQ,        // "eq",     // 15
  MIP_OP_GR,        // "gr",     // 16
  MIP_OP_LE,        // "le",     // 17
  MIP_OP_NE,        // "ne",     // 18
  MIP_OP_ADD,       // "add",    // 19
  MIP_OP_SUB,       // "sub",    // 20
  MIP_OP_MUL,       // "mul",    // 21
  MIP_OP_DIV,       // "div",    // 22
  MIP_OP_INC,       // "inc",    // 23
  MIP_OP_DEC,       // "dec",    // 24
  MIP_OP_PRINT,     // "print",  // 25
  MIP_OP_EXT,       // "ext"     // 26

  MIP_VM_NUM_OPCODES

};

//----------------------------------------------------------------------

//typedef union {
//  double    f;
//  uint64_t  i;
//  void*     p;
//} MIP_VMValue;

typedef intptr_t MIP_VMValue;

struct MIP_VMOpcode {
  int         type;
  MIP_VMValue value;
};


class MIP_VirtualMachine;
typedef void (*MIP_VMExternal)(MIP_VirtualMachine* vm);

typedef MIP_Stack<MIP_VMValue, MIP_VM_DATA_STACK_SIZE> MIP_VMDataStack;
typedef MIP_Stack<MIP_VMValue*,MIP_VM_CALL_STACK_SIZE> MIP_VMCallStack;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_VirtualMachine {

//------------------------------
private:
//------------------------------

  int             MCodeSegment_size                       = 0;
  MIP_VMValue     MCodeSegment[MIP_VM_CODE_SEGMENT_SIZE]  = {0};
  MIP_VMValue     MDataSegment[MIP_VM_DATA_SEGMENT_SIZE]  = {0};
  MIP_VMValue     MRegisters[MIP_VM_NUM_REGISTERS]        = {0};
  uint32_t        MNumExternals                           = 0;
  MIP_VMExternal  MExternals[MIP_VM_MAX_EXTERNALS]        = {0};
  int             MNumOpcodes                             = 0;
  MIP_VMOpcode*   MOpcodes                                = nullptr;
  MIP_VMValue*    MIP                                     = nullptr;
  MIP_VMCallStack MCallStack;
  MIP_VMDataStack MDataStack;

//------------------------------
public:
//------------------------------

  MIP_VirtualMachine() {
  }

  //----------

  ~MIP_VirtualMachine() {
  }

//------------------------------
public:
//------------------------------

  MIP_VMValue*  getRegisters()                    { return MRegisters; }
  MIP_VMValue   getRegister(int i)                { return MRegisters[i]; }
  void          setRegister(int i, MIP_VMValue v) { MRegisters[i] = v; }

//------------------------------
public:
//------------------------------

  int registerExternal(MIP_VMExternal AExternal) {
    int index = MNumExternals;
    MExternals[MNumExternals++] = AExternal;
    return index;
  }

  //----------

  // http://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
  // You may not use this mechanism to jump to code in a different function.
  // If you do that, totally unpredictable things will happen.

  void translate(MIP_VMOpcode* AOpcodes, int ASize) {
    MOpcodes = AOpcodes;
    MNumOpcodes = ASize;
    execute(-1);
  }

  //----------

  void execute(int APosition=0) {
    if (APosition >= 0) {

//------------------------------
// execute
//------------------------------

      #define VM_NEXT       goto **MIP++
      #define VM_NEXT_cell  *MIP++
      #define VM_PUSH_call  MCallStack.push(MIP)
      #define VM_POP_call   MIP = MCallStack.pop()
      #define VM_PUSH(x)    MDataStack.push((MIP_VMValue)x)
      #define VM_POP        MDataStack.pop()
      #define VM_DROP       MDataStack.drop()
      #define VM_DUP        MDataStack.dup()
      #define VM_DUP2       MDataStack.dup2()
      #define VM_POS(x)     MCodeSegment[x]

      MIP = &MCodeSegment[APosition];
      VM_NEXT;

      //----- opcodes -----

      _op_noop:
        VM_NEXT;

      _op_exit:
        //MIP_VMValue r = (intptr_t)VM_POP;
        return;

      //----- stack -----

      _op_push: // psh const
        VM_PUSH(VM_NEXT_cell);
        VM_NEXT;

      _op_pushr: // push register
        VM_PUSH( MRegisters[VM_NEXT_cell] );
        VM_NEXT;

      _op_popr: // pop register
        {
          MIP_VMValue reg = (MIP_VMValue)VM_NEXT_cell;
          MRegisters[reg] = (MIP_VMValue)VM_POP;
        VM_NEXT;
        }

      _op_pushd: // push from data
        {
          MIP_VMValue *dp = (MIP_VMValue*)VM_NEXT_cell;
          VM_PUSH(*dp);
        }
        VM_NEXT;

      _op_popd: // pop to data
        {
          MIP_VMValue *dp = (MIP_VMValue*)VM_NEXT_cell;
          *dp = (MIP_VMValue)VM_POP;
        }
        VM_NEXT;

      _op_drop:
        VM_DROP;
        VM_NEXT;

      _op_dup:
        VM_DUP;
        VM_NEXT;

      _op_dup2:
        VM_DUP2;
        VM_NEXT;

      //----- branching -----

      _op_call: // call (gosub)
        {
          MIP_VMValue *ip = (MIP_VMValue*)VM_NEXT_cell;
          VM_PUSH_call;
          MIP = ip;
        }
        VM_NEXT;

      _op_ret: // return
        VM_POP_call;
        VM_NEXT;

      _op_jmp: // unconditional jump (goto)
        {
          MIP_VMValue *ip = (MIP_VMValue*)VM_NEXT_cell;
          MIP = ip;
        }
        VM_NEXT;

      _op_jz: // jump if zero
        {
          MIP_VMValue* ip = (MIP_VMValue*)VM_NEXT_cell;
          if ((MIP_VMValue)VM_POP == 0 ) MIP = ip;
        }
        VM_NEXT;

      _op_jnz: // jump if not zero
        {
          MIP_VMValue* ip = (MIP_VMValue*)VM_NEXT_cell;
          if ((MIP_VMValue)VM_POP != 0) MIP = ip;
        }
        VM_NEXT;

      //----- conditional -----

      _op_eq: // equal
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 == v1));
        }
        VM_NEXT;

      _op_gr: // greater
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 > v1));
        }
        VM_NEXT;

      _op_le: // less
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 < v1));
        }
        VM_NEXT;

      _op_ne: // not equal
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 != v1));
        }
        VM_NEXT;

      //_op_feq: {
      //}
      //
      //_op_fgr: {
      //}
      //
      //_op_fle: {
      //}
      //
      //_op_fne: {
      //}

      //----- math -----

      _op_add: // s += s2
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 + v1));
        }
        VM_NEXT;

      _op_sub: // s -= s2
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 - v1));
        }
        VM_NEXT;

      _op_mul: // s *= s2
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 * v1));
        }
        VM_NEXT;

      _op_div: // s /= s2
        {
          MIP_VMValue v1 = (MIP_VMValue)VM_POP;
          MIP_VMValue v2 = (MIP_VMValue)VM_POP;
          VM_PUSH((v2 / v1));
        }
        VM_NEXT;

      //_op_fadd: {
      //  }
      //
      //_op_fsub: {
      //  }
      //
      //_op_fmul: {
      //  }
      //
      //_op_fdiv: {
      //  }

      _op_inc: // s += 1
        {
          MIP_VMValue* top = (MIP_VMValue*)MDataStack.getTop();
          *top += 1;
        }
        VM_NEXT;

      _op_dec: // s -= 1
        {
          MIP_VMValue* top = (MIP_VMValue*)MDataStack.getTop();
          *top -= 1;
        }
        VM_NEXT;

      //----- io -----

      _op_print: // print (int)
        {
          MIP_VMValue v = (MIP_VMValue)VM_POP;
          //printf("%i\n",(int)v);
          MIP_DPrint("%i\n",(int)v);
        }
        VM_NEXT;

      //----- external -----

      // call external

      _op_ext: // call external
        {
          MIP_VMValue external = (MIP_VMValue)VM_POP;
          MExternals[external](this);
        }
        VM_NEXT;

      //------------------------------

      #undef VM_NEXT
      #undef VM_NEXT_cell
      #undef VM_PUSH_call
      #undef VM_POP_call
      #undef VM_PUSH
      #undef VM_POP
      #undef VM_DROP
      #undef VM_DUP
      #undef VM_DUP2
      #undef VM_POS

    } // execute

    // relocate
    else if (APosition == -1) {

//------------------------------
// translate (relocate?)
//------------------------------

      #define VM_WRITE(op) MCodeSegment[pos++] = (MIP_VMValue)op;

      void* op_adr[] = {
        &&_op_noop,
        &&_op_exit,
        &&_op_push,
        &&_op_drop,
        &&_op_dup,
        &&_op_dup2,
        &&_op_pushr,
        &&_op_popr,
        &&_op_pushd,
        &&_op_popd,
        &&_op_call,
        &&_op_ret,
        &&_op_jmp,
        &&_op_jz,
        &&_op_jnz,
        &&_op_eq,
        &&_op_gr,
        &&_op_le,
        &&_op_ne,
        &&_op_add,
        &&_op_sub,
        &&_op_mul,
        &&_op_div,
        &&_op_inc,
        &&_op_dec,
        &&_op_print,
        &&_op_ext
      };

      int pos = 0;
      for (int i=0; i<MNumOpcodes; i++) {
        int type = MOpcodes[i].type;
        MIP_VMValue value = MOpcodes[i].value;
        switch (type) {
          //case MIP_VM_OP_TYPE_NONE:
          //  break;
          case MIP_VM_OP_TYPE_INTERNAL:
            VM_WRITE( op_adr[ value ] );
            break;
          case MIP_VM_OP_TYPE_EXTERNAL:
          case MIP_VM_OP_TYPE_WORD:
          case MIP_VM_OP_TYPE_INT:
          case MIP_VM_OP_TYPE_FLOAT:
          case MIP_VM_OP_TYPE_PTR:
            VM_WRITE(value);
            break;
          case MIP_VM_OP_TYPE_DATAPTR:
            VM_WRITE( &MDataSegment[ value ] );
            break;
          case MIP_VM_OP_TYPE_CODEPTR:
            VM_WRITE( &MCodeSegment[ value ] );
            break;
          //default:
          //  //error
          //  break;
        } // switch type
      } // for size
      MCodeSegment_size = pos;

      #undef VM_WRITE

    } // relocate

  }

};

//----------------------------------------------------------------------
#endif
