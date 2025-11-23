


static int	stt_ft_parse_cpr(char *buf, int *rows, int *cols)
{
	int	i;
	int	r;
	int	c;

	if (buf[0] != '\033' || buf[1] != '[')
		return (-1);
	i = 2;
	r = 0;
	while (buf[i] >= '0' && buf[i] <= '9')
		r = r * 10 + (buf[i++] - '0');
	if (buf[i++] != ';')
		return (-1);
	c = 0;
	while (buf[i] >= '0' && buf[i] <= '9')
		c = c * 10 + (buf[i++] - '0');
	if (buf[i] != 'R')
		return (-1);
	*rows = r;
	*cols = c;
	return (0);
}

int	get_cursor_absolute_position(int *rows, int *cols)
{
	char	buf[32];
	int		i;
	ssize_t	ret;

	if (write(STDOUT_FILENO, "\033[6n", 4) != 4)
		return (-1);
	i = 0;
	while (i < (int)sizeof(buf) - 1)
	{
		ret = read(STDIN_FILENO, &buf[i], 1);
		if (ret <= 0)
			return (-1);
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i + 1] = '\0';
	if (stt_ft_parse_cpr(buf, rows, cols) == -1)
		return (-1);
	(*rows)--;
	(*cols)--;
	return (0);
}

