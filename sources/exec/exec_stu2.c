/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stu2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:14:41 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 21:42:15 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include "msh_utils.h"
#include <stdlib.h>
#include <signal.h>

static
t_token	*stt_next_sep(t_token *tokens)
{
	int	in_paren;

	in_paren = 0;
	while (!(tokens->type & (E_STU_END | E_END)))
	{
		if (tokens->type & E_OPEN_PAREN)
			in_paren++;
		else if (tokens->type & E_CLOSE_PAREN)
			in_paren--;
		if (!in_paren && (tokens->type & E_PIPE))
			return (tokens);
		tokens++;
	}
	return (tokens);
}

// exec_stu loops through until END
// First thing exec_stu does is open all files within STU_END
// Within this loop, it has sub regions delimited by pipes or parenthesis
// These sub-regions have different kinds of execution:

// And execution needs to know two things:
// A) If it is a subshell command (within parenthesis)
// B) If it is a simple command (built-in and no pipeline)
// A B)
// 0 0) Regular fork and execute in child
// 0 1) No forking and execute in parent
// 1 0) Fork our shell, child runs in batch mode (exits, doesnt return)
// 1 1) Fork our shell, child runs in batch mode (exits, doesnt return)

// Batch mode is probably just exec_stu again without parenthesis,
// but first we mark the end to be the closing parenthesis.
// ^ This requires file opening to be taken out of exec_stu

// No need to even mark the end if exec_stu receives a start and an end parameter

// So the strat is to check if it is a subshell
// If it is, fork, and exec_batch

// exec_batch just calls exec_stu again with the proper ranges, and exits on return

// > out ls | < out2 (echo 1 > out && echo 2) > out3 | cmd3
// > out ls | < out2 {subshell} > out3 | cmd3

// < out2 echo 1 > out && echo 2
// This is a problem.. 

int	exec_stu(t_token *start, t_token *end, t_env *env)
{
	
}
