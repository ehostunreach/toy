#ifndef VM_STATE_H
#define VM_STATE_H

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>

struct vm_value;
struct symbol_table;

struct vm_state {
   LLVMModuleRef module;
   LLVMBuilderRef builder;
   struct symbol_table *symtab;
};

struct vm_state *vm_state_create(const char *module_name);
void vm_state_destroy(struct vm_state *vm);

void
vm_state_put_value(struct vm_state *vm, struct vm_value *vmval);

struct vm_value *
vm_state_get_value(struct vm_state *vm, const char *identifier);

#endif /* VM_VALUE_H */
