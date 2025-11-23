static
void	stt_remove_expand_from_cmd_name(t_token *tokens)
{
	t_token	*current;
	int	reset;

	reset = 1;
	current = tokens;
	while (!(current->type & (E_END)))
	{
		if (reset)
		{
			if (current->type & E_WORD)
			{
				current->type &= ~E_EXPAND;
				reset = 0;
			}
		}
		if (current->type & (E_OPERATOR | E_OPEN_PAREN))
			reset = 1;
		current++;
	}
}