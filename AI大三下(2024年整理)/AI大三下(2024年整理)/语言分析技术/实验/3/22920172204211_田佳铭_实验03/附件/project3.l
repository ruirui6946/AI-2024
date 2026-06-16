%{
#include <stdio.h>
#include <string.h>

#define HASHLEN 1024
int nline = 1;

struct id_entry {
	char * id;
	struct id_entry *next; /* next entry on hash chain */
};

struct id_entry * id_hash_table[HASHLEN];

int hash(char * name) 
{
  int h = *name++;
  while (*name) h = h << 4 + *name++;
  return h;
}

char * insert_id(char *name)
{ int h = hash(name) & (HASHLEN-1);
  struct id_entry *p=id_hash_table[h];
  while (p && (strcmp(p->id, name) != 0)) p = p->next;
  if (p) return p->id;
  p = (struct id_entry *)malloc(sizeof *p);
  p->id = (char *)malloc(strlen(name)+1);
  strcpy(p->id, name);
  p->next = id_hash_table[h];
  id_hash_table[h] = p; /* 新名字在表头 */
  return p->id;
}  

%}

/*正规定义*/
delim      [ \t]
ws         {delim}+
leter      [A-Za-z]
digit      [0-9]
star      "*"
id         {leter}({leter}|{digit})*
number     {digit}+(\.{digit}+)?(E[+\-]?{digit}+)?
comment    (\#[^\n]*)|(\/\*(((\*)*[^\*\/]+(\/)*)*|(\*)*|(\/)*)\*\/)|(\/\/[^\n]*)
string   \"(.)*\"
chartype   (\'.\')|(\'\\.\')
special    [\@\~\\\$\`]
type         (void)|(int)|(char)|(long)|(short)
pointer        {type}{delim}*{star}+
pun        "("|")"|"["|"]"|"{"|"}"|";"|"="|"*"|"/"|"%"|"+"|"-"|","
rel         (<=)|(>=)|(<)|(>)|(!=)|(==)

%%
"\n"          { nline++;}
{comment}  {/*nothing to do*/}
{ws}       {/*nothing to do*/}
{number}   {yylval._ident = insert_id(yytext); return NUM;}
"__stdcall" {return STDCALL;}
"__cdecl"   {return CDECL;}
"if"   {return IF;}
"int"   {return INT;}
"void"   {return VOID;}
"char"   {return CHAR;}
"else"   {return ELSE;}
"while"   {return WHILE;}
"return"   {return RETURN;}
{id}       {yylval._ident = insert_id(yytext); return ID;}
{string} {return STRING;}
"<="       {return LE;}
">="       {return GE;}
"<"        {return LT;}
">"        {return GT;}
"!="       {return NE;}
"=="       {return EQ;}
[+-]	   {yylval._op = yytext[0]; return ADDOP;}
.          { return yytext[0]; }

%%
int yywrap() {return 1;}

