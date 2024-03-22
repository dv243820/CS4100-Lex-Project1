
%{
//*****************************************************************************************
// File: inforScan.l
// Purpose: SORT THROUGH STUDENTS C CODE FOR PLAGARISM BY TOKENIZING THEIR CODE THAT IS OFTEN 
// CHANGED TO HIDE THEIR PLAGARISM
// build instructions
// flex infoScanner.l
// g++ -o infoScanner lex.yy.x
// ./infoScanner < data_file.txt
// ****************************************************************************************

//declarations

#include <iostream>
#include <fstream>

using namespace std;

#define TKVARNAME 1 // TO TOKENIZE PLAGARIZED NAMES THAT WERE CHANGED
#define TKFUNCTION 3 //
#define TKTYPE 4
#define TKVAL 5
#define TKCOMMENT 6
#define TKINCLUDE 7
#define TKLOOP 8
#define TKIF 9
#define TKELSE 10
#define TKIGNORE 255


int linecount = 1;

void Addline(char c)
{
  if(c=='\n')
  {
    linecount++;
  }
}

int line_num(){
  return linecount;
}




%}

D [0-9] //digit
A [a-zA-Z] //english alphabet
T int|char|float|double|void|bool|string 


%%

if[ \n]*[(]             {return TKIF;}

else[ ]if[ ]*[(]           {return TKELSE;}

[a-zA-Z]+[ \n][a-zA-Z_]+[(]      {  return TKFUNCTION;} //function empty declarations

[a-zA-Z_]+[ \n]*[(]                   {return TKFUNCTION; } //function call

[while|for|do]+[ \n][(]             { return TKLOOP;  }

{T}[ \t\n\f]                {    return TKTYPE;  } 

[0-9][;]            {    return TKVAL;  }

["|']([^"\n]|\\(.|\n))*["|']    { return TKVAL; }

_[{A}{D}]                   {    return TKVARNAME;  }

[a-zA-Z0-9]+[ \n]*         {    return TKVARNAME;  }

[{D}].[{D}]          {    return TKVAL;  }  

"//"(.*)           {    return TKCOMMENT;  }

"/*"([^*]|\*+[^*/])*\*+"/"     {    return TKCOMMENT;  }

[ \t\n\f]					{    Addline(yytext[0]);   } 

"#"(.*)           { return TKINCLUDE; }

.							{ 	//unmatched
                return TKIGNORE;
              }

%%

int yywrap(){
  return true;
}

int main() {

  ofstream outFile;
  outFile.open("scanner_out.txt");
  int token = 0;
  token = yylex();
  while(token){
    switch (token) {
      case TKIGNORE:
        break;
      case TKVARNAME:
        outFile << "aa" ;
        break;
      case TKTYPE:
        outFile << "ac" ;
        break;
      case TKFUNCTION:
        outFile << "ad";
        break;
      case TKVAL:
        outFile << "ae";
        break;
      case TKLOOP:
        outFile << "af";
        break;
      case TKIF:
        outFile << "ag";
        break;
      case TKELSE:
        outFile << "ah";
        break;
}
    token = yylex();
  }
outFile.close();
}	