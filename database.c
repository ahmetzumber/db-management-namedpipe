#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// clear all string char by char
void clearString(char *text, int row, int column){
    for (int i = 0; i < row; i++) {
        *(text+i*column) = '\0';
        for (int j = 0; j < column; j++) {
            *(text+i*column+j) = '\0';
        }
    }
}

int mySplit(char *input, char **words){
    char *text;
    text = strtok(input," =\n");

    int i = 0, length = 0;
    while(text != NULL){
        words[i] = text;
        text = strtok(NULL," =\n");
        i++; length++;
    }
    return length;
}

void fileOperations(char **queries, char *filePath, int length, char *allText){
    
    printf("\n-----%s-----\n",filePath);
    FILE *file;
    file = fopen(filePath,"r+");
    char concatenedText[3600] = {'\0'};
    char line[15] = {'\0'};
    char names[10] = {"\0"};
    char number[3] = {"\0"};
    char newLine = '\n';

    // Query controls
    if(strcmp(queries[1],"*") == 0){
        if(length > 4){
            if(strcmp(queries[4],"where") == 0){
                if(strcmp(queries[5],"ad") == 0){
                    while(!feof(file)){
                        fgets(line,15,file);
                        sscanf(line,"%s %s",names,number);
                        if(strcmp(names,queries[6]) == 0){
                            printf("%s\n",line);
                            strcat(line,&newLine);
                            strcat(concatenedText,line);
                        }
                    }
                } else if (strcmp(queries[5],"number") == 0){
                    while(!feof(file)){
                        fgets(line,15,file);
                        sscanf(line,"%s %s",names,number);
                        if(strcmp(number,queries[6]) == 0){
                            printf("%s\n",line);
                            strcat(line,&newLine);
                            strcat(concatenedText,line);
                        }
                    }
                }
            }
        }else{
            while(!feof(file)){
                fgets(line,15,file);
                printf("%s\n",line);
                strcat(line,&newLine);
                strcat(concatenedText,line);
            }
        }
    }else if(strcmp(queries[1],"ad") == 0){
        if(length > 4){
            if(strcmp(queries[4],"where") == 0){
                if(strcmp(queries[5],"ad") == 0){
                    while(!feof(file)){
                        fgets(line,15,file);
                        sscanf(line,"%s %s",names,number);
                        if(strcmp(names,queries[6]) == 0){
                            printf("%s\n",names);
                            strcat(names,&newLine);
                            strcat(concatenedText,names);
                        }
                    }
                } else if (strcmp(queries[5],"number") == 0){
                    while(!feof(file)){
                        fgets(line,15,file);
                        sscanf(line,"%s %s",names,number);
                        if(strcmp(number,queries[6])==0){
                            printf("%s\n",names);
                            strcat(names,&newLine);
                            strcat(concatenedText,names);
                        }
                    }
                }
            }
        }else{
            while(!feof(file)){
                fgets(line,15,file);
                sscanf(line,"%s %s",names,number);
                printf("%s\n",names);
                strcat(names,&newLine);
                strcat(concatenedText,names);
            }
        }
    }else if(strcmp(queries[1],"number") == 0){
        if(length > 4){
            if(strcmp(queries[4],"where") == 0){
                if(strcmp(queries[5],"ad") == 0){
                    while(!feof(file)){
                        fgets(line,15,file);
                        sscanf(line,"%s %s",names,number);
                        if(strcmp(names,queries[6]) == 0){
                            printf("%s\n",number);
                            strcat(concatenedText,number);    // add line to end of contanetedText
                        }
                    }
                } else if (strcmp(queries[5],"number") == 0){
                    while(!feof(file)){
                        fgets(line,15,file);
                        sscanf(line,"%s %s",names,number);
                        if(strcmp(number,queries[6]) == 0){
                            printf("%s\n",number);
                            strcat(number, &newLine);
                            strcat(concatenedText,number);    // add line to end of contanetedText
                        }
                    }
                }
            }
        }else{
            while(!feof(file)){
                fgets(line,15,file);
                sscanf(line,"%s %s",names,number);
                printf("%s\n",number);
                strcat(concatenedText,number);    // add line to end of contanetedText
            }
        }
    }
    // copy concatenedText to allText to send data with pipe
    strcpy(allText,concatenedText);
    fclose(file);

}

int main() {

    int pipe;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    int length;
    char enteredQuery[100] = {"\0"}, *queryWords[100];
    // file paths
    char *path01 = "veri1.txt";
    char *path02 = "veri2.txt";
    char concatanedText[3600] = {"\0"};
   
    while (1) {
        pipe = open(myfifo,O_RDONLY);
        read(pipe, enteredQuery, 100);

        // holding query length
        length = mySplit(enteredQuery,queryWords);

        // if 4th word "veri1.txt"
        if (strncmp(path01,queryWords[3],9) == 0){
            fileOperations(queryWords,path01,length,concatanedText);
        }else if (strncmp(path02,queryWords[3],9) == 0){
            fileOperations(queryWords,path02,length,concatanedText);
        }
        close(pipe);
        
        // send results to program.c with pipe
        pipe = open(myfifo,O_WRONLY);
        write(pipe,concatanedText,strlen(concatanedText));
        close(pipe);
        
        // cleaning data after every launch
        for (int i = 0; i < 3600; i++) {
            concatanedText[i] = '\0';
        }
        
    }
    
    return 0;
}

