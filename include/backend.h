#ifndef BACKEND_H
#define BACKEND_H

/*
* fc-top backend.h
*/

struct Memory{
    unsigned long MemTotal; // Total usable ram
    unsigned long MemUsed; // Total memory used
    unsigned long MemAvailable; // An estimate of how much memory is available for starting new applications, without swapping.
    double UsagePercentage; // Percentage of memory used
};

void get_memory(struct Memory *memory);

#endif