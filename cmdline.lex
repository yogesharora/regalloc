

%{
  #include <string.h>
  #define MAX_IN 1000

  int verbose=0;
  char *outfile=NULL;
  int k;

  static int infile_cnt=0;
  char *infile[MAX_IN] = { 0 };

  void usage();
  int myinput(char *buf, int max);
#undef YY_INPUT
#define YY_INPUT(buf,result,max) (result = myinput(buf,max))
%}

%x FNAME
%x K

%%

[ \t]*     {}


-h            { usage(); }
"-?"          { usage(); }

-v          { verbose = 1; }

-o          { BEGIN(FNAME); }
-k          { BEGIN(K); }

[^ \t]+        { infile[infile_cnt++] = strdup(yytext); }

<FNAME>[^ \t\n]+ { outfile = strdup(yytext); BEGIN(0); }
<FNAME>[ \t]+    { }

<K>[0-9]+  { k = atoi(yytext); BEGIN(0); }
<K>[ \t]+       { }


%%

void usage()
{
  printf("./ra <options> file\n"
	 "Options:                          \n"
	 "          -h,-?        : print this message\n"
         "          -v,-verbose  : print copious output to indicate state\n"
	 "          -o <file>,                                           \n"
	 "          -k num       : number of machine registers to use for coloring \n");	
}

char **targv;
char **arglim;

static unsigned int offset = 0;

int myinput(char *buf, int max)
{
  int len, copylen=0;

  if (targv >= arglim)
    return 0; /* EOF */

  len = strlen(*targv)-offset;
  if(len >= max)
    copylen = max-1;
  else
    copylen = len;

  if(len>0)
    memcpy(buf, targv[0]+offset, copylen);
  
  if(targv[0][offset+copylen] == '\0') {
    buf[copylen] = ' ';
    copylen++;
    offset = 0;
    targv++;
  } 

  return copylen;
}

int cmdwrap() {
  return 1;
}

