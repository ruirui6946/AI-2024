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
typedef union{       char * _ident;
              char _op;
              int value;
              symbol *_sym;
              expr _expr;
} YYSTYPE;
extern YYSTYPE yylval;
