#ifndef mip_gcc_vm_included
#define mip_gcc_vm_included
//----------------------------------------------------------------------

/*
  uses calculated gotos ('goto **ip++')..


  bk  : byte mip
  kbc : mip bytce code

  todo:
  - call external functions
*/

//----------

#include <stdio.h>  // printf
#include "base/mip_stack.h"

//----------

#define MIP_VM_CALL_STACK_SIZE   1024
#define MIP_VM_DATA_STACK_SIZE   1024
#define MIP_VM_CODE_SEGMENT_SIZE 65536
#define MIP_VM_DATA_SEGMENT_SIZE 65536
#define MIP_VM_MAX_EXTERNALS     256
#define MIP_VM_NUM_REGISTERS     256

//----------------------------------------------------------------------

typedef intptr_t MIP_Cell;

typedef MIP_Stack<MIP_Cell, MIP_VM_DATA_STACK_SIZE> MIP_DataStack;
typedef MIP_Stack<MIP_Cell*,MIP_VM_CALL_STACK_SIZE> MIP_CallStack;

struct MIP_Opcode {
  int       type;
  MIP_Cell value;
};

// opcode types
#define MIP_VM_OP_TYPE_NONE      0
#define MIP_VM_OP_TYPE_INTERNAL  1
#define MIP_VM_OP_TYPE_EXTERNAL  2
#define MIP_VM_OP_TYPE_WORD      3
#define MIP_VM_OP_TYPE_INT       4
#define MIP_VM_OP_TYPE_FLOAT     5
#define MIP_VM_OP_TYPE_PTR       6
#define MIP_VM_OP_TYPE_DATAPTR   7
#define MIP_VM_OP_TYPE_CODEPTR   8

class MIP_VirtualMachine;
typedef void (*MIP_External)(MIP_VirtualMachine* vm);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class MIP_VirtualMachine {

//------------------------------
private:
//------------------------------

  int             MCodeSegment_size                       = 0;
  MIP_Cell       MCodeSegment[MIP_VM_CODE_SEGMENT_SIZE] = {0};
  MIP_Cell       MDataSegment[MIP_VM_DATA_SEGMENT_SIZE] = {0};
  MIP_Cell       MRegisters[MIP_VM_NUM_REGISTERS]       = {0};
  uint32_t        MNumExternals                           = 0;
  MIP_External   MExternals[MIP_VM_MAX_EXTERNALS]       = {0};
  int             MNumOpcodes                             = 0;
  MIP_Opcode*    MOpcodes                                = nullptr;
  MIP_Cell*      MIP                                     = nullptr;
  MIP_CallStack  MCallStack;
  MIP_DataStack  MDataStack;

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

MIP_Cell*  getRegisters()                  { return MRegisters; }
MIP_Cell   getRegister(int i)              { return MRegisters[i]; }
void        setRegister(int i, MIP_Cell v) { MRegisters[i] = v; }

//------------------------------
public:
//------------------------------

  int registerExternal(MIP_External AExternal) {
    int index = MNumExternals;
    MExternals[MNumExternals++] = AExternal;
    return index;
  }

  //----------

  // http://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
  // You may not use this mechanism to jump to code in a different function.
  // If you do that, totally unpredictable things will happen.

  void translate(MIP_Opcode* AOpcodes, int ASize) {
    MOpcodes = AOpcodes;
    MNumOpcodes = ASize;
    execute(-1);
  }

  //----------

  void execute(int APosition=0) {

    if (APosition >= 0) {

      //dtrace( "MCodeSegment = " << (int)&MCodeSegment[0] );
      //dtrace( "MDataSegment = " << (int)&MDataSegment[0] );

      //------------------------------
      // execute
      //------------------------------

      #define VM_NEXT       goto **MIP++
      #define VM_NEXT_cell  *MIP++
      #define VM_PUSH_call  MCallStack.push(MIP)
      #define VM_POP_call   MIP = MCallStack.pop()
      #define VM_PUSH(x)    MDataStack.push((MIP_Cell)x)
      #define VM_POP        MDataStack.pop()
      #define VM_DROP       MDataStack.drop()
      #define VM_DUP        MDataStack.dup()
      #define VM_DUP2       MDataStack.dup2()
      #define VM_POS(x)     MCodeSegment[x]

      //----------

      MIP = &MCodeSegment[APosition];
      VM_NEXT;

      // script internals

      _op_noop:
        VM_NEXT;

      _op_exit:
        //intptr_t r = (intptr_t)VM_POP;
        return;

      _op_push:
        VM_PUSH(VM_NEXT_cell);
        VM_NEXT;

      _op_pushr:
        VM_PUSH( MRegisters[VM_NEXT_cell] );
        VM_NEXT;

      _op_popr:
        {
          MIP_Cell reg = (MIP_Cell)VM_NEXT_cell;
          MRegisters[reg] = (MIP_Cell)VM_POP;
        VM_NEXT;
        }

      _op_pushd: // push from data
        {
          MIP_Cell *dp = (MIP_Cell*)VM_NEXT_cell;
          VM_PUSH(*dp);
        }
        VM_NEXT;

      _op_popd: // pop to data
        {
          MIP_Cell *dp = (MIP_Cell*)VM_NEXT_cell;
          *dp = (MIP_Cell)VM_POP;
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

      //----------

      _op_call:
        {
          MIP_Cell *ip = (MIP_Cell*)VM_NEXT_cell;
          VM_PUSH_call;
          MIP = ip;
        }
        VM_NEXT;

      _op_ret:
        VM_POP_call;
        VM_NEXT;

      _op_jmp:
        {
          MIP_Cell *ip = (MIP_Cell*)VM_NEXT_cell;
          MIP = ip;
        }
        VM_NEXT;

      _op_jz:
        {
          MIP_Cell* ip = (MIP_Cell*)VM_NEXT_cell;
          if ( (intptr_t)VM_POP==0 ) MIP = ip;
        }
        VM_NEXT;

      _op_jnz:
        {
          MIP_Cell* ip = (MIP_Cell*)VM_NEXT_cell;
          if ( (intptr_t)VM_POP!=0 ) MIP = ip;
        }
        VM_NEXT;

      //----------

      _op_eq:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2==v1));
        }
        VM_NEXT;

      _op_gr:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2>v1));
        }
        VM_NEXT;

      _op_le:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2<v1));
        }
        VM_NEXT;

      _op_ne:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2!=v1));
        }
        VM_NEXT;

      //----------

      _op_add:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2+v1));
        }
        VM_NEXT;

      _op_sub:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2-v1));
        }
        VM_NEXT;

      _op_mul:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2*v1));
        }
        VM_NEXT;

      _op_div:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2/v1));
        }
        VM_NEXT;

      _op_inc:
        {
          intptr_t* top = (intptr_t*)MDataStack.getTop();
          *top+=1;
        }
        VM_NEXT;

      _op_dec:
        {
          intptr_t* top = (intptr_t*)MDataStack.getTop();
          *top-=1;
        }
        VM_NEXT;

      //----------

      _op_printi:
        {
          intptr_t v = (intptr_t)VM_POP;
          printf("%i\n",(int)v);
        }
        VM_NEXT;

      _op_ext:
        {
          intptr_t external = (intptr_t)VM_POP;
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

    } // 0+ (exec)

    //else
    //if (APosition==-2) // jit
    //{
    //} // -2 (jit)

    else if (APosition == -1) { // relocate

      //------------------------------
      // translate
      //------------------------------

      // MOpcodes -> MCodeSegment ++ ...

      #define VM_WRITE(op) MCodeSegment[pos++] = (MIP_Cell)op;

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
        &&_op_printi,
        &&_op_ext
      };

      int pos = 0;
      for (int i=0; i<MNumOpcodes; i++) {
        int type = MOpcodes[i].type;
        MIP_Cell value = MOpcodes[i].value;
        switch (type) {
          //case MIP_VM_OP_TYPE_NONE:
          //  break;
          case MIP_VM_OP_TYPE_INTERNAL:
            VM_WRITE( op_adr[ (intptr_t)value ] );
            break;
          case MIP_VM_OP_TYPE_EXTERNAL:
          case MIP_VM_OP_TYPE_WORD:
          case MIP_VM_OP_TYPE_INT:
          case MIP_VM_OP_TYPE_FLOAT:
          case MIP_VM_OP_TYPE_PTR:
            VM_WRITE(value);
            break;
          case MIP_VM_OP_TYPE_DATAPTR:
            VM_WRITE( &MDataSegment[ (intptr_t)value ] );
            break;
          case MIP_VM_OP_TYPE_CODEPTR:
            VM_WRITE( &MCodeSegment[ (intptr_t)value ] );
            break;
        } // switch type
      } // for size
      MCodeSegment_size = pos;

      #undef VM_WRITE

    } // -1 (relocate)

  }

};

//----------------------------------------------------------------------
#endif
