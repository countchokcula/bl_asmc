#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define CMD_COUNT 3
enum COMMANDS {NEW, RUN, BUILD};

int match_command(char*);
void parse_args(char*);
void new(void);
void run(void);
void build(void);
void print_usages(void);

int main(int argc, char* argv[]){

    if(argc<2){
        print_usages();
    }else{
        parse_args(argv[1]);
    }
    return 0;
}
void parse_args(char* command){
    int cmd_num = match_command(command);
    switch(cmd_num){
        case NEW:
            new();
        break;
        case RUN:
            run();
        break;

        case BUILD:
            build();
        break;
        default:
            print_usages();
        break;
    }
}
int match_command(char* command){
    char cmds[CMD_COUNT][10] = {"new", "run", "build"};
    for(int i = 0; i<CMD_COUNT; i++){
        if(!strcmp(cmds[i], command)){
            return i;
        }
    }
    return 0;
}
void print_usages(){
    printf("\t--ASMC Usages--\n New: Creates directories\n Build: Compiles asm\n Run: Runs under qemu-i386\n");
    exit(-1);
}

void new(){
    struct stat status = {0};
    char dirs[3][20] = {
        "asm",
        "asm/inc",
        "os"
    };
    for(int i = 0; i < 3; i++){
        if(stat(dirs[i], &status) == -1){
            mkdir(dirs[i], 0700);
            printf("-- Created -> %s\n", dirs[i]);
        }else{
            printf("-- Directory %s exists\n", dirs[i]);
        }
    };
}
void run(){
    system("qemu-system-i386 -fda os/os.bin");
}
void build(){
    struct dirent *d_entry;
    DIR *as = opendir("asm");
    char buffer[50];
    char d_name[20];
    char* token;
    if(as == NULL){
        new();
        fprintf(stderr, "Cannot build files!\n");
        exit(-1);
    }
    

    while((d_entry = readdir(as)) != NULL){
        if(strstr(d_entry->d_name, ".asm")){
            strcpy(d_name, d_entry->d_name);
            token = strtok(d_entry->d_name, ".");

            strcpy(buffer, "nasm -f bin asm/");
            strncat(buffer, d_name, sizeof(buffer)); //nasm -f bin asm/d_name
            strncat(buffer, " -o os/", sizeof(buffer)); //nasm -f bin asm/d_name -o os/
            strncat(buffer, token, sizeof(buffer)); //nasm -f bin asm/d_name -o os/d_name
            strncat(buffer, ".bin", sizeof(buffer)); ////nasm -f bin asm/d_name -o os/d_name.bin
            printf("%s\n", buffer);
            system(buffer);
        }
    }
    printf("\t-- Remember to CONCAT the binary files in the ./os folder!\n");
    closedir(as);
}