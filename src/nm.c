/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 18:10:04 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/12 18:10:04 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int	main(int ac, char **av)
{
	t_data	d;
	int		i;

	d = (t_data){0};
	if (ac == 1)
		av[ac++] = "a.out";
	if (ac > 2)
		d.print_filename = true;
	i = 1;
	while (i < ac)
		func(&d, av[i++]);
}
