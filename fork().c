#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 100 // �ڽ� ���μ��� �ִ밳�� 

pid_t pid[MAX]; // �ڽ����μ��� n�� �迭�� pid�� ���� (�ڽ� == 0 )

int main() {
    int n, status;
    pid_t child;
    printf("Write the number of child processes to create: ");
    scanf("%d", &n); // ������ �ڽ����μ��� ���� (n < MAX)
    printf("\nroot process pid: %d\n", getpid()); // �������α׷� ���μ��� pid��� 
    for(int i=0; i<n; i++) {
        pid[i] = fork(); // �ڽ����μ��� ���� 
        if(pid[i] > 0) {
            printf("parent wait child [%d]\n", pid[i]); 
            child = wait(&status); // �ڽĵ� ���� ��ٸ� 
            printf("child is exit [%d]\n", child); // ������ �ڽ� ���μ������� ���� 
            sleep(1);
        	exit(0); // ���μ��� ���� 
        }
        else if(pid[i] == 0) {
            printf("\nParent process #%d [%d]",i+1, getppid()); // �θ� ���μ��� ��� 
            printf("\nMy process #%d [%d] \n", i+1, getpid()); // �ڽ� ���μ��� ��� 
            sleep(1);
        } else return -1; // ������ -1 ���� 
    }
    exit(0); // ���� ���μ��� ���� 
}
