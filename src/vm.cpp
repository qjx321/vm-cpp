#include"../include/vm.h"

#include<iostream>

int main()
{
    char* filename = (char*)"../example/binary";
    VM vm(filename);
    Instruct instruct;


    while(1)
    {
        instruct = vm.read_instruct();

        vm.execute(instruct);
    }
    vm.print_data();

    return 0;
}