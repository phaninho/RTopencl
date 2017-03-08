/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 17:40:42 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/16 17:43:39 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_PARSER_H
# define RT_PARSER_H

# include "raytracer/rt_env.h"

# define WIDTH 0
# define HEIGHT 1
# define MAXREFLECT 2
# define MAXREFRACT 3
# define MATERIALID 4
# define BLINN 5
# define DEBUG 6

# define DOUBLE 7
# define FOV 7
# define ZNEAR 8
# define ZFAR 9
# define RADIUS 10
# define REFLECTION 11
# define REFRACTION 12
# define ATTENUATION 13
# define ANGLE 14
# define SHININESS 15

# define VEC 16
# define NORMAL 16
# define ROTATION 17
# define POSITION 18
# define DIRECTION 19

# define COLOR 20
# define SPECULAR 21
# define AMBIENT 22
# define FILTER 23

# define LOAD 24
# define MOD 25

typedef struct	s_arg
{
	int		type;
	int		aint[7];
	double	adbl[9];
	VEC3	avec[4];
	VEC4	color[4];
	char	*data;
	int		mod;
}				t_arg;

typedef struct	s_parse
{
	int		in_scene;
	int		in_object;
	int		scene;
	int		cam;
	int		object;
	int		light;
	int		mate;
	int		texture;
	int		type;
	char	*tag;
	t_arg	arg;
}				t_parse;

void			parse_file(t_env *e, const char *file);
t_arg			parse_args(const char *tag, int arg_type);

size_t			get_file(const char *name, char **str);
char			*get_tag(int i);
char			*get_arg(int i);
char			*get_mod(int i);

int				comment(const char *s, int i);
void			pass_spaces(const char *s, int *i);
void			pass_string(const char *s, int *i);
char			*parse_tag(const char *s, int *i);
int				parse_tag_type(const char *tag, int *i);
void			malloc_elements(t_env *e, t_parse *p);
void			close_tag(t_env *e, t_parse *p, char *file, int *i);

double			get_double(const char *tag, int *i, int *nb);
int				get_int(const char *tag, int *i, int *nb);
VEC4			get_color(const char *tag, int *i, int *val);
void			get_render_mod(t_arg *arg, const char *tag, int *i);
void			load_texture(t_arg *arg, const char *tag, int *i);
void			read_png(t_arg *arg, char *name);

void			init_arg(t_arg *arg);
void			init_parse(t_parse *p);
void			init_scene(t_env *e);
void			init_camera(t_env *e);
void			init_objects(t_env *e, int max);
void			init_lights(t_env *e, int max);
void			init_materials(t_env *e, int max);
void			init_textures(t_env *e, int max);

VEC4			check_color(VEC4 color);
void			check_values(t_env *e, t_parse *p, t_arg *arg);
VEC3			check_normal(VEC3 normal);
void			check_render(t_env *e, t_arg *arg);

#endif
