#include "backend.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

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

void get_processes(struct ProcessList *plist) {

    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        return;
    }

    plist->count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        if (!isdigit(entry->d_name[0])) {
            continue;
        }

        if (plist->count >= plist->capacity) {
            plist->capacity = (plist->capacity == 0) ? 256 : plist->capacity * 2;
            plist->processes = realloc(plist->processes, plist->capacity * sizeof(struct Process));
        }

        int pid = atoi(entry->d_name);
        struct Process *p = &plist->processes[plist->count];
        p->pid = pid;

        char path[256];
        snprintf(path, sizeof(path), "/proc/%d/stat", pid);
        FILE *f = fopen(path, "r");

        if (f) {
            char line[1024];
            if (fgets(line, sizeof(line), f)) {
                char *start = strchr(line, '(');
                char *end = strrchr(line, ')');

                if (start && end && end > start) {
                    int name_len = end - start - 1;
                    if (name_len >= sizeof(p->name)) name_len = sizeof(p->name) - 1;
                    strncpy(p->name, start + 1, name_len);
                    p->name[name_len] = '\0';

                    sscanf(end + 2, "%c", &p->state);

                    char *num_ptr = end + 4;
                    int offset;

                    for (int i = 0; i < 21; i++) {
                        if (sscanf(num_ptr, "%ld%n", &p->rss, &offset) != 1) {
                            break;
                        }
                        num_ptr += offset;
                    }
                    p->rss = p->rss * sysconf(_SC_PAGESIZE);

                }
            }
            fclose(f);
        }

        plist->count++;
    }

    closedir(dir);
}

void free_processes(struct ProcessList *plist) {
    if (plist->processes != NULL) {
        free(plist->processes);
        plist->processes = NULL;
    }
    plist->capacity = 0;
    plist->count = 0;
}
