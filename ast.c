#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

static void *
alloc_node(int tag)
{
   struct ast_node *node;
   size_t size = 0;

   switch (tag) {
      case AST_NODE:
         size = sizeof(struct ast_node);
         break;
      case AST_DECLARATION:
         size = sizeof(struct ast_declaration);
         break;
      case AST_EXPRESSION:
         size = sizeof(struct ast_expression);
         break;
      case AST_STATEMENT_LIST:
         size = sizeof(struct ast_statement_list);
         break;
      case AST_COMPOUND_STATEMENT:
         size = sizeof(struct ast_compound_statement);
         break;
      case AST_SELECTION_STATEMENT:
         size = sizeof(struct ast_selection_statement);
         break;
      case AST_WHILE_STATEMENT:
         size = sizeof(struct ast_while_statement);
         break;
      case AST_TRANSLATION_UNIT:
         size = sizeof(struct ast_translation_unit);
         break;
      default:
         fprintf(stderr, "Unknown node type: %d\n", tag);
         exit(EXIT_FAILURE);
   }

   node = calloc(1, size);
   if (node == NULL) {
      fprintf(stderr, "Memory allocation request failed.!\n");
      exit(EXIT_FAILURE);
   }

   node->tag = tag;
   return node;
}

struct ast_expression *
create_expression(int operator, struct ast_expression *lhs,
                                 struct ast_expression *rhs)
{
   struct ast_expression *expression;

   expression = alloc_node(AST_EXPRESSION);
   expression->operator = operator;
   expression->subexpr[0] = lhs;
   expression->subexpr[1] = rhs;

   return expression;
}

struct ast_declaration *
create_declaration(int type_specifier, const char *identifier)
{
   struct ast_declaration *declaration;

   declaration = alloc_node(AST_DECLARATION);
   declaration->type_specifier = type_specifier;
   declaration->identifier = identifier;

   return declaration;
}

struct ast_statement_list *
create_statement_list(struct ast_node *statement)
{
   struct ast_statement_list *statement_list;

   statement_list = alloc_node(AST_STATEMENT_LIST);

   statement_list->statement = calloc(1, sizeof(struct ast_node *));
   if (statement_list->statement == NULL) {
      fprintf(stderr, "Memory allocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   statement_list->statement[0] = statement;
   statement_list->number_of_statements = 1;

   return statement_list;
}

struct ast_statement_list *
statement_list_add_statement(struct ast_statement_list *statement_list,
                             struct ast_node *statement)
{
   int nstmts;
   size_t size;

   nstmts = statement_list->number_of_statements + 1;
   size = nstmts * sizeof(struct ast_statement *);

   statement_list->statement = realloc(statement_list->statement, size);
   if (statement_list->statement == NULL) {
      fprintf(stderr, "Memory reallocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   statement_list->statement[nstmts - 1] = statement;
   statement_list->number_of_statements++;

   return statement_list;
}

struct ast_compound_statement *
create_compound_statement(struct ast_statement_list *statement_list)
{
   struct ast_compound_statement *compound_statement;

   compound_statement = alloc_node(AST_COMPOUND_STATEMENT);
   compound_statement->statement_list = statement_list;

   return compound_statement;
}

struct ast_selection_statement *
create_selection_statement(struct ast_expression *condition,
                           struct ast_compound_statement *then_body,
                           struct ast_compound_statement *else_body)
{
   struct ast_selection_statement *selection_statement;

   selection_statement = alloc_node(AST_SELECTION_STATEMENT);

   selection_statement->condition = condition;
   selection_statement->then_body = then_body;
   selection_statement->else_body = else_body;

   return selection_statement;
}

struct ast_while_statement *
create_while_statement(struct ast_expression *condition,
                       struct ast_compound_statement *body)
{
   struct ast_while_statement *while_statement;

   while_statement = alloc_node(AST_WHILE_STATEMENT);

   while_statement->condition = condition;
   while_statement->body = body;

   return while_statement;
}

struct ast_translation_unit *
create_translation_unit(struct ast_statement_list *statement_list)
{
   struct ast_translation_unit *translation_unit;

   translation_unit = alloc_node(AST_TRANSLATION_UNIT);
   translation_unit->statement_list = statement_list;

   return translation_unit;
}
