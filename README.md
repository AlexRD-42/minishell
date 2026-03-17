The goal of this project was to engineer a fully functional Bash-like shell from scratch using C. Unlike standard implementations, this version was developed with a deterministic zero-dynamic-allocation architecture. We eliminated the non-determinism and fragmentation risks associated with heap memory, making it suitable for high-reliability systems. 

100% Malloc-free: Developed without dynamic allocation to ensure memory predictability. 

Proprietary Line Editor: A custom editor built by managing TTY attributes and raw terminal modes (termios) to handle user input without standard libraries. 

Safety-Critical Rigor: Applied a testing methodology against Bash to ensure 100% compliance with expected behaviours. 

Instructions
Compilation: Run make in the root directory.
Execution: ./minishell

Usage: Supports standard command execution, pipes, redirections, environment variable management, command history, "*", && and || operators.
