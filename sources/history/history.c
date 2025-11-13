/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:14:47 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/13 19:07:56 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"

#define HST_SIZE (1024 * 1024)   // 1 MiB
#define HST_MAX 255

typedef struct s_hst_entry
{
	uint32_t start;   // offset into buf
	uint32_t length;  // length in bytes (not including '\0')
}	t_hst_entry;

typedef struct s_hst
{
	size_t		free;	// bytes free
	size_t		head;	// next write position
	size_t		tail;	// first used byte (start of oldest entry)
	size_t		first;	// index of oldest entry in entries[]
	size_t		count;	// number of valid entries
	char		data[HST_SIZE];
	t_hst_entry entries[HST_MAX];
}	t_hst;

// Index goes between first and count

// hst_add, hst_read
// hst_free, hst_del, hst_write

static
void	stt_hst_write(const char *str, size_t length, t_hst *hst)
{
	const size_t	rem = (HST_SIZE - hst->head);

	if (rem >= length)
	{
		ft_memcpy(hst->data + hst->head, str, length);
		hst->head += length;
	}
	else
	{
		ft_memcpy(hst->data + hst->head, str, rem);
		length -= rem;
		ft_memcpy(hst->data, str + rem, length);
		hst->head = length;
	}
}

void	hst_read(size_t index, char *buffer, t_hst *hst)
{
	const size_t	start = hst->entries[index].start;
	const size_t	rem = (HST_SIZE - start);
	size_t			length;

	length = hst->entries[index].length;
	if (rem >= length)
		ft_memcpy(buffer, hst->data + start, length);
	else
	{
		ft_memcpy(buffer, hst->data + start, rem);
		length -= rem;
		ft_memcpy(buffer + rem, hst->data, length);
	}
}

static
uint8_t	stt_hst_free(size_t length, t_hst *hst)
{
	t_hst_entry	*entry;

	if (length > HST_SIZE / 2)	// needs to be in relation to max line length
		return (1);
	while (hst->free < length)
	{
		entry = &hst->entries[hst->first];
		hst->tail = entry->start + entry->length;
		hst->free += entry->length;
		hst->count--;
		hst->first = (hst->first + 1) % HST_MAX;
		entry->length = 0;
		entry->start = 0;
	}
	return (0);
}

void	hst_add_entry(const char *str, size_t length, t_hst *hst)
{
	if (stt_hst_free(length, hst) == 1)
		return ;
	stt_hst_write(str, length, hst);
	hst->entries[hst->count].start = hst->head;
	hst->entries[hst->count].length = length;
	hst->head = (hst->head + length) % HST_SIZE;
	hst->free -= length;
	hst->count = (hst->count + 1) % HST_MAX;
}

int	main(int argc, char **argv, char **envp)
{
	t_hst hst = {.head = 0, .tail = 0, .first = 0, .count = 0, .free = HST_SIZE};

}
