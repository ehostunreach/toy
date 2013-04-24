#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"
#include "vm_state.h"

struct vm_state *
vm_state_create(const char *module_name)
{
   struct vm_state *vm;

   LLVMTypeRef function_type;
   LLVMValueRef function_value;
   LLVMBasicBlockRef entry_block;

   vm = calloc(1, sizeof(struct vm_state));
   if (vm == NULL) {
      fprintf(stderr, "Memory allocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   vm->module = LLVMModuleCreateWithName(module_name);
   vm->builder = LLVMCreateBuilder();

   function_type = LLVMFunctionType(LLVMVoidType(), NULL, 0, 0);
   function_value = LLVMAddFunction(vm->module, "main", function_type);

   entry_block = LLVMAppendBasicBlock(function_value, "entry");
   LLVMPositionBuilderAtEnd(vm->builder, entry_block);

   vm->symtab = symbol_table_create();
   return vm;
}

void
vm_state_destroy(struct vm_state *vm)
{
   LLVMBasicBlockRef current_block, return_block;
   char *error;

   current_block = LLVMGetInsertBlock(vm->builder);
   return_block = LLVMInsertBasicBlock(current_block, "ret");

   LLVMPositionBuilderAtEnd(vm->builder, current_block);
   LLVMBuildBr(vm->builder, return_block);

   LLVMPositionBuilderAtEnd(vm->builder, return_block);
   LLVMBuildRetVoid(vm->builder);

   LLVMMoveBasicBlockAfter(return_block, current_block);

   LLVMDumpModule(vm->module);

   error = NULL;
   LLVMVerifyModule(vm->module, LLVMAbortProcessAction, &error);
   LLVMDisposeMessage(error);

   LLVMDisposeBuilder(vm->builder);
   LLVMDisposeModule(vm->module);
   symbol_table_destroy(vm->symtab);
}

void
vm_state_put_value(struct vm_state *vm, struct vm_value *vmval)
{
   symbol_table_put_value(vm->symtab, vmval);
}

struct vm_value *
vm_state_get_value(struct vm_state *vm, const char *identifier)
{
   return symbol_table_get_value(vm->symtab, identifier);
}
