/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 11:27:03 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TYPES_H
# define MSH_TYPES_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "msh_defines.h"

typedef enum e_type
{
	FT_UNSET = 0,
	FT_ERROR = 1,
	FT_WORD = 1 << 1,
	FT_OR = 1 << 2,
	FT_AND = 1 << 3,
	FT_PIPE = 1 << 4,
	FT_OPEN_PAREN = 1 << 5,
	FT_CLOSE_PAREN = 1 << 6,
	FT_REDIR_IN = 1 << 7,
	FT_REDIR_OUT = 1 << 8,
	FT_APPND = 1 << 9,
	FT_HRDOC = 1 << 10,
	FT_LIMITER = 1 << 11,
	FT_FILE = 1 << 12,
	FT_EXPAND = 1 << 13,
	FT_END = 1 << 14,
}	t_type;

enum e_ascii
{
	E_NULL = 0x00,
	E_CTRL = 0x01,
	E_SPACE = 0x02,
	E_SYMBOL = 0x04,
	E_DIGIT = 0x08,
	E_UPPER = 0x10,
	E_LOWER = 0x20,
	E_LETTER = 0x30,
	E_ALPHA = 0x38,
	E_IDENT = 0x40,
	E_HEX = 0x80
};

// STU_END: END | OR | AND

typedef struct s_env
{
	size_t	count;
	size_t	offset;
	char	data[FT_ENV_SIZE];		// 64kb
	char	*ptr[FT_ENV_ENTRIES];	// 8kb
}	t_env;

typedef struct s_str
{
	union
	{
		char		*ptr;
		const char	*kptr;
	};
	size_t	length;
}	t_str;

// Melhor manter como uint32_t, porque mantem o alinhamento de 16 bytes
// Caso seja necessario compactar, melhor usar token_small de 8 bytes
typedef struct s_token
{
	union
	{
		const char	*ptr;
		int32_t		fds[2];
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
	t_token	tokens[FT_TOKEN_MAX];
	bool	mode; // 0 for non_interactive, 1 for interactive
}	t_shell;

typedef int (*builtin_fn)(int argc, const char **argv, t_env *env);

#endif
