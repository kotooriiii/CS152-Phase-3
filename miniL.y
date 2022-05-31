    /* cs152-miniL phase3 */
%{

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <string>
#include <stack>
#include <vector>
#include <map>

using namespace std;

extern FILE * yyin;
extern int yylex();
extern int currLine;
extern int currPos; 

bool isError = 0;

const char* reservedWordsInit[] = {"function", "beginparams", "endparams", "beginlocals", "endlocals", "beginbody", "endbody", "integer", "array", "enum", "of", "if", "then", "endif", "else", "for", "while", "do", "beginloop", "endloop", "continue", "read", "write", "and", "or", "not", "true", "false", "return"};
vector<string> reservedWords(reservedWordsInit, reservedWordsInit + sizeof(reservedWordsInit)/sizeof(reservedWordsInit[0]));

enum SYMBOL_TYPE {INT, ARR, FUNC, IENUM};

string getSymbolTypeName(SYMBOL_TYPE type)
{
  if(type == INT)
  {
    return "INT";
  }
  else if (type == ARR)
  {
    return "ARR";
  }
  else if (type == FUNC)
  {
    return "FUNC";
  }
  else if (type == IENUM)
  {
    return "ENUM";
  }
  else
  {
    return "ERROR";
  }
}

void yyerror(const char * errorMessage)
{
	printf("Error: On line %d, column %d: %s\n", currLine, currPos, errorMessage);
  isError = 1;
}

struct Symbol 
{
  string name;
  SYMBOL_TYPE type; 
  
  int value;
  int size;

  Symbol(): value(0), size(0), name(), type() {};  
};

struct Function 
{
  string name;
  vector<Symbol> declarations;
};

vector<Function> symbol_table;
bool mainExists = 0;
bool mainDuplicateExists = 0;


Function *get_function() {
  int last = symbol_table.size()-1;
  return &symbol_table[last];
}

bool findSymbol(std::string &value) {
  Function *f = get_function();
  for(int i=0; i < f->declarations.size(); i++) {
    Symbol *s = &f->declarations[i];
    if (s->name == value) {
      return true;
    }
  }
  return false;
}

bool findFunction(std::string &value) {
  for(int i=0; i<symbol_table.size(); i++) {
     Function *f = &symbol_table[i];
    if (f->name == value) {
      return true;
    }
  }
  return false;
}

bool checkSymbolExists(std::string &value) {
  Function *f = get_function();
  for(int i=0; i < f->declarations.size(); i++) {
    Symbol *s = &f->declarations[i];
    if (s->name == value) {
      return true;
    }
  }

  string error = "Symbol \"" + value + "\" is not declared.";
  yyerror(error.c_str());
  return false;
}

bool checkSymbolIsOfType(std::string &value, SYMBOL_TYPE assertType) {
  Function *f = get_function();
  for(int i=0; i < f->declarations.size(); i++) {
    Symbol *s = &f->declarations[i];
    if (s->name == value)
    {
      if(s->type == assertType)
      {
        return true;
      }
      else
      {
        string error = "Symbol \"" + value + "\" is of type \"" + getSymbolTypeName(s->type) + ".";
        yyerror(error.c_str());
        return false;
      }
      
    }
  }


  return false;
}

bool checkFunctionExists(std::string &value) {
  for(int i=0; i<symbol_table.size(); i++) {
     Function *f = &symbol_table[i];
    if (f->name == value) {
      return true;
    }
  }
  
  string error = "Function \"" + value + "\" is not declared.";
  yyerror(error.c_str());
  return false;
}

void add_function_to_symbol_table(std::string &value) {

  if(findFunction(value))
  {
    string error = "Function \"" + value + "\" is already declared.";
    yyerror(error.c_str());
  }
  else
  {
    bool isFound = false;
    for(int i=0; i < reservedWords.size(); i++) 
    {
      string s = reservedWords[i];
      if (s == value) 
      {
        isFound = true;
        break;
      }
    }

    if(isFound)
    {
      string error = "Function \"" + value + "\" is not allowed to be used because it is a reserved keyword.";
      yyerror(error.c_str());
    }
    else
    {
      Function f; 
      f.name = value; 
      symbol_table.push_back(f);
      if(value == "main")
      {
        if(mainExists == 1)
        {
          mainDuplicateExists = 1;
        }
        mainExists = 1;
      }
    }
  }
}


void add_variable_to_symbol_table(std::string &name, int value, int size, SYMBOL_TYPE t) 
{
  
  if(findSymbol(name))
  {
    string error = "Symbol \"" + name + "\" is already declared.";
    yyerror(error.c_str());
  }
  else
  {

    bool isFound = false;
    for(int i=0; i < reservedWords.size(); i++) 
    {
      string s = reservedWords[i];
      if (s == name) 
      {
        isFound = true;
        break;
      }
    }

    if(isFound)
    {
      string error = "Symbol \"" + name + "\" is not allowed to be used because it is a reserved keyword.";
      yyerror(error.c_str());
    }
    else
    {
      Symbol s;
      s.name = name;
      s.type = t;
      s.value = value;
      s.size = size;
      Function *f = get_function();
      f->declarations.push_back(s);
    } 
  }
}

void print_symbol_table(void) {
  printf("symbol table:\n");
  printf("--------------------\n");
  for(int i=0; i<symbol_table.size(); i++) {
    printf("function: %s\n", symbol_table[i].name.c_str());
    for(int j=0; j<symbol_table[i].declarations.size(); j++) {
      printf("  locals: %s\n", symbol_table[i].declarations[j].name.c_str());
    }
  }
  printf("--------------------\n");
}

int tempCount = 0;
int labelCount = 0;
int paramCount = 0;

string makeTemp() {
  stringstream ss;
  ss << tempCount++;
  string temp = "__temp" + ss.str();
  return temp;
}

string makeLabel() {
  stringstream ss;
  ss << labelCount++;
  string temp = "__label" + ss.str();
  return temp;
}

stack<string> varStack; 
stack<string> identStack;
stack<string> expStack; 
vector<string> label_vector;
stack<string> paramStack;
ostringstream intermediateCodeStream;

stack<string> ands;
stack<string> ors;

%}

%union
{
  /* put your types here */
  char* identifierVal;
  int numberVal;
  struct attributesbison 
  {
    char name[100];
    char ind[100];
    int type;
    int value;
    int size;
  } attr;
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

//Grammars
%type <attr> bool_expr relation_expr relation_and_expr relation_expr_ending expression mul_exp term var
%type <attr> number ident
%type <attr> swrite sread sreturn
%type <identifierVal> comp


/* %start program */
%start program

%% 
/* write your rules here */
program:                  functions
                          {}
                          |
                          error {/*yyerrok; yyclearin;*/}
                          ;

functions:                function functions_spec 
                          {}
                          |
                          /*epsilon*/
                          {}
                          ;

functions_spec:           functions
                          {}
                          ;

function:                 FUNCTION ident 
                          {
                            intermediateCodeStream << "func " << $2.name << endl;
                            string name = string($2.name);
                            add_function_to_symbol_table(name);
                          }
                          SEMICOLON BEGIN_PARAMS declarations 
                          {
                            paramCount = 0;
                            while (!paramStack.empty())
                            {
                              intermediateCodeStream << "= " << paramStack.top() << ", " << "$" << paramCount++ << endl;
                              paramStack.pop();
                            }
                          }
                          END_PARAMS BEGIN_LOCALS declarations END_LOCALS BEGIN_BODY statements END_BODY
                          {
                            intermediateCodeStream << "endfunc\n";
                        
                            while (!paramStack.empty()) 
                            {
                              paramStack.pop();
                            }
                          }
                          ;

declarations:             declaration SEMICOLON declarations_spec
                          {}
                          |
                          /*epsilon*/
                          {}
                          ;

declarations_spec:        declarations
                          {}

statements:               statement SEMICOLON statements
                          {}
                          |
                          {}
                          ;

declaration:              identifiers COLON INTEGER
                          {
                            while(!identStack.empty())
                            {
                              string name = identStack.top();
                              SYMBOL_TYPE type = INT;
                              add_variable_to_symbol_table(name, 0, 0, type);
                              intermediateCodeStream << ". " << name << endl;
                              identStack.pop(); 
                            }
                          }
                          |
                          identifiers COLON ARRAY L_SQUARE_BRACKET number R_SQUARE_BRACKET OF INTEGER
                          {
                            while(!identStack.empty())
                            {
                              string name = identStack.top();
                              SYMBOL_TYPE type = ARR;
                              add_variable_to_symbol_table(name, 0, $5.value, type);
                              if($5.value <= 0)
                              {
                                yyerror("Error: Size of array should be a positive integer.");
                              }
                              intermediateCodeStream << ".[] " << name << ", " << $5.value << endl;
                              identStack.pop(); 
                            } 
                          }
                          |
                          identifiers COLON ENUM L_PAREN
                          {
                            while(!identStack.empty())
                            {
                              string name = identStack.top();
                              SYMBOL_TYPE type = IENUM;
                              add_variable_to_symbol_table(name, 0, 0, type);
                              intermediateCodeStream << ". " << name << endl;
                              identStack.pop();
                            } 
                          }
                          identifiers R_PAREN
                          {
                            while(!identStack.empty())
                            {
                              string name = identStack.top();
                              SYMBOL_TYPE type = INT;
                              add_variable_to_symbol_table(name, 0, 0, type);
                              intermediateCodeStream << ". " << name << endl;
                              identStack.pop();
                            }   
                          }
                          ;

identifiers:              ident COMMA identifiers
                          {
                            identStack.push(string($1.name));
                            paramStack.push(string($1.name));
                          }
                          |
                          ident
                          {
                            identStack.push(string($1.name));
                            paramStack.push(string($1.name));
                            
                          }
                          ;

statement:                svar
                          {}
                          |
                          sif
                          {}
                          |
                          swhile
                          {}
                          |
                          sdo
                          {}
                          |
                          sread
                          {}
                          |
                          swrite
                          {}
                          |
                          scontinue
                          {}
                          |
                          sreturn
                          {}
                          ;

svar:                     var ASSIGN expression
                          {
                            if($1.type == ARR)
                            {
                              intermediateCodeStream << "[]= " << const_cast<char*>($1.name) << ", " << const_cast<char*>($1.ind) << ", " << const_cast<char*>($3.name) << endl;
                            }
                            else 
                            {
                              intermediateCodeStream << "= " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;
                            }	
                          }
                          ;

sif:	                    IF bool_expr THEN 
                          {
                          string start = makeLabel();
                          string endif = makeLabel();
                          label_vector.push_back(endif); 
                          intermediateCodeStream << "?:= " << start << ", " << const_cast<char*>($2.name) << endl;
                          intermediateCodeStream << ":= " << endif << endl;
                          intermediateCodeStream << ": " << start << endl;
                          } 
                          statement SEMICOLON statements elses ENDIF 
                          {
                          intermediateCodeStream << ": " << label_vector.back() << endl;
                          label_vector.pop_back();             
                          }
                          ;

elses:                    /*epsilon*/
                          {}
                          | ELSE 
                          {
                              string label = makeLabel(); 
                              intermediateCodeStream << ":= " << label << endl;
                              intermediateCodeStream << ": " << label_vector.back() << endl;
                              label_vector.pop_back();
                              label_vector.push_back(label);
                          } statement SEMICOLON statements
                          ;

swhile:                   WHILE bool_expr BEGINLOOP
                          {
                            string condition = makeLabel();
                            string endlabel = makeLabel();
                            string start = makeLabel();
                            intermediateCodeStream << ": " << start << endl;
                            intermediateCodeStream << "?:= " << condition << ", " << const_cast<char*>($2.name) << endl;
                            intermediateCodeStream << ":= " << endlabel << endl;
                            intermediateCodeStream << ": " << condition << endl;
                            label_vector.push_back(start);
                            label_vector.push_back(endlabel);                            
                          }
                          statements ENDLOOP
                          {
                            string endlabel = label_vector.back();
                            label_vector.pop_back();
                            string start = label_vector.back();
                            label_vector.pop_back();
                            intermediateCodeStream << ":= " << start << endl;
                            intermediateCodeStream << ": " << endlabel << endl;
                          }
                          ;

sdo:                      DO BEGINLOOP
                          {
                            string start = makeLabel();
                            label_vector.push_back(start);
                            intermediateCodeStream << ": " << start << endl;
                          }
                          statements ENDLOOP WHILE bool_expr
                          {
                            string start = label_vector.back();
                            intermediateCodeStream << "?:= " << start << ", " << const_cast<char*>($7.name) << endl;
                            label_vector.pop_back();                          
                          }
                          ;

sread:                    READ var vars
                          {
                            varStack.push($2.name);
                            while (!varStack.empty()) 
                            {
                                if ($2.type == 0) {
                                    intermediateCodeStream << ".< " << varStack.top() << endl;
                                    varStack.pop();
                                }
                                else {
                                    intermediateCodeStream << ".[]< " << varStack.top() << ", "  <<  const_cast<char*>($2.ind) << endl;
                                    varStack.pop();
                                }
                            }                       
                          }
                          ;

vars:                     /*episolon*/
                          {}
                          |
                          COMMA var vars
                          {varStack.push($2.name);}
                          ;
                      
swrite:                   WRITE var vars
                          {
                            varStack.push($2.name);
                            while (!varStack.empty()) 
                            {
                              if ($2.type == 0) 
                              {
                                intermediateCodeStream << ".> " << varStack.top() << endl;
                                varStack.pop();
                              }
                              else 
                              {
                                string out = makeTemp();
                                strcpy($$.name, out.c_str());
                                intermediateCodeStream << ". " << out << endl;
                                intermediateCodeStream << "=[] " << out << ", " << const_cast<char*>($2.name) << ", " << const_cast<char*>($2.ind) << endl;
                                intermediateCodeStream << ".[]> " << varStack.top() << ", "  <<  const_cast<char*>($2.ind) << endl;
                                varStack.pop();
                              }
                            }                         
                          }
                          ;

scontinue:                CONTINUE
                          {
                            if (!label_vector.empty())
                            {
                              intermediateCodeStream << ":= " << label_vector.back() << endl;
                            }
                            else 
                            {
                              yyerror("Error: Continue used outside of loop");
                            }
                          }
                          ;

sreturn:                  RETURN expression
                          {
                            $$.value = $2.value;
                            strcpy($$.name, $2.name);
                            intermediateCodeStream << "ret " << const_cast<char*>($2.name) << endl;                         
                          }
                          ;

bool_expr:                relation_and_expr bool_exprs
                          {
                            string out = makeTemp();
                            strcpy($$.name, out.c_str());
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "= " << out << ", " << $1.name << endl;;

                            while(!ors.empty())
                            {
                              string name = ors.top();
                              ors.pop();
                              intermediateCodeStream << "|| " << out << ", " << out << ", " << name << endl; 
                            }
                          }
                          ;

bool_exprs:               OR relation_and_expr bool_exprs
                          {
                            ors.push(string($2.name));
                          }
                          |
                          /*epsilon*/
                          {}
                          ;

relation_and_expr:        relation_expr relation_and_exprs
                          {
                            string out = makeTemp();
                            strcpy($$.name, out.c_str());
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "= " << out << ", " << $1.name << endl;;

                            while(!ands.empty())
                            {
                              string name = ands.top();
                              ands.pop();
                              intermediateCodeStream << "&& " << out << ", " << out << ", " << name << endl; 
                            }
                          }
                          ;

relation_and_exprs:       AND relation_expr relation_and_exprs
                          {
                            ands.push(string($2.name));
                          }
                          |
                          /*epsilon*/
                          {}
                          ;

relation_expr:            NOT relation_expr_ending
                          {
                            string out = makeTemp();
                            strcpy($$.name, out.c_str());
                            intermediateCodeStream << "! " << out << ", " << const_cast<char*>($2.name) << endl;
                          }
                          |
                          relation_expr_ending
                          {
                            strcpy($$.name, $1.name);
                          }
                          ;

relation_expr_ending:     expression comp expression
                          {
                            string out = makeTemp();
                            strcpy($$.name, out.c_str());
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << $2 << " " << out << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;                                            
                          }
                          |
                          TRUE
                          {
                            string out = makeTemp();
                            strcpy($$.name, out.c_str());
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "= " << out << ", " << "1" << endl;
                          }
                          |
                          FALSE
                          {
                            string out = makeTemp();
                            strcpy($$.name, out.c_str());
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "= " << out << ", " << "0" << endl;
                          }
                          |
                          L_PAREN bool_expr R_PAREN
                          {
                            strcpy($$.name, $2.name);
                          }
                          ;

comp:                     EQ
                          {$$ = const_cast<char*>("==");}
                          |
                          NEQ
                          {$$ = const_cast<char*>("!=");}
                          |
                          LT
                          {$$ = const_cast<char*>("<");}
                          |
                          GT
                          {$$ = const_cast<char*>(">");}
                          |
                          LTE
                          {$$ = const_cast<char*>("<=");}
                          |
                          GTE
                          {$$ = const_cast<char*>(">=");}
                          ;

expression:               mul_exp 
                          {
                            strcpy($$.name,$1.name);
                            $$.type = $1.type;
                          }
                          |
                          expression ADD mul_exp 
                          {
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "+ " << out << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;
                            strcpy($$.name, out.c_str());
                          }
                          |
                          expression SUB mul_exp 
                          {
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "- " << out << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;
                            strcpy($$.name, out.c_str());
                          }
                          ;

mul_exp:                  term 
                          {
                            strcpy($$.name, $1.name);
                            $$.type = $1.type;
                          }
                          |
                          mul_exp MULT term 
                          {
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "* " << out << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;
                            strcpy($$.name, out.c_str());
                          }
                          |
                          mul_exp DIV term 
                          {
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "/ " << out << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;
                            strcpy($$.name, out.c_str());
                          }
                          |
                          mul_exp MOD term
                          {
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "% " << out << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($3.name) << endl;
                            strcpy($$.name, out.c_str());
                          }
                          ;

term:                     var
                          {
                            $$.value = $1.value;
                            $$.type = $1.type;
                            if ($1.type == 0) 
                            {
                              strcpy($$.name, $1.name);
                              strcpy($$.ind, $$.name);
                            }
                            else if ($1.type == 1) 
                            {
                              string out = makeTemp();
                              strcpy($$.name, out.c_str());
                              intermediateCodeStream << ". " <<  const_cast<char*>($$.name) << endl;
                              intermediateCodeStream << "=[] " << const_cast<char*>($$.name) << ", " << const_cast<char*>($1.name) << ", " << const_cast<char*>($1.ind) << endl;
                            }
                          }
                          |
                          SUB var
                          {
                            $$.value = $2.value * -1;
                            if ($2.type == 0) 
                            {
                              strcpy($$.name, $2.name);
                              strcpy($$.ind, $$.name);
                              $$.type = 0;

                              string minus = makeTemp();
                              intermediateCodeStream << ". " << minus << endl;
                              intermediateCodeStream << "= " << minus << ", " << "0" << endl;

                              string num = makeTemp();
                              intermediateCodeStream << ". " << num << endl;
                              intermediateCodeStream << "= " << num << ", " << $2.name << endl;

                              string out = makeTemp();
                              strcpy($$.name , out.c_str());
                              intermediateCodeStream << ". " << const_cast<char*>($$.name) << endl;
                              intermediateCodeStream << "- " << const_cast<char*>($$.name) << ", " << minus << ", " << num << endl;  
                            }
                            else if ($2.type == 1) 
                            {

                              string minus = makeTemp();
                              intermediateCodeStream << ". " << minus << endl;
                              intermediateCodeStream << "= " << minus << ", " << "0" << endl;

                              string num = makeTemp();
                              intermediateCodeStream << ". " <<  num << endl;
                              intermediateCodeStream << "=[] " << num << ", " << const_cast<char*>($2.name) << ", " << const_cast<char*>($2.ind) << endl;

                              string out = makeTemp();
                              strcpy($$.name , out.c_str());
                              strcpy($$.ind, $$.name);
                              $$.type = 0;
                              intermediateCodeStream << ". " << const_cast<char*>($$.name) << endl;
                              intermediateCodeStream << "- " << const_cast<char*>($$.name) << ", " << minus << ", " << num << endl;  
                            }                       
                          }
                          |
                          number
                          {
                            $$.value = $1.value;
                            $$.type = $1.type;
                            strcpy($$.name, $1.name);
                            strcpy($$.ind, $1.ind);                            
                          }
                          |
                          SUB number
                          {
                            $$.value = $2.value * -1;
                            $$.type = $2.type;
                            strcpy($$.name, $2.name);
                            strcpy($$.ind, $2.ind);  

                            string minus = makeTemp();
                            intermediateCodeStream << ". " << minus << endl;
                            intermediateCodeStream << "= " << minus << ", " << "0" << endl;

                            string num = makeTemp();
                            intermediateCodeStream << ". " << num << endl;
                            intermediateCodeStream << "= " << num << ", " << $2.value << endl;

                            string out = makeTemp();
                            strcpy($$.name , out.c_str());
                            intermediateCodeStream << ". " << const_cast<char*>($$.name) << endl;
                            intermediateCodeStream << "- " << const_cast<char*>($$.name) << ", " << minus << ", " << num << endl;  
                          }
                          |
                          L_PAREN expression R_PAREN
                          {
                            strcpy($$.name, $2.name);
                          }
                          |
                          SUB L_PAREN expression R_PAREN
                          {
                            string minus = makeTemp();
                            intermediateCodeStream << ". " << minus << endl;
                            intermediateCodeStream << "= " << minus << ", " << "0" << endl;

                            string num = makeTemp();
                            intermediateCodeStream << ". " << num << endl;
                            intermediateCodeStream << "= " << num << ", " << $3.name << endl;

                            string out = makeTemp();
                            strcpy($$.name , out.c_str());
                            intermediateCodeStream << ". " << const_cast<char*>($$.name) << endl;
                            intermediateCodeStream << "- " << const_cast<char*>($$.name) << ", " << minus << ", " << num << endl;  
                          }
                          |
                          ident L_PAREN expressions R_PAREN
                          {
                            string name = string($1.name);
                            checkFunctionExists(name);
                            while (!expStack.empty())
                            {
                              intermediateCodeStream << "param " << expStack.top() << endl;
                              expStack.pop();
                            }
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "call " << const_cast<char*>($1.name) << ", " << out << endl;
                            strcpy($$.name, out.c_str());
                          }
                          |
                          ident L_PAREN R_PAREN
                          {
                            string name = string($1.name);
                            checkFunctionExists(name);
                            string out = makeTemp();
                            intermediateCodeStream << ". " << out << endl;
                            intermediateCodeStream << "call " << const_cast<char*>($1.name) << ", " << out << endl;
                            strcpy($$.name, out.c_str());
                          }
                          ;

expressions:              expressions COMMA expression
                          {
                            expStack.push($3.name);
                          }
                          |
                          expression
                          {
                            expStack.push($1.name);
                          }
                          ;

var:                      ident L_SQUARE_BRACKET expression R_SQUARE_BRACKET
                          {
                            string name = string($1.name);
                            checkSymbolExists(name);
                            if(checkSymbolIsOfType(name, ARR))
                            {
                              if ($3.type == 1) 
                              {
                                string out = makeTemp();
                                $$.type = 1;
                                strcpy($$.ind, out.c_str());
                                strcpy($$.name, $1.name);
                                intermediateCodeStream << ". " << out << endl; 
                                intermediateCodeStream << "=[] " << out << ", " << const_cast<char*>($3.name) << ", " << const_cast<char*>($3.ind) << endl;
                              }
                              else
                              {
                                strcpy($$.name,$1.name);
                                $$.type = 1;
                                strcpy($$.ind, $3.name);
                              }
                            }
                          }
                          |
                          ident
                          {
                            string name = string($1.name);
                            checkSymbolExists(name);
                            if(checkSymbolIsOfType(name, INT))
                            {
                             	strcpy($$.name , $1.name);	
                              $$.type = 0;
                            }
                          }
                          ;

ident:                    IDENT
                          {
                            $$.value = 42;
                            $$.type = 0;
                            char chars[100];
                            strcpy($$.name, $1);
                            strcpy($$.ind, $$.name);
                          }
                          |
                          //ERROR
                          {

                            string error = "Identifier not allowed to be used because it is a reserved keyword.";
                            yyerror(error.c_str());
              
                            strcpy($$.name, "null");
                          }
                          ;

number:                   NUMBER
                          {
                            $$.value = $1;
                            $$.type = 0;
                            char chars[100];
                            sprintf(chars, "%d", $1);
                            strcpy($$.name, chars);
                            strcpy($$.ind, $$.name);
                          }
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

  if(mainExists==0)
  {
    yyerror("Error: main function was not able to be found.");
  }
  else if (mainExists==1)
  {
    if(mainDuplicateExists==1)
    {
          yyerror("Error: Duplicate main functions found.");
    }
  }

  if(!isError)
  {
    printf("The file \"%s\" was able to be compiled into out.mil successfully.\n", argv[1]);
    ofstream file;
    file.open("out.mil");
    file << intermediateCodeStream.str();
    file.close();
  }
  else
  {
    printf("The file \"%s\" produced errors.\n", argv[1]);
  }
	return 0;
}