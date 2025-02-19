/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kube <kube@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/02 14:30:35 by cfeijoo           #+#    #+#             */
/*   Updated: 2014/04/19 09:04:59 by kube             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_print.h>
#include <ft_math.h>
#include <ft_colors.h>
#include <ft_strings.h>
#include <ft_convert.h>
#include <ft_memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include <X11/X.h>
#include <rt.h>
#include <ray.h>
#include <parser.h>
#include <mouse.h>
#include <interpreter.h>
#include <pthread.h>
#include <time.h>

static int			view_loop()
{
	if (is_one_key_pressed(&env->pressed_keys))
		check_pressed_keys(&env->pressed_keys);
	if (!env->running_threads)
	{
		update_render_cam(&env->scene->render_cam, &env->scene->camera);
		if (env->last_image_refresh < env->last_light_refresh)
			render_to_image();
		if (env->last_light_refresh < env->last_scene_change)
			update_image();
	}
	usleep(4000);
	return (0);
}

int					update_image()
{
	unsigned int	i;
	unsigned int	j;
	t_thread_input	*input;

	j = 0;
	while (j < RENDER_SPLIT)
	{
		i = 0;
		while (i < RENDER_SPLIT)
		{
			input = (t_thread_input*)malloc(sizeof(*input));
			input->x1 = i * (env->scene->view_width / RENDER_SPLIT);
			input->y1 = j * (env->scene->view_height / RENDER_SPLIT);
			input->x2 = env->scene->view_width - (RENDER_SPLIT - 1 - i)
						* (env->scene->view_width / RENDER_SPLIT);
			input->y2 = env->scene->view_width - (RENDER_SPLIT - 1 - j)
						* (env->scene->view_height / RENDER_SPLIT);
			input->thread_number = j * RENDER_SPLIT + i;
			create_render_thread(input);
			i++;
		}
		j++;
	}
	return (0);
}

static int			expose_hook()
{
	env->last_scene_change = clock();
	return (0);
}

static void			init_main()
{
	env = (t_env*)ft_memalloc(sizeof(t_env));
	env->scene = (t_scene*)ft_memalloc(sizeof(t_scene));
	env->scene->view_width = DEFAULT_VIEW_WIDTH;
	env->scene->view_height = DEFAULT_VIEW_HEIGHT;
	env->scene->recursivity = 2;
	env->scene->antialias = 1;
	env->scene->diaphragm = 1.0;
	init_cam(&env->scene->camera, 0, 0, 0);
	env->render_threads = (pthread_t*)ft_memalloc(RENDER_SPLIT * RENDER_SPLIT
						* sizeof(pthread_t));
	env->last_scene_change = clock();
	init_pressed_keys(&env->pressed_keys);
}

static void			init_graphics()
{
	env->rendering = (t_light_color*)ft_memalloc(env->scene->view_width
					* env->scene->view_height * sizeof(t_light_color));
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, env->scene->view_width,
				env->scene->view_height, "RT");
	env->img = mlx_new_image(env->mlx, env->scene->view_width,
				env->scene->view_height);
	env->data = (int*)mlx_get_data_addr(env->img, &(env->bpp),
				&(env->size_line), &(env->endian));
}

int					main(int argc, char **argv)
{
	init_main();
	if (argc > 1 && ft_strequ(argv[1], "-edit"))
		env->fast_mode = 1;
	else
		env->block_events = 1;
	if (argc == 2 + (env->fast_mode == 1))
		parse_file(argv[1 + (env->fast_mode == 1)]);
	create_interpreter_thread(&env);
	init_graphics();
	mlx_expose_hook(env->win, expose_hook, NULL);
	mlx_hook(env->win, KeyPress, KeyPressMask, keypress_hook,
		&env->pressed_keys);
	mlx_hook(env->win, KeyRelease, KeyReleaseMask, keyrelease_hook,
		&env->pressed_keys);
	mlx_hook(env->win, ButtonPress, ButtonPressMask, mousepress_ev, NULL);
	mlx_hook(env->win, ButtonRelease, ButtonReleaseMask, mouserelease_ev, NULL);
	mlx_hook(env->win, MotionNotify, PointerMotionMask, motionnotify_ev, NULL);
	mlx_loop_hook(env->mlx, view_loop, NULL);
	mlx_loop(env->mlx);
	return (0);
}
