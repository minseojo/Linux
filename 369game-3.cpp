#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 100

int main() {
    int fd1, fd2, n;
    char buf[MAXLINE];
    printf("369! 369!\n");
    
    fd1 = open("./fifo3", O_RDWR);
    fd2 = open("./fifo2", O_RDWR);
    open("./fifo1", O_RDWR);
    open("./fifo4", O_RDWR);   
	if(fd1 == -1 || fd2 == -1) {
        perror("open");
        mkfifo("./fifo1", 0666);
        mkfifo("./fifo2", 0666);
        mkfifo("./fifo3", 0666);
        mkfifo("./fifo4", 0666);
    
	    fd1 = open("./fifo3", O_RDWR);
    	fd2 = open("./fifo2", O_RDWR);
    	open("./fifo1", O_RDWR);
    	open("./fifo4", O_RDWR);

     	int number = 0;
    	memset(buf, 0, sizeof(buf));
    	sprintf(buf, "%d", ++number);
    	n = write(fd2, buf, sizeof(buf));
    	if(n == -1) {
        	perror("write");
        	exit(1);
    	}    
    }
    
    while(1) {
   		n = read(fd2, buf, MAXLINE);
 		if(n == -1) {
            perror("read");
            exit(1);
        }
        
		int flag = 0;
        for(int i=0; i<MAXLINE; i++) {
            if(buf[i] == '3' || buf[i] == '6' || buf[i] == '9') {
                printf("clap! ");
                flag = 1;
            }
        }        
		int number = atoi(buf);        
		if(flag) printf("(%d)\n", number);
        else printf("%d!\n", number);
        
		memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d", ++number);
        n = write(fd1, buf, sizeof(buf));
        if(n == -1) {
            perror("write");
            exit(1);
        }
		
		if(number > 50) { 
   			if(number == 54) {
        		unlink("./fifo1");
        		unlink("./fifo2");
         		unlink("./fifo3");
         		unlink("./fifo4");
    		}               
    		break;
		}	 
        sleep(4);
    }
    printf("Á¾·á\n");
    
    return 0; 
}
