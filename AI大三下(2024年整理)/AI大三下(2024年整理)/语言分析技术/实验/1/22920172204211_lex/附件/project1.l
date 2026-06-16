 FILE* fp;
 //int nline=1;
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
keyword         (void)|(int)|(char)|(long)|(short)|(if)|(else)|(while)|(return)
type         (void)|(int)|(char)|(long)|(short)
pointer        {type}{delim}*{star}+
pun        "("|")"|"["|"]"|"{"|"}"|";"|"="|"*"|"/"|"%"|"+"|"-"|","
rel         (<=)|(>=)|(<)|(>)|(!=)|(==)
%%
"\n"          {/*nline++;*/}
{comment}  {/*fprintf(fp,"COMMOM %s\n",yytext);*/}
{ws}       {/*fprintf(fp,"BLANK %s\n",yytext);*/}
{number}       {fprintf(fp,"NUM %s\n",yytext);}
"__stdcall" {fprintf(fp,"STDCALL %s\n",yytext);}
"__cdecl"   {fprintf(fp,"CDECL %s\n",yytext);}
{keyword}       {fprintf(fp,"KEYWORD %s\n",yytext);}
{pointer}       {fprintf(fp,"POINTER %s\n",yytext);}
{id}       {fprintf(fp,"ID %s\n",yytext);}
{string}       {fprintf(fp,"STRING %s\n",yytext);}
{chartype}       {fprintf(fp,"CHAR_CONST %s\n",yytext);}
{rel}       {fprintf(fp,"REL_OP %s\n",yytext);}
{pun}       {fprintf(fp,"PUN %s\n",yytext);}
%%

void main(int argc,char **argv)
{
    ++argv;
    --argc;
    if (argc>0)
    {
        yyin = fopen( argv[0], "r" );
        if(argc>1)
        {
	fp=fopen(argv[1],"w");
        }
        else
        {
	fp=fopen("aout.out","w");
        }
    }
    else
    {
        yyin = stdin;
    }
    yylex();
    fclose(fp);
}

int yywrap() {return 1;}
