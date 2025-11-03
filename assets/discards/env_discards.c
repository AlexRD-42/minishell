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

int	msh_cd(int argc, const char **argv, t_env *env)
{
	char	buffer[FT_PATH_MAX];

	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	ft_memcpy(env->ptr[1], buffer, ft_strlen(buffer) + 1);
	chdir(argv[1]);
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	ft_memcpy(env->ptr[0], buffer, ft_strlen(buffer) + 1);
	return (0);
}

// TO DO: This wont work with reserved vars!
int	msh_env(int argc, const char **argv, t_env *env)
{
	ssize_t	rvalue;
	size_t	i;
	size_t	length;

	(void) argc;
	(void) argv;
	if (env->count == 0 || env->ptr[0] == NULL)
		return (1);
	i = 1;
	while (i < env->count)
	{
		env->ptr[i][-1] = '\n';
		i++;
	}
	length = ft_strlen(env->ptr[env->count - 1]);
	env->ptr[env->count - 1][length] = '\n';
	rvalue = ft_write(STDOUT_FILENO, env->ptr[0], env->offset);
	env->ptr[env->count - 1][length] = 0;
	i = 1;
	while (i < env->count)
	{
		env->ptr[i][-1] = '\n';
		i++;
	}
	return (rvalue <= 0);	// 1 on failure
}

int	msh_env(int argc, const char **argv, const char **envp)
{
	size_t	i;
	size_t	length;

	(void) argc;
	(void) argv;
	i = 0;
	while (envp[i] != NULL)
	{
		length = ft_strlen(envp[i]);
		if (ft_write(STDOUT_FILENO, envp[i], length) <= 0)
			return (1);
		if (ft_write(STDOUT_FILENO, "\n", 1) <= 0)
			return (1);
		i++;
	}
	return (0);
}