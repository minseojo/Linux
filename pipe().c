#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define MAX 100 // 프로세스 최대 개수 
#define READ 0 //  pipe[0]은 읽기 -> pipe[READ]보기 쉽게 지정 
#define WRITE 1 // pipe[1]은 쓰기 -> pipe[WRITE]보기 쉽게 지정 

pid_t pid[MAX];
int sum = 0, total = 0, num =0 , a = 0; // 순서대로 각 각의 프로세스 랜덤숫자 10개의합, 전체 프로세스 합, 랜덤숫자, 자료형변한(char->int) 

int main(void) {
        int n;
        int pipe_fds[2];
        char buf[1024];

        printf("Write the number of child processes to create: ");
        scanf("%d", &n); // 최대 자식 프로세스 개수 n < MAX 
        for(int i=0; i<n; i++) {
                memset(pipe_fds, 0, sizeof(pipe_fds));
                if (pipe(pipe_fds)) {
                        perror("pipe() error");
                        return -1;
                }
                memset(buf, 0, sizeof(buf)); // 초기화 
                pid[i] = fork(); // 자식 프로세스 생성 
                srand(time(NULL) ^ (getpid() << 16)); // 난수 설정 
                if(pid[i] > 0) {
                        /* 부모 프로세스*/
                        close(pipe_fds[WRITE]); // 읽기만 하므로 쓰기파이프 닫기 
                        read(pipe_fds[READ], buf, sizeof(buf)); // sum 읽어오기 
                        a = atoi(buf); // char배열(문자열) -> int형 변환 
                        total+=a; // 전체 합에 더함. 
                        close(pipe_fds[READ]); // 파이프읽기 닫기 
                }
                else if(pid[i] == 0) {
                        /* 자식 프로세스*/
						close(pipe_fds[READ]); // 쓰기만 하므로 파이프 읽기 닫기 
                        for(int i=0; i<10; i++) { //10개의 난수 
                                num = rand()%10; // 0~9사이의 난수 
                                sum+=num; // 10개의 난수 더해주기 
                        }
                        printf("[child no. %d] num = %d\n", i+1, sum);
                        sprintf(buf, "%d", sum); // buf에 'sum'초기화 
                        write(pipe_fds[WRITE], buf, sizeof(buf)); // buf내용 쓰기 
                        close(pipe_fds[WRITE]); // 쓰기 파이프 닫기 
                        exit(0); //자식 프로세스 종료 
                } else {
                        /*에러, 파일(읽기, 쓰기) 닫기*/
                        close(pipe_fds[0]);
                        close(pipe_fds[1]);
                        return -1;
                }
        }
        printf("[parent] total_sum = %d\n", total); // 전체 합 출력 
        return 0; 
}
