%{
#include 	<ctype.h>
#include 	<string.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include <assert.h>

#define HASHSIZE 128

typedef struct {
char *name; /* symbol name */
int scope; /* scope level */
int type;
int offset; /* for functions: size of local variables */
} symbol ;

struct sym_entry {
symbol sym; /* this symbol */
struct sym_entry *next; /* next entry on hash chain */
};

struct table {
int level; /* scope level for this table */
struct table *previous; /* table at lower scope */
struct sym_entry *buckets[HASHSIZE]; /* hash buckets */
};

typedef struct
{
    char op;
    symbol *x, *y, *z;
} Quadruple;


typedef struct
{
    char *translation;
    int variable;
} myattribute;

typedef struct
{  char * name;
} fun_type;

struct string_data {
  int index;
  char * value;
};
int n_const_strings = 0;
struct string_data const_strings[100];

struct table * table_stack[100];
struct table * global_table;
int table_len = 0;
int offset_stack[100];
int offset_len = 0;
int level = 0;
int verbose = 0;

void trace(char *s) { if (verbose) fprintf(stderr, s); }

symbol *lookup(char *name, struct table *tp) {
struct sym_entry *p;
unsigned h = ((unsigned)name)&(HASHSIZE-1); /*not素数 */
//printf("start do\n");
do
for (p = tp->buckets[h]; p; p = p->next)
{if (name == p->sym.name) return &p->sym;}
while (tp = tp->previous);
return 0;
}

struct table * mktable(struct table *previous, int level) {
	int i;
	struct table *new = (struct table *)malloc(sizeof *new);
	new->previous = previous; new->level = level;
	for (i = 0; i < HASHSIZE; i++) new->buckets[i] = 0;
	return new;
}

symbol *insert(char *name, struct table *tpp) {
//printf("insert start\n");
unsigned h = ((unsigned)name)&(HASHSIZE-1);
struct table *tp = tpp;
struct sym_entry *p = (struct entry *)malloc(sizeof *p);
if(p==NULL)
{
yyerror("malloc error\n");
}
//printf("malloc successful\n");
//printf("tp->level =%d",tp->level);
//printf("level =%d",level);
//printf("tp->level =%d level=%d  inserting %s\n", tp->level,level,name);
//printf("start scope level\n");
int tmp=0;
if (tp->level < level) { // 当前scope level
                //tmp++;
                //printf("scope: %d",tmp);
	tp = mktable(tp, level);
	table_stack[table_len] = tp;
	table_len ++;
	offset_stack[offset_len] = 0;
	offset_len ++;
}
//printf("pointing\n");
p->sym.name = name; p->sym.scope = level; p->sym.type = 0; /*假设是常数, 常数的类型为0*/
p->next = tp->buckets[h];
tp->buckets[h] = p; /* 新名字在表头 */
return &p->sym;
}

typedef struct {
  int is_const;
  int type;
  int value;
  symbol * place;
  symbol * offset; /* for array */
  char * code;
} expr;

char * insert_id(char *name);

symbol * newtemp()
{
    static int index = 1;
    char tmpname[20];
    char * p;
    symbol * s;
    sprintf(tmpname, "__t%d", index);
    index++;
    s = insert(insert_id(tmpname), table_stack[table_len-1]);
    s->offset = offset_stack[offset_len-1];
    offset_stack[offset_len-1] += 4;
    return s;
}

struct fun_list
{
    char* name;
    int type;
    struct fun_list *next;
};

struct fun_list *fun_head;

int fun_exist(char *name)
{
    struct fun_list *p=fun_head->next;
    for(;p!=NULL;p=p->next)
    {
        //printf("%s vs %s\n",name,p->name);
        if(strcmp(name,p->name)==0)
        {
            //printf("match!\n");
            return p->type;
        }
    }
    return 0;
}

void fun_insert(struct fun_list* p)
{
    //printf("%s\n",p->name);
    struct fun_list* cur=fun_head;
    for(;cur->next!=NULL;cur=cur->next);
    cur->next=p;
}

char char_tmp[1024];
%}

%token 	ID  NUM  INT  VOID  CHAR  IF  ELSE WHILE  RETURN
	CDECL  STDCALL  EQ  NE  LT  GT  LE  GE   AND  OR
                ADDOP
	STRING
%start 	program

%right 	'='
%left  	ADDOP
%left  	'*'     '/'

%union{       char * _ident;
              char _op;
              int value;
              symbol *_sym;
              expr _expr;
};

%token <_ident> NUM
%token <_ident> ID
%token <_op> ADDOP
%type <value> type_spec
%type <_sym> var
%type <_expr> factor
%type <_expr> unary_expr
%type <_expr> term
%type <_expr> add_expr
%type <_expr> simple_expr
%type <_expr> logicand_expr
%type <_expr> logic_expr
%type <_expr> expr
%type <_sym> fun_tag

%type <_sym> call

%%
program 	: M declar_list
	;

M 	:
	{
	    global_table = mktable(NULL, 0);
	    table_stack[table_len] = global_table;
	    table_len ++;
	    offset_stack[offset_len] = 0;
	    offset_len ++;
	}
	;

declar_list : declar_list declar
	 | declar
	 ;

declar	: var_declar
	| fun_declar
	;

var_declar : type_spec ID ";"
	{
	    struct table * tp = table_stack[table_len-1];
	    symbol *pp = lookup($2, table_stack[table_len-1]);
	    if(pp!=NULL)
	    {
	        yyerror("val redeclare!");
	    }
	    //printf("size of tp: %d",sizeof(*tp));
	    //printf("tp->level: %d\n",tp->level);
	    symbol * p = insert($2, tp);
	    int width = 0;
	    p->type = $1;
	    p->offset = offset_stack[offset_len-1];
	    if ($1 == INT) width = 4; else if ($1 == CHAR) width = 1;
	    offset_stack[offset_len-1] += width;
	}
	;

type_spec : INT	{$$=INT;}
	| VOID	{$$=VOID;}
	| CHAR	{$$=CHAR;}
	;

fun_declar : type_spec fun_tag '(' LL params ')' comp_stmt
	{
	    //$2->offset = offset_stack[offset_len-1];
	    if(fun_exist($2->name)!=0)
	    {
	        yyerror("function redeclared!\n");
	    }
                    //printf("fun declareing!\n");
	    //printf("name:%s\n",$2->name);
	    struct fun_list *p=(struct fun_list *)malloc(sizeof(struct fun_list));
	    if(p==NULL)
	    {
	        yyerror("malloc error\n");
	    }
	    p->type=$1;
	    p->name=$2->name;
	    p->next=NULL;
	    fun_insert(p);
	    offset_len--;
	    table_len--;
	    level--;
	}
	| type_spec fun_tag '(' LL params ')' ';'
	{
	    //$2->offset = offset_stack[offset_len-1];
	    int ttype=fun_exist($2->name);
	    if(ttype==0)
	    {
	        yyerror("function undeclared!\n");
	    }
	    //struct fun_list *p=(struct fun_list *)malloc(sizeof(struct fun_list));
	    if(ttype!=yyerror)
	    {
	         yyerror("fun declare type error!\n");
	    }
	}
	;

LL : {level++;};

fun_tag	: callconvent ID {$$->name=$2;}
	| ID	
	{
	    //printf("fun ID:%s\n",$1);
	    int i;
	    int len=strlen($1);
	    for(i=0;i<len;i++)
	    {
	        char_tmp[i]=$1[i];
	    }
	    char_tmp[i]='\0';
	    //$$->name=$1;
	    $$->name=(char *)malloc(10*sizeof(char));
	    for(i=0;i<len;i++)
	    {
	        $$->name[i]=char_tmp[i];
	    }
	    $$->name[i]='\0';
	}
	;

params	: param_list
	| VOID
	;

param_list : param_list ',' param
	| param
	;

param	: type_spec ID
	{
	    struct table * tp = table_stack[table_len-1];
	    //printf("param ID: %s\n",$2);
	    symbol *pp = lookup($2, table_stack[table_len-1]);
	    if(pp!=NULL)
	    {
	        yyerror("val redeclare!");
	    }
	    //printf("size of tp: %d",sizeof(*tp));
	    //printf("tp->level: %d\n",tp->level);
	    symbol * p = insert($2, tp);
	    int width = 0;
	    p->type = $1;
	    p->offset = offset_stack[offset_len-1];
	    if ($1 == INT) width = 4; else if ($1 == CHAR) width = 1;
	    offset_stack[offset_len-1] += width;
	}
	;

comp_stmt : '{' local_declars stmt_list '}'
	;

local_declars : local_declars var_declar
	|
	;

stmt_list	: stmt_list stmt
	|
	;

stmt	: expr_stmt
	| comp_stmt
	| if_stmt
	| while_stmt
	| return_stmt
	;

expr_stmt : expr ';'
	| ';'
	;

if_stmt	: IF '(' expr ')' stmt ELSE stmt
	| IF '(' expr ')' stmt
	;

while_stmt : WHILE '(' expr ')' stmt
	;

return_stmt : RETURN ';'
	| RETURN expr ';'
	;

expr	: var '=' expr
	{
	    if($3.type==STRING)
	    {
	        yyerror("error,=\n");
	    }
	    $$.type=$1->type;
	}
	| logic_expr	{$$.type=$1.type;}
	;

var	: ID
	{
	    //printf("searching ID\n");
	    //printf("var ID:%s\n",$1);
	    $$ = lookup($1, table_stack[table_len-1]);
	    if ($$ == NULL)
	    {
		yyerror("var undeclared!\n");
	    }
	    else
	    {
		//printf("var declared!\n");
	    }
	}
	;

logic_expr : logic_expr OR logicand_expr	{$$.type=INT;}
	| logicand_expr	{$$.type=$1.type;}
	;

logicand_expr : logicand_expr AND simple_expr	{$$.type=INT;}
	| simple_expr	{$$.type=$1.type;}
	;

simple_expr : add_expr relop add_expr
	{
	    if($1.type==STRING || $3.type==STRING)
	    {
	        yyerror("type error, relop\n");
	    }
	    $$.type=INT;
	}
	| add_expr	{$$.type=$1.type;}
	;

add_expr	: add_expr ADDOP term
	{
	    if($1.type!=INT || $3.type!=INT)
	    {
	        yyerror("type error, addop\n");
	    }
	    $$.type=INT;
	}
	| term	{$$.type=$1.type;}
	;

term	: term mulop unary_expr
	{
	    if($1.type!=INT || $3.type!=INT)
	    {
	        yyerror("type error, mulop\n");
	    }
	    $$.type=INT;
	}
	| unary_expr {$$.type=$1.type;}
	;

unary_expr : unaryop unary_expr {$$.type=$2.type;}
	| factor	{$$.type=$1.type;}
	;

factor	: '(' expr ')' {$$.type=$2.type;}
	| var	{$$.type=$1->type;}
	| call	{$$.type=$1->type;}
	| NUM	{$$.type=INT;}
	| STRING 	{$$.type=STRING;}
	;

call	: ID '(' args ')'
	{
	    //printf("call ID:%s\n",$1);
	    $$->type=fun_exist($1);
                    if($$->type==0)
	    {
	        yyerror("function undeclared!\n");
	    }
	}
	;

args	: arg_list
	|
	;

arg_list	: arg_list ',' expr
	{
	    if($3.type==STRING)
	    {
	        yyerror("fun val error\n");
	    }
	}
	| expr
	{
	    if($1.type==STRING)
	    {
	        yyerror("fun val error\n");
	    }
	}
	;

callconvent : CDECL
	| STDCALL

relop	: LE
	| LT
	| GT
	| GE
	| EQ
	| NE
	;

mulop	: '*'
	| '/'
	| '%'
	;

unaryop	: '!'
	| '-'
	;

%%
#include "lex.yy.c"
void main(int argc,char **argv)
{
    ++argv;
    --argc;
    if (argc>0)
    {
        yyin = fopen( argv[0], "r" );
    }
    else
    {
        yyin = stdin;
    }
    fun_head=(struct fun_list *)malloc(sizeof(struct fun_list));
    fun_head->next=NULL;
    yyparse();
    free(fun_head);
    printf("right!");
}

yyerror(char *s)
{
    printf("%s\n", s);
    exit(1);
}
