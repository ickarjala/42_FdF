/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarjala <ikarjala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 20:55:03 by ikarjala          #+#    #+#             */
/*   Updated: 2022/10/09 20:50:06 by ikarjala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "keysym_macos.h"

static inline int	print_usage(void)
{
	const char	msg[] = CYELLOW "usage:  " BIN_NAME " <map_file.fdf>\n" CNIL;

	write (2, msg, sizeof(msg) - 1);
	return (XC_ERROR);
}

static inline t_vars	initialize_vars(void)
{
	t_vars	v;

	v.mlxo = NULL;
	v.mlx_win = NULL;
	v.fdf = (t_fdf){0};
	v.img = (t_img){0};
	v.frame = 0;
	return (v);
}

int	main(int argc, char **argv)
{
	char	*wname;
	t_vars	v;

	if (argc != 2)
		return (print_usage());
	v = initialize_vars();
	v.fdf = parse_map_file(argv[1]);
	if (v.fdf.eparse != ECONTINUE)
		app_close (&v, XC_ERROR);
	wname = ft_strjoin (WIN_TITLE " : ", argv[1]);
	if (!wname)
		app_close(&v, perr_internal());
	v.mlxo = mlx_init();
	v.mlx_win = mlx_new_window(v.mlxo, WIN_RESX, WIN_RESY, wname);
	ft_strdel (&wname);
	v.img.o = mlx_new_image(v.mlxo, WIN_RESX, WIN_RESY);
	v.img.addr = mlx_get_data_addr(v.img.o, &v.img.bpp,
			&v.img.width, &v.img.endian);
	v.img.bpp /= __CHAR_BIT__;
	mlx_hook(v.mlx_win, ON_KEYDOWN, XKMASK_KB, &on_keydown, &v);
	mlx_key_hook (v.mlx_win, &on_keyup, &v);
	mlx_loop_hook (v.mlxo, &on_render, &v);
	mlx_loop (v.mlxo);
	return (XC_EXIT);
}
