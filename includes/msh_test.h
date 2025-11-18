# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# include "msh_types.h"

// Total of 1024 blocks
// If every block is 1 bit, that is 128 bytes needed
// If every block is 2 bytes, 2kb but describes length!

// Searches for env var equal to entry
// If a match was found, checks space remaining by looping through the sentinels
// if (free_space > entry length), we just overwrite
// else we mark for deletion and search for next block big enough
// If no blocks are available, we call compact. If still not enough, OOM
// Same rules for append, except we don't overwrite

// Bound checks are made by blocks, 
// and right now we assume that max block count == max count
typedef struct s_env
{
	char	*optr;
	size_t	count;
	size_t	max_count;	// max_count needs to match max_blocks
	char	*metadata;
	char	**ptr;
}	t_env;

typedef struct s_wpair
{
	size_t	upper;
	size_t	lower;
}	t_wpair;

typedef struct t_quad
{
	size_t	a;
	size_t	b;
	size_t	c;
	size_t	d;
}	t_quad;

enum e_meta
{
	E_META_FREE = 0u,
	E_META_HEAD = 1u,
	E_META_USED = 2u,
	E_META_RESERVED = 4u
};

#define BLOCK_SIZE 64

char	*request_blocks(t_env *env, size_t bytes);
char	*envx_find(t_env *env, const char *entry, size_t length, size_t *index);
size_t	mark_for_deletion(char *ptr, t_env *env);

int		envx_replace(t_str old_entry, t_kstr new_entry, size_t index, t_env *env);
int		envx_add(t_kstr new_entry, t_env *env);
int		envx_append(t_str str, t_kstr substr, size_t index, t_env *env);
