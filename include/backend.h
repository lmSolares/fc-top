#ifndef BACKEND_H
#define BACKEND_H

#define FCTOP_VERSION "0.1.0"

/*
* fc-top backend.h
*/

struct Memory{
    unsigned long MemTotal; // Total usable ram
    unsigned long MemUsed; // Total memory used
    unsigned long MemAvailable; // An estimate of how much memory is available for starting new applications, without swapping.
    double TotalGiB; // Total memory on GiB
    double UsedGiB; // Total used memory in GiB
    double UsagePercentage; // Percentage of memory used
};

void get_memory(struct Memory *memory);

#endif