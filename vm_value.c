#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "vm_state.h"
#include "vm_value.h"

static LLVMTypeRef
get_llvm_type(int type_specifier)
{
   switch (type_specifier) {
      case TYPE_INT:
         return LLVMInt32Type();
      case TYPE_FLOAT:
         return LLVMFloatType();
      default:
         fprintf(stderr, "Unknown type specifier: %d\n", type_specifier);
         exit(EXIT_FAILURE);
   }

   return NULL;
}

struct vm_value *
vm_value_new(int type_specifier, const char *identifier)
{
   struct vm_value *vmval;

   vmval = calloc(1, sizeof(struct vm_value));
   if (!vmval) {
      fprintf(stderr, "Memory allocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   vmval->type_specifier = type_specifier;
   vmval->identifier = identifier;
   vmval->llvm_type = get_llvm_type(type_specifier);

   return vmval;
}

struct vm_value *
vm_value_new_from_int_constant(int int_constant)
{
   struct vm_value *vmval;

   vmval = calloc(1, sizeof(struct vm_value));
   if (!vmval) {
      fprintf(stderr, "Memory allocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   vmval->type_specifier = TYPE_INT;
   vmval->llvm_type = get_llvm_type(TYPE_INT);
   vmval->llvm_value = LLVMConstInt(vmval->llvm_type, int_constant, 0);

   return vmval;
}

struct vm_value *
vm_value_new_from_float_constant(float float_constant)
{
   struct vm_value *vmval;

   vmval = calloc(1, sizeof(struct vm_value));
   if (!vmval) {
      fprintf(stderr, "Memory allocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   vmval->type_specifier = TYPE_FLOAT;
   vmval->llvm_type = get_llvm_type(TYPE_FLOAT);
   vmval->llvm_value = LLVMConstInt(vmval->llvm_type, float_constant, 0);

   return vmval;
}

LLVMValueRef
vm_value_alloca(struct vm_state *vm, struct vm_value *vmval)
{
   const char *identifier = vmval->identifier;

   if (identifier == NULL)
      identifier = "";

   vmval->llvm_value = LLVMBuildAlloca(vm->builder, vmval->llvm_type,
                                                    identifier);
   return vmval->llvm_value;
}

struct vm_value *
vm_value_build_math_op(struct vm_state *vm, int operation,
                                            struct vm_value *lhs,
                                            struct vm_value *rhs)
{
   struct vm_value *res;
   LLVMOpcode opcode;

   res = vm_value_new(lhs->type_specifier, "");

   switch (operation) {
      case AST_ADD:
         opcode = (res->type_specifier == TYPE_INT) ? LLVMAdd : LLVMFAdd;
         break;
      case AST_SUB:
         opcode = (res->type_specifier == TYPE_INT) ? LLVMSub : LLVMFSub;
         break;
      case AST_MUL:
         opcode = (res->type_specifier == TYPE_INT) ? LLVMMul: LLVMFMul;
         break;
      case AST_DIV:
         opcode = (res->type_specifier == TYPE_INT) ? LLVMMul: LLVMFMul;
         break;
      default:
         fprintf(stderr, "Unknown math operation: %d\n", operation);
         exit(EXIT_FAILURE);
   }

   res->llvm_value = LLVMBuildBinOp(vm->builder, opcode, lhs->llvm_value,
                                                         rhs->llvm_value, "");
   return res;
}

struct vm_value *
vm_value_build_cmp_op(struct vm_state *vm, int operation,
                                           struct vm_value *lhs,
                                           struct vm_value *rhs)
{
   struct vm_value *res;
   LLVMOpcode opcode;
   LLVMIntPredicate int_predicate;
   LLVMRealPredicate float_predicate;

   res = vm_value_new(lhs->type_specifier, "");

   opcode = (res->type_specifier == TYPE_INT) ? LLVMICmp : LLVMFCmp;

   switch (operation) {
      case AST_GT:
         if (res->type_specifier == TYPE_INT)
            int_predicate = LLVMIntSGT;
         else
            float_predicate = LLVMRealOGT;
         break;
      case AST_LT:
         if (res->type_specifier == TYPE_INT)
            int_predicate = LLVMIntSLT;
         else
            float_predicate = LLVMRealOLT;
         break;
      case AST_EQ:
         if (res->type_specifier == TYPE_INT)
            int_predicate = LLVMIntEQ;
         else
            float_predicate = LLVMRealOEQ;
         break;
      case AST_NE:
         if (res->type_specifier == TYPE_INT)
            int_predicate = LLVMIntNE;
         else
            float_predicate = LLVMRealONE;
         break;
      case AST_LE:
         if (res->type_specifier == TYPE_INT)
            int_predicate = LLVMIntSLE;
         else
            float_predicate = LLVMRealOLE;
         break;
      case AST_GE:
         if (res->type_specifier == TYPE_INT)
            int_predicate = LLVMIntSGE;
         else
            float_predicate = LLVMRealOGE;
         break;
      default:
         fprintf(stderr, "Unknown comparison operation: %d\n", operation);
         exit(EXIT_FAILURE);
   }

   if (opcode == LLVMICmp) {
      res->llvm_value = LLVMBuildICmp(vm->builder, int_predicate,
                                                   lhs->llvm_value,
                                                   rhs->llvm_value, "");
   } else {
      res->llvm_value = LLVMBuildFCmp(vm->builder, float_predicate,
                                                   lhs->llvm_value,
                                                   rhs->llvm_value, "");
   }

   return res;
}
