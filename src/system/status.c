#include "backend.h"
#include <stdio.h>
#include <string.h>

/*
* fc-top status.c
*/

void get_memory(struct Memory *memory, struct Swap *swap){

    if (memory == NULL) {
        fprintf(stderr, "Error: memory pointer is NULL\n");
        return;
    }

    FILE *fptr = fopen("/proc/meminfo","r"); // Read /proc/meminfo in reading mode.
    char line[256];

    // Memory
    unsigned long MemTotal = 0;
    unsigned long MemUsed = 0;
    unsigned long MemAvailable = 0;
    double UsagePercentage = 0;
    double TotalGiB = 0;
    double UsedGiB = 0;

    // SWAP
    unsigned long SwapTotal = 0;
    unsigned long SwapFree = 0;
    double UsedSwap = 0;
    double TotalGiBSwap = 0;
    double UsedGiBSwap = 0;
    double UsagePercentageSwap = 0;

    while (fgets(line, sizeof(line), fptr))
    {
        if(strstr(line, "MemTotal:") != NULL){
            sscanf(line, "MemTotal: %lu", &MemTotal);
        }
        if(strstr(line, "MemAvailable:") != NULL){
            sscanf(line, "MemAvailable: %lu", &MemAvailable);
        }
        if(strstr(line, "SwapTotal:") != NULL){
            sscanf(line,"SwapTotal: %lu", &SwapTotal);
        }
        if(strstr(line, "SwapFree:") != NULL){
            sscanf(line,"SwapFree: %lu", &SwapFree);
        }
    }

    // Asing Memory
    MemUsed = (MemTotal - MemAvailable);
    UsagePercentage = ((double) MemUsed * 100) / (double) MemTotal;
    TotalGiB = (double) MemTotal / 1048576.0;
    UsedGiB = (double) MemUsed /  1048576.0;

    memory->MemTotal = MemTotal;
    memory->MemAvailable = MemAvailable;
    memory->UsagePercentage = UsagePercentage;
    memory->MemUsed = MemUsed;
    memory->TotalGiB = TotalGiB;
    memory->UsedGiB = UsedGiB;
    
    // Asing Swap
    UsedSwap = (SwapTotal - SwapFree);

    if (SwapTotal > 0) {
        UsagePercentageSwap = ((double) UsedSwap * 100) / (double) SwapTotal;
    } else {
        UsagePercentageSwap = 0.0;
    }

    TotalGiBSwap = (double) SwapTotal / 1048576.0;
    UsedGiBSwap = (double) UsedSwap / 1048576.0;

    swap->SwapTotal = SwapTotal;
    swap->SwapFree = SwapFree;
    swap->UsedSwap = UsedSwap;
    swap->TotalGiB = TotalGiBSwap;
    swap->UsedGiB = UsedGiBSwap;
    swap->UsagePercentage = UsagePercentageSwap;

    fclose(fptr);
    return;
}

void get_uptime(struct Uptime *uptime){

    long second = 0;
    if(uptime == NULL){
        fprintf(stderr, "Error: memory pointer is NULL\n");
        return;
    }

    FILE *fptr = fopen("/proc/uptime","r");
    if(fptr == NULL) {
        fprintf(stderr, "Error: failed to open /proc/uptime\n"); // Read /proc/uptime in read mode
        return;
    }

    fscanf(fptr, "%li", &second);
    fclose(fptr);

    uptime->days    = second / 86400;
    uptime->hours   = (second / 3600) % 24;
    uptime->minutes = (second / 60) % 60;
    uptime->seconds = second % 60;
}
