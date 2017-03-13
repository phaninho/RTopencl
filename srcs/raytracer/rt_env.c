/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 02:31:23 by mgallo            #+#    #+#             */
/*   Updated: 2016/11/30 13:20:57 by mgallo           ###   ########.fr       */
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
}
