/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:12:28 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 13:59:30 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_UTILS_H
# define MSH_UTILS_H

# include <stdint.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>

size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t		ft_strlcat(char *dst, const char *src, size_t dst_size);
int32_t		ft_strncmp(const char *str1, const char *str2, size_t length);
int32_t		ft_strwcmp(const char *str, const char *pattern);

int64_t		ft_atoi(const char *str);
char		*ft_itoa_stack(int64_t number, char *ptr);
uint8_t		ft_ascii(const char c);

const char	*ft_strfind(const char *str, const char *charset, uint8_t ref);
const char	*ft_strchr(const char *str, unsigned char c);

void		*ft_memmove(void *vdst, const void *vsrc, size_t length);
void		*ft_memset(void *dst_void, const uint8_t byte, size_t length);
void		*ft_memcpy\
(void *restrict vdst, const void *restrict vsrc, size_t length);

int			ft_lmcpy\
(void *restrict vdst, const void *restrict vsrc, size_t length, char *end);

// Wrappers
ssize_t		ft_write(int fd, const void *buffer, size_t length);
ssize_t		ft_error(const char *prefix, const char *suffix, ssize_t rvalue);
ssize_t		ft_read(int fd, void *buffer, void *end, size_t read_size);

#endif
