# fc-top
A resource monitor for Linux written in C.

## Description
`fc-top` is a terminal-based monitor that provides real-time system information. It displays memory usage, swap usage, uptime, and a list of running processes. It is developed using the `ncurses` library and offers an interactive panel.

## Features
* **System Information:** Displays the percentage and total memory and swap usage.

* **Uptime:** Displays system uptime in days, hours, minutes, and seconds.

* **Process Management:** Displays a list of processes including the PID, status, RAM usage, and name of each process.

* **Interactive Interface:** Allows browsing the process list and terminating processes directly from the panel.

* **CPU:** Displays the model of CPU.

* **GPU:** Displays the model of GPU.

* **Distro**

* **Kernel**

* **Disk**

* **SO**

* **Host**

## Prerequisites
* Linux operating system with access to the `/proc` directory and files.

* `gcc` compiler.

* The `make` tool.

* The `ncurses` library installed on the system (`libncursesw`).

## Compiling and Running
You can build and manage the application using the `Makefile` included in the project:

* **Compile the project:** Running the `make` command will compile the source code and generate the final executable in the `build/fc-top` directory.

* **Run the monitor:** The `make run` command grants the necessary permissions and runs the application automatically.

* **Clean the project:** The `make clean` command safely removes the compiled executable.

## Controls
While running the tool, you can use the following keys to interact with the interface:
* **Up/Down Arrow Keys:** Navigate through the different processes in the list.

* * **Key `k` or `K`:** Send a termination signal to the selected process. The application will prompt you for on-screen confirmation before terminating the process.

* **Key `q`:** Exit the application and return to the terminal.
