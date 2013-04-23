#ifndef VM_VALUE_H
#define VM_VALUE_H

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>

#include "vm_state.h"

struct vm_value {
   LLVMTypeRef llvm_type;
   LLVMValueRef llvm_value;

   int type_specifier;
   const char *identifier;
};

struct vm_value *vm_value_new(int type_specifier, const char *identifier);
struct vm_value *vm_value_new_from_int_constant(int int_constant);
struct vm_value *vm_value_new_from_float_constant(float float_constant);

LLVMValueRef vm_value_alloca(struct vm_state *vm, struct vm_value *vmval);

struct vm_value *vm_value_build_math_op(struct vm_state *vm,
                                        int operation, struct vm_value *lhs,
                                                       struct vm_value *rhs);

struct vm_value *vm_value_build_cmp_op(struct vm_state *vm,
                                       int operation, struct vm_value *lhs,
                                                      struct vm_value *rhs);

#endif /* VM_VALUE_H */
