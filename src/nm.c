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

int func(t_data *d, int ac, char **av)
{
    int         fd;
    struct stat file_stat;

    if (!av[1])
        av[1] = "a.out";
    if ((fd = open(av[1], O_RDONLY)) < 0)
        return (ft_printf("open error: %s\n", av[1]));
    if (fstat(fd, &file_stat) != 0)
        return (ft_printf("fstat error: %s\n", av[1]));
    if (file_stat.st_size == 0)
        return (ft_printf("Invalid file: %s\n", av[1]));
    d->file = mmap(0, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (d->file == MAP_FAILED)
        return (ft_printf("mmap error\n"));
    return (0);
}


int main(int ac, char **av)
{
    t_data d = {0};
    if (func(&d, ac, av))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}
