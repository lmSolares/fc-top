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
 * @struct SystemInfo
 * @brief Representa la información general del sistema 
 */
struct SystemInfo {
    long seconds;
    int minutes;
    int hours;
    int days;
    char os[32];
    char hostname[64];
    char kernel[64];
    char shell[32];
    char cpu[128];
    char gpu[128];
    char distro[64];
    char vm_de[64];
    char fs_type[32];
    double fs_total_gib;
    double fs_used_gib;
    double fs_perc;
};

/**
 * @brief Función que obtiene la información del sistema operativo, entorno y tiempo de actividad.
 */
void get_system_info(struct SystemInfo *sys_info);

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
void get_system_info(struct SystemInfo *sys_info);
void get_processes(struct ProcessList *plist);
void free_processes(struct ProcessList *plist);


#endif
