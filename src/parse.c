#include <shell.h>

int k = 0;
char **string_split(char *s, char *op){
  k=0;
  char **splited;
  splited = malloc(sizeof(char*));
  char *token = strtok(s, op);

  while(token != NULL){
    splited[k] = malloc(sizeof(token)*sizeof(char));
    strcpy(splited[k], token);
    token = strtok(NULL, op);
    k++;
    splited = realloc(splited, (k+1)*sizeof(char*));
  }

  return splited;
}

int parse(char *buff, char **args){
  int cnt=0;
  while(*buff != '\0'){
    while (isspace ((unsigned char) *buff)){
      *buff++ = '\0';
    }
    *args++ = buff;
    cnt++;
    while((*buff != '\0') && (!isspace ((unsigned char) *buff))){
      buff++;
    }
  }
  *args = NULL;
  return cnt;
}
