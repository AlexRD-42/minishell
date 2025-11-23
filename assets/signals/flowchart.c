/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flowchart.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:18:34 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/12 10:13:33 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Para falar:
// - Error handling

// Tokenization, Save to working history, variable expansion, quote removal
// Parsing (STU SPLIT e STU SORT)

// 1) Initialize
//		Define se: A) Interativo B) Nao Interativo

// 2) Input Reading
//		Espera por input
//		Usando o termcap.h e suas funcoes

// 3) Save to working history

// 4) Tokenization
//		Transforma o input em tokens

// 5) Parsing
//		Separa a string em syntactic units (STU) e organiza elas por ordem de exec
//		Valida o input para garantir que nao contem erros

// Um token word é definido como um argumento (levando em conta o splitting do bash de whitespace)
// 6) Exec STU
// a) Todos os redirs da STU são interpretados e criados (token->ptr deixa de ser um ptr para string e vira fd)
//		Cada fd que é sobrescrito implica no close do anterior fd
// b) Pipeline é criada

// 7) Exec PIPE
// a) Os argumentos do execve são criados por build_argv
// b) No caso de parenteses, execve de nossa shell é chamada
// c) 



// *, SHLVL, export sem args


// bonus: implementar >1 ou <1, backslash

// ls          > out3 | echo a > out1 | echo b > out2
// fd_in fd_out
