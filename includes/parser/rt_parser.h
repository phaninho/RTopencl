/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 17:40:42 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/23 18:38:19 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_PARSER_H
# define RT_PARSER_H

# include "raytracer/rt_env.h"

# define WIDTH 0
# define HEIGHT (WIDTH + 1)
# define MAXREFLECT (HEIGHT + 1)
# define MAXREFRACT (MAXREFLECT + 1)
# define MATERIALID (MAXREFRACT + 1)
# define BLINN (MATERIALID + 1)
# define DEBUG (BLINN + 1)
# define PERLIN (DEBUG + 1)
# define DAMIER (PERLIN + 1)

# define DOUBLE (DAMIER + 1)
# define FOV (DOUBLE)
# define ZNEAR (FOV + 1)
# define ZFAR (ZNEAR + 1)
# define RADIUS (ZFAR + 1)
# define REFLECTION (RADIUS + 1)
# define REFRACTION (REFLECTION + 1)
# define ATTENUATION (REFRACTION + 1)
# define ANGLE (ATTENUATION + 1)
# define SHININESS (ANGLE + 1)
# define TILESIZE (SHININESS + 1)
# define RADIUS2 (TILESIZE + 1)
# define OBJ_A (RADIUS2 + 1)
# define OBJ_B (OBJ_A + 1)
# define OBJ_C (OBJ_B + 1)
# define OBJ_D (OBJ_C + 1)
# define DIST (OBJ_D + 1)

# define VEC (DIST + 1)
# define NORMAL (VEC)
# define ROTATION (NORMAL + 1)
# define POSITION (ROTATION + 1)
# define DIRECTION (POSITION + 1)
# define ENDPOS (DIRECTION + 1)

# define COLOR (ENDPOS + 1)
# define SPECULAR (COLOR + 1)
# define AMBIENT (SPECULAR + 1)
# define FILTER (AMBIENT + 1)

# define LOAD (FILTER + 1)
# define MOD (LOAD + 1)

typedef struct	s_arg
{
	int		type;
	int		aint[9];
	double	adbl[16];
	VEC3	avec[5];
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
