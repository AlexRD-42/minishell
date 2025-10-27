/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:43:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/26 16:59:41 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdint.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>

# include "msh_types.h"

size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t		ft_strlcat(char *dst, const char *src, size_t dst_size);
int32_t		ft_strncmp(const char *str1, const char *str2, size_t length);
int32_t		ft_strcmp(const char *str1, const char *str2);

const char	*ft_strfind(const char *str, const char *charset, uint8_t ref);
const char	*ft_strchr(const char *str, unsigned char c);

void		*ft_memmove(void *vdst, const void *vsrc, size_t length);
void		*ft_bzero(void *dst_void, size_t length);
void		*ft_memset(void *dst_void, const uint8_t byte, size_t length);
void		*ft_memcpy(void *restrict vdst, const void *restrict vsrc, size_t length);

ssize_t		ft_write(int fd, const void *buffer, size_t length);

int			env_print(t_env *env);
size_t		env_find(t_env *env, const char *str);
uint8_t		env_del(t_env *env, const char *entry);
uint8_t		env_add(t_env *env, const char *entry);
uint8_t		env_copy(t_env *env, const char **envp_src);
uint8_t		env_init(t_env *env, const char **envp_src);

int			ft_isspace(int c);

int			tokenize(t_shell *shell, char *input);
int			validate_syntax(t_shell *shell, int parenthesis_depth);
void		syntax_error(t_token token);
int			not_implemented(char **input);
int			end_error(t_token last_token);
int			start_error(t_token first_token);
void		handle_quote_token(t_token *token, char **input);
void		handle_word_token(t_token *token, char **input);
int			cmp_token(char **input, char *token);

#endif
