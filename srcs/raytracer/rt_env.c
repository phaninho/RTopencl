/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:42:06 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:42:11 by jeexpert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"
#include <stdlib.h>
#include <math.h>
t_env	*env_get(void)
{
	static t_env env;

	return (&env);
}

void	env_init(void)
{
	t_env *env;

	env = env_get();
	init_scene(env);
	init_camera(env);
	env->objects = NULL;
	env->light = NULL;
	env->material = NULL;

	// env->scene.znear = 0;
	// env->scene.zfar = 1000;
	// env->scene.fov = 70;
	// env->scene.max_reflect = 0;
	// env->scene.max_refract = 0;
	// env->scene.max_object = 4;
	// env->scene.max_material = 0;
	// env->scene.max_light = 0;
	// env->scene.focale = (float)2.0f * tan(env->scene.fov / 2.0);
	// env->camera.position.x = 0;
	// env->camera.position.y = 0;
	// env->camera.position.z = 0;
	// env->camera.rotation.x = 0;
	// env->camera.rotation.y = 0;
	// env->camera.rotation.z = 0;
	// env->objects = (t_objects *)malloc(sizeof(t_objects) * env->scene.max_object);
	// env->objects[0].type = SPHERE;
	// env->objects[0].position.x = 15;
	// env->objects[0].position.y = 10;
	// env->objects[0].position.z = 150;
	// env->objects[0].radius = 20.0;
	// env->objects[1].type = SPHERE;
	// env->objects[1].position.x = -15;
	// env->objects[1].position.y = 10;
	// env->objects[1].position.z = 150;
	// env->objects[1].radius = 20.0;
	// env->objects[2].type = PLANE;
	// env->objects[2].position.x = 0;
	// env->objects[2].position.y = 20;
	// env->objects[2].position.z = 0;
	// env->objects[2].normal.x = 0;
	// env->objects[2].normal.y = 1;
	// env->objects[2].normal.z = 0;
	// env->objects[3].type = CYLINDER;
	// env->objects[3].position.x = 0;
	// env->objects[3].position.y = 0;
	// env->objects[3].position.z = 155;
	// env->objects[3].normal.x = 0;
	// env->objects[3].normal.y = 1;
	// env->objects[3].normal.z = 0;
	// env->objects[3].radius = 15.0;

}
