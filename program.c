#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


int main() {

    int pipe;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    char inputQuery[100] = {'\0'};
    char result[3600];
    char control;
    
    while (1) {
        // clean input
        fflush(stdin);
        printf("QUERY: ");
        // write a query on pipe
        fgets(inputQuery, 80, stdin);
        pipe = open(myfifo, O_WRONLY);
        write(pipe, inputQuery, strlen(inputQuery)+1);
        close(pipe);

        pipe = open(myfifo,O_RDONLY);
        read(pipe,result,3600);
        
        if (result[0] == '\0') {
            printf("result is null/n");
            break;
        }else if(inputQuery[0] != '\0'){
            printf("Do you want to save a query result (T/F) ? \n");
            scanf("%c",&control);
            if(control == 'T'){
                pid_t process = fork();
                if(process < 0){
                    printf("Process fail !!\n");
                    break;
                }else if (process == 0){
                    char* arguments[] = {"/kaydet", result, NULL };
                    execv("/kaydet", arguments);
                }else{
                    sleep(15);
                }
                
            }else if(control == 'F'){
                printf("Signing out...\n");
                break;
            }
        }
    
        close(pipe);
        
        
        
    }
    return 0;
}

