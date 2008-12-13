#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

void critical_zone(char* process_name){
    printf("zona crítica ejecutada por proceso: %s\n", process_name);
    sleep(2);
}


int main(int argc, char** argv){
    int fds[2];
    char* message = "busy";
    char readbuffer[8];
    pipe(fds);

    if(0 == fork()){
        close(fds[0]);
        printf("hijo1 ejecuta zona crítica\n");
        sleep(1);
        critical_zone("hijo1");
        printf("hijo1 termina de ejecutar zona crítica\n");
        write(fds[1], message, (strlen(message)+1));
    } else {
        if(0 == fork()){
            close(fds[1]);
            printf("hijo2 intenta ejecutar zona crítica\n");
            read(fds[0], readbuffer, sizeof(readbuffer));
            printf("hijo2 ejecuta zona crítica\n");
            critical_zone("hijo2");
            printf("hijo2 termina de ejecutar zona crítica\n");
        } else {
            close(fds[0]);
            close(fds[1]);
            wait(0);
            wait(0);
        }
    }
  return 0;
}
