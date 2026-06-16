#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.8 (Berkeley) 01/02/91\n\
 Modified 11/4/96 by Wensong Zhang to make getenv() function right\n\
 and to remove the warning of unreferenced yyerrlab and yynewerror labels";
#endif
#define YYBYACC 1
#line 2 "test20.txt"
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
/*printf("start do\n");*/
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
/*printf("insert start\n");*/
unsigned h = ((unsigned)name)&(HASHSIZE-1);
struct table *tp = tpp;
struct sym_entry *p = (struct entry *)malloc(sizeof *p);
if(p==NULL)
{
yyerror("malloc error\n");
}
/*printf("malloc successful\n");*/
/*printf("tp->level =%d",tp->level);*/
/*printf("level =%d",level);*/
/*printf("tp->level =%d level=%d  inserting %s\n", tp->level,level,name);*/
/*printf("start scope level\n");*/
int tmp=0;
if (tp->level < level) { /* 当前scope level*/
                /*tmp++;*/
                /*printf("scope: %d",tmp);*/
	tp = mktable(tp, level);
	table_stack[table_len] = tp;
	table_len ++;
	offset_stack[offset_len] = 0;
	offset_len ++;
}
/*printf("pointing\n");*/
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
        /*printf("%s vs %s\n",name,p->name);*/
        if(strcmp(name,p->name)==0)
        {
            /*printf("match!\n");*/
            return p->type;
        }
    }
    return 0;
}

void fun_insert(struct fun_list* p)
{
    /*printf("%s\n",p->name);*/
    struct fun_list* cur=fun_head;
    for(;cur->next!=NULL;cur=cur->next);
    cur->next=p;
}

char char_tmp[1024];
#line 182 "test20.txt"
typedef union{       char * _ident;
              char _op;
              int value;
              symbol *_sym;
              expr _expr;
} YYSTYPE;
#line 184 "y.tab.c"
#define ID 257
#define NUM 258
#define INT 259
#define VOID 260
#define CHAR 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define RETURN 265
#define CDECL 266
#define STDCALL 267
#define EQ 268
#define NE 269
#define LT 270
#define GT 271
#define LE 272
#define GE 273
#define AND 274
#define OR 275
#define ADDOP 276
#define STRING 277
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,   13,   14,   14,   15,   15,   16,    1,    1,    1,
   17,   17,   18,   11,   11,   19,   19,   22,   22,   23,
   20,   24,   24,   25,   25,   26,   26,   26,   26,   26,
   27,   27,   28,   28,   29,   30,   30,   10,   10,    2,
    9,    9,    8,    8,    7,    7,    6,    6,    5,    5,
    4,    4,    3,    3,    3,    3,    3,   12,   34,   34,
   35,   35,   21,   21,   31,   31,   31,   31,   31,   31,
   32,   32,   32,   33,   33,
};
short yylen[] = {                                         2,
    2,    0,    2,    1,    1,    1,    3,    1,    1,    1,
    7,    7,    0,    2,    1,    1,    1,    3,    1,    2,
    4,    2,    0,    2,    0,    1,    1,    1,    1,    1,
    2,    1,    7,    5,    5,    2,    3,    3,    1,    1,
    3,    1,    3,    1,    3,    1,    3,    1,    3,    1,
    2,    1,    3,    1,    1,    1,    1,    4,    1,    0,
    3,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,
};
short yydefred[] = {                                      2,
    0,    0,    8,    9,   10,    0,    0,    4,    5,    6,
    0,   63,   64,    0,    0,    3,    7,   13,   14,    0,
    0,    0,    0,    0,   19,   20,    0,    0,   12,   23,
   11,   18,    0,    0,   22,    0,    0,    0,   56,    0,
    0,    0,   57,   32,    0,   21,   74,   75,    0,   52,
   50,    0,    0,   44,    0,    0,    0,   55,   27,   24,
   26,   28,   29,   30,    0,    0,    0,    0,   36,    0,
    0,    0,   71,   72,   73,    0,   69,   70,   66,   67,
   65,   68,    0,    0,    0,    0,   31,   54,   51,   62,
    0,    0,    0,    0,   37,   53,   38,   49,    0,    0,
   43,    0,   58,    0,    0,    0,   61,    0,   35,    0,
   33,
};
short yydgoto[] = {                                       1,
    6,   49,   50,   51,   52,   53,   54,   55,   56,   57,
   14,   58,    2,    7,    8,    9,   10,   20,   23,   59,
   15,   24,   25,   33,   36,   60,   61,   62,   63,   64,
   84,   76,   65,   91,   92,
};
short yysindex[] = {                                      0,
    0, -163,    0,    0,    0, -192, -163,    0,    0,    0,
  -56,    0,    0,  -34, -240,    0,    0,    0,    0, -145,
    0, -238,  -19,  -21,    0,    0,  -51, -163,    0,    0,
    0,    0, -163, -226,    0,  -33,  -56,   -7,    0,    6,
    9,    3,    0,    0,    7,    0,    0,    0,   -4,    0,
    0,   46, -144,    0, -208, -206,   12,    0,    0,    0,
    0,    0,    0,    0,    7,    7,    7,    7,    0,   20,
   41,    7,    0,    0,    0,    7,    0,    0,    0,    0,
    0,    0,    7,    7,    7,    7,    0,    0,    0,    0,
   50,   43,   53,   59,    0,    0,    0,    0,   46, -173,
    0, -208,    0,    7,   -6,   -6,    0, -154,    0,   -6,
    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,  112,    0,    0,    0,
   78,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -40,    0,    0,   89,    0,    0,    0,    0,    0,    0,
    0,    0,  -24,    0,    0,    0,    0,   17,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   26,    0,
    0,   36,  -39,    0,  -30,  -31,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   90,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   92,    0,    0,    0,    0,    0,    0,   45,    1,
    0,   -3,    0,    0,    0,    0,    0,  -15,    0,    0,
    0,
};
short yygindex[] = {                                      0,
    4,   37,    0,  -61,   51,   35,   52,   49,    0,   39,
    0,    0,    0,    0,  129,  105,    0,    0,    0,  113,
    0,    0,  111,    0,    0,  -55,    0,    0,    0,    0,
    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 321
short yytable[] = {                                      47,
   17,   46,   17,   89,   46,   18,   45,   29,   25,   39,
   42,   48,   39,   42,   98,   25,   19,   34,   26,   46,
   25,   27,   28,   22,   34,   44,   47,   39,   42,   34,
   37,   22,   66,   45,   25,   47,   34,   41,   48,   47,
   41,   45,   45,   34,   45,   67,   45,   48,   68,  108,
  109,   48,   44,   40,  111,   41,   72,   40,   40,   45,
   40,   69,   54,   40,   11,   85,   54,   54,   86,   54,
   87,   30,   54,   12,   13,   40,   48,   40,   95,   48,
   70,   96,   75,   71,   54,   47,  104,   73,   47,   30,
  103,   46,   74,  105,   48,    3,    4,    5,   25,  106,
   25,   88,   83,   47,   90,   93,   94,   34,  110,   34,
   97,    1,   88,    3,   21,    5,   30,   15,  100,   88,
   88,   88,   88,   77,   78,   79,   80,   81,   82,   16,
   60,   83,   59,   99,  102,   16,  101,   35,   32,   31,
    0,    0,  107,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    9,    0,    0,    0,
    0,    0,    0,   38,   39,    0,    0,    0,   40,    0,
   41,   42,   25,   25,   46,   46,    0,   25,    0,   25,
   25,   34,   34,   43,   42,    0,   34,    0,   34,   34,
   38,   39,   25,    0,    0,   40,    0,   41,   42,   38,
   39,   34,    0,   38,   39,    0,    0,    0,    0,    0,
   43,   41,    0,    0,   45,   45,    0,    0,    0,   43,
    0,    0,    0,   43,   40,   40,   40,   40,   40,   40,
   40,   40,   40,   54,   54,   54,   54,   54,   54,   54,
   54,   54,    0,   48,   48,   48,   48,   48,   48,   48,
   48,   48,   47,   47,   47,   47,   47,   47,   47,   47,
   47,
};
short yycheck[] = {                                      33,
   41,   41,   59,   65,   44,   40,   40,   59,   33,   41,
   41,   45,   44,   44,   76,   40,  257,   33,  257,   59,
   45,   41,   44,   20,   40,   59,   33,   59,   59,   45,
  257,   28,   40,   40,   59,   33,   33,   41,   45,   33,
   44,   41,   40,   59,   44,   40,   40,   45,   40,  105,
  106,   45,   59,   37,  110,   59,   61,   41,   42,   59,
   44,   59,   37,   47,  257,  274,   41,   42,  275,   44,
   59,  123,   47,  266,  267,   59,   41,   61,   59,   44,
   42,   41,   37,   45,   59,   41,   44,   42,   44,  123,
   41,  125,   47,   41,   59,  259,  260,  261,  123,   41,
  125,   65,  276,   59,   66,   67,   68,  123,  263,  125,
   72,    0,   76,  259,  260,  261,  123,   40,   84,   83,
   84,   85,   86,  268,  269,  270,  271,  272,  273,   41,
   41,  276,   41,   83,   86,    7,   85,   33,   28,   27,
   -1,   -1,  104,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,   -1,   -1,
   -1,   -1,   -1,  257,  258,   -1,   -1,   -1,  262,   -1,
  264,  265,  257,  258,  274,  275,   -1,  262,   -1,  264,
  265,  257,  258,  277,  275,   -1,  262,   -1,  264,  265,
  257,  258,  277,   -1,   -1,  262,   -1,  264,  265,  257,
  258,  277,   -1,  257,  258,   -1,   -1,   -1,   -1,   -1,
  277,  275,   -1,   -1,  274,  275,   -1,   -1,   -1,  277,
   -1,   -1,   -1,  277,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  268,  269,  270,  271,  272,  273,  274,
  275,  276,   -1,  268,  269,  270,  271,  272,  273,  274,
  275,  276,  268,  269,  270,  271,  272,  273,  274,  275,
  276,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 277
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'",0,0,"'('","')'","'*'",0,"','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,
0,0,"';'",0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,"ID","NUM","INT","VOID","CHAR","IF","ELSE","WHILE","RETURN",
"CDECL","STDCALL","EQ","NE","LT","GT","LE","GE","AND","OR","ADDOP","STRING",
};
char *yyrule[] = {
"$accept : program",
"program : M declar_list",
"M :",
"declar_list : declar_list declar",
"declar_list : declar",
"declar : var_declar",
"declar : fun_declar",
"var_declar : type_spec ID ';'",
"type_spec : INT",
"type_spec : VOID",
"type_spec : CHAR",
"fun_declar : type_spec fun_tag '(' LL params ')' comp_stmt",
"fun_declar : type_spec fun_tag '(' LL params ')' ';'",
"LL :",
"fun_tag : callconvent ID",
"fun_tag : ID",
"params : param_list",
"params : VOID",
"param_list : param_list ',' param",
"param_list : param",
"param : type_spec ID",
"comp_stmt : '{' local_declars stmt_list '}'",
"local_declars : local_declars var_declar",
"local_declars :",
"stmt_list : stmt_list stmt",
"stmt_list :",
"stmt : expr_stmt",
"stmt : comp_stmt",
"stmt : if_stmt",
"stmt : while_stmt",
"stmt : return_stmt",
"expr_stmt : expr ';'",
"expr_stmt : ';'",
"if_stmt : IF '(' expr ')' stmt ELSE stmt",
"if_stmt : IF '(' expr ')' stmt",
"while_stmt : WHILE '(' expr ')' stmt",
"return_stmt : RETURN ';'",
"return_stmt : RETURN expr ';'",
"expr : var '=' expr",
"expr : logic_expr",
"var : ID",
"logic_expr : logic_expr OR logicand_expr",
"logic_expr : logicand_expr",
"logicand_expr : logicand_expr AND simple_expr",
"logicand_expr : simple_expr",
"simple_expr : add_expr relop add_expr",
"simple_expr : add_expr",
"add_expr : add_expr ADDOP term",
"add_expr : term",
"term : term mulop unary_expr",
"term : unary_expr",
"unary_expr : unaryop unary_expr",
"unary_expr : factor",
"factor : '(' expr ')'",
"factor : var",
"factor : call",
"factor : NUM",
"factor : STRING",
"call : ID '(' args ')'",
"args : arg_list",
"args :",
"arg_list : arg_list ',' expr",
"arg_list : expr",
"callconvent : CDECL",
"callconvent : STDCALL",
"relop : LE",
"relop : LT",
"relop : GT",
"relop : GE",
"relop : EQ",
"relop : NE",
"mulop : '*'",
"mulop : '/'",
"mulop : '%'",
"unaryop : '!'",
"unaryop : '-'",
};
#endif
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#ifdef YYSTACKSIZE
#ifndef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#endif
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 600
#define YYMAXDEPTH 600
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 505 "test20.txt"
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
#line 499 "y.tab.c"
#define YYABORT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("yydebug: state %d, reading %d (%s)\n", yystate,
                    yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("yydebug: state %d, shifting to state %d\n",
                    yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
yynewerror:
#endif
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
yyerrlab:
#endif
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("yydebug: state %d, error recovery shifting\
 to state %d\n", *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("yydebug: error recovery discarding state %d\n",
                            *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("yydebug: state %d, error recovery discards token %d (%s)\n",
                    yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("yydebug: state %d, reducing by rule %d (%s)\n",
                yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 2:
#line 211 "test20.txt"
{
	    global_table = mktable(NULL, 0);
	    table_stack[table_len] = global_table;
	    table_len ++;
	    offset_stack[offset_len] = 0;
	    offset_len ++;
	}
break;
case 7:
#line 229 "test20.txt"
{
	    struct table * tp = table_stack[table_len-1];
	    symbol *pp = lookup(yyvsp[-1]._ident, table_stack[table_len-1]);
	    if(pp!=NULL)
	    {
	        yyerror("val redeclare!");
	    }
	    /*printf("size of tp: %d",sizeof(*tp));*/
	    /*printf("tp->level: %d\n",tp->level);*/
	    symbol * p = insert(yyvsp[-1]._ident, tp);
	    int width = 0;
	    p->type = yyvsp[-2].value;
	    p->offset = offset_stack[offset_len-1];
	    if (yyvsp[-2].value == INT) width = 4; else if (yyvsp[-2].value == CHAR) width = 1;
	    offset_stack[offset_len-1] += width;
	}
break;
case 8:
#line 247 "test20.txt"
{yyval.value=INT;}
break;
case 9:
#line 248 "test20.txt"
{yyval.value=VOID;}
break;
case 10:
#line 249 "test20.txt"
{yyval.value=CHAR;}
break;
case 11:
#line 253 "test20.txt"
{
	    /*$2->offset = offset_stack[offset_len-1];*/
	    if(fun_exist(yyvsp[-5]._sym->name)!=0)
	    {
	        yyerror("function redeclared!\n");
	    }
                    /*printf("fun declareing!\n");*/
	    /*printf("name:%s\n",$2->name);*/
	    struct fun_list *p=(struct fun_list *)malloc(sizeof(struct fun_list));
	    if(p==NULL)
	    {
	        yyerror("malloc error\n");
	    }
	    p->type=yyvsp[-6].value;
	    p->name=yyvsp[-5]._sym->name;
	    p->next=NULL;
	    fun_insert(p);
	    offset_len--;
	    table_len--;
	    level--;
	}
break;
case 12:
#line 275 "test20.txt"
{
	    /*$2->offset = offset_stack[offset_len-1];*/
	    int ttype=fun_exist(yyvsp[-5]._sym->name);
	    if(ttype==0)
	    {
	        yyerror("function undeclared!\n");
	    }
	    /*struct fun_list *p=(struct fun_list *)malloc(sizeof(struct fun_list));*/
	    if(ttype!=yyerror)
	    {
	         yyerror("fun declare type error!\n");
	    }
	}
break;
case 13:
#line 290 "test20.txt"
{level++;}
break;
case 14:
#line 292 "test20.txt"
{yyval._sym->name=yyvsp[0]._ident;}
break;
case 15:
#line 294 "test20.txt"
{
	    /*printf("fun ID:%s\n",$1);*/
	    int i;
	    int len=strlen(yyvsp[0]._ident);
	    for(i=0;i<len;i++)
	    {
	        char_tmp[i]=yyvsp[0]._ident[i];
	    }
	    char_tmp[i]='\0';
	    /*$$->name=$1;*/
	    yyval._sym->name=(char *)malloc(10*sizeof(char));
	    for(i=0;i<len;i++)
	    {
	        yyval._sym->name[i]=char_tmp[i];
	    }
	    yyval._sym->name[i]='\0';
	}
break;
case 20:
#line 322 "test20.txt"
{
	    struct table * tp = table_stack[table_len-1];
	    /*printf("param ID: %s\n",$2);*/
	    symbol *pp = lookup(yyvsp[0]._ident, table_stack[table_len-1]);
	    if(pp!=NULL)
	    {
	        yyerror("val redeclare!");
	    }
	    /*printf("size of tp: %d",sizeof(*tp));*/
	    /*printf("tp->level: %d\n",tp->level);*/
	    symbol * p = insert(yyvsp[0]._ident, tp);
	    int width = 0;
	    p->type = yyvsp[-1].value;
	    p->offset = offset_stack[offset_len-1];
	    if (yyvsp[-1].value == INT) width = 4; else if (yyvsp[-1].value == CHAR) width = 1;
	    offset_stack[offset_len-1] += width;
	}
break;
case 38:
#line 375 "test20.txt"
{
	    if(yyvsp[0]._expr.type==STRING)
	    {
	        yyerror("error,=\n");
	    }
	    yyval._expr.type=yyvsp[-2]._sym->type;
	}
break;
case 39:
#line 382 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 40:
#line 386 "test20.txt"
{
	    /*printf("searching ID\n");*/
	    /*printf("var ID:%s\n",$1);*/
	    yyval._sym = lookup(yyvsp[0]._ident, table_stack[table_len-1]);
	    if (yyval._sym == NULL)
	    {
		yyerror("var undeclared!\n");
	    }
	    else
	    {
		/*printf("var declared!\n");*/
	    }
	}
break;
case 41:
#line 401 "test20.txt"
{yyval._expr.type=INT;}
break;
case 42:
#line 402 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 43:
#line 405 "test20.txt"
{yyval._expr.type=INT;}
break;
case 44:
#line 406 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 45:
#line 410 "test20.txt"
{
	    if(yyvsp[-2]._expr.type==STRING || yyvsp[0]._expr.type==STRING)
	    {
	        yyerror("type error, relop\n");
	    }
	    yyval._expr.type=INT;
	}
break;
case 46:
#line 417 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 47:
#line 421 "test20.txt"
{
	    if(yyvsp[-2]._expr.type!=INT || yyvsp[0]._expr.type!=INT)
	    {
	        yyerror("type error, addop\n");
	    }
	    yyval._expr.type=INT;
	}
break;
case 48:
#line 428 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 49:
#line 432 "test20.txt"
{
	    if(yyvsp[-2]._expr.type!=INT || yyvsp[0]._expr.type!=INT)
	    {
	        yyerror("type error, mulop\n");
	    }
	    yyval._expr.type=INT;
	}
break;
case 50:
#line 439 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 51:
#line 442 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 52:
#line 443 "test20.txt"
{yyval._expr.type=yyvsp[0]._expr.type;}
break;
case 53:
#line 446 "test20.txt"
{yyval._expr.type=yyvsp[-1]._expr.type;}
break;
case 54:
#line 447 "test20.txt"
{yyval._expr.type=yyvsp[0]._sym->type;}
break;
case 55:
#line 448 "test20.txt"
{yyval._expr.type=yyvsp[0]._sym->type;}
break;
case 56:
#line 449 "test20.txt"
{yyval._expr.type=INT;}
break;
case 57:
#line 450 "test20.txt"
{yyval._expr.type=STRING;}
break;
case 58:
#line 454 "test20.txt"
{
	    /*printf("call ID:%s\n",$1);*/
	    yyval._sym->type=fun_exist(yyvsp[-3]._ident);
                    if(yyval._sym->type==0)
	    {
	        yyerror("function undeclared!\n");
	    }
	}
break;
case 61:
#line 469 "test20.txt"
{
	    if(yyvsp[0]._expr.type==STRING)
	    {
	        yyerror("fun val error\n");
	    }
	}
break;
case 62:
#line 476 "test20.txt"
{
	    if(yyvsp[0]._expr.type==STRING)
	    {
	        yyerror("fun val error\n");
	    }
	}
break;
#line 912 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("yydebug: after reduction, shifting from state 0 to\
 state %d\n", YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("yydebug: state %d, reading %d (%s)\n",
                        YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("yydebug: after reduction, shifting from state %d \
to state %d\n", *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
