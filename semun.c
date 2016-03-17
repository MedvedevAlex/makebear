// Программа из конспекта "Системное программное обеспечение"
// Семафоры в Linux
// стр. 98 
// Название: semun.c
// Описание: описание структуры для работы с семафором
union semun {
 int val;                  /* значение для SETVAL */
 struct semid_ds *buf;     /* буферы для  IPC_STAT, IPC_SET */
 unsigned short *array;    /* массивы для GETALL, SETALL */
                           /* часть, особенная для Linux: */
 struct seminfo *__buf;    /* буфер для IPC_INFO */
};
//struct sembuf {
//            short sem_num;  /* semaphore number: 0 = first */
//            short sem_op;   /* semaphore operation */
//            short sem_flg;  /* operation flags */
//}

