#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int degerAl(char *str) //kullanicidan komut alir.
{
    char buf[150]; // stringin bir boyutu olmali...
    printf("\n sorgu giriniz> ");
    fgets(buf, 150, stdin); // basina otomatik ekliyor, sen bisey yazip entera bastigin anda buffera ekler stringi...
    if (strlen(buf) != 0)
    {
        buf[strlen(buf) - 1] = '\0';  //enter'a basildiginda enter karakteri geldigi icin null atanir.
        strcpy(str, buf);
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    int fd;

    char myfifo[] = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char arr1[80], arr2[80];
    char input[100];

    while (1)
    {

        fd = open(myfifo, O_WRONLY);
        degerAl(input);

        write(fd, input, strlen(input) + 1);  //database string gonderilir.

        close(fd);

        fd = open(myfifo, O_RDONLY);
        read(fd, arr1, 80);   // databasedan donus okunur.

        close(fd);

        printf("%s \n", arr1);
        //kontroller
        if(strcmp(arr1,"NULL\n")==0){
            continue;  // null ise donguye devam
        }
        else if(strcmp(arr1,"hatali komut girildi...")==0){
            continue; // hatali komut girilmisse donguye devam.
        }
        printf("sonuc kaydedilsin mi ?  e - h \n"); // e ye basarsak kaydet programi cagrilir. 
        fgets(arr2, 80, stdin);
        if (strcmp(arr2, "e\n") == 0)
        {
            int pipefd[2];

            int c;
            //pipe olustur
            if (pipe(pipefd) < 0)
            {
                perror("pipe");
                exit(1);
            }
            int pid = fork();

            if (pid == 0)
            {
                //sayiyi pipe yaz
                write(pipefd[1], arr1, 80);
                // kaydet programi cagir
                c = execv("kaydet", NULL);
                // hata olusursa
                perror("");
                close(pipefd[1]);
            }
            else
            {

                wait(&c); //kaydet programinin cagirildigi prosesi bekle

                close(pipefd[1]);
                close(pipefd[0]);
                printf("dosyaya kaydedilmiştir.\n");

            }
        }
        else if(strcmp(arr2,"h\n")==0){
            continue;
        }
    }
    return 0;
}
