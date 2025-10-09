cp $(find -iname "opendir.c") ../minishell/assets/libc_source

## stdlib
	malloc, free, exit, getenv

## unistd
	write, access, read, close, unlink
	fork, pipe, execve, dup, dup2
	getcwd, chdir, isatty, ttyname, ttyslot

## dirent
	opendir, readdir, closedir,

## fcntl
	open

## stdio
	printf, perror

## signal
	signal, sigaction, sigemptyset, sigaddset, kill

## sys/wait
	wait, waitpid, wait3, wait4

## sys/stat
	stat, lstat, fstat

## string
	strerror

## sys/ioctl.h
	ioctl

## termcap.h
	tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

## readline/readline
	readline, rl_on_new_line, rl_replace_line, rl_redisplay

## readline/history
	rl_clear_history, add_history
