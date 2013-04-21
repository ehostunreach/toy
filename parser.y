%{

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(char *s);

%}

%union {
   int int_const;
   float float_const;

   int type_specifier;
   char *identifier;

   struct ast_node *node;
   struct ast_declaration *declaration;
   struct ast_expression *expression;
   struct ast_statement_list *statement_list;
   struct ast_selection_statement *selection_statement;
   struct ast_while_statement *while_statement;
   struct ast_compound_statement *compound_statement;
   struct ast_translation_unit *translation_unit;
}

 /* math operators */
%left    OP_ADD OP_SUB
%left    OP_MUL OP_DIV

 /* comparison operators */
%left    OP_GT OP_LT OP_EQ OP_NE OP_LE OP_GE

 /* assignment operator */
%token   OP_ASSIGN

 /* statement terminator */
%token   SEMICOLON

 /* grouping symbols */
%token   LBRACE
%token   RBRACE
%token   LPAREN
%token   RPAREN

 /* keywords */
%token   KW_INT
%token   KW_FLOAT
%token   KW_IF
%token   KW_ELSE
%token   KW_WHILE

 /* integer and floating constants */
%token <int_const>      INT_CONSTANT
%token <float_const>    FLOAT_CONSTANT

 /* identifier's name */
%token <identifier>     IDENTIFIER

 /* rule types */
%type <type_specifier>       type_specifier
%type <expression>           primary_expression expression assignment
%type <declaration>          declaration
%type <node>                 statement
%type <statement_list>       statement_list
%type <compound_statement>   compound_statement
%type <selection_statement>  selection_statement selection_rest_statement
%type <while_statement>      while_statement
%type <translation_unit>     translation_unit

%%

 /* rule that matches a translation unit (aka. a source file) */
translation_unit: statement_list {
   $$ = create_translation_unit($1);
   print_node((struct ast_node *) $$);
}
;

 /* rules for: 1) statements
               2) list of statements
               3) compound statements */
compound_statement: LBRACE statement_list RBRACE {
   $$ = create_compound_statement($2);
}
;

statement_list: statement {
   $$ = create_statement_list($1);
}
| statement_list statement {
   $$ = statement_list_add_statement($1, $2);
}
;

statement: declaration SEMICOLON { $$ = (struct ast_node *) $1; }
         | expression SEMICOLON  { $$ = (struct ast_node *) $1; }
         | assignment SEMICOLON  { $$ = (struct ast_node *) $1; }
         | selection_statement   { $$ = (struct ast_node *) $1; }
         | while_statement       { $$ = (struct ast_node *) $1; }
         ;

 /* while rule */
while_statement: KW_WHILE LPAREN expression RPAREN compound_statement {
   $$ = create_while_statement($3, $5);
}
;

 /* if/then/else rule */
selection_statement: KW_IF LPAREN expression RPAREN selection_rest_statement {
   $$ = $5;
   $$->condition = $3;
}
;

selection_rest_statement: compound_statement {
   $$ = create_selection_statement(NULL, $1, NULL);
}
| compound_statement KW_ELSE compound_statement {
   $$ = create_selection_statement(NULL, $1, $3);
}
;

 /* rule to match an assigment statement */
assignment: IDENTIFIER OP_ASSIGN expression {
   $$ = create_expression(AST_ASSIGN, $3, NULL);
   $$->primary_expr.identifier = $1;
}
;

 /* expression evaluation rules */
expression: expression OP_ADD expression {
   $$ = create_expression(AST_ADD, $1, $3);
}
| expression OP_SUB expression {
   $$ = create_expression(AST_SUB, $1, $3);
}
| expression OP_MUL expression {
   $$ = create_expression(AST_MUL, $1, $3);
}
| expression OP_DIV expression {
   $$ = create_expression(AST_DIV, $1, $3);
}
| expression OP_GT  expression {
   $$ = create_expression(AST_GT, $1, $3);
}
| expression OP_LT  expression {
   $$ = create_expression(AST_LT, $1, $3);
}
| expression OP_EQ  expression {
   $$ = create_expression(AST_EQ, $1, $3);
}
| expression OP_NE  expression {
   $$ = create_expression(AST_NE, $1, $3);
}
| LPAREN expression RPAREN {
   $$ = $2;
}
| primary_expression {
   $$ = $1;
}
;

primary_expression: IDENTIFIER {
   $$ = create_expression(AST_IDENTIFIER, NULL, NULL);
   $$->primary_expr.identifier = $1;
}
| INT_CONSTANT {
   $$ = create_expression(AST_INT_CONSTANT, NULL, NULL);
   $$->primary_expr.int_constant = $1;
}
| FLOAT_CONSTANT {
   $$ = create_expression(AST_FLOAT_CONSTANT, NULL, NULL);
   $$->primary_expr.float_constant = $1;
}
;

 /* variable declaration rules */
declaration: type_specifier IDENTIFIER {
   $$ = create_declaration($1, $2);
}
;

type_specifier: KW_INT     { $$ = TYPE_INT;     }
              | KW_FLOAT   { $$ = TYPE_FLOAT;   }
              ;

%%

void
yyerror(char *s)
{
   printf("%s\n", s);
}

int main(int argc, char *argv[])
{
   yyparse();
   return 0;
}
