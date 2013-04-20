%{

#include <stdio.h>
#include <stdlib.h>

void yyerror(char *s);

%}

%union {
   int int_const;
   float float_const;
   char *identifier;
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

%%

 /* rule that matches a translation unit (aka. a source file) */
translation_unit: statement_list
                ;

 /* rules for: 1) statements
               2) list of statements
               3) compound statements */
compound_statement: LBRACE statement_list RBRACE
                  ;

statement_list: statement
              | statement_list statement
              ;

statement: declaration SEMICOLON
         | expression SEMICOLON
         | assignment SEMICOLON
         | selection_statement
         | while_statement
         ;

 /* while rule */
while_statement: KW_WHILE LPAREN expression RPAREN compound_statement
               ;

 /* if/then/else rule */
selection_statement: KW_IF LPAREN expression RPAREN selection_rest_statement
                   ;

selection_rest_statement: compound_statement
                        | compound_statement KW_ELSE compound_statement
                        ;


 /* rule to match an assigment statement */
assignment: IDENTIFIER OP_ASSIGN expression
          ;

 /* expression evaluation rules */
expression: expression OP_ADD expression
          | expression OP_SUB expression
          | expression OP_MUL expression
          | expression OP_DIV expression
          | expression OP_GT  expression
          | expression OP_LT  expression
          | expression OP_EQ  expression
          | expression OP_NE  expression
          | LPAREN expression RPAREN
          | primary_expression
          ;

primary_expression: IDENTIFIER
                  | INT_CONSTANT
                  | FLOAT_CONSTANT
                  ;

 /* variable declaration rules */
declaration: type IDENTIFIER
           ;

type: KW_INT
    | KW_FLOAT
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
