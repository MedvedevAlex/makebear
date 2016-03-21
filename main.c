#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void freestr(char *s, int n){
    int i;
    for (i = 0; i<n; ++i){
        s[i] = ' ';
    }
}

void *thread_function(void *arg);
char message[] = "poly";
int rez=0;


int main(int argc, char *argv[]) {
    int res;
    argv[1] = "poly";
    pthread_t a_thread;
    void *thread_result;
    res = pthread_create(&a_thread, NULL, thread_function, (void *)message);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join-failed");
        exit(EXIT_FAILURE);
    }
    printf("Поток завершился и вернул %s\n", (char *)thread_result);
    //printf("Message is now %s\n", message);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    printf("Поток запущен в качестве аргумента %s\n", (char *)arg);
    int find = 0,j = 0;
    char str[80];

    //sleep(3);

    FILE *fp;
    if((fp=fopen("in", "r"))==NULL) {
      printf("Ошибка при открытии файла.\n");
      exit(1);
    }

    freestr(str, 80);
    while(fscanf(fp, "%s", str) != NULL) {
        if(strcmp(str,message) == 0) {
            find = 1;
            break;
        } else {
           find = 0,j++;
           freestr(str, 80);
        }
    }

    if (find == 1){
        /* Посылаем "string" через выход канала */
        pthread_exit("Строка найдена");
    } else {
        pthread_exit("Строка не найдена!");
    }

    //strcpy(message, "Bye!");
}
