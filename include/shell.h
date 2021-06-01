#ifndef SHELL_H
#define SHELL_H
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

#ifdef DEFINE_I
int TT = 0;
pthread_t th;
#else
extern int TT;
extern pthread_t th;
#endif

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
void redirect_pipe(int oldfd, int newfd);
void execute_pipeline(char* const* args[], int index, int in_fd);
int countPipes(int argc, char **args);
char **strArr(char **args, int p);
char ***build3DArr(int argc, char **args);
void trigger_thread(char **args);
#endif
