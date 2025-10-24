static inline
uint8_t	env_cmp(const char *var, const char *str)
{
	while (*var == *str && *str != 0)
	{
		var++;
		str++;
	}
	return (*str == 0 && *var == '=');
}

char	*env_find(t_env *env, const char *str)
{
	size_t	i;

	i = 0;
	while (i < env->count)
	{
		if (env_cmp(env->ptr[i], str) == 1)
			return (env->ptr[i]);
		i++;
	}
	return (NULL);
}