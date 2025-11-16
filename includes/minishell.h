/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:43:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 12:44:21 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# include "msh_types.h"

size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t		ft_strlcat(char *dst, const char *src, size_t dst_size);
int32_t		ft_strncmp(const char *str1, const char *str2, size_t length);
int32_t		ft_strwcmp(const char *str, const char *pattern);

char		*ft_itoa_stack(int64_t number, char *ptr);
uint8_t		ft_ascii(const char c);

const char	*ft_strfind(const char *str, const char *charset, uint8_t ref);
const char	*ft_strchr(const char *str, unsigned char c);

void		*ft_memmove(void *vdst, const void *vsrc, size_t length);
void		*ft_memset(void *dst_void, const uint8_t byte, size_t length);
void		*ft_memcpy(void *restrict vdst, const void *restrict vsrc, size_t length);
int			ft_lmcpy(void *restrict vdst, const void *restrict vsrc, size_t length, char *end);

ssize_t		ft_write(int fd, const void *buffer, size_t length);

size_t		env_find(t_env *env, const char *entry, size_t length);
char		*env_get_entry(t_env *env, const char *entry, size_t var_length[2]);
uint8_t		env_del(t_env *env, const char *entry);
uint8_t		env_init(t_env *env, const char **envp_src);
const char	*env_expand(const char *str, const char *end, t_argv *arg, t_env *env);
int			tokenize(t_shell *shell, char *input);
int			token_not_implemented(char **input);
void		token_word_handler(t_token *token, char **input);

int			syntax_validation(t_shell *shell, int parenthesis_depth);
int			syntax_check_end(t_token last_token);
int			syntax_check_start(t_token first_token);
void		syntax_print_error(t_token token);

int			expand_token(t_token *token, t_env *env, t_argv *arg, size_t count);
int			parse_interval(const char *str, const char *end, t_env *env, t_argv *arg);

int			msh_cd(int argc, const char **argv, t_env *env);
int			msh_pwd(int argc, const char **argv, t_env *env);
int			msh_exit(int argc, const char **argv, t_env *env);
int			msh_unset(int argc, const char **argv, t_env *env);
int			msh_env(int argc, const char **argv, t_env *env);
int			msh_export(int argc, const char **argv, t_env *env);
int			msh_echo(int argc, const char **argv, t_env *env);

#endif
