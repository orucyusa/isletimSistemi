#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 

int main(int argc, char *argv[])
{   
    char *elemanlar[4];
    elemanlar[0]=argv[1];
    elemanlar[1]=argv[2];
    elemanlar[2]=argv[3];
    elemanlar[3]=NULL;

    int i=fork();
    
    int k;
    if (i==0)
    {
        k = execv(elemanlar[0],elemanlar);
        
        

    }
    else{
        wait(&k);
        if(strcmp("topla",elemanlar[0])==0){
            printf("%s + %s = %d \n",elemanlar[1],elemanlar[2],WEXITSTATUS(k));   //strcmp ile kontrol ediyor...
        }                                                                      // WEXİTSTATUS topla veya çıkardan dönen değeri veriyor 
        else if(strcmp("cikar",elemanlar[0])==0){
             printf("%s - %s = %d \n",elemanlar[1],elemanlar[2],WEXITSTATUS(k));
        }
  
  
    }
    

    return 0;
}

    

