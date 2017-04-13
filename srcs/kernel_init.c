/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:46:40 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:46:47 by jeexpert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window/window.h"
#include "window/interface.h"
#include "thread/thread.h"
#include "raytracer/rt_env.h"
#include "utils/error.h"
#include "utils/file.h"
#include "parser/rt_parser.h"

static void kernel_init2(t_window *window, t_thread *cl, t_env *env, cl_int err)
{
	if (env->scene.max_material > 0)
		cl->mem_material = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_material) * env->scene.max_material, NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer material");
	if (env->scene.max_light > 0)
		cl->mem_light = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_light) * env->scene.max_light, NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer ligths");
}

void				kernel_init(t_window *window)
{
	t_thread	*cl;
	t_env		*env;
	cl_int		err;

	cl = thread_get();
	env = env_get();
	cl->mem_pixel = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, (window->width * window->height *  4) * sizeof(unsigned char), NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer pixel");
	cl->mem_scene = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_scene), NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer scene");
	cl->mem_camera = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_cam), NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer camera");
	if (env->scene.max_object > 0)
		cl->mem_object = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_objects) * env->scene.max_object, NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer objects");
		kernel_init2(window, cl, env, err);
}
