#include <stdio.h>
#include <string.h>
 
#define SIZE 40
 
int main(void)
{
  char buffer1[SIZE] = "xxxx ccccc";
  char * ptr;
  int    ch = 'p';
 
  ptr = strchr( buffer1, ch );
  printf( "The first occurrence of %c in '%s' is '%s'\n",
            ch, buffer1, ptr );
 
}
