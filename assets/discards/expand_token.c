// Return: 0) OK, 1) OOM
static
uint8_t	stt_expand_word(t_token *token, t_env *env, t_argv *arg)
{
	const char	*end = token->ptr + token->length;
	const char	*str = token->ptr;
	size_t		interval;

	while (str < end)
	{
		interval = stt_find_interval(str, end);
		if (stt_parse_interval(str, interval, env, arg))
			return (1);
		str += interval;
	}
	return (0);
}

uint8_t	expand_token(t_token *token, t_env *env, t_argv *arg, size_t count)
{
	t_argv	*arg_tmp;
	char	buffer[FT_WCARD_SIZE];
	uint8_t	rvalue;

	if (token->type & E_EXPAND)
	{
		arg_tmp = &(t_argv){0, 0, buffer, NULL, buffer + sizeof(buffer)};
		rvalue = stt_expand_word(token, env, arg_tmp);
		if (rvalue == 0)
			rvalue = expand_glob(buffer, arg, count);
	}
	else
		rvalue = stt_expand_word(token, env, arg);
	return (rvalue);
}