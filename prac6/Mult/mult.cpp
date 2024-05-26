#include <iostream>
#include <cassert>


int mult(int x, int y){
    // if only one of them is negative, negate that one and perform the multiplication 
    int t1 = 0, t2 = 0;
    if (x < 0)
    {
        t1 = x;
        x = - x;
    } 
    if (y < 0) {
        t2 = y;
        y = -y;
    }

    int sum;
    sum = 0;
    for (int j=y; j != 0; j--){
        sum = sum + x;
    }    

    if (t1 < 0 || t2 < 0) sum = -sum;
    
    if (t1 < 0 && t2 < 0) sum = -sum;
    
    return sum;
}




int main(int argc, char** argv){    
    // assert(mult(-1, 2) == -2);
    // assert(mult(1, -2) == -2);
    // assert(mult(-1, -2) == 2);
    // assert(mult(-1, -2) == -2);

    int x = 3, y = 10;


    // Mult procedure 
    int sum;
    sum = 0;

    int j;
    j = y;

    while (j != 0){
        sum = sum + x;
        j = j - 1;
    }
    std::cout << sum << std::endl;




    return 0;
}