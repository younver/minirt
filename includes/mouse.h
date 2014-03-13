/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 02:32:09 by cfeijoo           #+#    #+#             */
/*   Updated: 2014/03/13 03:06:22 by cfeijoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOUSE_H
# define MOUSE_H

# include <rt.h>

int			buttonpress_hook(int button, int x, int y, t_env *env);
int			buttonrelease_hook(int button, int x, int y, t_env *env);
int			motionnotify_hook(int x, int y, t_env *env);


#endif
