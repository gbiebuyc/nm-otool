/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 04:05:26 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 04:05:28 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int	main2(t_data *d, char *filename)
{
	int			fd;

	d->filename = filename;
	if ((fd = open(d->filename, O_RDONLY)) < 0)
		return (ft_dprintf(STDERR_FILENO, "open error: %s\n", d->filename));
	if (fstat(fd, &d->file_stat) != 0)
		return (ft_dprintf(STDERR_FILENO, "fstat error: %s\n", d->filename));
	if (d->file_stat.st_size == 0 && d->is_otool)
		return (ft_printf("%s: is not an object file\n", d->filename));
	if (d->file_stat.st_size == 0)
		return (ft_dprintf(STDERR_FILENO, "Invalid file: %s\n", d->filename));
	d->file = mmap(0, d->file_stat.st_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (d->file == MAP_FAILED)
		return (ft_dprintf(STDERR_FILENO, "mmap error\n"));
	d->file_start = d->file;
	d->i_sect = 0;
	d->nsyms = 0;
	if (!parse_header(d, d->file, false))
		return (munmap(d->file, d->file_stat.st_size) + 42);
	munmap(d->file, d->file_stat.st_size);
	return (0);
}
