#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "vm_state.h"
#include "vm_value.h"

struct vm_state *
ast_to_llvm(struct ast_translation_unit *translation_unit)
{
   struct vm_state *vm;

   vm = vm_state_create("Toy");
   vm_state_destroy(vm);
   return vm;
}

