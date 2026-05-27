#ifndef BACKEND_H
#define BACKEND_H

#define FCTOP_VERSION "0.4.0"

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
 * @struct Swap
 */
struct Swap{
    unsigned long SwapTotal;
    unsigned long SwapFree;
    double UsedSwap;
    double TotalGiB;
    double UsedGiB;
    double UsagePercentage;
};

/**
 * @struct Hardware
 * @brief
 */
struct Hardware{
};


/**
 * @struct Uptime
 * @brief Represents the total time since the last system startup.
 * This structure contains the information necessary to represent system time in days, hours, minutes, and seconds.
 *
 */
struct Uptime{
    long seconds;
    int minutes;
    int hours;
    int days;
};

/**
 * @struct Process
 */
struct Process{
    int pid; /**/
    char name[256];
    char state;
    unsigned long rss;
};

/**
 *  @struct ProcessList
 */
struct ProcessList{
    struct Process *processes;
    int count;
    int capacity;
};

/**
 * @brief Function that reads /proc/meminfo to obtain the current state of memory.
 *
 * @param memory Memory structure in which the data will be stored.
 * @param swap Swap structure in which the data will be stored.
 *
 * @return void
 */
void get_memory(struct Memory *memory, struct Swap *swap);

/**
 * @brief Function that reads /proc/uptime to obtain the current system time status.}
 * @param uptime Uptime structure in which the data will be stored.
 *
 * @return void
 */
void get_uptime(struct Uptime *uptime);

/**
 *
 */
void get_processes(struct ProcessList *plist);

/**
 *
 */
void free_processes(struct ProcessList *plist);

#endif
