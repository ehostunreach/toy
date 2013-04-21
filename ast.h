#ifndef AST_H
#define AST_H

struct ast_node;
struct ast_declaration;
struct ast_expression;
struct ast_statement_list;
struct ast_compound_statement;
struct ast_selection_statement;
struct ast_while_statement;
struct ast_translation_unit;

#define AST_NODE                    1
#define AST_DECLARATION             2
#define AST_EXPRESSION              3
#define AST_STATEMENT_LIST          4
#define AST_COMPOUND_STATEMENT      5
#define AST_SELECTION_STATEMENT     6
#define AST_WHILE_STATEMENT         7
#define AST_TRANSLATION_UNIT        8

struct ast_node {
   int tag;
};

struct ast_declaration {
   int tag;

#define TYPE_INT     1
#define TYPE_FLOAT   2

   int type_specifier;
   const char *identifier;
};

struct ast_expression {
   int tag;

#define AST_ADD            1     /* '+'   */
#define AST_SUB            2     /* '-'   */
#define AST_MUL            3     /* '*'   */
#define AST_DIV            4     /* '/'   */

#define AST_GT             5     /* '>'   */
#define AST_LT             6     /* '<'   */
#define AST_EQ             7     /* '=='  */
#define AST_NE             8     /* '!='  */
#define AST_LE             9     /* '<='  */
#define AST_GE             10    /* '>='  */

#define AST_ASSIGN         11    /* '='   */

#define AST_INT_CONSTANT   12
#define AST_FLOAT_CONSTANT 13
#define AST_IDENTIFIER     14

   int operator;
   struct ast_expression *subexpr[2];
   union {
      int int_constant;
      float float_constant;
      const char *identifier;
   } primary_expr;
};

struct ast_statement_list {
   int tag;

   struct ast_node **statement;
   int number_of_statements;
};

struct ast_compound_statement {
   int tag;

   struct ast_statement_list *statement_list;
};

struct ast_selection_statement {
   int tag;

   /* if (condition) { then_body } else { else_body } */
   struct ast_expression *condition;
   struct ast_compound_statement *then_body;
   struct ast_compound_statement *else_body;
};

struct ast_while_statement {
   int tag;

   /* while (condition) { body } */
   struct ast_expression *condition;
   struct ast_compound_statement *body;
};

struct ast_translation_unit {
   int tag;

   struct ast_statement_list *statement_list;
};

struct ast_declaration *
create_declaration(int type_specifier, const char *identifier);

struct ast_expression *
create_expression(int operation, struct ast_expression *lhs,
                                 struct ast_expression *rhs);

struct ast_statement_list *
create_statement_list(struct ast_node *statement);

struct ast_statement_list *
statement_list_add_statement(struct ast_statement_list *statement_list,
                             struct ast_node *statement);

struct ast_compound_statement *
create_compound_statement(struct ast_statement_list *statement_list);

struct ast_selection_statement *
create_selection_statement(struct ast_expression *condition,
                           struct ast_compound_statement *then_body,
                           struct ast_compound_statement *else_body);

struct ast_while_statement *
create_while_statement(struct ast_expression *condition,
                       struct ast_compound_statement *body);

struct ast_translation_unit *
create_translation_unit(struct ast_statement_list *statement_list);

void
print_node(struct ast_node *node);

#endif /* AST_H */
