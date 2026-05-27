#include "backend.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>

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

    // Asignar a la estructura
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

void get_system_info(struct SystemInfo *sys_info) {
    if (sys_info == NULL) return;

    FILE *fptr = fopen("/proc/uptime", "r");
    if (fptr == NULL) {
        fprintf(stderr, "Error: failed to open /proc/uptime\n");
        return;
    }
    long second = 0;
    if (fscanf(fptr, "%ld", &second) == 1) {
        sys_info->days    = second / 86400;
        sys_info->hours   = (second / 3600) % 24;
        sys_info->minutes = (second / 60) % 60;
        sys_info->seconds = second % 60;
    }
    fclose(fptr);

    // Para que se vea el OS, Hostname y Kernel
    struct utsname name;
    if (uname(&name) == 0) {
        strncpy(sys_info->os, name.sysname, sizeof(sys_info->os) - 1);
        sys_info->os[sizeof(sys_info->os) - 1] = '\0';
        
        strncpy(sys_info->hostname, name.nodename, sizeof(sys_info->hostname) - 1);
        sys_info->hostname[sizeof(sys_info->hostname) - 1] = '\0';
        
        strncpy(sys_info->kernel, name.release, sizeof(sys_info->kernel) - 1);
        sys_info->kernel[sizeof(sys_info->kernel) - 1] = '\0';
    } else {
        strcpy(sys_info->os, "Linux");
        strcpy(sys_info->hostname, "unknown");
        strcpy(sys_info->kernel, "unknown");
    }

    // VM (Hyrland / X11 / Wayland)
    strcpy(sys_info->vm_de, "unknown");
    if (getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL) {
        strcpy(sys_info->vm_de, "Hyprland (Wayland)");
    } else {
        char *desktop = getenv("XDG_CURRENT_DESKTOP");
        if (!desktop) desktop = getenv("DESKTOP_SESSION");
        if (desktop) {
            strncpy(sys_info->vm_de, desktop, sizeof(sys_info->vm_de) - 1);
            sys_info->vm_de[sizeof(sys_info->vm_de) - 1] = '\0';
        }
    }


    // Distro
    strcpy(sys_info->distro, "Linux");
    FILE *os_file = fopen("/etc/os-release", "r");
    if (os_file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), os_file)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *start = line + 12;
                if (*start == '"') start++; 
                char *end = strchr(start, '"');
                if (!end) end = strchr(start, '\n');
                if (end) *end = '\0';
                strncpy(sys_info->distro, start, sizeof(sys_info->distro) - 1);
                sys_info->distro[sizeof(sys_info->distro) - 1] = '\0';
                break;
            }
        }
        fclose(os_file);
    }

    // Shell
    char *shell_env = getenv("SHELL");
    if (shell_env) {
        char *base = strrchr(shell_env, '/');
        strncpy(sys_info->shell, base ? base + 1 : shell_env, sizeof(sys_info->shell) - 1);
        sys_info->shell[sizeof(sys_info->shell) - 1] = '\0';
    } else {
        strcpy(sys_info->shell, "unknown");
    }

    // CPU
    strcpy(sys_info->cpu, "unknown");
    FILE *cpu_file = fopen("/proc/cpuinfo", "r");
    if (cpu_file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), cpu_file)) {
            if (strncmp(line, "model name", 10) == 0) {
                char *colon = strchr(line, ':');
                if (colon) {
                    colon++; // saltar el ':'
                    while (*colon == ' ') colon++; // saltar espacios en blanco
                    
                    // Eliminar salto de línea al final
                    char *newline = strchr(colon, '\n');
                    if (newline) *newline = '\0';
                    
                    strncpy(sys_info->cpu, colon, sizeof(sys_info->cpu) - 1);
                    sys_info->cpu[sizeof(sys_info->cpu) - 1] = '\0';
                    break; // Solo necesitamos el nombre del primer núcleo
                }
            }
        }
        fclose(cpu_file);
    }

    // lspci para GPU
    strcpy(sys_info->gpu, "unknown");
    FILE *gpu_pipe = popen("lspci | grep -E -i \"vga|3d|display\" 2>/dev/null", "r");
    if (gpu_pipe != NULL) {
        char line[256];
        if (fgets(line, sizeof(line), gpu_pipe)) {
            char *colon = strchr(line, ':');
            if (colon) {
                colon = strchr(colon + 1, ':');
                if (colon) {
                    colon++;
                    while (*colon == ' ') colon++;
                    char *newline = strchr(colon, '\n');
                    if (newline) *newline = '\0';
                    
                    strncpy(sys_info->gpu, colon, sizeof(sys_info->gpu) - 1);
                    sys_info->gpu[sizeof(sys_info->gpu) - 1] = '\0';
                } else {
                    colon = strchr(line, ':');
                    colon++;
                    while (*colon == ' ') colon++;
                    char *newline = strchr(colon, '\n');
                    if (newline) *newline = '\0';
                    strncpy(sys_info->gpu, colon, sizeof(sys_info->gpu) - 1);
                }
            }
        }
        pclose(gpu_pipe);
    }

    // tipo de FS
FILE *mounts = fopen("/proc/mounts", "r");
    if (mounts != NULL) {
        char line[512];
        while (fgets(line, sizeof(line), mounts)) {
            char dev[128], mount_p[128], type[64];
            if (sscanf(line, "%127s %127s %63s", dev, mount_p, type) >= 3) {
                if (strcmp(mount_p, "/") == 0) {
                    strncpy(sys_info->fs_type, type, sizeof(sys_info->fs_type) - 1);
                    sys_info->fs_type[sizeof(sys_info->fs_type) - 1] = '\0';
                    break;
                }
            }
        }
        fclose(mounts);
    }

    // calcular el tamaño con statvfs
struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        double total_bytes = (double)vfs.f_blocks * vfs.f_frsize;
        double free_bytes = (double)vfs.f_bfree * vfs.f_frsize;
        double used_bytes = total_bytes - free_bytes;

        sys_info->fs_total_gib = total_bytes / (1024.0 * 1024.0 * 1024.0);
        sys_info->fs_used_gib = used_bytes / (1024.0 * 1024.0 * 1024.0);
        if (total_bytes > 0) {
            sys_info->fs_perc = (used_bytes / total_bytes) * 100.0;
        }
    }
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
