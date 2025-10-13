#include <stdio.h>
#include <string.h>
#include <unistd.h> // for open()
#include <fcntl.h> // for read(), write(), lseek(), close()
#include <stdlib.h> // for exit 



int main(int argc, char* argv[]){
    int fd;
    char buffer[100];
    ssize_t bytes_read, bytes_written;

    //1. Open the file "test.txt" with read/write access.
    // If it doesn't exist, create it with permission 0644(rw-r--r--).
    fd = open("text.txt", O_RDWR | O_CREAT, 0644); // Read and write, Create file if file does not exist
    if (fd < 0){
        perror("open"); // print error if opening fails
        exit(1);
    }

    //2.Write a string to the file 

    const char *text = "Hello, this is a test.\n";
    bytes_written = write(fd, text, strlen(text));
    if(bytes_written < 0){
        perror("write"); //print error if string fails
        close(fd);
        exit(1);
    }
    
    //3.Move the file pointer back to the beginning of the file
    if (lseek(fd, 0, SEEK_SET) < 0){
        perror("lseek"); //print error if seeking fails
        close(fd);
        exit(1);
    }

    //4.Read the content from the file into the buffer
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if(bytes_read < 0){
        perror("read"); //print error if reading fails
        close(fd);
        exit(1);
    }
    buffer[bytes_read] = '\0'; //Null terminate the string
    printf("Content read from file:\n%s", buffer);

    //5.Close the file
    close(fd);

    return 0;

}