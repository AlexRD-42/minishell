// Precisa do ponteiro inicial para manter referencia do offset. Ex:
// char *optr = posicao da str do primeiro token;
// depois pra usar o token, seria *(optr + offset);
typedef struct s_token_small
{
	uint32_t	offset;
	uint16_t	type;
	uint16_t	length;
}	t_token_small;

typedef struct s_env
{
	size_t	count;
	size_t	offset;
	char	data[FT_ENV_SIZE];	// 64kb
	char	*ptr[FT_ENV_COUNT];	// 8kb
}	t_env;

typedef struct s_argv
{
	size_t	count;
	size_t	offset;
	char	*data;
	char	**ptr;
	char	*end;
}	t_argv;