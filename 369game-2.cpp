#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 100 // buf 최대 크기 (buf는 369 숫자를 가져온다. ex) 1 = 1, 100 = 3, 10000 = 5)

int main() {
    int fd1, fd2, n;
    char buf[MAXLINE];
    printf("369! 369!\n");
        
    fd1 = open("./fifo1", O_RDWR); // 파이프1를 읽기 쓰기 가능하게 open. 
    fd2 = open("./fifo4", O_RDWR); // 파이프4를 읽기 쓰기 가능하게 open. 
    open("./fifo3", O_RDWR); // 파이프3을 읽기 쓰기 가능하게 open. 
    open("./fifo2", O_RDWR); // 파이프2를 읽기 쓰기 가능하게 open. 
    if(fd1 == -1 || fd2 == -1) { // 이 프로그램에서 필요한 파이프가 아직 생성되지 않았을 경우 (= 369 게임 시작 전)  
        perror("open"); 
        mkfifo("./fifo1", 0666); // 파이프1 생성.
        mkfifo("./fifo2", 0666); // 파이프2 생성.
        mkfifo("./fifo3", 0666); // 파이프3 생성. 
        mkfifo("./fifo4", 0666); // 파이프4 생성.
   
   		fd1 = open("./fifo1", O_RDWR); //파이프1 open .
    	fd2 = open("./fifo4", O_RDWR); // 파이프4 open. 
    	open("./fifo3", O_RDWR); // 파이프3 open. 
    	open("./fifo2", O_RDWR); // 파이프2 open.
		// 모든 파이프를 생성과 동시에 여는 이유: 파이프를 생성하고 처음 프로그램에서 열어두고 다음 프로그램에서는 오픈만 한다. ex) 15~18줄을 실행하고 그아래 19~39 에러부분은 실행하지 않음. -> * 4개의 프로그램 동일 * 
		int number = 0; // 스타트 값 - 1  
   	 	memset(buf, 0, sizeof(buf)); // buf 초기화 
    	sprintf(buf, "%d", ++number); // number = 1 + 0을 해주고 buf에 초기화 
    	n = write(fd2, buf, sizeof(buf)); // fd2에 buf 초기화 
    	if(n == -1) {  
        	perror("write");
        	exit(1); // 에러 발생시 탈출 
    	}
    }
 
    while(1) { // 지정한 숫자 number가 50보다 큰 경우 까지 반복. 
        n = read(fd2, buf, MAXLINE); // fd2에 write한 값을 읽어옴. 
        if(n == -1) { // write한 값이 없어 read를 못할 경우 에러, (파이프는 처음에 write 하지 않으면 read가 블로킹 되어있음.) 
            perror("read");
            exit(1);
        }
 		// 369 규칙 적용 
        int flag = 0; // 박수를 쳤을경우, 안쳤을 경우 구분 
        for(int i=0; i<MAXLINE; i++) {
            if(buf[i] == '3' || buf[i] == '6' || buf[i] == '9') { // 숫자에 3, 6, 9가 있을경우 박수 
                printf("clap! ");
                flag = 1; // 박수를 쳤을경우, flag=1  
            }
 		}		
        int number = atoi(buf); // buf로 받아온 369 숫자를 char -> int형으로 자료형 변환 
        if(flag) printf("(%d)\n", number); // 박수를 쳤을경우 어떤 숫자에 박수를 쳤는지 알려줌. 
        else printf("%d!\n", number); // 박수를 치지 않았을경우 숫자를 강조해서 (= !붙여서) 말함.  
        
		memset(buf, 0, sizeof(buf)); // buf초기화 
        sprintf(buf, "%d", ++number); // buf에 이전에 읽었던 number+= 1해서 buf에 초기화 
        n = write(fd1, buf, sizeof(buf)); // fd1값에 buf초기화 
        if(n == -1) { // wirte 에러일경우 탈출 
        	perror("write");
        	exit(1);
        }
		    	
		if(number > 50) { // 숫자가 50이 넘을경우 브레이크.  * wirte를 해주고 다음 파이프 숫자를 넘겨주고 브레이크 해줌. (= 순차적으로 모든 파이프 종료) 
   			if(number == 54) { // 파이프는 각 각 51, 52, 53, 54 숫자를 외치는 경우 브레이크, 따라서 54가 최종. 54인 경우 모든 파이프 파일을 삭제하고 브레이크. 
			   								// 프로그램을 마치고 file exists인 경우 프로그램이 실행되지 않기 마지막에 파이프 파일 삭제해야함. 
         		unlink("./fifo1");
         		unlink("./fifo2");
         		unlink("./fifo3");
         		unlink("./fifo4");
   			}                        
    		break;
		}		  
        sleep(4); // 프로그램4개가 순차적으로 돌기위해 4초 sleep. 
    }
    printf("종료\n"); // 종료 출력 
    
    return 0; 
}
// https://youtu.be/n95GSaRPSuk
