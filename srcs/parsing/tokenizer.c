/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 10:55:24 by jmanet            #+#    #+#             */
/*   Updated: 2023/04/14 22:17:06 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_inquote_token(char *str)
{
	int		i;
	char	c;

	i = 0;
	c = str[i];
	while (str[i + 1] != c)
		i++;
	return (i + 2);
}

int	get_next_token(char *str)
{
	int	i;

	i = 0;
	if (str[i] && ft_lexing(str[i]) == LEX_PIPE)
		return (1);
	if (str[i] && (ft_lexing(str[i]) == LEX_GREATER
			|| ft_lexing(str[i]) == LEX_SMALLER))
	{
		if (ft_lexing(str[i + 1]) == ft_lexing(str[i]))
			return (2);
		else
			return (1);
	}
	if (str[i] && (ft_lexing(str[i]) == LEX_SQUOTE
			|| ft_lexing(str[i]) == LEX_DQUOTE))
		return (get_inquote_token(str));
	if (str[i] && ft_lexing(str[i]) == LEX_WORD)
		while (str[i] && ft_lexing(str[i]) == LEX_WORD)
			i++;
	return (i);
}

void	get_token_type(t_token_node *token_list)
{
	while (token_list)
	{
		if ((ft_lexing(token_list->token[0]) == LEX_WORD)
			|| token_list->q_state <= S_IN_DQUOTE)
			token_list->type = T_ARG;
		if ((ft_lexing(token_list->token[0]) == LEX_GREATER)
			&& token_list->q_state == S_NOT_IN_QUOTE)
		{
			token_list->type = T_REDIR_OUT;
			token_list = token_list->next;
			token_list->type = T_REDIR_OUT;
		}
		if ((ft_lexing(token_list->token[0]) == LEX_SMALLER)
			&& token_list->q_state == S_NOT_IN_QUOTE)
		{
			token_list->type = T_REDIR_IN;
			token_list = token_list->next;
			token_list->type = T_REDIR_IN;
		}
		if ((ft_lexing(token_list->token[0]) == LEX_PIPE)
			&& token_list->q_state == S_NOT_IN_QUOTE)
			token_list->type = T_PIPE;
		token_list = token_list->next;
	}
}

t_token_node	*tokenizer(char *commandline)
{
	t_token_node	*token_list;
	char			*temp;
	int				start;
	size_t			len;

	start = 0;
	token_list = NULL;
	while (commandline[start])
	{
		len = get_next_token(&commandline[start]);
		if (ft_lexing(commandline[start]) <= LEX_DQUOTE)
		{
			if (ft_lexing(commandline[start]) <= LEX_PIPE)
				temp = ft_substr(commandline, start, len);
			else
				temp = ft_substr(commandline, start + 1, len - 2);
			add_token_node(&token_list, temp, ft_lexing(commandline[start]));
			start += (int)len;
			free(temp);
		}
		else
			start++;
	}
	get_token_type(token_list);
	return (token_list);
}

int	unexpected_token(char *command_line)
{
	int	i;

	i = 0;
	while(command_line[i])
		i++;
	i = i - 1;
	if (command_line[i] == '<' || command_line[i] == '>' || command_line[i] == '|')
	{
		printf("syntax error: unexpected token\n");
		return (1);
	}
	return (0);
}

// int	unexpected_token_2(t_data *data)
// {
// 	// (void)data;
// 	// return (0);
// 	t_token_node	*tmp;

// 	tmp = data->token_list;
// 	while(tmp && tmp->next)
// 	{
// 		if ((tmp->type == 1 || tmp->type == 2 || tmp->type == 4)
// 			&& (tmp->next->type == 1 || tmp->next->type == 2 || tmp->next->type == 4))
// 		{
// 			if(!(tmp->type == 4 && tmp->next->type == 1))
// 			{
// 				if (((tmp->type == 1 || tmp->type == 2 || tmp->type == 4)) && is_only_compose_by_redir_char(tmp->next->token))
// 				{
// 					printf("syntax error: unexpected token `%s'\n", tmp->next->token);
// 					return (1);
// 				}
// 			}
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

// int	unexpected_token_2(t_data *data)
// {
// 	t_token_node	*tmp;

// 	tmp = data->token_list;
// 	while(tmp && tmp->next)
// 	{
// 		if ((tmp->type == 1 || tmp->type == 2 || tmp->type == 4)
// 			&& (tmp->next->type == 1 || tmp->next->type == 2 || tmp->next->type == 4))
// 		{
// 			printf("syntax error: unexpected token `%s'\n", tmp->next->token);
// 			return (1);
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

int unexpected_token_2(t_data *data)
{
    t_token_node	*tmp;

    tmp = data->token_list;
    while (tmp && tmp->next)
    {
        if ((tmp->type == 1 || tmp->type == 2 || tmp->type == 4) && 
            (tmp->next->type == 1 || tmp->next->type == 2 || tmp->next->type == 4))
        {
            if (!(tmp->type == 4 && tmp->next->type == 1))
            {
                if (((tmp->type == 1 || tmp->type == 2 || tmp->type == 4)) &&
                    is_only_compose_by_redir_char(tmp->next->token))
                {
                    if (tmp->type == 1 && tmp->next->type == 2)
                    {
                        tmp = tmp->next;
                        continue;
                    }
                    printf("syntax error: unexpected token `%s'\n", tmp->next->token);
                    return (1);
                }
            }
        }
        tmp = tmp->next;
    }
    return (0);
}