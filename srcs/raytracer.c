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

static void	program_init(void)
{
	t_thread	*cl;
	cl_int		err;
	char		*str;
	size_t		size;

	size = get_file("raytracer.cl", &str);
	if (size <= 0)
		die("Error: File doesn't exist or is empty!");
	cl = thread_get();
	cl->program = clCreateProgramWithSource(cl->context, 1, (const char **)&str, (const size_t *)&size, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: program source");
	if ((err = clBuildProgram(cl->program, 1, &(cl->device_id), NULL, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: build program");
	cl->kernel = clCreateKernel(cl->program , "raytracer", &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: kernel");
}

static void	cl_render3(t_thread	*cl, cl_int err, t_window *window, t_env *env)
{
	if (env->scene.max_material > 0 && cl->mem_material != NULL)
	{
		if ((err = clSetKernelArg(cl->kernel, 5, sizeof(cl_mem), (void *) &(cl->mem_material))) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Material");
		if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_material, CL_TRUE, 0, sizeof(t_material) * env->scene.max_material, env->material, 0, NULL, NULL)) != CL_SUCCESS)
			die("OpenCL error: Enqueue Write Buffer, Material");
	}
	else
	{
		if ((err = clSetKernelArg(cl->kernel, 5, sizeof(cl_mem), NULL)) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Material");
	}
	if ((err = clEnqueueNDRangeKernel(cl->queue, cl->kernel, 2, 0, (size_t[2]){window->width, window->height}, NULL, 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Range Kernel");
	clFinish(cl->queue);
	if ((err = clEnqueueReadBuffer(cl->queue, cl->mem_pixel, CL_TRUE, 0, (window->width * window->height *  4) * sizeof(unsigned char), window->pixels, 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Read Buffer, pixel");
}

static void cl_render2(t_thread	*cl, cl_int err, t_window *window, t_env *env)
{
	if (env->scene.max_object > 0 && cl->mem_object != NULL)
	{
		if ((err = clSetKernelArg(cl->kernel, 3, sizeof(cl_mem), (void *) &(cl->mem_object))) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Objects");
		if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_object, CL_TRUE, 0, sizeof(t_objects) * env->scene.max_object, env->objects, 0, NULL, NULL)) != CL_SUCCESS)
			die("OpenCL error: Enqueue Write Buffer, Objects");
	}
	else
	{
		if ((err = clSetKernelArg(cl->kernel, 3, sizeof(cl_mem), NULL)) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Objects");
	}
	if (env->scene.max_light > 0 && cl->mem_light != NULL)
	{
		if ((err = clSetKernelArg(cl->kernel, 4, sizeof(cl_mem), (void *) &(cl->mem_light))) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Light");
		if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_light, CL_TRUE, 0, sizeof(t_light) * env->scene.max_light, env->light, 0, NULL, NULL)) != CL_SUCCESS)
			die("OpenCL error: Enqueue Write Buffer, Light");
	}
	else
	{
		if ((err = clSetKernelArg(cl->kernel, 4, sizeof(cl_mem), NULL)) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Light");
	}
	cl_render3(cl, err, window, env);
}

static void cl_render(void)
{
	t_thread	*cl;
	cl_int		err;
	t_window	*window;
	t_env		*env;

	window = window_get();
	cl = thread_get();
	env = env_get();
	if ((err = clSetKernelArg(cl->kernel, 0, sizeof(cl_mem), (void *) &(cl->mem_pixel))) != CL_SUCCESS)
		die("OpenCL error: Kernel set arg 0, pixel array");
	if ((err = clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), (void *) &(cl->mem_scene))) != CL_SUCCESS)
		die("OpenCL error: Kernel set arg 1, Scene");
	if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_scene, CL_TRUE, 0, sizeof(t_scene), &(env->scene), 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Write Buffer, Scene");
	if ((err = clSetKernelArg(cl->kernel, 2, sizeof(cl_mem), (void *) &(cl->mem_camera))) != CL_SUCCESS)
		die("OpenCL error: Kernel set arg 1, Camera");
	if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_camera, CL_TRUE, 0, sizeof(t_cam), &(env->camera), 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Write Buffer, Camera");
	cl_render2(cl, err, window, env);
}

static void	rt_init(void)
{
	t_window	*window;
	t_env	*env;

	env = env_get();
	env->interface.index = 0;
	env->interface.type = 2;
	window = window_get();
	window->render = &cl_render;
	window->update = &update;
	env->scene.focale = (float)2.0f * tan(env->scene.fov / 2.0);
	if (env->scene.znear < EPSILON)
		env->scene.znear = EPSILON;
	init_font();
	program_init();
	kernel_init(window);
}

int				main(int ac, char **av)
{
	t_window	*window;

	window = window_get();
	window->width = WIN_W;
	window->height = WIN_H;
	if (ac < 2)
		die("Usage: ./RT [scene_file]");
	if (!window_init())
		die("Error: Window init!");
	parse_file(env_get(), av[1]);
	if (!env_get()->scene.max_object)
		die("Error, bad file!");
	thread_init();
	rt_init();
	window_loop(window_get());
	window_end();
	thread_end();
	return (0);
}
