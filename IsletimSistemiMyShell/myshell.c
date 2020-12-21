#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXCOM 1000 // maksimum harf sayisi
#define MAXLIST 100 // maksimum komut sayisi

#define clear() printf("\033[H\033[J")

int takeInput(char *str) //kullanicidan komut alir.
{
    char buf[150]; // stringin bir boyutu olmali...
    printf("\n myshell>> ");
    gets(buf); // basina otomatik ekliyor, sen bisey yazip entera bastigin anda buffera ekler stringi...
    if (strlen(buf) != 0)
    {
        strcpy(str, buf);
        return 1;
    }
    else
    {
        return 0;
    }
}
void parseSpace(char *str, char **parsed)  // bosluga gore parcalama islemi yapan fonksiyon
{
    int i;

    for (i = 0; i < MAXLIST; i++)
    {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
int parseKomut(char *str, char **parsed) // dik cizgiye gore parcalama islemi yapan fonksiyon.
{
    int i;

    for (i = 0; i < MAXLIST; i++)
    {
        parsed[i] = strsep(&str, "|");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
int sayiBulma(char **parsed){ //girilen komutun kaç tane parametre girildiğini bulmaya yarar.
    int counter=0;
    for (int i = 0; i < strlen(parsed); i++)
    {
        if(parsed[i]==NULL){
            break;
        }
        
        else{
            counter++;
        }
    }
    return counter;
    
}
void execArgs(char **parsed)        // komutları girilen parametre ile uygun olduysa calistirir.
{
    
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("\n yanlis bir komut girdiniz...");
        return;
    }
    else if (pid == 0)
    {
        if (strcmp("cat", parsed[0]) == 0)
        {

            if (execv("/bin/cat", parsed) < 0)
            {
                printf("\n yanlis bir komut girdiniz...");
            }
        }
        else if (strcmp("tekrar", parsed[0]) == 0)
        {
            

            if (atoi(parsed[2]) < 0  || sayiBulma(parsed)!=3)
            {
                printf("yanlis bir komut girdiniz...");
            }
            else
            {
                if (execv(parsed[0], parsed) < 0)
                {
                    printf("\n yanlis bir komut girdiniz...");
                }
            }
        }else if (strcmp("islem", parsed[0]) == 0)
        {
            

            if (sayiBulma(parsed)!=4)
            {
                printf("yanlis bir komut girdiniz...");
            }
            else
            {
                if (execv(parsed[0], parsed) < 0)
                {
                    printf("\n yanlis bir komut girdiniz...");
                }
            }
        }
        else if(strcmp("topla",parsed[0])==0){
            printf("yanlis bir komut girdiniz...");
        }
        else if(strcmp("cikar",parsed[0])==0){
            printf("yanlis bir komut girdiniz...");
        }
        else if(strcmp("clear",parsed[0])==0){
            clear();
        }
        else if (execv(parsed[0], parsed) < 0)
        {
            printf("\n yanlis bir komut girdiniz...");
        }
        

        exit(0);
    }
    else
    {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}
int main(int argc, char *argv[])
{
    char *komutlar[150] = {NULL};
    char stringDizisi[150] = {NULL};
    char *elemanlar[150] = {NULL};
    while (1)
    {
        if (!takeInput(stringDizisi))
        {
            continue;
        }
        parseKomut(stringDizisi, komutlar);
        for (int i = 0; i < 150; i++)
        {
            if (komutlar[i] == NULL)
            {
                break;
            }
            else
            {
                parseSpace(komutlar[i], elemanlar);
                if (strcmp("exit", komutlar[i]) == 0)
                {
                    exit(0);
                }
                else
                {
                    execArgs(elemanlar);
                }
            }
        }
    }
    /* code */
    return 0;
}
