#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semun.c"

int main(int argc, char *argv[]) {
  /* IPC */
  pid_t pid[5];
  key_t key;
  int semid,shmid,status;
  char *segptr;
  union semun arg;
  struct sembuf lock_res = {0, -1, 0};
  struct sembuf rel_res = {0, 1, 0};

//    if(argc < 2){
//        printf("Usage: bufdemo [dimensione]\n");
//        exit(0);
//      }
    /* Семафоры */
      key = ftok("/etc/fstab", getpid());
    /* Создать набор из одного семафора */
      semid = semget(key, 1, 0666 | IPC_CREAT);
    /*Установить в семафоре № 0 (Контроллер ресурсов) значение "1"*/
      arg.val = 1;
      semctl(semid, 0, SETVAL, arg);

      /* Создаем особый ключ через вызов ftok() */
      key = ftok(".", 'S');
      /* Открываем разделяемый сегмент памяти - если нужно, создаем */
      if((shmid = shmget(key, 100, IPC_CREAT|0660)) == -1) {
         printf("Открыли существующий сегмент\n");

      } else {

         printf("Создали сегмент памяти %d\n", shmid);
      }
      /* Привязывем (распределяем) разделяемый сегмент памяти в текущем
         процессе */

      int i;
      for (i = 0; i < 5; i++){
          printf("for\n");
          pid[i] = fork();
          if (pid[i] == 0){
              //sleep(rand()%6);
              printf("fork\n");
              if((segptr = (char *)shmat(shmid, 0, 0)) == -1) {
                 perror("shmat");
                 //exit(1);
              }
              /* Попытаться заблокировать ресурс (семафор номер 0) */
              if (semop(semid, &lock_res, 1) == -1){
                perror("semop:lock_res");
                printf("if\n");

              } else {
                  printf("else\n");
                  strcpy(segptr, "Wake up");
                  printf("segptr: %s\n", segptr);
              }
          /* Разблокировать ресурс */
            semop(semid, &rel_res, 1);
          }

      }
      waitpid(-1, &status, 0);
      printf("%d\n",WEXITSTATUS(status));
      exit(status);

    //semctl(semid, 0, IPC_RMID);
    return 0;
}
