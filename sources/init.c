/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 10:04:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 10:42:37 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>
#include <termios.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_utils.h"

// Return: !SIZE_MAX) Env Count, SIZE_MAX) OOM
static
size_t	stt_env_copy(t_env *env, const char **envp)
{
	const char	*entry;

	ft_memset(env->optr, 0, FT_ENV_SIZE);
	ft_memset(env->metadata, 0, FT_ENV_COUNT);	// Careful
	ft_memset(env->ptr, 0, FT_ENV_COUNT * sizeof(char **));	// Check
	while (envp[env->count] != NULL)	// no guard for env->count, env_add responsability
	{
		entry = envp[env->count];
		if (env_add(entry, env))
			return (SIZE_MAX);
	}
	env->ptr[env->count] = NULL;
	return (env->count);
}

// All processes have default actions for signals received
// You can override this action by registering a handler (registering SIG_IGN)
static
int	stt_setup_signals(void)
{
	struct sigaction	sigmain;

	ft_memset(&sigmain, 0, sizeof(sigmain));
	sigemptyset(&sigmain.sa_mask);			// Clears the mask
	sigmain.sa_flags = 0;
	sigmain.sa_handler = signal_handler;			// Registers a handler
	if (sigaction(SIGWINCH, &sigmain, NULL) < 0)	// Now listening to SIGWINCH
		return (ft_error("msh_signals: ", NULL, -1));
	if (sigaction(SIGINT, &sigmain, NULL) < 0)
		return (ft_error("msh_signals: ", NULL, -1));
	if (sigaction(SIGQUIT, &sigmain, NULL) < 0)
		return (ft_error("msh_signals: ", NULL, -1));
	return (0);
}

static
void	*stt_signal_dfl(void)
{
	static struct sigaction	sigmain;

	ft_memset(&sigmain, 0, sizeof(sigmain));
	sigemptyset(&sigmain.sa_mask);
	sigmain.sa_flags = 0;
	sigmain.sa_handler = SIG_DFL;
	return (&sigmain);
}

// Are signals relevant to non-tty mode?
int	msh_init(t_env *env, t_hst *hst, const char **envp)
{
	ft_memset(env, 0, sizeof(t_env));
	ft_memset(hst, 0, sizeof(t_hst));
	hst->free = FT_HST_SIZE;
	env->max_count = FT_ENV_COUNT;
	env->sig_dfl = stt_signal_dfl();
	env->exit_status = 0;
	if (stt_setup_signals())
		return (-1);
	if (stt_env_copy(env, envp) == SIZE_MAX)
		return (-2);	// OOM
	if (env_add_shlvl(env) != 0)
		return (-2);	// OOM
	return (0);
}
