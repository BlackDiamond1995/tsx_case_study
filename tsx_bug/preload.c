#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define s_t unsigned long long

inline __attribute__((always_inline)) void maccess_preload(volatile void* p)
{
  asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (p)
    : "rax");
}

void traverse_preload(void* addr, s_t addl){
    if ((long)addr == 0xffffffffffffffff){
        return;
    }
    for (s_t i=3; i<addl - 3; i++){
        maccess_preload(addr + i - 3);
        maccess_preload(addr + i - 2);
        maccess_preload(addr + i - 1);
        maccess_preload(addr + i - 0);
        maccess_preload(addr + i + 1);
        maccess_preload(addr + i + 2);
        maccess_preload(addr + i + 3);
    }
}

void preload(char targetfileName[])
{
    int fd_target = open(targetfileName, O_RDONLY);
    if (fd_target == -1)
    {
        return;
    }
    s_t size_target = (s_t)lseek(fd_target, 0, SEEK_END);
    int pc_target = size_target / (4096);
    unsigned char v[pc_target];
    void* addr_target = mmap(NULL, size_target, PROT_READ, MAP_SHARED, fd_target, 0);
    puts("preloading...");
    traverse_preload(addr_target, size_target);
}

void reload(char targetfileName[])
{
    puts("reloading...");
    int fd_target = open(targetfileName, O_RDONLY);
    if (fd_target == -1)
    {
        return;
    }
    s_t size_target = (s_t)lseek(fd_target, 0, SEEK_END);
    int pc_target = size_target / (4096);
    unsigned char v[pc_target];
    void* addr_target = mmap(NULL, size_target, PROT_READ, MAP_SHARED, fd_target, 0);
    mincore(addr_target, size_target, v);
    for (int i=0; i<pc_target; i++)
    {
        if (v[i] == 0)
        {
            traverse_preload(addr_target+(i*4096), 4096);
            break;
        }
    }
}

void stop_tsx(){return;}