#ifndef BACKEND_H
#define BACKEND_H

#define FCTOP_VERSION "0.1.0"

/*
* fc-top backend.h
*/

/**
* @struct Memory
* @brief It represents the state of memory.
*
* This structure contains the data necessary to represent memory usage.
*/
struct Memory{
    unsigned long MemTotal; /* Total usable ram. */
    unsigned long MemUsed; /* Total memory used. */
    unsigned long MemAvailable; /* An estimate of how much memory is available for starting new applications, without swapping. */
    double TotalGiB; /* Total memory on GiB. */
    double UsedGiB; /* Total used memory in GiB. */
    double UsagePercentage; /* Percentage of memory used. */
};

/**
* @brief Function that reads /proc/meminfo to obtain the current state of memory.
* 
* @param memory Memory structure in which the data will be stored.
* 
* @return void
*/
void get_memory(struct Memory *memory);

/**
 * 
 */
struct Uptime{
    long seconds;
    int minutes;
    int hours;
    int days;
};

#endif