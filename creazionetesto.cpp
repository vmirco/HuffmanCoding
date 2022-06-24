#include <iostream>
#include <time.h>
using namespace std;

int main(){
  char chars[10] = {'a','b','c','d','e','f',' ',',','.','x'};
  srand(time(NULL));
  int index;
  int counter = 0;
  for(int i = 0; i < 100000; i++){
    index = rand()%10;
    cout << chars[index];
    counter++;
    if(counter == 100){
      counter = 0;
      cout << endl;
      }
    }
}
