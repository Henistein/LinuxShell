#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define FG 1
#define BG 0
#define FILE_MODE ( S_IRUSR | S_IWUSR )

typedef struct { char msg[100] ; int tempo;} aviso_t;

typedef struct { 
  char fonte[100]; 
  char destino[100];
} copiar_t;


char prompt[100];

int parse(char *buff, char **args);
int execute(int numargs, char **args);
int buitlin_size();
int builtin_cd(char **args);
int builtin_exit(char **args);
int builtin(char **args);
int builtin_ps1(char **args);
int quemsoueu(char **args);
int obterinfo(char **args);
int socp(char **args);
int calc(char **args);
int bits(char **args);
int isjpg(char **args);
int redirects(int numargs, char *args[]);
char **string_split(char *s, char *op);
void aviso(char *mesg, int tempo);
void *avisowrapper(void *args);
void soocp(char *fonte, char *destino);
void *socpth(void *args);
void ioCopy(int IN, int OUT);
void rcv_message();
void *mesg_wrapper(void *args);
