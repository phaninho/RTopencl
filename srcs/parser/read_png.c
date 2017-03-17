/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_png.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/16 19:43:28 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/16 19:43:56 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include "utils/error.h"
#include <png.h>

static void	fill_arg(t_arg *arg, png_bytep *row_pointers)
{
	png_byte	*row;
	int			x;
	int			y;

	y = -1;
	while (++y < arg->aint[HEIGHT])
	{
		row = row_pointers[y];
		x = -1;
		while (++x < arg->aint[WIDTH])
		{
			arg->data[y * arg->aint[WIDTH] + x] = row[x * 4];
			arg->data[y * arg->aint[WIDTH] + x + 1] = row[x * 4 + 1];
			arg->data[y * arg->aint[WIDTH] + x + 2] = row[x * 4 + 2];
			arg->data[y * arg->aint[WIDTH] + x + 3] = row[x * 4 + 3];
		}
	}
}

static void	get_datas(t_arg *arg, png_structp png_ptr, png_infop info_ptr)
{
	png_bytep	*row_pointers;
	png_byte	*row;
	int			y;

	y = -1;
	if (!(row_pointers = (png_bytep*)malloc(sizeof(png_bytep)\
	* arg->aint[HEIGHT])))
		die("[read_png_file] Malloc failure");
	while (++y < arg->aint[HEIGHT])
		if (!(row_pointers[y] = (png_byte*)malloc(\
		png_get_rowbytes(png_ptr, info_ptr))))
			die("[read_png_file] Malloc failure");
	png_read_image(png_ptr, row_pointers);
	fill_arg(arg, row_pointers);
}

static void	check_file(FILE *fd, png_structp *png_ptr, png_infop *info_ptr)
{
	char	header[8];

	fread(header, 1, 8, fd);
	if (png_sig_cmp((unsigned char*)header, 0, 8))
		die("[read_png] File is not recognized as a PNG file");
	if (!(*png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0)))
		die("[read_png] png_create_read_struct failed");
	if (!(*info_ptr = png_create_info_struct(*png_ptr)))
		die("[read_png] png_create_info_struct failed");
	if (setjmp(png_jmpbuf(*png_ptr)))
		die("[read_png] Error during init_io");
}

/*
** color_type = png_get_color_type(png_ptr, info_ptr);
** bit_depth = png_get_bit_depth(png_ptr, info_ptr);
** number_of_passes = png_set_interlace_handling(png_ptr);
*/

void		read_png(t_arg *arg, char *name)
{
	FILE		*fd;
	png_structp	png_ptr;
	png_infop	info_ptr;
	int			y;
	int			x;

	if (!(fd = fopen(name, "rb")))
		die("[read_png] File could not be opened for reading");
	check_file(fd, &png_ptr, &info_ptr);
	png_init_io(png_ptr, fd);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	arg->aint[WIDTH] = png_get_image_width(png_ptr, info_ptr);
	arg->aint[HEIGHT] = png_get_image_height(png_ptr, info_ptr);
	png_read_update_info(png_ptr, info_ptr);
	if (setjmp(png_jmpbuf(png_ptr)))
		die("[read_png_file] Error during read_image");
	if (!(arg->data = (char*)malloc(sizeof(char) * (arg->aint[HEIGHT]\
	* png_get_rowbytes(png_ptr, info_ptr)))))
		die("[read_png_file] Malloc failure");
	get_datas(arg, png_ptr, info_ptr);
	fclose(fd);
}
