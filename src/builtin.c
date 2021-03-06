#include <shell.h>

char *builtin_str[] = {
  "cd", 
  "exit", 
  "quemsoueu",
  "obterinfo", 
  "socp", 
  "calc",
  "bits",
  "isjpg"
};

int (*builtin_func[])(char **) = {
  &builtin_cd,
  &builtin_exit, 
  &quemsoueu, 
  &obterinfo, 
  &socp,
  &calc,
  &bits,
  &isjpg
};

int builtin_size(){
  return sizeof(builtin_str) / sizeof(char *);
}

int socp(char **args){
  int src = open(args[1], O_RDONLY);
  int dest = open(args[2], O_CREAT | O_WRONLY, 0666);
  if(src == -1 && dest == -1){
    perror("socp"); 
  }
  int n;
  char buff[4096];
  while((n = read(src, buff, 4096)) > 0){
    if(write(dest, buff, n) != n){ perror("socp");}
  }
  if(n < 0) { perror("socp"); }
  return 1;
}

int obterinfo(char **args){
  printf("Henistein SO Shell 2021 version 1.0\n");  
  return 1;
}

int quemsoueu(char **args){
  system("id");
  return 1;
}

int builtin_ps1(char **args){
  char *s = string_split(args[0], "=")[1];
  if(s != NULL){
    strcpy(prompt, s); 
  } else{
    perror("PS1");
  }
  return 1;
}

int builtin_cd(char **args){
  if(args[1] == NULL){
    chdir(getenv("HOME"));
  } else{
    if(chdir(args[1]) != 0){
      perror("cd");
    }
  }
  return 1;
}

int builtin_exit(char **args){
  exit(0);
  return 0;
}

int calc(char **args){
  char operator = args[2][0];
  switch(operator){
    // Operators
    case '+' : printf("%d\n", atoi(args[1]) + atoi(args[3]));
    break;
    case '-' : printf("%d\n", atoi(args[1]) - atoi(args[3]));
    break;
    case '*' : printf("%d\n", atoi(args[1]) * atoi(args[3]));
    break;
    case '/' : printf("%d\n", atoi(args[1]) / atoi(args[3]));
    break;
    default : printf("Bad opperand\n");
  }
  return 1;
}

int bits(char **args){
  char operator = args[2][0];
  int resultado;
  //AND
    if(operator == '&'){ 
        //Atoi transforma o string em int
        resultado = atoi(args[1]) & atoi(args[3]);
        printf("%d\n",resultado);
        return 1;
    }
    //Inclusive OR
    if(operator == '|'){         
        resultado = atoi(args[1]) | atoi(args[3]);
        printf("%d\n",resultado);
        return 1;
    }
    //Exclusive OR
    if(operator == '^'){         
        resultado = atoi(args[1]) ^ atoi(args[3]);
        printf("%d\n",resultado);
        return 1;
    }
    //Negation
    if((args[1][0]) == '~'){         
        resultado = ~atoi(args[2]);
        printf("%d\n",resultado);
        return 1;
    }
    //Shift right
    if(strcmp(args[2], ">>")==0){ 
        resultado = atoi(args[1])>>atoi(args[3]);
        printf("%d\n",resultado);
        return 1;
    }
    //Shift left
    if(strcmp(args[2], "<<")==0){ 
        resultado = atoi(args[1])<<atoi(args[3]);
        printf("%d\n",resultado);
        return 1;
    }
    else
      printf("Valores inv??lidos!\n");
  return 1;
}

int isjpg(char **args){
  unsigned char b[4];
  int filedescriptor = open(args[1], O_RDONLY); 
  read(filedescriptor, b, 4);
  if (b[0]==0xff && b[1]==0xd8 && b[2]==0xff && b[3]==0xe0){
    printf("It is a jpg\n");
    return 1;
  }

  printf("It isn??t a jpg\n");
  return 1;
}

// structure for message queue
struct mesg_buffer {
  long mesg_type;
  char mesg_text[100];
} message;

int builtin(char **args){
  if(args[0] == NULL){
    return 1;
  }
  //Check args[0] commands
  if(strstr(args[0], "PS1=") != NULL){
    return builtin_ps1(args);
  }

  if(strcmp (args[0], "avisoTeste")==0 ) {
    aviso( args[1], atoi ( args[2]) ) ;
    return 1;
  }

  if (strcmp (args[0], "aviso") == 0){
    pthread_t th;
    aviso_t * ptr = (aviso_t *)malloc( sizeof(aviso_t) );
    strcpy(ptr->msg, args[1]);
    ptr->tempo=atoi(args[2]);
    pthread_create(&th, NULL, avisowrapper, (void *)ptr);
    return 1;
  }
  if (strcmp (args[0], "socpth") == 0){
    pthread_t th;
    copiar_t * ptr = (copiar_t *)malloc( sizeof(copiar_t) );
    strcpy(ptr->fonte, args[1]);
    strcpy(ptr->destino,args[2]);
    pthread_create(&th, NULL, socpth, (void *)ptr);
    return 1;
  }

  if(strcmp(args[0],"users")==0){
    printf("Avaiable users:\n");
    system("ls /dev/pts/ | grep -v ptmx");
    printf("\nYou are:\n");
    system("tty | cut -d / -f 4");
    return 1;
  }

  if(strcmp(args[0], "message") == 0){
    int msgid;
    //This message get generates a message id by the given key in the arguments
    msgid = msgget((atoi(args[1])+1), 0666 | IPC_CREAT);
    message.mesg_type = 1;

    //Copies the argument given (one word) in the buffer to the structure
    strcpy(message.mesg_text, args[2]);
    
    //Send the message to the queue with the id+1
    msgsnd(msgid, &message, sizeof(message), 0);
    return 1;
  }

  if(strcmp(args[0], "receive") == 0){
    if(strcmp(args[1], "on") == 0){   
      TT = 1;
      trigger_thread(args);
    }
    if(strcmp(args[1], "off") == 0){   
      pthread_cancel(th);
      TT = 0;
    }
   
    return 1;
  }

  for(int i=0; i<builtin_size(); i++){
    if(strcmp(args[0], builtin_str[i]) == 0){
      return (*builtin_func[i])(args);
    }
  }
  return 0;
}
