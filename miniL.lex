   /* cs152-miniL phase1 */
   
%{   
   /* write your C code here for definitions of variables and including headers */
   #include "y.tab.h"
   int currLine = 1;
   int currPos = 1;
   int errorNum = 0;
%}

   /* some common rules */
NUMBER                        [0-9]+
IDENT                         [A-Za-z][A-Za-z0-9_]*[A-Za-z0-9]|[A-Za-z]
IDENT_ERROR_ENDS_UNDERSCORE   [A-Za-z][A-Za-z0-9_]*[_]
IDENT_ERROR_STARTS_NUMBER     [0-9][A-Za-z0-9_]*[A-Za-z0-9]
IDENT_ERROR_ANY               [0-9][A-Za-z0-9_]*[_]
COMMENT                       ##[^\n]*


%%
   /* specific lexer rules in regex */

"\n"                 {currLine++; currPos = 1;}
[ \t]+               {/*ignore spaces*/ currPos += yyleng;}

"function"           {currPos += yyleng; return FUNCTION;}
"beginparams"        {currPos += yyleng; return BEGIN_PARAMS;}
"endparams"          {currPos += yyleng; return END_PARAMS;}
"beginlocals"        {currPos += yyleng; return BEGIN_LOCALS;}
"endlocals"          {currPos += yyleng; return END_LOCALS;}
"beginbody"          {currPos += yyleng; return BEGIN_BODY;}
"endbody"            {currPos += yyleng; return END_BODY;}
"integer"            {currPos += yyleng; return INTEGER;}
"array"              {currPos += yyleng; return ARRAY;}
"enum"               {currPos += yyleng; return ENUM;}
"of"                 {currPos += yyleng; return OF;}
"if"                 {currPos += yyleng; return IF;}
"then"               {currPos += yyleng; return THEN;}
"endif"              {currPos += yyleng; return ENDIF;}
"else"               {currPos += yyleng; return ELSE;}
"for"                {currPos += yyleng; return FOR;}
"while"              {currPos += yyleng; return WHILE;}
"do"                 {currPos += yyleng; return DO;}
"beginloop"          {currPos += yyleng; return BEGINLOOP;}
"endloop"            {currPos += yyleng; return ENDLOOP;}
"continue"           {currPos += yyleng; return CONTINUE;}
"read"               {currPos += yyleng; return READ;}
"write"              {currPos += yyleng; return WRITE;}
"and"                {currPos += yyleng; return AND;}
"or"                 {currPos += yyleng; return OR;}
"not"                {currPos += yyleng; return NOT;}
"true"               {currPos += yyleng; return TRUE;}
"false"              {currPos += yyleng; return FALSE;}
"return"             {currPos += yyleng; return RETURN;}

"-"                  {currPos += yyleng; return SUB;}
"+"                  {currPos += yyleng; return ADD;}
"*"                  {currPos += yyleng; return MULT;}
"/"                  {currPos += yyleng; return DIV;}
"%"                  {currPos += yyleng; return MOD;}

"=="                 {currPos += yyleng; return EQ;}
"<>"                 {currPos += yyleng; return NEQ;}
"<"                  {currPos += yyleng; return LT;}
">"                  {currPos += yyleng; return GT;}
"<="                 {currPos += yyleng; return LTE;}
">="                 {currPos += yyleng; return GTE;}

";"                  {currPos += yyleng; return SEMICOLON;}
":"                  {currPos += yyleng; return COLON;}
","                  {currPos += yyleng; return COMMA;}
"("                  {currPos += yyleng; return L_PAREN;}
")"                  {currPos += yyleng; return R_PAREN;}
"["                  {currPos += yyleng; return L_SQUARE_BRACKET;}
"]"                  {currPos += yyleng; return R_SQUARE_BRACKET;}
":="                 {currPos += yyleng; return ASSIGN;}

{COMMENT}                     {currPos += yyleng;}

{NUMBER}                      {currPos += yyleng; yylval.numberVal = atoi(yytext); return NUMBER;}

{IDENT}                       {currPos += yyleng; yylval.identifierVal = yytext; return IDENT;}

{IDENT_ERROR_ENDS_UNDERSCORE} {printf("Error at line %d, column %d: identifier \"%s\" cannot end with an underscore\n", currLine, currPos, yytext); errorNum++; currPos += yyleng; exit(0);}
{IDENT_ERROR_STARTS_NUMBER}   {printf("Error at line %d, column %d: identifier \"%s\" must begin with a letter\n", currLine, currPos, yytext); errorNum++; currPos += yyleng; exit(0);}
{IDENT_ERROR_ANY}             {printf("Error at line %d, column %d: identifier \"%s\" must begin with a letter and not end with an underscore\n", currLine, currPos, yytext); errorNum++; currPos += yyleng; exit(0);}
.                             {printf("Error at line %d, column %d: unrecognized symbol \"%s\"\n", currLine, currPos, yytext); errorNum++; currPos += yyleng; exit(0);}



%%
/* C functions */
/*

int main(int argc, char ** argv)
{
   if(argc >= 2)
   {
      yyin = fopen(argv[1], "r");
      if(yyin == NULL)
      {
         yyin = stdin;
      }
   }
   else
   {
      yyin = stdin;
   }
   yylex();
   if(errorNum == 0)
   {
      printf("\n\nThe program compiled successfully.\n");
   }
   else
   {
      printf("\n\nThe program failed to compile with %d errors.\n", errorNum);
   }
}

*/
