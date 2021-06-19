/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE		80 /* 80 chars per line, per command */

void func(char *args[],int args2){
    int i=0;
    while((i<args2) && args[i] !=NULL){
        free(args[i]);
        i++;
        if(i==80){ 
        	break;
        }
    }
}

void inputfunc(char *args[],int *inarg,int *args2){
    char inputchar[MAX_LINE], str[]= " ";
    int l=0;
    l=read(STDIN_FILENO,inputchar,80);
    if(inputchar[l-1]=='\n'){
        inputchar[l-1]='\0';
    }
    if(strcmp(inputchar,"!!")==0){
        if(*args2==0){
            printf("Error History is empty.\n");
        }
        return 0;
    }
    func(args,*args2);
    *inarg = 0;
    *args2 = 0;
    char *startpointer=strtok(inputchar, str);
    while(startpointer != NULL){
        if(startpointer[0]=='&'){
            *inarg=1;
            startpointer=strtok(NULL, str);
            continue;
        }
        *args2 +=1;
        args[*args2-1]=strdup(startpointer);
        startpointer=strtok(NULL,str);
    }
    args[*args2] = NULL;
}

int main(void){
    char *args[MAX_LINE/2+1];
    int should_run=1,inarg=0,args2=0,pipe_flag=0;
    int i,j;
    pid_t pid;
    while(should_run){   
        pipe_flag = 0;
        printf("osh>");
        fflush(stdout);
        inputfunc(args,&inarg,&args2);
        pid=fork();
        if(pid == 0){
            if(args2==0){
                continue;
            } 
            else{
                int file_flag=0,file;
                for(i=1;i<=args2-1;i++){
                    if(strcmp(args[i],"<")==0){
                        file=open(args[i+1], O_RDONLY);
                        if((args[i+1] || file==-1)== NULL){
                            printf("Error <\n");
                            exit(1);
                        }
                        dup2(file, STDIN_FILENO);
                        args[i]=NULL;
                        args[i+1]=NULL;
                        file_flag=1;
                        break;
                    } 
                    else if(strcmp(args[i],">")==0){
                        file=open(args[i+1],O_WRONLY | O_CREAT, 0644);
                        if((args[i+1] || file==-1)== NULL){
                            printf("Error >\n");
                            exit(1);
                        }
                        dup2(file, STDOUT_FILENO);
                        args[i]=NULL;
                        args[i + 1]=NULL;
                        file_flag=2;
                        break;
                    } 
                    else if(strcmp(args[i],"|")==0){
                        pipe_flag = 1;
                        int arr[2];
                        if(pipe(arr)==-1){
                            fprintf(stderr,"Pipe not working\n");
                            return 0;
                        }
                        char *arr2[i+1];
                        char *arr3[args2-i];
                        for(j = 0; j < i; j++){
                            arr2[j]=args[j];
                        }
                        arr2[i]=NULL;
                        for(j=0;j<args2-i-1;j++){
                            arr3[j]=args[j+i+1];
                        }
                        arr3[args2-i-1]=NULL;
                        int pipeint=fork();
                        if(pipeint>0){
                            wait(NULL);
                            close(arr[1]);
                            dup2(arr[0],STDIN_FILENO);
                            close(arr[0]);
                            if(execvp(arr3[0],arr3)==-1){
                                printf("Error 1!\n");
                                return 0;
                            }

                        } 
                        else if (pipeint == 0){
                            close(arr[0]);
                            dup2(arr[1],STDOUT_FILENO);
                            close(arr[1]);
                            if(execvp(arr2[0],arr2)==-1){
                                printf("Error 2!\n");
                                return 0;
                            }
                            exit(1);
                        }
                        close(arr[0]);
                        close(arr[1]);
                        break;
                    }
                }
                if(pipe_flag==0){
                    if(execvp(args[0],args)==-1){
                        printf("Error 3!\n");
                        return 0;
                    }
                }
                if(file_flag==1){
                    close(STDIN_FILENO);
                } 
                else if (file_flag==2){
                    close(STDOUT_FILENO);
                }
                close(file);
            }
            exit(1);
        } 
        else if(pid >0){
            if(inarg==0){
            	wait(NULL);
            }
        } 
        else{
            printf("fork is messed up");
        }
    }
    return 0;
}
