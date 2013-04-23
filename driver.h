#ifndef DRIVER_H
#define DRIVER_H

#include "vm_state.h"

struct vm_state *ast_to_llvm(struct ast_translation_unit *translation_unit);

#endif /* DRIVER_H */
