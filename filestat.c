#include <stdio.h>
#include <stdlib.h> // exit 
#include <string.h>
#include <unistd.h> // open()
#include <fcntl.h> //read(), write(), lseek(), close()
#include <sys/stat.h> // có các macro S_ISREG(), S_ISDIR(), S_ISLNK()
#include <time.h> // strftime() định dạng thời gian


void ObjectClassification(char *filename){
    struct stat fs;
    int type = 0;
    if (lstat(filename, &fs) == -1){
        perror("lstat");
        exit(1);
    }

    printf("File path: %s\n", filename);

    if(S_ISREG(fs.st_mode)) type = 1;
    else if(S_ISDIR(fs.st_mode)) type = 2;
    else if(S_ISLNK(fs.st_mode)) type = 3;
    else if(S_ISBLK(fs.st_mode)) type = 4;
    else if(S_ISCHR(fs.st_mode)) type = 5;
    else if(S_ISFIFO(fs.st_mode)) type = 6;
    switch(type){
        case 1:
            printf("File type: regular file");
            break;
        case 2:
            printf("File type: directory");
            break;
        case 3:
            printf("File type: Symbolic Link");
            break;
        case 4:
            printf("File type: Block");
            break;
        case 5:
            printf("File type: Character");
            break;
        case 6:
            printf("File type: named pipe");
            break;
        default:
            printf("File type: Unknown\n");
    }
    printf("Size: %ld bytes\n", fs.st_size);

    char timebuf[100];
    struct tm *tm_info = localtime(&fs.st_mtime);

    strftime(timebuf, sizeof(timebuf), "%d-%m-%Y %H:%M:%S", tm_info);
    printf("Last modified: %s\n", timebuf);
}




int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <file.path>\n", argv[0]);
        exit(1);
    } 
    ObjectClassification(argv[1]);
    return 0;
}








