#include <shell.h>
void soocp(char *fonte, char *destino)
{
    int fdIn = open(fonte,S_IRUSR); //read only
    //printf("%d\n",fdIn);
    if(fdIn < 3)
        perror("Erro ao abrir file de fonte\n");
    int fdOut = creat(destino,S_IRWXU); //read write execute
    //printf("%d\n",fdOut);
    if(fdOut < 3)
        perror("Erro ao abrir file de destino\n");
    ioCopy(fdIn,fdOut);
}

void * socpth(void *args) 
{
    copiar_t * ptr = (copiar_t *)args;
    soocp( ptr->fonte, ptr->destino );
    free(ptr);
    return NULL;
}

void ioCopy (int IN, int OUT) //no error reporting
{
    int n;
    char buf[128];
    while (( n = read (IN, buf, 128)) > 0)
    {
        if (write (OUT, buf, n) != n)
        perror("Erro de escrita!\n");
    }
    if (n < 0)
        perror("Erro de leitura!\n");
}
