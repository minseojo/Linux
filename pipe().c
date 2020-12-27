#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define MAX 100 // ���μ��� �ִ� ���� 
#define READ 0 //  pipe[0]�� �б� -> pipe[READ]���� ���� ���� 
#define WRITE 1 // pipe[1]�� ���� -> pipe[WRITE]���� ���� ���� 

pid_t pid[MAX];
int sum = 0, total = 0, num =0 , a = 0; // ������� �� ���� ���μ��� �������� 10������, ��ü ���μ��� ��, ��������, �ڷ�������(char->int) 

int main(void) {
        int n;
        int pipe_fds[2];
        char buf[1024];

        printf("Write the number of child processes to create: ");
        scanf("%d", &n); // �ִ� �ڽ� ���μ��� ���� n < MAX 
        for(int i=0; i<n; i++) {
                memset(pipe_fds, 0, sizeof(pipe_fds));
                if (pipe(pipe_fds)) {
                        perror("pipe() error");
                        return -1;
                }
                memset(buf, 0, sizeof(buf)); // �ʱ�ȭ 
                pid[i] = fork(); // �ڽ� ���μ��� ���� 
                srand(time(NULL) ^ (getpid() << 16)); // ���� ���� 
                if(pid[i] > 0) {
                        /* �θ� ���μ���*/
                        close(pipe_fds[WRITE]); // �б⸸ �ϹǷ� ���������� �ݱ� 
                        read(pipe_fds[READ], buf, sizeof(buf)); // sum �о���� 
                        a = atoi(buf); // char�迭(���ڿ�) -> int�� ��ȯ 
                        total+=a; // ��ü �տ� ����. 
                        close(pipe_fds[READ]); // �������б� �ݱ� 
                }
                else if(pid[i] == 0) {
                        /* �ڽ� ���μ���*/
						close(pipe_fds[READ]); // ���⸸ �ϹǷ� ������ �б� �ݱ� 
                        for(int i=0; i<10; i++) { //10���� ���� 
                                num = rand()%10; // 0~9������ ���� 
                                sum+=num; // 10���� ���� �����ֱ� 
                        }
                        printf("[child no. %d] num = %d\n", i+1, sum);
                        sprintf(buf, "%d", sum); // buf�� 'sum'�ʱ�ȭ 
                        write(pipe_fds[WRITE], buf, sizeof(buf)); // buf���� ���� 
                        close(pipe_fds[WRITE]); // ���� ������ �ݱ� 
                        exit(0); //�ڽ� ���μ��� ���� 
                } else {
                        /*����, ����(�б�, ����) �ݱ�*/
                        close(pipe_fds[0]);
                        close(pipe_fds[1]);
                        return -1;
                }
        }
        printf("[parent] total_sum = %d\n", total); // ��ü �� ��� 
        return 0; 
}
