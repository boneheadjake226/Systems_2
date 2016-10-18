#include <stdio.h>
#define XX "xx"
struct x{
  int num;
  char *chars;
} x;

int main(){
  struct x x_arr[20];
  char a[] = "XXX";
  char *a_ptr = a;
  int i = 0;
  
  for(i = 0; i < 20; i++){
    struct x item_x = {.num = i, .chars = XX};
    //char b[] = "XXX";
    //item_x.chars = b;
    x_arr[i] = item_x;
    
  }
  for(i = 0; i < 20; i++){
    struct x print_x = x_arr[i];
    printf("\n%d %s", i, print_x.chars);
  }

  return 0;
}
