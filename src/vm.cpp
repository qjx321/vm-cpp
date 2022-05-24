#include"../include/vm.h"

#include<iostream>

int main(int argc, char* argv[])
{

    if(argc < 2)
    {
        cerr << "Please specify a binary" << endl;
        exit(-1);
    }

    char* filename = argv[1];
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