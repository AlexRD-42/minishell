#include <termios.h>
#include <stdio.h>
#include "../../includes/minishell.h"
#include "read_input.h"

int	g_signal;

static struct termios	g_original_term;

void	save_terminal_config(void)
{
	tcgetattr(STDIN_FILENO, &g_original_term);
}

void	restore_terminal_config(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_original_term);
}

void	setup_terminal(void)
{
	struct termios	new_term;

	save_terminal_config();
	new_term = g_original_term;
	
	new_term.c_lflag &= ~(ICANON | ECHO);
	
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

static
void	stt_hst_write(const char *str, size_t length, t_hst *hst)
{
	const size_t	rem = (FT_HST_SIZE - hst->head);
	const size_t	index = (hst->first + hst->count) % FT_HST_COUNT;

	hst->entries[index].start = hst->head;
	hst->entries[index].length = length;
	hst->free -= length;
	if (rem >= length)
	{
		ft_memcpy(hst->data + hst->head, str, length);
		hst->head = (hst->head + length) * (rem != length);
	}
	else
	{
		ft_memcpy(hst->data + hst->head, str, rem);
		length -= rem;
		ft_memcpy(hst->data, str + rem, length);
		hst->head = length;
	}
	hst->count++;
}

// FT_HST_SIZE is guaranteed to be at least 2x line length
static
void	stt_hst_free(size_t length, t_hst *hst)
{
	t_hst_entry	*entry;

	while (hst->free < length || hst->count >= FT_HST_COUNT)
	{
		entry = &hst->entries[hst->first];
		hst->free += entry->length;
		hst->count--;
		hst->first = (hst->first + 1) % FT_HST_COUNT;
		entry->length = 0;
		entry->start = 0;
	}
}

size_t	hst_read(size_t index, char *buffer, const t_hst *hst)
{
	size_t	start;
	size_t	rem;
	size_t	length;

	if (index >= hst->count)
		return (SIZE_MAX);
	index = (index + hst->first) % FT_HST_COUNT;
	start = hst->entries[index].start;
	length = hst->entries[index].length;
	rem = (FT_HST_SIZE - start);
	if (rem >= length)
		ft_memcpy(buffer, hst->data + start, length);
	else
	{
		ft_memcpy(buffer, hst->data + start, rem);
		ft_memcpy(buffer + rem, hst->data, length - rem);
	}
	buffer[length] = 0;
	return (length);
}

size_t	hst_add_entry(const char *str, size_t length, t_hst *hst)
{
	if (length >= (FT_HST_SIZE / 2))
		return (SIZE_MAX);
	stt_hst_free(length, hst);
	stt_hst_write(str, length, hst);
	return (hst->count);
}

int main()
{
	t_shell shell;
	char	line[4096];
	size_t	n;

	setup_terminal();
	n = read_input(&shell.history, (char **)&line, sizeof(line));
	restore_terminal_config();
	if (n == -1)
		write(STDOUT_FILENO, "exit\n", 5);
	if (!line[0])
		write(STDOUT_FILENO, "empty\n", 6);
	hst_add_entry(line, n, &shell.history);
	printf("%s", line);
}