#include <shell.h>

// structure for message queue
struct mesg_buffer {          
  long mesg_type;             
  char mesg_text[100];        
} message;                

void rcv_message(){
  int id = 0; 
  //Extract id from tty
  char *s = ttyname(STDIN_FILENO);
  for(int i=0; i<s[i]; i++){      
    if(isdigit(s[i])){            
      id = id*10 + (s[i] - '0');  
    }                             
  }                               
  id++;
  int msgid;
  //creates the message queue and returns identifier
  msgid = msgget(id, 0666 | IPC_CREAT); 

  //receives the message
  msgrcv(msgid, &message, sizeof(message), 1, 0);
  printf("Message received: %s\n", message.mesg_text);

  //destroy the message queue
  msgctl(msgid, IPC_RMID, NULL);
}

void * mesg_wrapper(void *args){
  rcv_message();
  return NULL;
}
