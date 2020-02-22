/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_filename.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 04:00:55 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 04:00:56 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

char	*get_arch_name(t_data *d, struct fat_arch *arch)
{
	if (arch->cputype == CPU_TYPE_I386)
		return ("i386");
	if (arch->cputype == CPU_TYPE_POWERPC &&
		arch->cpusubtype != CPU_SUBTYPE_POWERPC_7400)
		return ("ppc");
	if (d->is_otool)
		return (NULL);
	return ("");
}

void	print_filename_and_arch(t_data *d, struct fat_arch *arch)
{
	if (arch && get_arch_name(d, arch))
		ft_printf(d->is_otool ?
			"%s (architecture %s):\n" :
			"\n%s (for architecture %s):\n",
			d->filename, get_arch_name(d, arch));
	else
		ft_printf(d->is_otool ? "%s:\n" : "\n%s:\n", d->filename);
	if (d->is_otool)
		ft_printf("Contents of (__TEXT,__text) section\n");
}
