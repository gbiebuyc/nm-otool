/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 03:24:35 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/20 03:24:37 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int	main(int ac, char **av)
{
	t_data	d;
	int		i;

	ft_memset(&d, 0, sizeof(t_data));
	d.is_otool = true;
	d.otool_display_arch = false;
	if (ac == 1)
		av[ac++] = "a.out";
	if (ac > 2)
		d.print_filename = true;
	i = 1;
	while (i < ac)
		if (main2(&d, av[i++]) != 0)
			break ;
}
