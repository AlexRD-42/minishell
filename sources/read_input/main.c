#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define FT_LINE_MAX 4096
#define FT_HST_SIZE 65536
#define FT_HST_COUNT 1024
#define PROMPT "msh: "

#include "read_input.h"

volatile sig_atomic_t g_signal = 0;
static char			g_history_data[FT_HST_SIZE];
static t_hst_entry	g_history_entries[FT_HST_COUNT];



void handle_sigint(int sig)
{
    (void)sig;
    g_signal = SIGINT;
}

void handle_sigwinch(int sig)
{
    (void)sig;
    g_signal = SIGWINCH;
}



void setup_signals(void)
{
    struct sigaction sa;
    
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    
    sa.sa_handler = handle_sigwinch;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGWINCH, &sa, NULL);
    
    signal(SIGQUIT, SIG_IGN);
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

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	while (n--)
	{
		if (*p1 != *p2)
			return (*p1 - *p2);
		p1++;
		p2++;
	}
	return (0);
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
		*ptr++ = (unsigned char)c;
	return (s);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int main(void)
{
    t_hst history;
    char line_buffer[FT_LINE_MAX];
    int len;
    
    memset(&history, 0, sizeof(t_hst));

	history.data = g_history_data;
    history.entries = g_history_entries;

    history.free = FT_HST_SIZE;
    
    setup_signals();
    
    write(STDOUT_FILENO, "Minishell Test\n", 15);
    write(STDOUT_FILENO, "Digite 'exit' para sair\n\n", 26);
    
    while (1)
    {
        
        len = stt_init_read(line_buffer, &history);
        
        if (len == -1)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        
        if (len == 0)
            continue;
        
        if (strncmp(line_buffer, "exit", 4) == 0)
            break;
        
        hst_add_entry(line_buffer, len, &history);
        
        write(STDOUT_FILENO, "Você digitou: ", 15);
        write(STDOUT_FILENO, line_buffer, len);
        write(STDOUT_FILENO, "\n", 1);
    }
    
    return (0);
}