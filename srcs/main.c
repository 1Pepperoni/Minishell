/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 22:03:37 by jmanet            #+#    #+#             */
/*   Updated: 2023/04/30 19:51:12 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_global global;

int	check_quotes(char *str)
{
	int	i;
	int	nb_quotes;
	char	c;

	i = 0;
	nb_quotes = 0;
	if (!str || !*str)
		return (0);
	while (str[i])
	{
		if ((ft_lexing(str[i]) == LEX_SQUOTE)
			|| (ft_lexing(str[i]) == LEX_DQUOTE))
		{
			c = str[i++];
			nb_quotes++;
			while (str[i] && str[i] != c)
				i++;
			if (str[i] == c)
				nb_quotes++;
		}
		i++;
	}
	if (nb_quotes % 2 != 0)
		print_error("Error in command, some quotes aren't closed \n", 2);
	return (!(nb_quotes % 2));
}

void	print_error(char *str, int error)
{
	global.exit_code = error;
	printf("%s", str);
}

void	join_var_was_splited_in_tokenizer(t_data *data)
{
	t_token_node	*list_tokens;
	char		*new_str;

	list_tokens = data->token_list;
	while(list_tokens)
	{
		if (list_tokens->flag_for_join_with_prev_token == 1)
		{
			new_str = ft_strjoin(list_tokens->prev->token, list_tokens->token);
			free(list_tokens->prev->token);
			list_tokens->prev->token = ft_strdup(new_str);
			free(new_str);
			delete_token_node(&list_tokens);
            		list_tokens = data->token_list;
		}
		else
			list_tokens = list_tokens->next;
	}
}

int	unexpected_var_assignment(t_data *data)
{
	t_token_node	*list_token;

	list_token = data->token_list;
	while(list_token)
	{
		if (list_token->type == T_ARG)
		{
			if (list_token->token && ft_ischarset(list_token->token, '='))
			{
				if (list_token->token[ft_strlen(list_token->token) - 1] == '=')
					return (1);
				if ((list_token->next == NULL || list_token->next->type < 4))
				{
					if (unexpected_char_in_name(list_token->token))
					{
						printf("%s: command not found\n", list_token->token);
						global.exit_code = 127;
						return (1);
					}
				}
			}
		}
		list_token = list_token->next;
	}
	return (0);
}

void	invalid_assignment(t_data *data)
{
	t_token_node	*list_token;

	list_token = data->token_list;
	while(list_token)
	{
		if (list_token->type == T_ARG)
		{
			if (list_token->token && ft_ischarset(list_token->token, '='))
				list_token->var_assignment = 0;
		}
		list_token = list_token->next;
	}
}

// void	ft_add_var(t_data *data)
// {
// 	t_token_node	*list_tokens;
// 	char		*cmd;

// 	list_tokens = data->token_list;
// 	join_var_was_splited_in_tokenizer(data);
// 	while(list_tokens)
// 	{
// 		if (cmd_is_builtin_2(list_tokens->token))
// 			return ;
// 		if ((cmd = str_is_cmd(list_tokens->token, data->envp)))
// 		{
// 			free(cmd);
// 			return ;
// 		}
// 		list_tokens = list_tokens->next;
// 	}
// 	if (unexpected_var_assignment(data))
// 	 	invalid_assignment(data);
// 	list_tokens = data->token_list;
// 	while (list_tokens)
// 	{
// 		if (list_tokens->type == T_ARG)
// 		{
// 			if (list_tokens->token && ft_ischarset(list_tokens->token, '='))
// 			{
// 				list_tokens->type = T_VAR;
// 				if ((list_tokens->next == NULL || list_tokens->next->type < 4))
// 				{
// 					if (list_tokens->var_assignment == 1)
// 						add_var_in_list(data, list_tokens->token);
// 				}
// 			}
// 		}
// 		list_tokens = list_tokens->next;
// 	}
// }

void	ft_add_var(t_data *data)
{
	t_token_node	*list_tokens;
	char		*cmd;

	list_tokens = data->token_list;
	join_var_was_splited_in_tokenizer(data);
	while(list_tokens)
	{
		if (cmd_is_builtin_2(list_tokens->token))
			return ;
		if ((cmd = str_is_cmd(list_tokens->token, data->envp)))
		{
			free(cmd);
			return ;
		}
		list_tokens = list_tokens->next;
	}
	if (unexpected_var_assignment(data))
	 	invalid_assignment(data);
	var_assignment(data);
}

void var_assignment(t_data *data)
{
	t_token_node	*list_tokens;
	
	list_tokens = data->token_list;
	while (list_tokens)
	{
		if (list_tokens->type == T_ARG && list_tokens->token && ft_ischarset(list_tokens->token, '='))
		{
			list_tokens->type = T_VAR;
			if (list_tokens->next == NULL || list_tokens->next->type < 4)
			{
				if (list_tokens->var_assignment == 1)
					add_var_in_list(data, list_tokens->token);
			}
		}
		list_tokens = list_tokens->next;
	}
}

int	unexpected_char_in_name(char *str)
{
	int	i;
	char	*name;

	i = -1;
	name = NULL;
	if ((str[0] != '$' && !ft_isalpha(str[0]) ) || !ft_isalpha(str[0]))
		return (1);
	name = extract_name_in_assignment(str);
	i = -1;
	while(name[++i])
	{
		if ((!ft_isalnum(str[i]) && !ft_isalpha(str[i])) && (str[i] != '_'))
		{
			free(name);
			return (1);
		}
	}
	if (name)
		free(name);
	return (0);
}

void	prompt(t_data *data)
{
	ft_signal_handler();
	data->command_line = readline("minishell > ");
	add_history(data->command_line);
	if (data->command_line)
	{
		ft_command_line(data);
		if (global.g_pid != 0)
		{
			kill(global.g_pid, SIGINT);
			global.g_pid = 0;
		}
	}
	else
	{
		printf("exit\n");
		system("leaks minishell");
		exit(0);
	}
}

void	ft_command_line(t_data *data)
{
	if (ft_strlen(data->command_line) > 0
			&& check_quotes(data->command_line) && !unexpected_token(data->command_line))
	{
		data->token_list = tokenizer(data->command_line);
		//t_token_node *tmp = data->token_list;
		// while(data->token_list)
		// {
		// 	printf("token = %s type = %d  flag = %d\n", data->token_list->token, data->token_list->type, data->token_list->flag_for_join_with_prev_token);
		// 	data->token_list = data->token_list->next;
		// }
		if (!unexpected_token_2(data))
		{
			if (replace_var_by_value(data))
				ft_add_var(data);
			parse_token_list(data);
			execute_ast(data);
		}
		free_mem(data);
	}
	else
		free(data->command_line);
}

t_data	*data_init(char **envp)
{
	t_data	*data;

	global.g_pid = 0;
	global.exit_code = 0;
	data = malloc(sizeof(*data));
	if (!data)
		ft_exit_error("Memory allocation error \n");
	data->commands_tree = malloc(sizeof(t_ast));
	if (!data->commands_tree)
		ft_exit_error("Memory allocation error \n");
	data->commands_tree->root = NULL;
	data->envp = ft_import_envp(envp);
	data->pid = 0;
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void)argv;
	data = data_init(envp);
	if (argc > 1)
	{
		printf("Invalid argument(s), ");
		printf("this minishell doesn't execute scriptfiles !\n");
	}
	else
		while (1)
			prompt(data);
	return (0);
}
