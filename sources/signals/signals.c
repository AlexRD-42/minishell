/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:48:26 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/15 18:48:42 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#define FT_SIGWINCH 1
#define FT_SIGINT 2


void	stt_handle_sigint(int sig)
{
	(void)sig;
	g_signal = FT_SIGINT;
}

void	stt_handle_sigwinch(int sig)
{
	(void)sig;
	g_signal = FT_SIGWINCH;
}

void	setup_signals(void)
{
	signal(SIGINT, stt_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGWINCH, stt_handle_sigwinch);
}
