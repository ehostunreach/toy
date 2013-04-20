%{

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

%}

%%

 /* math operators */
"+"      { return OP_ADD;     }
"-"      { return OP_SUB;     }
"*"      { return OP_MUL;     }
"/"      { return OP_DIV;     }

 /* comparison operators */
">"      { return OP_GT;      }
"<"      { return OP_LT;      }
"=="     { return OP_EQ;      }
"!="     { return OP_NE;      }
"<="     { return OP_LE;      }
">="     { return OP_GE;      }

 /* assignment operator */
"="      { return OP_ASSIGN;  }

 /* statement terminator */
";"      { return SEMICOLON;  }

 /* grouping operators */
"{"      { return LBRACE;     }
"}"      { return RBRACE;     }
"("      { return LPAREN;     }
")"      { return RPAREN;     }

 /* data type keywords */
"int"    { return KW_INT;     }
"float"  { return KW_FLOAT;   }

 /* control flow keywords */
"if"     { return KW_IF;      }
"else"   { return KW_ELSE;    }
"while"  { return KW_WHILE;   }

 /* regular expression for identifier names */
[_a-zA-Z][_a-zA-Z0-9]* { return IDENTIFIER;  }

 /* regular expression for integer constants */
[0-9]+            { return INT_CONSTANT;     }
[0-9]+\.[0-9]+    { return FLOAT_CONSTANT;   }

 /* ignore whitespace */
[ \r\t]+ { }

 /* ignore newlines */
\n       { }

 /* ignore unknown characters */
.        { }

%%
