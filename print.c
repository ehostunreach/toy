#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

static void
print_declaration(struct ast_declaration *);

static void
print_expression(struct ast_expression *);

static void
print_statement_list(struct ast_statement_list *);

static void
print_compound_statement(struct ast_compound_statement *);

static void
print_selection_statement(struct ast_selection_statement *);

static void
print_while_statement(struct ast_while_statement *);

static void
print_translation_unit(struct ast_translation_unit *);

static int ntabs = 0;

static void
print_tabs(void)
{
   int i;

   for (i = 0; i < ntabs; i++)
      printf("\t");
}

static void
print_declaration(struct ast_declaration *declaration)
{
   switch (declaration->type_specifier) {
      case TYPE_INT:
         printf("int");
         break;
      case TYPE_FLOAT:
         printf("float");
         break;
      default:
         fprintf(stderr, "Unknown type specifier: %d\n",
                         declaration->type_specifier);
         exit(EXIT_FAILURE);
   }

   printf(" %s;", declaration->identifier);
}

static void
print_expression(struct ast_expression *expression)
{
   switch (expression->operator) {
      case AST_ADD:
         print_expression(expression->subexpr[0]);
         printf(" + ");
         print_expression(expression->subexpr[1]);
         break;
      case AST_SUB:
         print_expression(expression->subexpr[0]);
         printf(" - ");
         print_expression(expression->subexpr[1]);
         break;
      case AST_MUL:
         print_expression(expression->subexpr[0]);
         printf(" * ");
         print_expression(expression->subexpr[1]);
         break;
      case AST_DIV:
         print_expression(expression->subexpr[0]);
         printf(" / ");
         print_expression(expression->subexpr[1]);
         break;

      case AST_GT:
         print_expression(expression->subexpr[0]);
         printf(" > ");
         print_expression(expression->subexpr[1]);
         break;
      case AST_LT:
         print_expression(expression->subexpr[0]);
         printf(" < ");
         print_expression(expression->subexpr[1]);
         break;
      case AST_EQ:
         print_expression(expression->subexpr[0]);
         printf(" == ");
         print_expression(expression->subexpr[1]);
         break;
      case AST_NE:
         print_expression(expression->subexpr[0]);
         printf(" != ");
         print_expression(expression->subexpr[1]);
         break;

      case AST_ASSIGN:
         printf("%s = ", expression->primary_expr.identifier);
         print_expression(expression->subexpr[0]);
         break;

      case AST_INT_CONSTANT:
         printf("%d", expression->primary_expr.int_constant);
         break;
      case AST_FLOAT_CONSTANT:
         printf("%f", expression->primary_expr.float_constant);
         break;
      case AST_IDENTIFIER:
         printf("%s", expression->primary_expr.identifier);
         break;

      default:
         fprintf(stderr, "Unknown expression operator: %d\n",
                         expression->operator);
         exit(EXIT_FAILURE);
   }
}

static void
print_statement_list(struct ast_statement_list *statement_list)
{
   int i;

   for (i = 0; i < statement_list->number_of_statements; i++) {
      print_tabs();
      print_node(statement_list->statement[i]);
      printf("\n");
   }
}

static void
print_compound_statement(struct ast_compound_statement *compound_statement)
{
   print_tabs();
   printf("{\n");
   ntabs++;

   print_statement_list(compound_statement->statement_list);

   ntabs--;
   print_tabs();
   printf("}\n");
}

static void
print_selection_statement(struct ast_selection_statement *selection_statement)
{
   printf("\n");
   print_tabs();
   printf("if (");
   print_expression(selection_statement->condition);
   printf(")\n");

   print_compound_statement(selection_statement->then_body);

   if (selection_statement->else_body) {
      print_tabs();
      printf("else\n");
      print_compound_statement(selection_statement->else_body);
      printf("\n");
   }
}

static void
print_while_statement(struct ast_while_statement *while_statement)
{
   printf("\n");
   print_tabs();
   printf("while (");
   print_expression(while_statement->condition);
   printf(")\n");

   print_compound_statement(while_statement->body);
   printf("\n");
}

static void
print_translation_unit(struct ast_translation_unit *translation_unit)
{
   print_statement_list(translation_unit->statement_list);
}

void
print_node(struct ast_node *node)
{
   int tag = node->tag;
   void *nodep = node;

   switch (tag) {
      case AST_NODE:
         fprintf(stderr, "Request to print generic AST node.\n");
         exit(EXIT_FAILURE);

      case AST_DECLARATION:
         print_declaration(nodep);
         break;
      case AST_EXPRESSION:
         print_expression(nodep);
         printf(";");
         break;
      case AST_STATEMENT_LIST:
         print_statement_list(nodep);
         break;
      case AST_COMPOUND_STATEMENT:
         print_compound_statement(nodep);
         break;
      case AST_SELECTION_STATEMENT:
         print_selection_statement(nodep);
         break;
      case AST_WHILE_STATEMENT:
         print_while_statement(nodep);
         break;
      case AST_TRANSLATION_UNIT:
         print_translation_unit(nodep);
         break;

      default:
         fprintf(stderr, "Request to print unknown node type: %d\n", tag);
   }
}
