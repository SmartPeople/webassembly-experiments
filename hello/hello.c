#include <stdio.h>
#include <string.h>

/* function returning the max between two numbers */
int max(int num1, int num2) {

   /* local variable declaration */
   int result;
 
   if (num1 > num2)
      result = num1;
   else
      result = num2;
 
   return result; 
}

const char *  say_hi() {
  const char *x = "Hi!";
  return x;
}

//int main(int argc, char ** argv) {
//  printf("Hello, world!");
//}
