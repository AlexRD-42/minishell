/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_defines.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 20:34:26 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_DEFINES_H
# define MSH_DEFINES_H

// --- Kernel Constants -----------
// Bytes (1 byte) (Kernel = 4 KB)
# ifndef FT_PAGE_SIZE
#  define FT_PAGE_SIZE 4096
# endif

// Bytes (1 byte) (Kernel = 4 KB)
# ifndef FT_PATH_SIZE
#  define FT_PATH_SIZE 4096
# endif

// Bytes (1 byte) (Kernel = 64 KB)
# ifndef FT_PIPE_SIZE
#  define FT_PIPE_SIZE 65536
# endif

// Bytes (1 byte) (Kernel = 128 KB)
# ifndef FT_ARG_SIZE
#  define FT_ARG_SIZE 131072
# endif

// Bytes (1 byte) (Kernel = 256 bytes)
# ifndef FT_NAME_MAX
#  define FT_NAME_MAX 256
# endif

// Bytes (1 byte) (Kernel = 2 MB)
# ifndef FT_ARG_MAX
#  define FT_ARG_MAX 524288
# endif

// --- Variables ------------------
// Pointers (8 bytes)
# ifndef FT_ARG_COUNT
#  define FT_ARG_COUNT 2048
# endif

// --- Persistent Storage ---------
// Struct (16 bytes) (MSH = 64 KB)
# ifndef FT_TOKEN_COUNT
#  define FT_TOKEN_COUNT 4096
# endif

// Pointers (8 bytes)
# ifndef FT_ENV_COUNT
#  define FT_ENV_COUNT 1024
# endif

// Pointers (8 bytes)
# ifndef FT_HST_COUNT
#  define FT_HST_COUNT 1024
# endif

// Bytes (1 byte)
# ifndef FT_ENV_SIZE
#  define FT_ENV_SIZE 65536
# endif

// Bytes (1 byte)
# ifndef FT_HST_SIZE
#  define FT_HST_SIZE 2097152
# endif

# define FT_MAX_CHILDREN 128
# define FT_SYSCALL_RETRIES 16
# define FT_BLOCK_SIZE 64		// Block count needs to map to env count

// Bytes (1 byte)
# ifndef FT_LINE_MAX
#  define FT_LINE_MAX 524288
# endif

# ifndef FT_PROMPT
#  define FT_PROMPT "msh: "
# endif

enum e_type
{
	E_UNSET = 0u,	// remove
	E_ERROR = 1u,	
	E_WORD = 1u << 1,
	E_OR = 1u << 2,
	E_AND = 1u << 3,
	E_PIPE = 1u << 4,
	E_OPAREN = 1u << 5,
	E_CPAREN = 1u << 6,
	E_REDIR_IN = 1u << 7,
	E_REDIR_OUT = 1u << 8,
	E_APPND = 1u << 9,
	E_HRDOC = 1u << 10,
	E_LIMITER = 1u << 11,
	E_EXPAND = 1u << 12,
	E_END = 1u << 13,
	E_FILENAME = 1u << 14,
	E_REDIR = E_REDIR_IN | E_REDIR_OUT | E_APPND | E_HRDOC,
	E_OPERATOR = E_OR | E_AND | E_PIPE,
	E_CMD_END = E_AND | E_OR | E_PIPE | E_END | E_OPAREN | E_CPAREN
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

enum e_meta
{
	E_META_FREE = 0u,
	E_META_HEAD = 1u,
	E_META_USED = 2u,
	E_META_RESERVED = 4u
};

#endif
