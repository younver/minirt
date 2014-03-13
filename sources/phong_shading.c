/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong_shading.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/11 19:10:43 by lbinet            #+#    #+#             */
/*   Updated: 2014/03/13 22:06:24 by cfeijoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt.h>
#include <ray.h>
#include <object.h>
#include <ft_math.h>
#include <ft_memory.h>
#include <math.h>

static t_point	get_point_from_ray_intersection(t_ray *ray, float t)
{
	t_point		point;

	point.x = ray->origin.x + ray->direction.x * t;
	point.y = ray->origin.y + ray->direction.y * t;
	point.z = ray->origin.z + ray->direction.z * t;
	return (point);
}

static int		is_point_exposed_to_light(t_env *env, t_point point, t_light *light)
{
	t_ray		ray;
	float		distance_to_light;

	ft_memcpy(&ray.origin, &point, sizeof(t_point));

	/* ERROR CAN BE HERE, SWAP LIGHT.ORIGIN AND RAY.ORIGIN */
	ray.direction.x = light->origin.x - ray.origin.x;
	ray.direction.y = light->origin.y - ray.origin.y;
	ray.direction.z = light->origin.z - ray.origin.z;
	distance_to_light = vect_norm(&ray.direction);
	normalize_vector(&ray.direction);
	throw_ray(env, &ray, 0);
	if (ray.inter_t <= distance_to_light)
		return (0);
	return (1);
}

static void		ambient_lighting(t_ray *ray)
{
	ray->color.red = ((float)ray->closest->color.red) * ray->closest->ambient / 255.0;
	ray->color.green = ((float)ray->closest->color.green) * ray->closest->ambient / 255.0;
	ray->color.blue = ((float)ray->closest->color.blue) * ray->closest->ambient / 255.0;
}

static float	phong_lighting(t_env *env, t_ray *ray)
{
	t_vector	light_vector;
	t_vector	normal;
	t_point		intersection;
	float		lambert;
	t_light		*current_light;

	intersection.x = ray->origin.x + ray->direction.x * ray->inter_t;
	intersection.y = ray->origin.y + ray->direction.y * ray->inter_t;
	intersection.z = ray->origin.z + ray->direction.z * ray->inter_t;
	normal.x = intersection.x - ray->closest->origin.x;
	normal.y = intersection.y - ray->closest->origin.y;
	normal.z = intersection.z - ray->closest->origin.z;
	normalize_vector(&normal);
	current_light = env->lights;
	while (current_light)
	{
		/*
		**	Check if point is exposed to current light
		*/
		(void)is_point_exposed_to_light;
		(void)get_point_from_ray_intersection;

		if (is_point_exposed_to_light(env,
			get_point_from_ray_intersection(ray, ray->inter_t), current_light))
		{
			/*
			**	Diffuse Lighting
			*/
			light_vector.x = current_light->origin.x - intersection.x;
			light_vector.y = current_light->origin.y - intersection.y;
			light_vector.z = current_light->origin.z - intersection.z;
			normalize_vector(&light_vector);
			lambert = fmax(vect_dot(&normal, &light_vector), 0);
			lambert *= lambert * lambert;
			ray->color.red += lambert * ((float)ray->closest->color.red * ray->closest->diffuse * current_light->color.red * current_light->intensity / 255.0);
			ray->color.green += lambert * ((float)ray->closest->color.green * ray->closest->diffuse * current_light->color.green * current_light->intensity / 255.0);
			ray->color.blue += lambert * ((float)ray->closest->color.blue * ray->closest->diffuse * current_light->color.blue * current_light->intensity / 255.0);
			/*
			**	Specular (Faster to treat it here)
			*/
			if (env->pressed_keys.specular_enabled && lambert > 0.94)
			{
				lambert = pow(lambert, 900);
				ray->color.red += lambert * ((float)ray->closest->color.red * ray->closest->specular * current_light->color.red * current_light->intensity / 255.0);
				ray->color.green += lambert * ((float)ray->closest->color.green * ray->closest->specular * current_light->color.green * current_light->intensity / 255.0);
				ray->color.blue += lambert * ((float)ray->closest->color.blue * ray->closest->specular * current_light->color.blue * current_light->intensity / 255.0);
			}
		}
		current_light = current_light->next;
	}
	return (lambert);
}

void			phong_shading(t_env *env, t_ray *ray)
{
	ambient_lighting(ray);
	phong_lighting(env, ray);
}