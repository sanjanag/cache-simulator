#include<iostream>
#include <fstream>
#include <sstream>
#include <string>


int main()
{/*
  char s;
  unsigned addr;
  scanf("%c%x", &s, &addr);
  printf("%c %u\n",s,addr+1);*/
  
  FILE*  fp;
  fp = fopen("try.out","r");
  long long int i=1;
  char state;
  unsigned addr;
  while(fscanf(fp,"%c%x , ",&state, &addr)!= EOF)
    {
      //      printf("%u\n",addr);
      i++;
    }
  printf("i = %lld\n",i);
   return 0;
}
