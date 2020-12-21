#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#define MAXCOM 1000 // maksimum harf sayisi
#define MAXLIST 100 // maksimum komut sayisi

char *kontroller(char *ilk_girdi, char sonuc[])
{
}

int hata = 0;

int parseSpace(char *str, char **parsed) // bosluga gore parcalama islemi yapan fonksiyon
{
    int i;
    int counter = 0;

    for (i = 0; i < MAXLIST; i++)
    {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
        counter++;
    }
    return counter;
}
int parseEquals(char *str, char **parsed) // bosluga gore parcalama islemi yapan fonksiyon
{
    int i;
    int counter = 0;

    for (i = 0; i < MAXLIST; i++)
    {
        parsed[i] = strsep(&str, "=");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
        counter++;
    }
    return counter;
}
// kontrollerden sonra, parsed 3teki dosyayı ac , sonra bu dosyadan satır satır okuyosun

int main()
{
    char *boslukP[100] = {NULL};
    char *esittirP[100] = {NULL};

    int fd1;
    char myfifo[] = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char oku[190] = {'\0'};
    char gonder[80] = {'\0'};
    char dosyaOku[150] = {'\0'};
    char name[100] = {'\0'};
    char number[100] = {'\0'};

    while (1)
    {

        fd1 = open(myfifo, O_RDONLY);
        read(fd1, oku, 190);
        close(fd1);

        printf("sonucb: %s\n", oku);
        int c1 = parseSpace(oku, boslukP);
        int c2 = parseEquals(boslukP[5], esittirP);                
        int hataVarMi = 0;
        //kontrollerim
        if (c1 + c2 != 8)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strcmp(boslukP[0], "SELECT") != 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strcmp(boslukP[1], "*") != 0 && strcmp(boslukP[1], "ad") != 0 && strcmp(boslukP[1], "number") != 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strcmp(boslukP[2], "FROM") != 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strcmp(boslukP[3], "veri1.txt") != 0 && strcmp(boslukP[3], "veri2.txt") != 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strcmp(boslukP[4], "WHERE") != 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strcmp(esittirP[0], "ad") != 0 && strcmp(esittirP[0], "number") != 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        else if (strlen(esittirP[1]) == 0)
        {
            strcat(gonder, "hatali komut girildi...");
            hataVarMi = 1;
        }
        if (hataVarMi == 1)
        {
            fd1 = open(myfifo, O_WRONLY);
            write(fd1, gonder, strlen(gonder) + 1); // str2 string oldugu icin, stringin null karakterini de gondeririz +1 ile
            close(fd1);
            int i;
            for (i = 0; i < 80; i++)
            {
                gonder[i] = '\0';
            }
            hataVarMi = 0;
            continue;
        }

        FILE *file;
        file = fopen(boslukP[3], "r");
        int counter = 0;
        //dosya sonuna kadar okunur.
        while (!(feof(file)))
        {
            fgets(dosyaOku, 150, file);
            sscanf(dosyaOku, "%s %s\n", name, number);
            if (strcmp(esittirP[0], "ad") == 0 && strcmp(esittirP[1], name) == 0)
            {
                counter++; // bir kayit bulundugunda artar.
                if (strcmp(boslukP[1], "*") == 0)
                {
                    strcat(gonder, name);
                    strcat(gonder, " ");
                    strcat(gonder, number);
                    strcat(gonder, "\n");
                }
                else if (strcmp(boslukP[1], "ad") == 0)
                {
                    strcat(gonder, name);
                    strcat(gonder, "\n");
                }
                else if (strcmp(boslukP[1], "number") == 0)
                {
                    strcat(gonder, number);
                    strcat(gonder, "\n");
                }
            }
            else if (strcmp(esittirP[0], "number") == 0 && strcmp(esittirP[1], number) == 0)
            {
                counter++;
                if (strcmp(boslukP[1], "*") == 0)
                {
                    strcat(gonder, name);
                    strcat(gonder, " ");
                    strcat(gonder, number);
                    strcat(gonder, "\n");
                }
                else if (strcmp(boslukP[1], "ad") == 0)
                {
                    strcat(gonder, name);
                    strcat(gonder, "\n");
                }
                else if (strcmp(boslukP[1], "number") == 0)
                {
                    strcat(gonder, number);
                    strcat(gonder, "\n");
                }
            }
        }
        if (counter == 0)
        {
            strcat(gonder, "NULL\n"); // hic bir kayit bulunmazsa null doner
        }

        fd1 = open(myfifo, O_WRONLY);
        write(fd1, gonder, strlen(gonder) + 1); // str2 string oldugu icin, stringin null karakterini de gondeririz +1 ile
        close(fd1);
        int i;
        for (i = 0; i < 80; i++)
        {
            gonder[i] = '\0'; // array temizlenir.
        }
    }
    return 0;
}
