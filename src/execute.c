#include "shell.h"

int ultimo ( int *numargs, char **args )
{
 if ( args[*numargs-1][0]=='&') {
 *numargs=*numargs-1;
 args[*numargs]=NULL ;
 return BG;
 }
 return FG; /*return FG ou BG definidos no shell.h */
}

int containsPipe(int numArgs,char **args){
  for(int index=0;index<numArgs;index++){
    if(args[index][0]=='|'){
      return index;
    }
  }
  return -1;
}

int redirects(int numargs, char *args[]){
  if(numargs<3){
    return numargs;
  }
  if(strcmp(args[numargs -2],"2>")==0){
    int fd=creat(args[numargs-1],S_IRUSR|S_IWUSR);
    if(fd<0){
      perror(NULL); 
      return -1;
    }
    dup2(fd,STDERR_FILENO);
    close(fd);
    args[numargs-2]=NULL;
    numargs=numargs-2;
  }
  else if(strcmp(args[numargs -2],"2>>")==0){
    int fd=open(args[numargs-1],O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
    if(fd<0){
      perror(NULL); 
      return -1;
    }
    dup2(fd,STDERR_FILENO);
    close(fd);
    args[numargs-2]=NULL;
    numargs=numargs-2;
  }
  if(numargs<3){
    return numargs;
  }
  if(strcmp(args[numargs -2],">")==0){
    int fd=creat(args[numargs-1],S_IRUSR|S_IWUSR);
    if(fd<0){
      perror(NULL); 
      return -1;
    }
    dup2(fd,STDOUT_FILENO);
    close(fd);
    args[numargs-2]=NULL;
    numargs=numargs-2;
  }
  else if(strcmp(args[numargs -2],">>")==0){
    int fd=open(args[numargs-1],O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
    if(fd<0){
      perror(NULL); 
      return -1;
    }
    dup2(fd,STDOUT_FILENO);
    close(fd);
    args[numargs-2]=NULL;
    numargs=numargs-2;
  }
  if(numargs<3){
    return numargs;
  }
  if(strcmp(args[numargs-2],"<")==0){
    int fd=open(args[numargs-1],O_RDONLY,S_IRUSR|S_IWUSR);
    if(fd<0){
      perror(NULL); 
      return -1;
    }
    dup2(fd,STDIN_FILENO);
    close(fd);
    args[numargs-2]=NULL;
    numargs=numargs-2;
  }
  return numargs;
}

int execute (int numargs, char **args){
  int indicePipe, pidFilho, fd[2];
  int pid, status;
  int code = ultimo(&numargs, args);
  if ((pid = fork()) < 0)
    { /* cria um processo progenito */
      perror ("forks");/* NOTE: perror() produz uma pequema mensagem de erro para o stream */
      exit (1);/* estandardizado de erros que descreve o ultimo erro encontrado */
               /* durante uma chamada ao sistema ou funcao duma biblioteca */
    }

  if (pid == 0){
    indicePipe=containsPipe(numargs, args);
    if(indicePipe > 0 ){
      //args[indicePipe] = NULL;
      //pipe (fd);    //criar pipe
      //pidFilho = fork(); //fork

			char ***args2 = build3DArr(numargs, args);
      execute_pipeline((char* const**)args2, 0, STDIN_FILENO);
     /* 
      if(pidFilho ==0){
        numargs=indicePipe;
        dup2(fd[1],STDOUT_FILENO);
        close (fd[0]);
        close (fd[1]); //fd[0] -Input(leitura) fd[1]-output(escrita)
      }else{
        args = args+ indicePipe+1;
        numargs = numargs - indicePipe - 1;

        dup2(fd[0], STDIN_FILENO);  //duplicar o descritor de ficheiro de leitura do PIPE para a posição na tabele de FD do STDIN
        close(fd[0]); //fechar o descritor do ficheiro do pipe que este processo não necessita.
        close(fd[1]);
      }
      */
    }

    numargs = redirects(numargs, args);
    if(numargs ==-1){
      exit(0);//fecha o filho
    }
    execvp (*args, args);/* NOTE: as versoes execv() e
                            * execvp() de execl() sao uteis quando */
    perror (*args);      /* o numero de argumentos nao e. conhecido.
                              * Os argumentos de  */
    exit (1);           /* execv() e execvp() sao o nome do ficheiro
                           * a ser executado e um */
  }             
  if ( FG == code ){
    while (wait (&status) != pid)
    /*spin fazer nada */ ;

  }
              //ls & -bash executa antes do ls & (Quando não aparece o ubuntu> após clicar enter)
  
  return 1;
}
