/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 03:47:18 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 03:47:20 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

char	get_type_char(t_data *d)
{
	char c;

	c = '?';
	if (!d->sym.n_type)
		c = '?';
	else if ((d->sym.n_type & N_TYPE) == N_ABS)
		c = 'a';
	else if ((d->sym.n_type & N_TYPE) == N_INDR)
		c = 'i';
	else if ((d->sym.n_type & N_SECT) && d->sect_chars[d->sym.n_sect])
		c = d->sect_chars[d->sym.n_sect];
	else if ((d->sym.n_type & N_TYPE) == N_UNDF && !d->sym.n_value)
		c = 'u';
	else if ((d->sym.n_type & N_TYPE) == N_UNDF && d->sym.n_value)
		c = 'C';
	if (d->sym.n_type & N_EXT)
		c = ft_toupper(c);
	return (c);
}
