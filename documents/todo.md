# (BUG):
Wildcard matching considers asterisks within quotes if one asterisk was valid within an argument

Ex: abc*"t*"d 		-> abc*t*d
when it should be	-> abc*t\*d


# Implement an FD LIMIT
 to avoid having to deal with FD MAX

# (Extra):
- Change HST to use ptrs and vecp

# (Extra): buffered putstr with newline

# (Extra): Multi-char LUT
stt_token_finder char checks go from ASCII 32 (0x20) to 64 (0x40)
This means that a LUT is possible if bitmasking is applied, in an expression like:
a = str[0] > 32 && < 64
b = 4 * str[1] > 32 && < 64
(str[0] & 0b00111111) >> a  + (str[1] & 0b00111111) << b

The size of the LUT is manageable (32 * 32 = 1024), and could even grow to be 32k for 3 letter matches
To avoid constant checks for length, we always pad the source string with at least 8 bytes of zeroes

# (Extra): Env Function 
Could make it so there is a function that requests new blocks
Something like (ptr, env, size), to isolate the functions from mark for deletion

# Investigate:

__attribute__((const))
alignas
assume
hot path
__attribute__((hot)) void fast_path()

if (__builtin_expect(x != 0, 1))