/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:04:39 by mgallo            #+#    #+#             */
/*   Updated: 2017/04/06 16:57:54 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <OpenCL/opencl.h>
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

static void kernel_init(t_window *window)
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
	if (env->scene.max_material > 0)
		cl->mem_material = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_material) * env->scene.max_material, NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer material");
	if (env->scene.max_light > 0)
		cl->mem_light = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_light) * env->scene.max_light, NULL, &err);
	if (err != CL_SUCCESS)
		die("OpenCL error: buffer ligths");
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
	// Scene
	if ((err = clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), (void *) &(cl->mem_scene))) != CL_SUCCESS)
		die("OpenCL error: Kernel set arg 1, Scene");
	if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_scene, CL_TRUE, 0, sizeof(t_scene), &(env->scene), 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Write Buffer, Scene");
	//Camera
	if ((err = clSetKernelArg(cl->kernel, 2, sizeof(cl_mem), (void *) &(cl->mem_camera))) != CL_SUCCESS)
		die("OpenCL error: Kernel set arg 1, Camera");
	if ((err = clEnqueueWriteBuffer(cl->queue, cl->mem_camera, CL_TRUE, 0, sizeof(t_cam), &(env->camera), 0, NULL, NULL)) != CL_SUCCESS)
		die("OpenCL error: Enqueue Write Buffer, Camera");
	// Object
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
	// Light
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
	// Material
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

static void		print_env(void)
{
	t_env	*e;

	e = env_get();
	printf("==================== Scene ====================\n> width: %d\n> height: %d\n> znear: %f\n> zfar: %f\n> fov: %f\n> max_reflect: %d\n> max_refract: %d\n> max_object: %d\n> max_light: %d\n> max_material: %d\n> max_texture: %d\n", e->scene.width, e->scene.height, e->scene.znear, e->scene.zfar, e->scene.fov, e->scene.max_reflect, e->scene.max_refract, e->scene.max_object, e->scene.max_light, e->scene.max_material, e->scene.max_texture);
	printf("> debug: %d\n> mod: %d\n> filter: [%f, %f, %f, %f]\n", e->scene.render_debug, e->scene.render_mod, e->scene.render_filter.x, e->scene.render_filter.y, e->scene.render_filter.z, e->scene.render_filter.w);

	printf("==================== Camera ====================\n> position: [%f, %f, %f]\n> rotation: [%f, %f, %f]\n", e->camera.position.x, e->camera.position.y, e->camera.position.z, e->camera.rotation.x, e->camera.rotation.y, e->camera.rotation.z);

	for (int i = 0; i < e->scene.max_object; i++)
	{
		t_objects obj = e->objects[i];
		printf("==================== Object %s ====================\n> position: [%f, %f, %f]\n> rotation: [%f, %f, %f]\n> normal: [%f, %f, %f]\n> color: [%f, %f", get_tag(obj.type), obj.position.x, obj.position.y, obj.position.z, obj.rotation.x, obj.rotation.y, obj.rotation.z, obj.normal.x, obj.normal.y, obj.normal.z, obj.color.x, obj.color.y);
		printf(", %f, %f]\n> radius: %f\n> material_id: %d\n> in_object: %d\n> endpos: [%f, %f, %f]\n> radius2: %f\n> a: %f\n> b: %f\n> c: %f\n> d: %f\n> dist: %f\n> texture_id: %d\n", obj.color.z, obj.color.w, obj.radius, obj.material_id, obj.in_object, obj.endpos.x, obj.endpos.y, obj.endpos.z, obj.radius2, obj.a, obj.b, obj.c, obj.d, obj.dist, obj.texture_id);
	}

	for (int i = 0; i < e->scene.max_light; i++)
	{
		t_light light = e->light[i];
		printf("==================== Light %s ====================\n> position: [%f, %f, %f]\n> direction: [%f, %f, %f]\n> color: [%f, %f, %f, %f]\n> attenuation: %f\n> angle: %f\n", get_tag(light.type), light.position.x, light.position.y, light.position.z, light.direction.x, light.direction.y, light.direction.z, light.color.x, light.color.y, light.color.z, light.color.w, light.attenuation, light.angle);
	}

	for (int i = 0; i < e->scene.max_material; i++)
	{
		t_material mate = e->material[i];
		printf("==================== Material ====================\n> ambient: [%f, %f, %f, %f]\n> specular: [%f, %f, %f, %f]\n> blinn: %d\n> shininess: %f\n> reflection: %f\n> refraction: %f\n> refract_coef: %f\n> perlin: %d\n> damier: %f\n", mate.ambient_color.x, mate.ambient_color.y, mate.ambient_color.z, mate.ambient_color.w, mate.specular_color.x, mate.specular_color.y, mate.specular_color.z, mate.specular_color.w, mate.blinn, mate.shininess, mate.reflection, mate.refraction, mate.refract_coef, mate.perlin, mate.damier);
	}

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
	print_env();
	thread_init();
	rt_init();
	window_loop(window_get());
	window_end();
	thread_end();
	return (0);
}
