/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:43:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 12:22:55 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# include "msh_types.h"

int		tokenize(t_shell *shell, char *input);
int		token_not_implemented(char **input);
void	token_word_handler(t_token *token, char **input);

int		syntax_validation(t_shell *shell, int parenthesis_depth);
int		syntax_check_end(t_token last_token);
int		syntax_check_start(t_token first_token);
void	syntax_print_error(t_token token);

ssize_t	expand_token(t_token *token, t_env *env, t_vecp *vec);
int		parse_interval(t_buf src, t_env *env, t_buf *dst);

// BUILT-INS
int 	msh_dispatch(t_vecp *argv, t_env *env);
int		msh_cd(t_env *env);
int		msh_pwd(void);
int		msh_exit(void);
int		msh_unset(t_vecp *argv, t_env *env);
int		msh_env(t_env *env);
int		msh_export(t_vecp *argv, t_env *env);
int		msh_echo(t_vecp *argv);

// ENV
int		env_add(t_kstr new_entry, size_t index, bool append, t_env *env);
size_t	env_del(const char *entry, t_env *env);
char	*env_find(const char *entry, size_t length, size_t *index, t_env *env);
char	*env_expand(t_buf src, t_buf *dst, t_env *env);
char	*allocate_blocks(t_env *env, size_t bytes);
size_t	mark_for_deletion(char *ptr, t_env *env);

#endif
