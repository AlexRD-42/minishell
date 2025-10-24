/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:52:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/24 14:31:34 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdint.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>

# ifndef FT_PAGE_SIZE
#  define FT_PAGE_SIZE 4096
# endif

# ifndef FT_PATH_MAX
#  define FT_PATH_MAX 4096
# endif

# ifndef FT_ENV_ENTRIES
#  define FT_ENV_ENTRIES 1024
# endif

# ifndef FT_ENV_SIZE
#  define FT_ENV_SIZE 65536
# endif

typedef struct s_env
{
	size_t	count;
	size_t	offset;
	char	data[FT_ENV_SIZE];		// 64kb
	char	*ptr[FT_ENV_ENTRIES];	// 8kb
}	t_env;

typedef struct s_shell
{
	char	*input;
	t_env	env;
	bool	mode; // 0 for non_interactive, 1 for interactive
}	t_shell;

typedef struct s_str
{
	union
	{
		char		*ptr;
		const char	*kptr;
	};
	size_t	length;
}	t_str;
