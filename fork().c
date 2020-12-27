#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 100 // 자식 프로세스 최대개수 

pid_t pid[MAX]; // 자식프로세스 n개 배열로 pid값 저장 (자식 == 0 )

int main() {
    int n, status;
    pid_t child;
    printf("Write the number of child processes to create: ");
    scanf("%d", &n); // 실행할 자식프로세스 개수 (n < MAX)
    printf("\nroot process pid: %d\n", getpid()); // 실행프로그램 프로세스 pid출력 
    for(int i=0; i<n; i++) {
        pid[i] = fork(); // 자식프로세스 실행 
        if(pid[i] > 0) {
            printf("parent wait child [%d]\n", pid[i]); 
            child = wait(&status); // 자식들 종료 기다림 
            printf("child is exit [%d]\n", child); // 마지막 자식 프로세스부터 종료 
            sleep(1);
        	exit(0); // 프로세스 종료 
        }
        else if(pid[i] == 0) {
            printf("\nParent process #%d [%d]",i+1, getppid()); // 부모 프로세스 출력 
            printf("\nMy process #%d [%d] \n", i+1, getpid()); // 자신 프로세스 출력 
            sleep(1);
        } else return -1; // 에러시 -1 리턴 
    }
    exit(0); // 메인 프로세스 종료 
}
