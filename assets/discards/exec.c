// var1="echo hello $VAR2"
// var1="echo hello "

// $var ls | > out0 | (<< EOF cat > out1) | << EOF cat | grep '$VAR' && << EOF2

// Things to watch out for:
// No commands in pipe region

// I should perform variable expansion as a first step, patching the token pointers;
// Left to right read until I find a pipe, henceforth named pipe region;
// Within a pipe region, the first word type (meaning not an operator) is the command to execute, 
// and all WORDS inside the pipe region that follow are arguments
// There may be multiple redirects, creating multiple files. Because its left to right the last one is the one thats in effect 
// Redirects breaks the pipeline in the sense that it will override what is being sent or received
// It is possible that one pipe region has no commands, but it may not be empty

// Parenthesis:
// cmd1 (cmd2 && (cmd3 || cmd4)):
// SH1 executes "cmd1" and sends "cmd2 && (cmd3 || cmd4)" to SH2, 
// SH2 executes "cmd2" and sends "cmd3 || cmd4" 			to SH3, 
// SH3 executes "cmd3 || cmd4"
// A syntactic unit should never have parenthesis inside

// $var ls | > out0 | wc > out1 "ahsdecho hello jkala" | << EOF cat | grep '$VAR'
// Token WORD: PTR *, 9
// Token WORD: PTR *, 20

// echo > out1 arg1 "arg2"
// echo arg1 arg2
// echo alex'dasds'$VAR
// alex WORD = alex
// dasds WORD | SINGLE QUOTE = dasds
// $VAR WORD = deimling

	// char	*argv[1024];			// 8kb
	// char	buffer[1024 * 1024];	// 1 mb

static const
char	*find_path(const char **envp)
{
	const char	*ptr;

	if (envp == NULL)
		return (NULL);
	ptr = *envp;
	while (ptr != NULL)
	{
		if (ptr[0] == 'P' && ptr[1] == 'A' && ptr[2] == 'T' && ptr[3] == 'H'
			&& ptr[4] == '=')
			return (ptr + 5);
		ptr = *(envp++);
	}
	return (NULL);
}