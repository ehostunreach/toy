#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "vm_state.h"
#include "vm_value.h"

static void
drive_declaration(struct vm_state *, struct ast_declaration *);

static struct vm_value *
drive_expression(struct vm_state *, struct ast_expression *);

static void
drive_statement_list(struct vm_state *, struct ast_statement_list *);

static void
drive_compound_statement(struct vm_state *, struct ast_compound_statement *);

static void
drive_selection_statement(struct vm_state *, struct ast_selection_statement *);

static void
drive_while_statement(struct vm_state *, struct ast_while_statement *);

static void
drive_translation_unit(struct vm_state *, struct ast_translation_unit *);


static void
drive_node(struct vm_state *vm, struct ast_node *node)
{
   switch (node->tag) {
      case AST_NODE:
         fprintf(stderr, "Request to drive generic ast node\n");
         exit(EXIT_FAILURE);

      case AST_DECLARATION:
         drive_declaration(vm, (struct ast_declaration *) node);
         return;
      case AST_EXPRESSION:
         drive_expression(vm, (struct ast_expression *) node);
         return;
      case AST_STATEMENT_LIST:
         drive_statement_list(vm, (struct ast_statement_list *) node);
         return;
      case AST_COMPOUND_STATEMENT:
         drive_compound_statement(vm, (struct ast_compound_statement *) node);
         return;
      case AST_SELECTION_STATEMENT:
         drive_selection_statement(vm, (struct ast_selection_statement *) node);
         return;
      case AST_WHILE_STATEMENT:
         drive_while_statement(vm, (struct ast_while_statement *) node);
         return;
      case AST_TRANSLATION_UNIT:
         drive_translation_unit(vm, (struct ast_translation_unit *) node);
         return;

      default:
         fprintf(stderr, "Request to drive unknonwn node: %d\n", node->tag);
         exit(EXIT_FAILURE);
   }
}

static void
drive_declaration(struct vm_state *vm,
                  struct ast_declaration *declaration)
{
   struct vm_value *vmval;

   vmval = vm_value_new(declaration->type_specifier,
                        declaration->identifier);

   vmval->llvm_value = LLVMBuildAlloca(vm->builder,
                                       vmval->llvm_type,
                                       vmval->identifier);
   vm_state_put_value(vm, vmval);
}

static struct vm_value *
drive_expression(struct vm_state *vm,
                 struct ast_expression *expression)
{
   switch (expression->operator) {
      case AST_ADD: case AST_SUB:
      case AST_MUL: case AST_DIV: {
         struct vm_value *lhs, *rhs;

         lhs = drive_expression(vm, expression->subexpr[0]);
         rhs = drive_expression(vm, expression->subexpr[1]);

         return vm_value_build_math_op(vm, expression->operator, lhs, rhs);
      }
      case AST_GT: case AST_LT:
      case AST_EQ: case AST_NE:
      case AST_LE: case AST_GE: {
         struct vm_value *lhs, *rhs;

         lhs = drive_expression(vm, expression->subexpr[0]);
         rhs = drive_expression(vm, expression->subexpr[1]);

         return vm_value_build_cmp_op(vm, expression->operator, lhs, rhs);
      }
      case AST_ASSIGN: {
         struct vm_value *ret, *lhs, *rhs;

         lhs = vm_state_get_value(vm, expression->primary_expr.identifier);
         rhs = drive_expression(vm, expression->subexpr[0]);

         ret = vm_value_dup(lhs);
         LLVMBuildStore(vm->builder, rhs->llvm_value, ret->llvm_value);

         return ret;
      }
      case AST_IDENTIFIER: {
         struct vm_value *vmval, *ret;

         vmval = vm_state_get_value(vm, expression->primary_expr.identifier);
         ret = vm_value_dup(vmval);
         ret->llvm_value = LLVMBuildLoad(vm->builder, vmval->llvm_value, "");
         return ret;
      }
      case AST_INT_CONSTANT:
         return vm_value_new_from_int_constant(
               expression->primary_expr.int_constant);
      case AST_FLOAT_CONSTANT:
         return vm_value_new_from_float_constant(
               expression->primary_expr.float_constant);
      default:
         fprintf(stderr,
                 "Request to drive unknown expression operator: %d\n",
                 expression->operator);
         exit(EXIT_FAILURE);
   }

   return NULL;
}

static void
drive_statement_list(struct vm_state *vm,
                     struct ast_statement_list *statement_list)
{
   int i;

   for (i = 0; i < statement_list->number_of_statements; i++)
      drive_node(vm, statement_list->statement[i]);
}

static void
drive_selection_statement(struct vm_state *vm,
                          struct ast_selection_statement *selection_statement)
{
   return;
}

static void
drive_while_statement(struct vm_state *vm,
                      struct ast_while_statement *while_statement)
{
   return;
}

static void
drive_compound_statement(struct vm_state *vm,
                         struct ast_compound_statement *compound_statement)
{
   drive_statement_list(vm, compound_statement->statement_list);
}

static void
drive_translation_unit(struct vm_state *vm,
                       struct ast_translation_unit *translation_unit)
{
   drive_statement_list(vm, translation_unit->statement_list);
}

void
ast_to_llvm(struct ast_translation_unit *translation_unit)
{
   struct vm_state *vm;

   vm = vm_state_create("Toy");
   drive_translation_unit(vm, translation_unit);
   vm_state_destroy(vm);
}
