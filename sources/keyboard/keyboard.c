/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/08 15:50:12 by cfeijoo           #+#    #+#             */
/*   Updated: 2014/03/27 16:00:27 by cfeijoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ft_memory.h>

void		init_pressed_keys(t_pressedkeys *keys)
{
	ft_bzero(keys, sizeof(*keys));
}

int			is_one_key_pressed(t_pressedkeys *keys)
{
	int		*key;

	key = (int*)keys;
	while ((size_t)key - (size_t)(keys) < sizeof(*keys))
	{
		if (*key)
			return (1);
		key++;
	}
	return (0);
}
