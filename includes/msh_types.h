/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 17:45:27 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TYPES_H
# define MSH_TYPES_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "msh_defines.h"

typedef struct s_env
{
	size_t	count;
	size_t	max_count;	// max_count needs to match max_blocks
	char	metadata[FT_ENV_COUNT];
	char	optr[FT_ENV_SIZE];
	char	*ptr[FT_ENV_COUNT];
	void	*sig_dfl;
	int32_t	exit_status;
}	t_env;

// General use buffer (24 bytes)
typedef struct s_buf
{
	char	*optr;	// Origin pointer (ideally const)
	char	*end;	// End Pointer (ideally const)
	char	*wptr;	// Write pointer (next write position of the buffer)
}	t_buf;

// General use buffer array of pointers (48 bytes)
typedef struct s_vecp
{
	t_buf	buf;
	size_t	count;
	size_t	max_count;
	char	**ptr;
}	t_vecp;

typedef struct s_hst_entry
{
	uint32_t	start;// offset into buf
	uint32_t	length;// length in bytes (not including '\0')
}	t_hst_entry;

// Ideally should be vecp too but fuck that
typedef struct s_hst
{
	size_t		free;		// free space in bytes
	size_t		head;		// next write position
	size_t		first;		// index of oldest entry
	size_t		count;		// number of valid entries
	char		data[FT_HST_SIZE];
	t_hst_entry	entries[FT_HST_COUNT];
}	t_hst;

typedef struct s_str
{
	char	*ptr;
	size_t	length;
}	t_str;

typedef struct s_kstr
{
	const char	*ptr;
	size_t		length;
}	t_kstr;

// Melhor manter como uint32_t, porque mantem o alinhamento de 16 bytes
// Caso seja necessario compactar, melhor usar token_small de 8 bytes
typedef struct s_token
{
	union
	{
		const char	*ptr;
		int32_t		fd[2];
	};
	uint32_t	type;
	uint32_t	length;
}	t_token;

#endif
