/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_idea.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:58:26 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/17 12:44:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// All env blocks start with an 8 byte value where the first 4 bytes refer to 
// the name of the var, and the last 4 bytes refer to the length of the value
// Example:							  [ 8 bytes  ][ 9 bytes                 ]	
// var=abcd, in memory will look like [0004][0005][v][a][r][=][a][b][c][d][0]

// Having this index enables mark for deletion, where you just delete the first
// byte, 

// 32	4096	8
// 16	1024	16
// 8	256	32
// 8	64	128

// Allocate in 256 byte blocks so that appends are cheaper

// Ignore everything above: Have a normal env but del now marks for deletion
// Compaction happens when either: Memory is needed OR env runs without arguments

// env_del: Now only null terminates the beginning of the string

// env_compact: 
// Goes to every pointer to check if it points to a null string and memshifts
// Sorts the ptr in alphabetical order