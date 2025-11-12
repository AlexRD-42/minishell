/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stu.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:14:41 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/12 10:27:15 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"

static void
stt_open(t_token *tokens, t_env *env)
{
	
}

void	exec_stu(t_token *tokens, t_env *env)
{
	while (tokens->type & E_STU_END)
	{
		if (tokens->type & (E_REDIR_IN | E_REDIR_OUT | E_APPND))
		{
			
		}
		tokens++;
	}
}
