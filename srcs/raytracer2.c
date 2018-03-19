/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/14 20:33:19 by stmartin          #+#    #+#             */
/*   Updated: 2017/04/14 20:33:29 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window/window.h"
#include "window/interface.h"
#include "thread/thread.h"
#include "raytracer/rt_env.h"
#include "utils/error.h"
#include "utils/file.h"
#include "parser/rt_parser.h"

void	cl_render4(t_thread *cl, cl_int err, t_window *window, t_env *env)
{
	if (env->scene.max_material > 0 && cl->mem_material != NULL)
	{
		if ((err = clSetKernelArg(cl->kernel, 5, sizeof(cl_mem), \
						(void*)&(cl->mem_material))) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Material");
		if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_material, CL_TRUE, \
			0, sizeof(t_material) * env->scene.max_material, env->material, 0, \
						NULL, NULL)) != CL_SUCCESS)
			die("OpenCL error: Enqueue Write Buffer, Material");
	}
	else
	{
		if ((err = clSetKernelArg(cl->kernel, 5, sizeof(cl_mem), NULL)) \
				!= CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Material");
	}
	if ((err = clEnqueueNDRangeKernel(cl->queue, cl->kernel, 2, 0, \
					(size_t[2]){window->width, window->height}, NULL, 0, NULL, \
					NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Range Kernel");
	clFinish(cl->queue);
	if ((err = clEnqueueReadBuffer(cl->queue, cl->mem_pixel, CL_TRUE, 0, \
					(window->width * window->height * 4) * \
		sizeof(unsigned char), window->pixels, 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Read Buffer, pixel");
}

void	cl_render3(t_thread *cl, cl_int err, t_window *window, t_env *env)
{
	if (env->scene.max_light > 0 && cl->mem_light != NULL)
	{
		if ((err = clSetKernelArg(cl->kernel, 4, sizeof(cl_mem), \
						(void *)&(cl->mem_light))) != CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Light");
		if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_light, CL_TRUE, 0, \
				sizeof(t_light) * env->scene.max_light, env->light, 0, NULL, \
						NULL)) != CL_SUCCESS)
			die("OpenCL error: Enqueue Write Buffer, Light");
	}
	else
	{
		if ((err = clSetKernelArg(cl->kernel, 4, sizeof(cl_mem), NULL)) \
				!= CL_SUCCESS)
			die("OpenCL error: Kernel set arg 1, Light");
	}
	cl_render4(cl, err, window, env);
}
