- No Dynamic Memory:
In Linux, the maximum argument size execve can handle is 2mb, with a 128kb limit per argument
This is well beneath the 8mb stack limit, meaning that each command can comfortably be executed without exceeding memory constraints.

Solution:
- 128kb reading buffer
- 2mb execve buffer

Therefore sane memory limits:
- 2mb	line length (with expansions)
- 2mb 	execve buffer
- 8mb	working history
- 128kb word buffer

line length == execve buffer allows the guarantee of pipe region always fitting inside execve buffer

As the line is read, the program executes as soon as a single sintactic unit is formed.
Given that code executes from left to right, and values on the RHS do not affect the LHS, the buffer can be passed off to execve and cleared after execution

Despite the line length being unbounded, code execution works because the values don't need to be saved.

However, this presents a problem for the working history, because it needs to be saved in its entirety.

Possible solution is to have the working history be saved in the stack (4mb), and when it needs more space 

# ENV

// All env blocks start with an 8 byte value where the first 4 bytes refer to 
// the name of the var, and the last 4 bytes refer to the length of the value
// Example:							  [ 8 bytes  ][ 9 bytes                 ]	
// var=abcd, in memory will look like [0004][0005][v][a][r][=][a][b][c][d][0]

// Having this index enables mark for deletion, where you just delete the first
// byte, 

// 32	4096	8
// 16	1024	16
// 8	256	32
// 8	64	128

// Allocate in 256 byte blocks so that appends are cheaper

// Ignore everything above: Have a normal env but del now marks for deletion
// Compaction happens when either: Memory is needed OR env runs without arguments

// env_del: Now only null terminates the beginning of the string

// env_compact: 
// Goes to every pointer to check if it points to a null string and memshifts
// Sorts the ptr in alphabetical order