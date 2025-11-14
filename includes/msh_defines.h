/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_defines.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/14 13:06:12 by adeimlin         ###   ########.fr       */
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

// --- Variables ------------------
// Pointers (8 bytes)
# ifndef FT_ARG_COUNT
#  define FT_ARG_COUNT 2048
# endif

// Bytes (1 byte) (Kernel = 2 MB)
# ifndef FT_ARG_MAX
#  define FT_ARG_MAX 2097152
# endif

// --- Persistent Storage ---------
// Struct (16 bytes)
# ifndef FT_TOKEN_COUNT
#  define FT_TOKEN_COUNT 1024
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

#endif
