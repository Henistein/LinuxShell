#include <shell.h>
void redirect_pipe(int oldfd, int newfd){
  if(oldfd != newfd){
    if(dup2(oldfd, newfd) != -1){
      close(oldfd);
    }
    else{
      perror("dup2");
    }
  }
}

void execute_pipeline(char* const* args[], int index, int in_fd){
  if(args[index+1] == NULL) { //Inductive case, last command
    redirect_pipe(in_fd, STDIN_FILENO); //read from in, write to STDOUT
    execvp(args[index][0], args[index]);
    perror("execvp");
  }
  else{
    int fd[2];
    if(pipe(fd) == -1){
      perror("pipe"); 
    }
    switch(fork()){
      case -1:
        perror("fork");
      case 0: 
        close(fd[0]);
        redirect_pipe(in_fd, STDIN_FILENO); //read from in_fd
        redirect_pipe(fd[1], STDOUT_FILENO); //write to fd[1]
        execvp(args[index][0], args[index]);
        perror("execvp");
      default :
        close(fd[1]);
        close(in_fd);
        execute_pipeline(args, index + 1, fd[0]);
    }
  }
}
int countPipes(int argc, char **args){
  int count = 0;
  for(int i=0; i<argc; i++){
    if(args[i][0] == '|'){
      count++;
    }
  }
  return count;
}

char **strArr(char **args, int p){
  int count = 0; 
  int tam = 0;
  char **arr = malloc(tam * sizeof(char*));

  for(int i=0; args[i]; i++){
    if(count == p && args[i][0] != '|'){
      tam++;
      arr = realloc(arr, tam * sizeof(*arr));
      arr[tam-1] = strdup(args[i]);
    }
    if(args[i][0] == '|'){
      count++;
    }
    if(count > p){
      break;
    }
  }
  //Set last command to null
  tam++;
  arr = realloc(arr, tam * sizeof(*arr));
  arr[tam-1] = NULL;
  
  return arr;
}

char ***build3DArr(int argc, char **args){
  int n = countPipes(argc, args) + 1;
  char ***arr = malloc((n+1) * sizeof(char**));
  for(int i=0; i<n; i++){
    arr[i] = strArr(args, i);
  }

  //Set last command to null
  arr[n] = NULL;
  return arr;
}

