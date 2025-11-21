/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:43:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 18:23:51 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# include "msh_types.h"

size_t	tokenize(t_token *tokens, const char *input);
int		syntax_validation(t_token *tokens);

ssize_t	expand_token(t_token *token, t_env *env, t_vecp *vec);
int		parse_interval(t_buf src, t_env *env, t_buf *dst);

// Core
int		msh_init(t_memory *mem, t_shell *shell, const char **envp);

// Signals
void	signal_handler(int sig);

// BUILT-INS
int 	msh_dispatch(t_vecp *argv, t_env *env);
uint8_t	msh_is_builtin(const char *str);
int		msh_cd(t_vecp *argv, t_env *env);
int		msh_pwd(void);
int		msh_exit(int argc, const char **argv, t_env *env);
int		msh_unset(int argc, const char **argv, t_env *env);
int		msh_env(int argc, const char **argv, t_env *env);
int		msh_export(t_vecp *argv, t_env *env);
int		msh_echo(int argc, const char **argv, t_env *env);

// ENV
int		env_replace(const char *new_entry, size_t index, t_env *env);
int		env_append(const char *new_entry, size_t index, t_env *env);
int		env_add(const char *new_entry, t_env *env);
int		env_export(const char *entry, t_env *env);

size_t	env_del(const char *entry, t_env *env);
char	*env_find(const char *entry, size_t length, size_t *index, t_env *env);
char	*env_expand(t_buf src, t_buf *dst, t_env *env);
int		env_add_shlvl(t_env *env);
char	*allocate_blocks(t_env *env, size_t bytes);
size_t	mark_for_deletion(char *ptr, t_env *env);

// HST
size_t	hst_add_entry(const char *str, size_t length, t_hst *hst);
size_t	hst_read(size_t index, char *buffer, const t_hst *hst);

// EXIT
int		att_exit(int code, int change);

// READ_LINE
size_t	init_read(char *buffer, t_hst *hst);

// HEREDOCE
int		heredoc(t_token *token, t_env *env);

// EXECUTE
int		execute(t_token *tokens, t_env *env);
int		exec_stu(t_token *tokens, t_env *env);
ssize_t	msh_build_argv(t_token *token, t_env *env, t_vecp *argv);
int		exec_pipe(t_token *tokens, t_env *env);

#endif
