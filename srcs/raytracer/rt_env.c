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
}
