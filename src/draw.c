/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 19:48:14 by hbalasan          #+#    #+#             */
/*   Updated: 2023/09/11 18:23:43 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

#define MAX(a, b) (a > b ? a : b)
#define MOD(a) (a > 0 ? a : -a)

void	isometric_change(float *x, float *y, int z, double angle)
{
	*x = (*x - *y) * cos(angle);
	*y = (*x + *y) * sin(angle) - z;
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < data->width && y >= 0 && y < data->height)
	{
		// printf ("x: %d, y: %d\n", x, y);
		dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
	}
}

void	bresenham_algo(t_fdf *fdf, float x, float y, float x1, float y1)
{
	float	x_step;
	float	y_step;
	int		max;
	int		z;
	int		z1;

	z = fdf->matrix[(int)y][(int)x];
	// printf ("x: %d, y: %d, z: %d\n", (int)x, (int)y, (int)z);
	z1 = fdf->matrix[(int)y1][(int)x1];
//____________________________________zoom_
	x *= fdf->zoom;
	x1 *= fdf->zoom;
	y *= fdf->zoom;
	y1 *= fdf->zoom;
//______________________________________3D_
	isometric_change(&x, &y, z, fdf->angle);
	isometric_change(&x1, &y1, z1, fdf->angle);
//____________________________________shift_
	x += fdf->shift_x;
	y += fdf->shift_y;
	x1 += fdf->shift_x;
	y1 += fdf->shift_y;
//____________________________________color_
	fdf->color = (z > 0 || z1 > 0) ? 0x0000FFCC : 0x003366FF;

	x_step = x1 - x;
	y_step = y1 - y;
	max = MAX(MOD(x_step), MOD(y_step));
	x_step /= max;
	y_step /= max;
	while ((int)(x - x1) || (int)(y - y1))
	{
		my_mlx_pixel_put(&(fdf->img), x, y, fdf->color);
		x += x_step;
		y += y_step;
	}
}

void	draw(t_fdf *fdf)
{
	int	x;
	int	y;

	y = 0;
	while (y < fdf->height)
	{
		x = 0;
		while (x < fdf->width)
		{
			if (x < fdf->width - 1)
				bresenham_algo(fdf, x, y, x + 1, y);
			if (y < fdf->height - 1)
				bresenham_algo(fdf, x, y, x, y + 1);
			x++;
		}
		y++;
	}
}