#include <stdio.h>

int * getRandom( )
{
  static int  r[10];
  int i;

  r[0]=0;
  r[1]=1;
  r[2]=2;
  r[3]=3;
  r[4]=4;
  r[5]=5;
  r[6]=6;
  r[7]=7;
  r[8]=8;
  r[9]=9;
  for ( i = 0; i < 10; ++i)
  {
    printf( "r[%d] = %d\n", i, r[i]);
  }
  return r;
}

int main ()
{
   int *p;
   int i;
   p = getRandom();
   for ( i = 0; i < 10; i++ )
   {
       //printf( "*(p + %d) : %d\n", i, *(p + i));
       printf( "p:) %d\n",p[i]);
   }
   return 0;
}
