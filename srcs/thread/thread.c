/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:46:51 by mgallo            #+#    #+#             */
/*   Updated: 2016/11/26 09:19:34 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenCL/opencl.h>
#include "thread/thread.h"
#include "utils/error.h"

t_thread	*thread_get(void)
{
	static t_thread thread;

	return (&thread);
}

void		thread_init(void)
{
	t_thread	*cl;
	cl_int		err;

	cl = thread_get();
	cl->platform_id = NULL;
	err = clGetPlatformIDs(1, &(cl->platform_id), &(cl->platform_count));
	if (err != CL_SUCCESS)
		die("OpenCL error: platform_id");
	cl->device_id = NULL;
	err = clGetDeviceIDs(cl->platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &(cl->device_id), &(cl->device_count));
	if (err != CL_SUCCESS)
		die("OpenCL error: device_id");
	cl->context = NULL;
	cl->context = clCreateContext(NULL, 1, &(cl->device_id), opencl_error, NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: context");
	cl->queue = NULL;
	cl->queue = clCreateCommandQueue(cl->context, cl->device_id, 0, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: queue");
	cl->program = NULL;
	cl->kernel = NULL;
	cl->mem_pixel = NULL;
	cl->mem_scene = NULL;
	cl->mem_camera = NULL;
	cl->mem_object = NULL;
	cl->mem_material = NULL;
	cl->mem_light = NULL;
}

void		thread_end(void)
{
	t_thread	*cl;
	cl_int		err;

	cl = thread_get();
	if (cl->queue != NULL)
 		err = clReleaseCommandQueue(cl->queue);
	if (cl->context != NULL)
		err = clReleaseContext(cl->context);
	if (cl->kernel != NULL)
		err = clReleaseKernel(cl->kernel);
	if (cl->program != NULL)
		err = clReleaseProgram(cl->program);
	if (cl->mem_pixel != NULL)
		err = clReleaseMemObject(cl->mem_pixel);
	if (cl->mem_scene != NULL)
		err = clReleaseMemObject(cl->mem_scene);
	if (cl->mem_camera != NULL)
		err = clReleaseMemObject(cl->mem_camera);
	if (cl->mem_object != NULL)
		err = clReleaseMemObject(cl->mem_object);
	if (cl->mem_material != NULL)
		err = clReleaseMemObject(cl->mem_material);
	if (cl->mem_light != NULL)
		err = clReleaseMemObject(cl->mem_light);
}