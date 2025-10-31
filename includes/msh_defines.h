/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_defines.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/31 12:19:37 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_DEFINES_H
# define MSH_DEFINES_H

// Bytes
# ifndef FT_PAGE_SIZE
#  define FT_PAGE_SIZE 4096
# endif

// Bytes
# ifndef FT_PATH_MAX
#  define FT_PATH_MAX 4096
# endif

// Pointers (8 bytes)
# ifndef FT_ENV_ENTRIES
#  define FT_ENV_ENTRIES 1024
# endif

// Bytes 
# ifndef FT_ENV_SIZE
#  define FT_ENV_SIZE 65536
# endif

// Struct (8/12/16 bytes)
# ifndef FT_TOKEN_MAX
#  define FT_TOKEN_MAX 256
# endif

// Pointers (8 bytes)
# ifndef FT_ARG_MAX
#  define FT_ARG_MAX 1024
# endif

// END HDOC APPEND RDIN RDOUT CLPAR OPPAR PIPE AND OR WORD UNSET ERROR
# define REDIR 1920
//   (REDIR_IN | REDIR_OUT | APPEND | HEREDOC)
// # define REDIR 0b000011110000000

# define OPERATOR 28
//   (OR | AND | PIPE)
// # define OPERATOR 0b0000000000011100

#endif
