#include"../include/vm.h"

#include<iostream>

int main()
{
    char* filename = (char*)"test.txt";
    VM vm(filename);
    Instruct instruct;

    // cout << vm.get_code_size() << endl;
    // vm.print_code();
    // cout << "\n";

    // for (uint32_t i =0; i < (vm.get_code_size())/3; i++)
    while(1)
    {
        instruct = vm.read_instruct();
        // printf("\ninstruct: %08x %08x %08x\n", instruct.opcode, instruct.left, instruct.right);
        vm.execute(instruct);
        // vm.print_stack();
        // vm.print_regs();
    }
    vm.print_data();

    return 0;
}