#include<cstdio>
#include<cstdint>
#include<cstdlib>

int main()
{
    int32_t instruct[] = 
    {
        3, 72, 0,
        52, 0, 0,
        3, 101, 0,
        52, 0, 0,
        3, 108, 0,
        52, 0, 0,
        3, 108, 0,
        52, 0, 0,
        3, 111, 0,
        52, 0, 0,
        3, 32, 0,
        52, 0, 0,
        3, 87, 0,
        52, 0, 0,
        3, 111, 0,
        52, 0, 0,
        3, 114, 0,
        52, 0, 0,
        3, 108, 0,
        52, 0, 0,
        3, 100, 0,
        52, 0, 0,
        3, 10, 0,
        52, 0, 0,
        54, 0, 0
    };

    FILE* outfile = fopen("../example/hello", "wb");
    if (!outfile)
    {
        exit(-1);
    }
    uint32_t instruct_size = sizeof(instruct) / sizeof(int32_t);
    for(uint32_t i = 0; i < instruct_size; i++)
    {
        fwrite(&instruct[i], sizeof(int32_t), 1, outfile);
    }
    fclose(outfile);

    return 0;
}