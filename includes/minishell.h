/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:43:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/17 11:33:10 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdint.h>
#include <stddef.h>

#ifndef PAGE_SIZE
# define PAGE_SIZE 4096
#endif

size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t	ft_strlcat(char *dst, const char *src, size_t dst_size);
int32_t	ft_strncmp(const char *str1, const char *str2, size_t length);
int32_t	ft_strcmp(const char *str1, const char *str2);

void	*ft_memcpy(void *dst_void, const void *src_void, size_t length);
void	*ft_bzero(void *dst_void, size_t length);
void	*ft_memset(void *dst_void, const uint8_t byte, size_t length);

#endif
