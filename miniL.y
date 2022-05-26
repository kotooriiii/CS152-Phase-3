    /* cs152-miniL phase2 */
%{
#include <stdio.h>
extern FILE * yyin;
extern int currLine;
extern int currPos; 
void yyerror(const char * errorMessage)
{
	printf("Error: On line %d, column %d: %s\n", currLine, currPos, errorMessage);
}
%}

%union
{
  /* put your types here */
  char* identifierVal;
  int numberVal;
}

%error-verbose

%type<identifierVal> IDENT
%type<numberVal> NUMBER

%locations

%token FUNCTION BEGIN_PARAMS END_PARAMS BEGIN_LOCALS END_LOCALS BEGIN_BODY END_BODY INTEGER ARRAY ENUM OF IF THEN ENDIF ELSE FOR WHILE DO BEGINLOOP ENDLOOP CONTINUE READ WRITE TRUE FALSE RETURN SEMICOLON COLON COMMA

%right ASSIGN

%left OR

%left AND

%right NOT

%left NEQ EQ GTE GT LTE LT

%left ADD SUB

%left MULT DIV MOD 

%right UMINUS

%left L_SQUARE_BRACKET R_SQUARE_BRACKET

%left L_PAREN R_PAREN

%token IDENT
%token NUMBER

/* %start program */
%start program

%% 
/* write your rules here */
program:                  functions
                          {printf("program->functions\n");}
                          |
                          error {yyerrok; yyclearin;}
                          ;

functions:                function functions_spec 
                          {printf("functions->function functions_spec\n");}
                          |
                          /*epsilon*/
                          {printf("functions->epsilon\n");}
                          ;

functions_spec:           functions
                          {printf("functions_spec->functions\n");}
                          ;

function:                 FUNCTION ident SEMICOLON BEGIN_PARAMS declarations END_PARAMS BEGIN_LOCALS declarations END_LOCALS BEGIN_BODY statements END_BODY
                          {printf("function->FUNCTION ident SEMICOLON BEGIN_PARAMS declarations END_PARAMS BEGIN_LOCALS declarations END_LOCALS BEGIN_BODY statements END_BODY\n");}
                          ;

declarations:             declaration SEMICOLON declarations_spec
                          {printf("declarations->declaration SEMICOLON declarations_spec\n");}
                          |
                          /*epsilon*/
                          {printf("declarations->epsilon\n");}
                          ;

declarations_spec:        declarations
                          {printf("declarations_spec->declarations\n");}

statements:               statement SEMICOLON statementsEnding
                          {printf("statements->statement SEMICOLON statementsEnding\n");}
                          ;

statementsEnding:         statements 
                          {printf("statementsEnding->statements\n");}
                          |
                          /*epsilon*/
                          {printf("statementsEnding->epsilon\n");}
                          ;

declaration:              identifiers COLON INTEGER
                          {printf("declaration->identifiers COLON INTEGER\n");}
                          |
                          identifiers COLON ARRAY L_SQUARE_BRACKET number R_SQUARE_BRACKET OF INTEGER
                          {printf("declaration->identifiers COLON ARRAY L_SQUARE_BRACKET number R_SQUARE_BRACKET OF INTEGER\n");}
                          |
                          identifiers COLON ENUM L_PAREN identifiers R_PAREN
                          {printf("declaration->identifiers COLON ENUM L_PAREN identifiers R_PAREN\n");}
                          ;

identifiers:              ident COMMA identifiers
                          {printf("identifiers->ident COMMA identifiers\n");}
                          |
                          ident
                          {printf("identifiers->ident\n");}
                          ;

statement:                svar
                          {printf("statement->svar\n");}
                          |
                          sif
                          {printf("statement->sif\n");}
                          |
                          swhile
                          {printf("statement->swhile\n");}
                          |
                          sdo
                          {printf("statement->sdo\n");}
                          |
                          sread
                          {printf("statement->sread\n");}
                          |
                          swrite
                          {printf("statement->swrite\n");}
                          |
                          scontinue
                          {printf("statement->scontinue\n");}
                          |
                          sreturn
                          {printf("statement->sreturn\n");}
                          ;

svar:                     var ASSIGN expression
                          {printf("svar->var ASSIGN expression\n");}
                          ;

sif:                      IF bool_expr THEN statements ENDIF
                          {printf("sif->IF bool_expr THEN statements ENDIF\n");}
                          |
                          IF bool_expr THEN statements ELSE statements ENDIF
                          {printf("sif->IF bool_expr THEN statements ELSE statements ENDIF\n");}
                          ;

swhile:                   WHILE bool_expr BEGINLOOP statements ENDLOOP
                          {printf("swhile->WHILE bool_expr BEGINLOOP statements ENDLOOP\n");}
                          ;

sdo:                      DO BEGINLOOP statements ENDLOOP WHILE bool_expr
                          {printf("sdo->DO BEGINLOOP statements ENDLOOP WHILE bool_expr\n");}
                          ;

sread:                    READ vars
                          {printf("sread->READ vars\n");}
                          ;

vars:                     var
                          {printf("vars->var\n");}
                          |
                          var COMMA vars
                          {printf("vars->var COMMA vars\n");}
                          ;
                      
swrite:                   WRITE vars
                          {printf("swrite->WRITE vars\n");}
                          ;

scontinue:                CONTINUE
                          {printf("scontinue->CONTINUE\n");}
                          ;

sreturn:                  RETURN expression
                          {printf("sreturn->RETURN expression\n");}
                          ;

bool_expr:                relation_and_expr bool_exprs
                          {printf("bool_expr->relation_and_expr bool_exprs\n");}
                          ;

bool_exprs:               OR relation_and_expr bool_exprs
                          {printf("bool_exprs->OR relation_and_expr bool_exprs\n");}
                          |
                          /*epsilon*/
                          {printf("bool_exprs->epsilon\n");}
                          ;

relation_and_expr:        relation_expr relation_and_exprs
                          {printf("relation_and_expr->relation_expr relation_and_exprs\n");}
                          ;

relation_and_exprs:       AND relation_expr relation_and_exprs
                          {printf("relation_and_exprs->AND relation_expr relation_and_exprs\n");}
                          |
                          /*epsilon*/
                          {printf("relation_and_exprs->epsilon\n");}
                          ;

relation_expr:            nots relation_expr_ending
                          {printf("relation_expr->nots relation_expr_ending\n");}
                          ;

relation_expr_ending:     expression comp expression
                          {printf("relation_expr_ending->expression comp expression\n");}
                          |
                          TRUE
                          {printf("relation_expr_ending->TRUE\n");}
                          |
                          FALSE
                          {printf("relation_expr_ending->FALSE\n");}
                          |
                          L_PAREN bool_expr R_PAREN
                          {printf("relation_expr_ending->L_PAREN bool_expr R_PAREN\n");}
                          ;

nots:                     NOT
                          {printf("nots->NOT\n");}
                          |
                          /*epsilon*/
                          {printf("nots->epsilon\n");}
                          ;

comp:                     EQ
                          {printf("comp->EQ\n");}
                          |
                          NEQ
                          {printf("comp->NEQ\n");}
                          |
                          LT
                          {printf("comp->LT\n");}
                          |
                          GT
                          {printf("comp->GT\n");}
                          |
                          LTE
                          {printf("comp->LTE\n");}
                          |
                          GTE
                          {printf("comp->GTE\n");}
                          ;

expression:               multiplicative_expr expression_operators
                          {printf("expression->multiplicative_expr expression_operators\n");}
                          ;

expression_operators:     ADD multiplicative_expr expression_operators
                          {printf("expression_operators->ADD multiplicative_expr expression_operators\n");}
                          |
                          SUB multiplicative_expr expression_operators
                          {printf("expression_operators->SUB multiplicative_expr expression_operators\n");}
                          |
                          /*epsilon*/
                          {printf("expression_operators->epsilon\n");}
                          ;

multiplicative_expr:      term multiplicative_operators
                          {printf("multiplicative_expr->term multiplicative_operators\n");}
                          ;

multiplicative_operators: MOD term multiplicative_operators
                          {printf("multiplicative_operators->MOD term multiplicative_operators\n");}
                          |
                          DIV term multiplicative_operators
                          {printf("multiplicative_operators->DIV term multiplicative_operators\n");}
                          |
                          MULT term multiplicative_operators
                          {printf("multiplicative_operators->MULT term multiplicative_operators\n");}
                          |
                          /*epsilon*/
                          {printf("multiplicative_operators->epsilon\n");}
                          ;

term:                     term_spec
                          {printf("term->term_spec\n");}
                          |
                          subs number
                          {printf("term->subs number\n");}
                          |
                          subs L_PAREN expression R_PAREN
                          {printf("term->subs L_PAREN expression R_PAREN\n");}
                          ;

term_spec:                subs ident var_or_bottom
                          {printf("term_spec->subs ident var_or_bottom\n");}
                          ;

subs:                     SUB
                          {printf("subs->SUB\n");}
                          |
                          /*epsilon*/
                          {printf("subs->epsilon\n");}
                          ;

expressions:              expression COMMA expressions
                          {printf("expressions->expression COMMA expressions\n");}
                          |
                          expression
                          {printf("expressions->expression\n");}
                          |
                          /*epsilon*/
                          {printf("expressions->epsilon\n");}
                          ;

var:                      ident var_spec
                          {printf("var->ident var_spec\n");}
                          ;

var_or_bottom:            var_spec
                          {printf("var_or_bottom->var_spec\n");}
                          |
                          L_PAREN expressions R_PAREN
                          {printf("var_or_bottom->L_PAREN expressions R_PAREN\n");}
                          ;

var_spec:                 L_SQUARE_BRACKET expression R_SQUARE_BRACKET
                          {printf("var_spec->L_SQUARE_BRACKET expression R_SQUARE_BRACKET\n");}
                          |
                          /*epsilon*/
                          {printf("var_spec->epsilon\n");}
                          ;

ident:                    IDENT
                          {printf("ident->IDENT %s\n", $1);}
                          ;

number:                   NUMBER
                          {printf("number->NUMBER %d\n", $1);}
                          ;

%% 

int main(int argc, char ** argv) 
{
	if (argc >= 2) 
  {
		yyin = fopen(argv[1], "r");
		if (yyin == NULL) 
    {
			yyin = stdin;
		}
	}
	else 
  {
		yyin = stdin;
	}
	yyparse();
	return 1;
}