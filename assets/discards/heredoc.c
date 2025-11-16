static
int	stt_expand_buffer(char *buffer, size_t buf_len, t_env *env)
{
	const char	*end = buffer + buf_len;
	char		*entry;
	size_t		vlength[2];
	size_t		tail_len;

	while (buffer < end)
	{
		if (*buffer == '$')
		{
			entry = env_get_entry(env, buffer + 1, vlength);
			tail_len = (size_t)(end - buffer) - vlength[0] - 1;
			end += vlength[1] - vlength[0];
			buf_len += vlength[1] - vlength[0];
			if (buf_len >= FT_PIPE_SIZE)
			{
				ft_memmove(buffer + vlength[1], buffer + vlength[0] + 1, tail_len);
				ft_memcpy(buffer, entry, vlength[1]);
				buffer += vlength[1];
			}
		}
		buffer++;
	}
	return (0);
}