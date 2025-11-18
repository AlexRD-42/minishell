static
size_t	stt_get_blocks_needed(t_env *env, size_t index, const char *entry, size_t length)
{
	size_t			i;
	size_t			meta_index;
	char			*metadata;
	const char		*ptr = env->ptr[index];
	const size_t	count = env->max_count;

	metadata = env->metadata;
	meta_index = (size_t)(ptr - env->optr) / BLOCK_SIZE;
	i = meta_index + (metadata[meta_index] == 1);
	while (i < count && metadata[i] >= 2)
		i++;
	return (i - meta_index);
}