#ifndef __VM__
#define __VM__

#include<array>
#include<cstdint>
#include<cstdio>
#include<cerrno>
#include<cstdlib>
#include"instruct.h"
#include<iostream>


using namespace std;
class VM
{
    private:
        //需要一个栈，一个内存，内存又包括代码段和数据段
        array<int32_t, 1000> stack = {0};
        array<int32_t, 1000> code  = {0};
        array<int32_t, 1000> data  = {0};


        //需要几个寄存器
        int32_t *rip, *rsp, *rbp;
        int32_t *regs[4] = {NULL, NULL, NULL, NULL};

        //标志位寄存器
        int32_t flag;

        //保存代码段的大小
        uint32_t code_size;

        //指令集
        enum OPCODE {PUSH_DATA, PUSH_REG, POP, MOV_DATA_TO_REG, MOV_MEM_TO_REG, ADD_REGS, SUB_REGS, 
                    MUL_REGS, DIV_REGS, XOR_REGS, MOV_REG_TO_REG, MOV_REG_TO_DATA,
                    MOV_REG_BY_REG, INC, DEC, CMP_REG_TO_DATA, CMP_REGS, JLE,
                    CALL, PUSH_RBP, MOV_RSP_RBP, MOV_RBP_RSP, POP_RBP, RET, MOV_BY_OFF, 
                    ADD_DATA_TO_REG, SUB_DATA_TO_REG, MOV_MEM_TO_DATA_BY_REG, 
                    MOV_REG_TO_DATA_BY_REG, JNZ, JMP, };

        //系统调用
        enum SYSCALL {READ = 50, WRITE_D, WRITE_C, GETC, EXIT, FFLUSH, };
    
    public:
        //默认构造函数，初始化rip，rsp，rbp
        VM() : rip(&(code[0])), rsp(&(stack[0])), rbp(&(stack[0])) 
        {
            for (uint32_t i = 0; i < 4; i++)
            {
                regs[i] = (int32_t*)malloc(sizeof(int32_t));
            }
        }

        //带参构造函数, 载入可执行文件到code段
        VM(char* filename) : VM()
        {
            code_size = load_src2code(filename);
        }

        //析构函数
        ~VM()
        {
            for (uint32_t i = 0; i < 4; i++)
            {
                free(regs[i]);
                regs[i] = NULL;
            }
        }

        //设置标志位寄存器
        void set_flag(int32_t x);

        //载入可执行文件到code段
        uint32_t load_src2code(char* filename);

        //获取代码段大小
        uint32_t get_code_size() { return code_size; }

        //打印代码段数据
        void print_code();

        //逐条读取指令，每条三字节
        Instruct read_instruct();

        //执行一条指令
        void execute(Instruct instruct);

        //将一个立即数压栈
        void push(int32_t item);

        //将一个寄存器压栈
        void push(int32_t* reg);

        //出栈
        void pop(int32_t* reg);

        //将一个立即数送入寄存器
        void mov(int32_t item, int32_t* reg);

        //在寄存器之间传送数据
        void mov(int32_t* l, int32_t* r);

        //打印栈信息
        void print_stack();

        //打印四个通用寄存器的信息
        void print_regs();

        //将两个寄存器相加， 减，乘，除，异或
        void add(int32_t* l, int32_t* r);
        void sub(int32_t* l, int32_t* r);
        void mul(int32_t* l, int32_t* r);
        void div(int32_t* l, int32_t* r);
        void myxor(int32_t* l, int32_t* r);

        //系统调用
        void myread();
        void mywrite_D();
        void mywrite_C();
        void mygetc();
        void myexit();

        //将寄存器的值存入data
        void mov_reg2data(int32_t* reg, int32_t index);

        //打印data的内容
        void print_data();

        //将一个寄存器的值写入另一个寄存器为下标的data中
        void mov_reg_by_reg(int32_t* l, int32_t* r);

        //将寄存器递增递减
        void myinc(int32_t* reg);
        void mydec(int32_t* reg);

        //比较，寄存器与数据，寄存器与寄存器
        void cmp(int32_t* reg, int32_t r);
        void cmp(int32_t* l, int32_t* r);

        //跳转指令
        void jle(int32_t line);
        void jnz(int32_t line);
        void myjmp(int32_t line);

        //从data中取数据到寄存器
        void mov_mem_to_reg(int32_t index, int32_t* reg);

        //函数调用
        void call(int32_t line, int32_t ret);

        //刷新输入缓冲区
        void myfflush();

        //保存rbp的值到栈
        void push_rbp();

        //将rbp置为rsp的值
        void mov_rsp_rbp();

        //将rsp置为rbp的值
        void mov_rbp_rsp();

        //通过栈地址查找栈下标
        int32_t get_index_by_addr(int64_t addr);

        //恢复rbp的值
        void pop_rbp();

        //函数调用返回
        void myret();

        //根据对于rbp的偏移取值放入寄存器
        void mov_by_off(int32_t off, int32_t* reg);

        //往寄存器加上或减去一个立即数
        void add_data_to_reg(int32_t item, int32_t* reg);
        void sub_data_to_reg(int32_t item, int32_t* reg);
};

void VM::myjmp(int32_t line)
{
    rip = &code[line*3];
}

void VM::jnz(int32_t line)
{
    if(flag != 0)
        rip = &code[line*3];
}

void VM::sub_data_to_reg(int32_t item, int32_t* reg)
{
    *reg -= item;
}

void VM::add_data_to_reg(int32_t item, int32_t* reg)
{
    *reg += item;
}

void VM::mov_by_off(int32_t off, int32_t* reg)
{
    *reg = *(rbp+off);
}

void VM::myret()
{
    rip = &code[(*rsp)*3];
    rsp--;
}

void VM::pop_rbp()
{
    rbp = &stack[*rsp];
    rsp--;
}

int32_t VM::get_index_by_addr(int64_t addr)
{
    int32_t i = 0;
    while((int32_t*)addr != &stack[i])
    {
        i++;
    }
    return i;
    
}

void VM::mov_rbp_rsp()
{
    rsp = rbp;
}

void VM::mov_rsp_rbp()
{
    rbp = rsp;
}

void VM::push_rbp()
{
    char buf[30];
    int64_t addr;
    sprintf(buf, "%p", rbp);
    sscanf(buf, "%lx", &addr);
    int32_t index = get_index_by_addr(addr);
    push(index);
}

void VM::myfflush()
{
    fflush(stdin);
}

void VM::call(int32_t line, int32_t ret)
{
    push(ret);
    rip = &code[line*3];
}

void VM::mov_mem_to_reg(int32_t index, int32_t* reg)
{
    *reg = data[index];
}

void VM::myexit()
{
    exit(0);
}

void VM::jle(int32_t line)
{
    if (flag != 1)
        rip = &code[line*3];
}

void VM::cmp(int32_t* l, int32_t* r)
{
    int32_t temp;
    if (*l == *r)
        temp = 0;
    else if (*l < *r)
        temp = -1;
    else 
        temp = 1;
    set_flag(temp);
}

void VM::cmp(int32_t* reg, int32_t r)
{
    int32_t temp;
    if(*reg == r) 
        temp = 0;
    else if(*reg < r)
        temp = -1;
    else 
        temp = 1;
    set_flag(temp);
}


void VM::myinc(int32_t* reg)
{
    (*reg)++;
}

void VM::mydec(int32_t* reg)
{
    (*reg)--;
}

void VM::mov_reg_by_reg(int32_t* l, int32_t* r)
{
    data[*r] = *l;
}

void VM::set_flag(int32_t x)
{
    flag = x;
}

void VM::mygetc()
{
    int32_t ch = (int32_t)getchar();
    *regs[0] = ch;
}

void VM::print_data()
{
    for(uint32_t i = 0; data[i] != 0; i++)
    {
        printf("%d ", data[i]);
    }
    cout << endl;
}

void VM::mov_reg2data(int32_t* reg, int32_t index)
{
    data[index] = *reg;
}

void VM::mywrite_C()
{
    printf("%c", *regs[0]);
}

void VM::mov(int32_t* l, int32_t* r)
{
    *r = *l;
}

void VM::mywrite_D()
{
    printf("%d", *regs[0]);
}

void VM::myread()
{
    scanf("%d", regs[0]);
}

void VM::myxor(int32_t* l, int32_t* r)
{
    *r ^= *l;
}

void VM::div(int32_t* l, int32_t* r)
{
    *r /= *l;
}

void VM::mul(int32_t* l, int32_t* r)
{
    *r *= *l;
}

void VM::sub(int32_t* l, int32_t* r)
{
    *r -= *l;
}

void VM::add(int32_t* l, int32_t* r)
{
    *r += *l;
}

void VM::print_regs()
{
    printf("===================REGISTER==================\n");
    for(uint32_t i = 0; i < 4; i++)
    {
        printf("r%d: %08x\n", i, *(regs[i]));
    }
}

void VM::print_stack()
{
    printf("====================STACK====================\n");
    int32_t* ptemp = &(stack[0]);
    while(ptemp != rsp)
    {
        printf("%08x ", *ptemp);
        ptemp++;
    }
    printf("%08x\n", *ptemp);
}

void VM::mov(int32_t item, int32_t* reg)
{
    *reg = item;
}

void VM::pop(int32_t* reg)
{
    *reg = *rsp--;
}


uint32_t VM::load_src2code(char* filename)
{
    FILE* src = fopen(filename, "rb");
    uint32_t count = 0;
    if (!src)
    {
        fprintf(stderr, "Failed to open file '%s'", filename);
        exit(-1);
    }
    while(fread(&code[count], sizeof(int32_t), 1, src) == 1) { count++; }
    fclose(src);
    return count;
}

void VM::print_code()
{
    for(uint32_t i = 0; i < code_size; i++)
    {
        printf("%08x ", code[i]);
    }
}

Instruct VM::read_instruct()
{
    Instruct temp;
    temp.opcode = *rip;
    temp.left   = *(rip+1);
    temp.right  = *(rip+2);
    rip +=3;
    return temp;
}

void VM::execute(Instruct instruct)
{
    int32_t op, l, r;
    op = instruct.opcode;
    l  = instruct.left;
    r  = instruct.right;

    switch (op)
    {
    case PUSH_DATA:
        push((int32_t)l);
        break;
    case PUSH_REG:
        push((int32_t*)regs[l]);
        break;
    case POP:
        pop(regs[l]);
        break;
    case MOV_DATA_TO_REG:
        mov(l, regs[r]);
        break;
    case MOV_MEM_TO_REG:
        mov_mem_to_reg(l, regs[r]);
        break;
    case ADD_REGS:
        add(regs[l], regs[r]);
        break;
    case SUB_REGS:
        sub(regs[l], regs[r]);
        break;
    case MUL_REGS:
        mul(regs[l], regs[r]);
        break;
    case DIV_REGS:
        div(regs[l], regs[r]);
        break;
    case XOR_REGS:
        myxor(regs[l], regs[r]);
        break;
    case READ:
        myread();
        break;
    case WRITE_D:
        mywrite_D();
        break;
    case MOV_REG_TO_REG:
        mov(regs[l], regs[r]);
        break;
    case WRITE_C:
        mywrite_C();
        break;
    case MOV_REG_TO_DATA:
        mov_reg2data(regs[l], r);
        break;
    case GETC:
        mygetc();
        break;
    case MOV_REG_BY_REG:
        mov_reg_by_reg(regs[l], regs[r]);
        break;
    case INC:
        myinc(regs[l]);
        break;
    case DEC:
        mydec(regs[l]);
        break;
    case CMP_REG_TO_DATA:
        cmp(regs[l], r);
        break;
    case CMP_REGS:
        cmp(regs[l], regs[r]);
        break;
    case JLE:
        jle(l);
        break;
    case EXIT:
        myexit();
        break;
    case CALL:
        call(l, r);
        break;
    case FFLUSH:
        myfflush();
        break;
    case PUSH_RBP:
        push_rbp();
        break;
    case MOV_RSP_RBP:
        mov_rsp_rbp();
        break;
    case MOV_RBP_RSP:
        mov_rbp_rsp();
        break;
    case POP_RBP:
        pop_rbp();
        break;
    case RET:
        myret();
        break;
    case MOV_BY_OFF:
        mov_by_off(l, regs[r]);
        break;
    case ADD_DATA_TO_REG:
        add_data_to_reg(l, regs[r]);
        break;
    case SUB_DATA_TO_REG:
        sub_data_to_reg(l, regs[r]);
        break;
    case MOV_MEM_TO_DATA_BY_REG:
        mov_mem_to_reg(*regs[l], regs[r]);
        break;
    case MOV_REG_TO_DATA_BY_REG:
        mov_reg2data(regs[l], *regs[r]);
        break;
    case JNZ:
        jnz(l);
        break;
    case JMP:
        myjmp(l);
        break;
    
    default:
        break;
    }

}

void VM::push(int32_t item)
{
    *(++rsp) = item;
}

void VM::push(int32_t* reg)
{
    *(++rsp) = *reg;
}

#endif

