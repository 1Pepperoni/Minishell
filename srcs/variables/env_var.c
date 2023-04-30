/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ory <ory@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:55:01 by ory               #+#    #+#             */
/*   Updated: 2023/04/30 20:56:15 by ory              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_if_var_is_for_env(char *name, char *value, t_data *data, int flag_plus)
{
	t_lst_var	*tmp;
	char		*tmp_str;
	char		*str;

	tmp = data->var_list;
	while(tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0 && tmp->flag_var_for_env == 1)
		{
			delete_var_in_lst(&data->var_list, name);
			tmp_str = ft_strjoin(name, "=");
			str = ft_strjoin(tmp_str, value);
			ft_setenv(name, str, 1, data, 0);
			free(tmp_str);
			free(str);
			return (1);
		}
		tmp = tmp->next;
	}
	if (var_is_in_env(name, data))
	{
		set_env_var(name, value, data, flag_plus);
		return (1);
	}
	return (0);
}

void	set_env_var(char *name, char *value, t_data *data, int flag_plus)
{
    	char	*tmp_str;
    	char	*str;

    	tmp_str = ft_strjoin(name, "=");
    	str = ft_strjoin(tmp_str, value);
    	ft_setenv(name, str, 1, data, flag_plus);
    	free(tmp_str);
    	free(str);
}

int	var_is_in_env(char *name, t_data *data)
{
	int	i;
	char	*name_with_equal;
	char	*new_name;

	i = 0;
	if (!ft_ischarset(name, '='))
	{
		name_with_equal = ft_strjoin(name, "=");
		new_name = ft_strdup(name_with_equal);
		free(name_with_equal);
	}
	else
		new_name = ft_strdup(name);
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], new_name, ft_strlen(new_name)))
		{
			free(new_name);
			return (1);
		}
		i++;
	}
	free(new_name);
	return (0);
}

char	*get_env_variable_value(char *var_name, t_data *data)
{
	char	*var_with_equal;
	char	*tmp_value;
	char	*value;

	value = NULL;
	tmp_value = NULL;
	if (!var_name)
		return (NULL);
	var_with_equal = ft_strjoin(var_name, "=");
	tmp_value = ft_getenv(var_with_equal, data);
	free(var_with_equal);
	if (tmp_value == NULL)
		return (NULL);
	else if (tmp_value != NULL)
	{
		value = ft_strdup(tmp_value);
		free(tmp_value);
	}
	return (value);
}