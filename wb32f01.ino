#include "wb32f00.h"
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
int iLine=0;             // input line index
void prompt(){ ////////////////////////////////////////////////////////////////////////////////
  showDataStack();
  Serial.printf("--------------------------------------------------\n");
  Serial.printf("input%02d: ",++iLine);
}
int nErr=0;
void eval(char*token){ char *remain; //////////////////////////////////////////////////////////
  char *p=token, c; int b=base; if(remain=hexPrefix(token)) p=remain, b=16; 
  int n=strtol(p, &remain, b);
  if(c=*remain)
    Serial.printf("\nerr%02d %s ?? illigal '%c' as digit %d of a base%d number",++nErr,token,c,remain-token,base);
  else dsPush(n);
}
void interpret(char*src){ // handle input /////////////////////////////////////////////////////
  parseOpen(src); char*token;
  while(parseAvailable && (token=parseWord())) eval(token);
  Serial.println();
}
void setup() { ///////////////////////////////////////////////////////////////////////////////
  Serial.begin(115200);
  Serial.printf("==================================================\n");
  Serial.printf("Simple Console Parsing Input For Numbers Of Base%d\n",base);
  Serial.printf("20170428 derek@wifiboy.org & samsuanchen@gmail.com\n");
  Serial.printf("==================================================\n");
  tibOpen(); // wait for input
}
char c;
char getChar(char*p){
  while(Serial.available()&&(c=Serial.read()))
  if(Serial.available()){
    char c = Serial.read();   // get input char
    if (baskSpace(c)) { // if backspace ------------------------
      if (! tibEmpty) tibPop(),Serial.printf("\b \b");  //    erase last input char
    }
    && (c=Serial.read()) && c!='\r' && c!='\n' && p<limit
  }
}
char*aquire(char*tib,int size){
  char c, *p=tib, *limit=p+size-1;
  while(Serial.available() && (c=Serial.read()) && c!='\b' && p<limit && c!='\r' && c!='\n' ) {
    if(c=='b){
      if(p>tib)p--, Serial.printf("\b \b");
      
    } else *p++=c; *p='\0';
  }
  if(p==limit) Serial.printf("??? aquire buffer overflow ???\n lib=\"%s\"\nsize=%d\n",tib,size);
  return strlen(tib);
}
#define tibSize 1024
char tib[tibSize];
void loop() { ////////////////////////////////////////////////////////////////////////////////
  if(aquire(tib,tibSize)) interpret(tib);
  if (Serial.available()){ char c=Serial.read(); // get input char c
    if (baskSpace(c)) {                          // if c backspace ------------------------
      if (! tibEmpty) tibPop(),Serial.printf("\b \b"); //  pop and erase last input char
    } else if (!EOL(c) && !tibFull) {            // if c not '\0', '\r', nor '\n' and tib not full
      tibPush(c), Serial.write(c);                     // collect and echo c
    } else { // if end of line or buffer full -------------------------------------------------------
      if (tibFull) Serial.printf("\n??? terminal input buffer full ???\n");
      tibClose(), interpret(tib), tibOpen();       //    interpret and wait for next input
    }
  }
}
