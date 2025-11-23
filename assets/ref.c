#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include "minishell.h"

int	expand_word(t_token token, char *buffer, int offset)
{
	int		initial_offset;
	char	quote;
	int	i;

	initial_offset = offset;
	quote = 0;
	while (i < token.length)
	{
		if ((!quote || quote == '"') && token.ptr[i] == '$')
			expand_var(&token.ptr[i], buffer);
		else if (!quote && token.ptr[i] == '*')
			expand_wildcard(&token.ptr[i], buffer);
		else if (token.ptr[i] == '\'')
		{
			i++;
			quote = '\'';
		}
		else if (token.ptr[i] == '"')
		{
			quote = '"';
			i++;
		}
		else
			buffer[offset++] = token.ptr[i++];
	}
	token.length = (size_t)(offset - initial_offset);
	buffer[++offset] = ' ';
	token.ptr = &buffer[initial_offset];
}

int    open_token(t_token token, int flags)
{
    int    fd;

    token.ptr[token.length] = '\0';
    if (flags & O_CREAT)
        fd = open(token.ptr, flags, 0777);
    else
        fd = open(token.ptr, flags);
    if (fd == -1);
        perror("open:");
    return (fd);
}

void	handle_redirection(t_token *current, int *fd[2])
{
	if (current->type & APPEND)
		*fd[1] = open_token(*(current + 1), O_APPEND | O_WRONLY | O_RDONLY | O_CREAT);
	else if (current->type & REDIRECT_OUT)
		*fd[1] = open_token(*(current + 1), O_WRONLY | O_CREAT);
	else if (current->type & REDIRECT_IN)
		*fd[0] = open_token(*(current + 1), O_RDONLY | O_CREAT);
	else if (current->type & HEREDOC)
		*fd[0] = handle_heredoc();
}

char **parse_tokens(t_token *tokens, t_shell *shell)
{
	char	new_input[BUFFER_SIZE];
	int		offset;
	char	**cmds;
	int		fds[COMMANDS_MAX][2]; //fds[0] == comando um;    fds[0][0] stdin do comando,  fds[0][1] stdout do comando
	int		i;
	t_token	*current;

	i = 0;
	current = &tokens[0];
	offset = 0;
	while (!(current->type & (AND | OR | END)))
	{
		if (current->type & REDIR)
		{
			handle_redirection(current, &fds[i]);
			current += 2;
		}
		else if (current->type & WORD)
		{
			expand_word(*current, new_input, offset);
			offset += current->length;
		}
		if (current->type & OPEN_PARENTHESIS)
			//?? não estamos executando ainda, apenas parsing. Mas redirects se aplicam aos subshells.
			// ls > out1 | (ls > out2 | echo arara) > out3
			// quando vc for executar o primeiro ls, todos os outs (incluindo o do subshell) já tem que existir.
			// e o stdout do subshell n vai ser o 1, vai ser o fd do out3.
		if (current->type & PIPE)
		{
			new_input[offset] = '\0';
			i++;
		}
	}
	cmds = new_input;
	return (cmds);
}
