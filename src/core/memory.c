#include "../../include/backend.h"
#include <stdio.h>
#include <string.h>

/*
* fc-top memory.c
*/

/*
* Function: get_memory
* Function that reads /proc/meminfo to obtain the current state of memory.
* memory: Memory structure in which the data will be stored.
* 
* return: void
*/
void get_memory(struct Memory *memory){

    if (memory == NULL) {
        fprintf(stderr, "Error: memory pointer is NULL\n");
        return;
    }

    FILE *fptr = fopen("/proc/meminfo","r"); // Read /proc/meminfo in reading mode.
    char line[256];
    unsigned long MemTotal = 0;
    unsigned long MemUsed = 0;
    unsigned long MemAvailable = 0;
    double UsagePercentage = 0;

    while (fgets(line, sizeof(line), fptr))
    {
        if(strstr(line, "MemTotal:") != NULL){
            sscanf(line, "MemTotal: %lu", &MemTotal);
        }
        if(strstr(line, "MemAvailable:") != NULL){
            sscanf(line, "MemAvailable: %lu", &MemAvailable);
        }
    }

    MemUsed = (MemTotal - MemAvailable);
    UsagePercentage = ((double) MemUsed * 100) / (double) MemTotal;

    memory->MemTotal = MemTotal;
    memory->MemAvailable = MemAvailable;
    memory->UsagePercentage = UsagePercentage;
    memory->MemUsed = MemUsed;
    
    fclose(fptr);
    return;
}