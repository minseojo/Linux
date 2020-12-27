#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 100 // buf �ִ� ũ�� (buf�� 369 ���ڸ� �����´�. ex) 1 = 1, 100 = 3, 10000 = 5)

int main() {
    int fd1, fd2, n;
    char buf[MAXLINE];
    printf("369! 369!\n");
        
    fd1 = open("./fifo1", O_RDWR); // ������1�� �б� ���� �����ϰ� open. 
    fd2 = open("./fifo4", O_RDWR); // ������4�� �б� ���� �����ϰ� open. 
    open("./fifo3", O_RDWR); // ������3�� �б� ���� �����ϰ� open. 
    open("./fifo2", O_RDWR); // ������2�� �б� ���� �����ϰ� open. 
    if(fd1 == -1 || fd2 == -1) { // �� ���α׷����� �ʿ��� �������� ���� �������� �ʾ��� ��� (= 369 ���� ���� ��)  
        perror("open"); 
        mkfifo("./fifo1", 0666); // ������1 ����.
        mkfifo("./fifo2", 0666); // ������2 ����.
        mkfifo("./fifo3", 0666); // ������3 ����. 
        mkfifo("./fifo4", 0666); // ������4 ����.
   
   		fd1 = open("./fifo1", O_RDWR); //������1 open .
    	fd2 = open("./fifo4", O_RDWR); // ������4 open. 
    	open("./fifo3", O_RDWR); // ������3 open. 
    	open("./fifo2", O_RDWR); // ������2 open.
		// ��� �������� ������ ���ÿ� ���� ����: �������� �����ϰ� ó�� ���α׷����� ����ΰ� ���� ���α׷������� ���¸� �Ѵ�. ex) 15~18���� �����ϰ� �׾Ʒ� 19~39 �����κ��� �������� ����. -> * 4���� ���α׷� ���� * 
		int number = 0; // ��ŸƮ �� - 1  
   	 	memset(buf, 0, sizeof(buf)); // buf �ʱ�ȭ 
    	sprintf(buf, "%d", ++number); // number = 1 + 0�� ���ְ� buf�� �ʱ�ȭ 
    	n = write(fd2, buf, sizeof(buf)); // fd2�� buf �ʱ�ȭ 
    	if(n == -1) {  
        	perror("write");
        	exit(1); // ���� �߻��� Ż�� 
    	}
    }
 
    while(1) { // ������ ���� number�� 50���� ū ��� ���� �ݺ�. 
        n = read(fd2, buf, MAXLINE); // fd2�� write�� ���� �о��. 
        if(n == -1) { // write�� ���� ���� read�� ���� ��� ����, (�������� ó���� write ���� ������ read�� ���ŷ �Ǿ�����.) 
            perror("read");
            exit(1);
        }
 		// 369 ��Ģ ���� 
        int flag = 0; // �ڼ��� �������, ������ ��� ���� 
        for(int i=0; i<MAXLINE; i++) {
            if(buf[i] == '3' || buf[i] == '6' || buf[i] == '9') { // ���ڿ� 3, 6, 9�� ������� �ڼ� 
                printf("clap! ");
                flag = 1; // �ڼ��� �������, flag=1  
            }
 		}		
        int number = atoi(buf); // buf�� �޾ƿ� 369 ���ڸ� char -> int������ �ڷ��� ��ȯ 
        if(flag) printf("(%d)\n", number); // �ڼ��� ������� � ���ڿ� �ڼ��� �ƴ��� �˷���. 
        else printf("%d!\n", number); // �ڼ��� ġ�� �ʾ������ ���ڸ� �����ؼ� (= !�ٿ���) ����.  
        
		memset(buf, 0, sizeof(buf)); // buf�ʱ�ȭ 
        sprintf(buf, "%d", ++number); // buf�� ������ �о��� number+= 1�ؼ� buf�� �ʱ�ȭ 
        n = write(fd1, buf, sizeof(buf)); // fd1���� buf�ʱ�ȭ 
        if(n == -1) { // wirte �����ϰ�� Ż�� 
        	perror("write");
        	exit(1);
        }
		    	
		if(number > 50) { // ���ڰ� 50�� ������� �극��ũ.  * wirte�� ���ְ� ���� ������ ���ڸ� �Ѱ��ְ� �극��ũ ����. (= ���������� ��� ������ ����) 
   			if(number == 54) { // �������� �� �� 51, 52, 53, 54 ���ڸ� ��ġ�� ��� �극��ũ, ���� 54�� ����. 54�� ��� ��� ������ ������ �����ϰ� �극��ũ. 
			   								// ���α׷��� ��ġ�� file exists�� ��� ���α׷��� ������� �ʱ� �������� ������ ���� �����ؾ���. 
         		unlink("./fifo1");
         		unlink("./fifo2");
         		unlink("./fifo3");
         		unlink("./fifo4");
   			}                        
    		break;
		}		  
        sleep(4); // ���α׷�4���� ���������� �������� 4�� sleep. 
    }
    printf("����\n"); // ���� ��� 
    
    return 0; 
}
// https://youtu.be/n95GSaRPSuk
