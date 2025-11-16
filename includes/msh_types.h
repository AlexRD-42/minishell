/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 14:03:50 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TYPES_H
# define MSH_TYPES_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "msh_defines.h"

enum e_type
{
	E_UNSET = 0u,	// remove
	E_ERROR = 1u,	
	E_WORD = 1u << 1,
	E_OR = 1u << 2,
	E_AND = 1u << 3,
	E_PIPE = 1u << 4,
	E_OPEN_PAREN = 1u << 5,
	E_CLOSE_PAREN = 1u << 6,
	E_REDIR_IN = 1u << 7,
	E_REDIR_OUT = 1u << 8,
	E_APPND = 1u << 9,
	E_HRDOC = 1u << 10,
	E_LIMITER = 1u << 11,
	E_FILE = 1u << 12,
	E_EXPAND = 1u << 13,
	E_STU_END = 1u << 14,
	E_END = 1u << 15,
	E_REDIR = E_REDIR_IN | E_REDIR_OUT | E_APPND | E_HRDOC,
	E_OPERATOR = E_OR | E_AND | E_PIPE,
	E_CMD_END = E_AND | E_OR | E_PIPE | E_END
};

enum e_env_index
{
	E_PATH = 0u,
	E_PWD = 1u,
	E_OLDPWD = 2u
};

enum e_ascii
{
	E_NULL = 0u,
	E_CTRL = 1u,
	E_SPACE = 1u << 1,
	E_SYMBOL = 1u << 2,
	E_DIGIT = 1u << 3,
	E_UPPER = 1u << 4,
	E_LOWER = 1u << 5,
	E_IDENT = 1u << 6,
	E_HEX = 1u << 7,
	E_LETTER = E_LOWER | E_UPPER,
	E_ALPHA = E_LOWER | E_UPPER | E_DIGIT
};

typedef struct s_env
{
	size_t	count;
	size_t	offset;
	char	data[FT_ENV_SIZE];	// 64kb
	char	*ptr[FT_ENV_COUNT];	// 8kb
}	t_env;

typedef struct s_buf
{
	char		*wptr;
	char *const	optr;
	char *const	end;
}	t_buf;

typedef struct s_vecp
{
	union
	{
		t_buf	buf;
		struct
		{
			char		*wptr;
			char *const	optr;
			char *const	end;
		};
	};
	size_t	count;
	char	**ptr;
}	t_vecp;

typedef struct s_argv
{
	size_t	count;
	size_t	offset;
	char	*data;
	char	**ptr;
	char	*end;
}	t_argv;

typedef struct s_hst_entry
{
	uint32_t start;   // offset into buf
	uint32_t length;  // length in bytes (not including '\0')
}	t_hst_entry;

typedef struct s_hst
{
	size_t		free;	// free space in bytes
	size_t		head;	// next write position
	size_t		first;	// index of oldest entry
	size_t		count;	// number of valid entries
	size_t		current; //current line we're changing on read_input
	char		data[FT_HST_SIZE];
	t_hst_entry entries[FT_HST_COUNT];
}	t_hst;

typedef struct s_str
{
	const char	*ptr;
	union
	{
		const char	*end;
		size_t		length;
	};
}	t_str;

typedef struct s_kstr
{
	const char	*ptr;
	union
	{
		const char	*end;
		size_t		length;
	};
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

// Precisa do ponteiro inicial para manter referencia do offset. Ex:
// char *optr = posicao da str do primeiro token;
// depois pra usar o token, seria *(optr + offset);
typedef struct s_token_small
{
	uint32_t	offset;
	uint16_t	type;
	uint16_t	length;
}	t_token_small;

typedef struct s_shell
{
	char	*input;
	t_env	env;
	t_token	tokens[FT_TOKEN_COUNT];
	t_hst	history;
}	t_shell;

#endif
