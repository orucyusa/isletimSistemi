#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    char s[150];

    read(3, &s, 150); // ilk once pipe oku
    FILE *file;
    file = fopen("sonuc.txt", "a");
    if (file == NULL)
    {
        printf("sonuc.c dosya acilmadi.\n");
    }
    else
    {
        if (strlen(s) > 0)
        {
            fputs(s, file); //dosyaya kaydebilir.
        }
        fclose(file);
    }
    return 0;
}
