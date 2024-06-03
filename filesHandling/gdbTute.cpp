#include <iostream>

void fun3(){
    int var3 = 25;
    std::cout << var3 << std::endl;
}

void fun2(){
    int var3 = 515;
    std::cout << var3 << std::endl;
    fun3();
}


void fun1(){
    int var3 = 234;
    std::cout << var3 << std::endl;
    fun2();
}





int main(int agrc, char** argv){
    int mainVar = 10;
    std::cout << mainVar << std::endl;
    fun1();
    return 0;
}
