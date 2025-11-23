/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strwcmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:49:23 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/11 14:05:59 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

// ref: Music 
// str: M*s*c, *sic, Mu*
int	ft_strwcmp(const char *str, const char *pattern)
{
	const char	*ptr = NULL;
	const char	*star = NULL;

	while (*str != 0)
	{
		if (*str == *pattern)
		{
			str++;
			pattern++;
		}
		else if (*pattern == '*')
		{
			star = pattern++;
			ptr = str;
		}
		else if (star != NULL)
		{
			pattern = star + 1;
			str = ++ptr;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == 0);
}

int	main(int argc, char **argv, char **envp)
{
	printf("%d\n", ft_strwcmp("aaab", "*ab") == 1);

	// empties
	printf("%d\n", ft_strwcmp("", "") == 1);
	printf("%d\n", ft_strwcmp("", "*") == 1);
	printf("%d\n", ft_strwcmp("", "**") == 1);
	printf("%d\n", ft_strwcmp("", "a") == 0);

	// literals
	printf("%d\n", ft_strwcmp("abc", "abc") == 1);
	printf("%d\n", ft_strwcmp("abc", "abcd") == 0);
	printf("%d\n", ft_strwcmp("abc", "") == 0);

	// single '*'
	printf("%d\n", ft_strwcmp("abc", "*") == 1);
	printf("%d\n", ft_strwcmp("abc", "a*") == 1);
	printf("%d\n", ft_strwcmp("abc", "*c") == 1);
	printf("%d\n", ft_strwcmp("abc", "ab*") == 1);
	printf("%d\n", ft_strwcmp("abc", "*d") == 0);
	printf("%d\n", ft_strwcmp("aaab", "*ab") == 1);

	// multiple '*'
	printf("%d\n", ft_strwcmp("Music", "M*s*c") == 1);
	printf("%d\n", ft_strwcmp("abdc", "a*b*c") == 1);
	printf("%d\n", ft_strwcmp("abcd", "a*b*d") == 1);
	printf("%d\n", ft_strwcmp("abcd", "a*b*e") == 0);
	printf("%d\n", ft_strwcmp("aaaaab", "*a*a*b") == 1);
	printf("%d\n", ft_strwcmp("aaaaab", "*a*a*c") == 0);
	printf("%d\n", ft_strwcmp("foobarbaz", "*bar*") == 1);
	printf("%d\n", ft_strwcmp("foobazbar", "*bar*") == 1);
	printf("%d\n", ft_strwcmp("bar", "*bar*") == 1);

	// consecutive '*'
	printf("%d\n", ft_strwcmp("abc", "**a**b**c**") == 1);
	printf("%d\n", ft_strwcmp("abc", "***") == 1);
	printf("%d\n", ft_strwcmp("abc", "**d**") == 0);

	// star at ends
	printf("%d\n", ft_strwcmp("abcdef", "*def") == 1);
	printf("%d\n", ft_strwcmp("abcdef", "abc*") == 1);
	printf("%d\n", ft_strwcmp("abcdef", "*cde*") == 1);

	// hard backtracking
	printf("%d\n", ft_strwcmp("aaab", "*ab") == 1);
	printf("%d\n", ft_strwcmp("aaab", "*aab") == 1);
	printf("%d\n", ft_strwcmp("abcxabcdabxabcdabcdabcy", "*abcdabcy") == 1);
	printf("%d\n", ft_strwcmp("abef", "ab*def") == 0);
	printf("%d\n", ft_strwcmp("abcdabcdef", "ab*def") == 1);

	// case sensitivity (expected: case-sensitive match)
	printf("%d\n", ft_strwcmp("file.TXT", "file.*") == 1);
	printf("%d\n", ft_strwcmp("file.txt", "FILE.*") == 0);

	// single chars
	printf("%d\n", ft_strwcmp("a", "*") == 1);
	printf("%d\n", ft_strwcmp("a", "a*") == 1);
	printf("%d\n", ft_strwcmp("a", "*a") == 1);
	printf("%d\n", ft_strwcmp("a", "b*") == 0);

	// near-end mismatches
	printf("%d\n", ft_strwcmp("abc", "ab*cd") == 0);
	printf("%d\n", ft_strwcmp("abcd", "ab*ce") == 0);

	// long repeats
	printf("%d\n", ft_strwcmp("aaaaaaaaaaaaaaaaab", "a*b") == 1);
	printf("%d\n", ft_strwcmp("aaaaaaaaaaaaaaaaab", "a*c") == 0);

	// star at start (hard backtracks)
	printf("%d\n", ft_strwcmp("aaab", "*ab") == 1);
	printf("%d\n", ft_strwcmp("baaaab", "*ab") == 1);
	printf("%d\n", ft_strwcmp("aaaab", "*aab") == 1);
	printf("%d\n", ft_strwcmp("aaaab", "*aaab") == 1);
	printf("%d\n", ft_strwcmp("abcd", "*bcd") == 1);
	printf("%d\n", ft_strwcmp("abcd", "*abc") == 0);
	printf("%d\n", ft_strwcmp("abc", "*a") == 0);
	printf("%d\n", ft_strwcmp("", "*a") == 0);

	// multiple stars requiring re-growth
	printf("%d\n", ft_strwcmp("abcde", "a*cd*e") == 1);
	printf("%d\n", ft_strwcmp("abxcdye", "a*b*c*d*e") == 1);
	printf("%d\n", ft_strwcmp("abxycd", "a*b*cd") == 1);
	printf("%d\n", ft_strwcmp("axbyc", "a*b*c") == 1);
	printf("%d\n", ft_strwcmp("axbyd", "a*b*c") == 0);

	// consecutive stars
	printf("%d\n", ft_strwcmp("abc", "****abc") == 1);
	printf("%d\n", ft_strwcmp("abc", "**a**b**c**") == 1);
	printf("%d\n", ft_strwcmp("abc", "**d**") == 0);

	// star empty vs non-empty
	printf("%d\n", ft_strwcmp("ab", "*ab") == 1);   // '*' empty
	printf("%d\n", ft_strwcmp("ab", "a*b") == 1);   // '*' empty
	printf("%d\n", ft_strwcmp("ab", "a*c") == 0);

	// repeats around same letters (greedy traps)
	printf("%d\n", ft_strwcmp("aaaaab", "*ab") == 1);
	printf("%d\n", ft_strwcmp("aaaaaab", "*aab") == 1);
	printf("%d\n", ft_strwcmp("abbbbbc", "a*bc") == 1);
	printf("%d\n", ft_strwcmp("abbbbbd", "a*b*c") == 0);

	// near-end mismatches
	printf("%d\n", ft_strwcmp("abc", "ab*cd") == 0);
	printf("%d\n", ft_strwcmp("abcd", "ab*ce") == 0);

	// empty / tiny texts
	printf("%d\n", ft_strwcmp("", "*") == 1);
	printf("%d\n", ft_strwcmp("", "**") == 1);
	printf("%d\n", ft_strwcmp("", "*a*") == 0);
	printf("%d\n", ft_strwcmp("x", "*x*") == 1);
	printf("%d\n", ft_strwcmp("x", "*y*") == 0);

	// longer stress-ish
	printf("%d\n", ft_strwcmp("aaaaaaaaaaaaaaaaab", "a*b") == 1);
	printf("%d\n", ft_strwcmp("aaaaaaaaaaaaaaaaab", "a*c") == 0);
	printf("%d\n", ft_strwcmp("abababababac", "*abac") == 1);
	printf("%d\n", ft_strwcmp("abababababac", "*ababac") == 1);
}
