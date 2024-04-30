#include <stdio.h>


int main(int argc, char** argv){
    int result = 0;
    int x = 3;
    int y = 5;

    int i = 0;
    while (i<y){
        result = result + x;
        i++;
    }

    printf("x * y = %d", result);


    return 0;
}
