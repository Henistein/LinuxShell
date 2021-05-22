#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include <shell.h>

int main(){
  int length;
  strcpy(prompt, "prompt>");

  char buff[1024];
  char *args[64];

  while(1){

    printf("%s", prompt); 
    if(fgets(buff, sizeof(buff), stdin) == NULL){
      exit(0);
    }
    length = strlen(buff);

    if(length == 1){ continue; }

    if(buff[length-1] == '\n'){
      buff[length-1]  = '\0';
    }
    int numargs = parse(buff, args);

    if(!builtin(args)){
      execute(numargs, args); 
    }
  }
    
  return 0; 
}
