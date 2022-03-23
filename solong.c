/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solong.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dduraku <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 18:16:22 by dduraku           #+#    #+#             */
/*   Updated: 2022/03/08 18:16:25 by dduraku          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GNL/get_next_line.h"
#include "libft/libft.h"
#include "mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "so_long.h"


//Fonction qui verifie la colision avec un mur
int	ft_is_valid(int keycode, t_game *game)
{
	if ((keycode == 2 || keycode == 124) && game->map[game->char_position_y / 32][(game->char_position_x + 32) / 32] != '1')
		return (1);
	else if ((keycode == 0 || keycode == 123) && game->map[game->char_position_y / 32][(game->char_position_x - 32) / 32] != '1')
		return (1);
	else if ((keycode == 1 || keycode == 125) && game->map[(game->char_position_y + 32) / 32][game->char_position_x / 32] != '1')
		return (1);
	else if ((keycode == 13 || keycode == 126) && game->map[(game->char_position_y - 32) / 32][game->char_position_x / 32] != '1')
		return (1);
	return (0);
}


//Fonction qui calcule la haute Y de la map
int	ft_line_len(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}


//Creation de la window dynamiquement avec la bonne taille
int ft_window_size(t_game *game, char *file_name)
{
    int		fd;
    char 	*line;
    int x;
    int	y;

    fd = open(file_name, O_RDONLY);
    line = get_next_line(fd);
    y = 0;
    while (line)
    {
        x = 0;
        while (line[x])
            x++;
        line = get_next_line(fd);
        y++;
    }
    game->mlx_win = mlx_new_window(game->mlx, x * 32, y * 32, "So_Long");
    return (0);
}


//Protection du cas ou il y a differentes tailles de lignes
int ft_protection_lines(t_game *game)
{
	int	x;
	int	y;
	int	sample;

	y = 0;
	sample = ft_strlen(game->map[y]);
	printf("Sample size : %d\n", sample);
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
			x++;
		if (sample != x)
		{
			printf("La map n'est pas un rectangle\n");
			exit(0);
		}
		y++;
	}
	return (0);
}
//Protection si y a pas de collectibles
int	ft_protection_items(t_game *game)
{
	int x;
	int y;
	int c;

	y = 0;
	c = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'C')
				c++;
			x++;
		}
		y++;
	}
	if (c == 0)
	{
		printf("Il n'y a pas de collectibles dans la map\n");
		exit(0);
	}
	return (0);
}

//Protection du cas ou il y a plusieurs spawns ou pas de spawns
int	ft_protection_player(t_game *game) {
	int x;
	int y;
	int p;

	y = 0;
	p = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'P')
				p++;
			x++;
		}
		y++;
	}
	if ((p == 0) || (p > 1))
	{
		printf("Verifiez le nombre des points d'apparition\n");
		exit(0);
	}
	return (0);
}

//Protection du cas ou il y a pas ou plusieurs sorties, il en faut qu'une seule
int	ft_protection_exit(t_game *game)
{
	int x;
	int y;
	int e;

	y = 0;
	e = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'E')
				e++;
			x++;
		}
		y++;
	}
	if (e == 0 || e > 1)
	{
		printf("Verifier le nombre de sorties\n");
		exit(0);
	}
	return (0);
}


//Protection pour les murs, si != de '1' alors on retourne une erreur
int	ft_protection_wall(t_game *game)
{
	int x;
	int	y;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[0][x] != '1')
			{
				printf("Il faut que des 1 en haut de la map\n");
				exit(0);
			}
			else if (game->map[y][0] != '1')
			{
				printf("Il faut que des 1 a gauche de la map\n");
				exit(0);
			}
			else if (game->map[y][ft_strlen(game->map[y]) - 1] != '1')
			{
				printf("Il faut que des 1 a droite de la map\n");
				exit(0);
			}
			else if (game->map[ft_line_len(game->map) - 1][x] != '1')
			{
				printf("Il faut que des 1 en bas de la map\n");
				exit(0);
			}
			x++;
		}
		y++;
	}
	return (0);
}


//Quand tous les collectibles sont pris, on peut sortir de la map
int	ft_escape(t_game *game)
{
	if (game->collection == game->item_max)
		return (1);
	return (0);
}


//Generation de la map de base
int ft_map(t_game *game, char *file_name)
{
	int		fd;
	char 	*line;
	char	*result = malloc(1);
	int x;
	int	y;
	result[0] = 0;
	game->item_max = 0;
	fd = open(file_name, O_RDONLY);
	line = get_next_line(fd);
	y = 0;

	while (line)
	{
		x = 0;
		while (line[x])
		{
			if (line[x] == '1')
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->wall, x * 32, y * 32);
			if (line[x] == '0')
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, x * 32, y * 32);
			if (line[x] == 'C')
			{
				game->item_max++;
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, x * 32, y * 32);
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->item, x * 32, y * 32);

			}
			if (line[x] == 'P')
			{
				game->char_position_x = x * 32;
				game->char_position_y = y * 32;
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, x * 32, y * 32);
			}
			if (line[x] == 'E')
			{
				if (ft_escape(game) == 1)
				{
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, x * 32, y * 32);
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->portal, x * 32, y * 32);
				}
				else
				{
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, x * 32, y * 32);
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->locked_portal, x * 32, y * 32);
				}
			}
			x++;
		}
		result = ft_strjoin(result, line);
		line = get_next_line(fd);
		y++;
	}
	game->map = ft_split(result, '\n');
	close(fd);
	return (0);
}


//Map qui s'update quand il y a un deplacement. Fonction appellee dans ft_key, donc elle regenere la map avec les bonnes textures
int ft_change_map(t_game *game)
{
	int i;
	int j = 0;

	while (game->map[j])
	{
		i = 0;
		while (game->map[j][i])
		{
			if (game->map[j][i] == '1')
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->wall, i * 32, j * 32);
			if (game->map[j][i] == '0')
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, i * 32, j * 32);
			if (game->map[j][i] == 'C')
			{
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, i * 32, j * 32);
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->item, i * 32, j * 32);
			}
			if (game->map[j][i] == 'P')
				mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, i * 32, j * 32);
			if (game->map[j][i] == 'E')
			{
				if (ft_escape(game) == 1)
				{
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, i * 32, j * 32);
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->portal, i * 32, j * 32);
				}
				else
				{
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->floor, i * 32, j * 32);
					mlx_put_image_to_window(game->mlx, game->mlx_win, game->locked_portal, i * 32, j * 32);
				}
			}
			i++;
		}
		j++;
	}
	return (0);
}


//Pour tous les deplacements dans le jeu + orientation + compter les collectibles + exit de fin de jeu
int	ft_key(int keycode, t_game *game)
{
	mlx_clear_window(game->mlx, game->mlx_win);
	if (keycode == 53)
		exit(0);
	else if ((keycode == 2 || keycode == 124) && ft_is_valid(keycode, game))
	{
		game->orientation = 0;
		game->char_position_x += 32;
		game->moves++;
	}
	else if ((keycode == 13 || keycode == 126) && ft_is_valid(keycode, game))
	{
		if (game->orientation == 1)
			game->char_position_y -= 32;
		else
			game->char_position_y -= 32;
		game->moves++;
	}
	else if ((keycode == 1 || keycode == 125) && ft_is_valid(keycode, game))
	{
		if (game->orientation == 1)
			game->char_position_y += 32;
		else
			game->char_position_y += 32;
		game->moves++;
	}
	else if ((keycode == 0 || keycode == 123) && ft_is_valid(keycode, game))
	{
		game->orientation = 1;
		game->char_position_x -= 32;
		game->moves++;
	}
	if (game->map[game->char_position_y / 32][game->char_position_x / 32] == 'C')
	{
		game->collection++;
		game->map[game->char_position_y / 32][game->char_position_x / 32] = '0';
	}
	if ((game->map[game->char_position_y / 32][game->char_position_x / 32]) == 'E' && (game->collection == game->item_max))
	{
		exit(0);
	}
	ft_change_map(game);
	if (game->orientation == 1)
		mlx_put_image_to_window(game->mlx, game->mlx_win, game->left_hero, game->char_position_x, game->char_position_y);
	else
		mlx_put_image_to_window(game->mlx, game->mlx_win, game->right_hero, game->char_position_x, game->char_position_y);
	printf("Collected Items : %d\n", game->collection);
	return (0);
}
//Fermer le jeu quand on appuye sur le X de la fenetre
int ft_close_game(int keycode)
{
	if (keycode)
		exit(EXIT_SUCCESS);
	return (0);
}

void	ft_protection_args(int argc, char **argv)
{
	int	x;

	if (argc < 2)
	{
		printf("Il faut le nom de la map\n");
		exit(EXIT_FAILURE);
	}
	else if (argc > 2)
	{
		printf("Il faut une seule map\n");
		exit(EXIT_FAILURE);
	}
	x = ft_strlen(argv[1]) - 1;
	if ((argv[1][x] != 'r') || (argv[1][x - 1] != 'e') || (argv[1][x - 2] != 'b') || (argv[1][x - 3] != '.'))
	{
		printf("La map doit finir par .ber !\n");
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	t_game	game;
	int 	width;
	int		height;
	//Fonction qui verifie bien tous les arguments. Doit etre placee en tout premier
	ft_protection_args(argc, argv);
	game.mlx = mlx_init();
	game.moves = 0;
	game.eated = 0;
	game.collection = 0;
	game.right_hero = mlx_xpm_file_to_image(game.mlx, "textures/right_hero.xpm", &width, &height);
	game.left_hero = mlx_xpm_file_to_image(game.mlx, "textures/left_hero.xpm", &width, &height);
	game.item = mlx_xpm_file_to_image(game.mlx, "textures/item.xpm", &width, &height);
	game.floor = mlx_xpm_file_to_image(game.mlx, "textures/floor.xpm", &width, &height);
	game.wall = mlx_xpm_file_to_image(game.mlx, "textures/wall.xpm", &width, &height);
	game.portal = mlx_xpm_file_to_image(game.mlx, "textures/portal.xpm", &width, &height);
	game.locked_portal = mlx_xpm_file_to_image(game.mlx, "textures/portal_lock.xpm", &width, &height);
	ft_window_size(&game, argv[1]);
	ft_map(&game, argv[1]);
	ft_protection_lines(&game);
	ft_protection_wall(&game);
	ft_protection_items(&game);
	ft_protection_exit(&game);
	ft_protection_player(&game);
    mlx_put_image_to_window(game.mlx, game.mlx_win, game.right_hero, game.char_position_x, game.char_position_y);
    mlx_key_hook(game.mlx_win, ft_key, &game);
	mlx_hook(game.mlx_win, 17, 0L, ft_close_game, game.mlx_win);
	mlx_loop(game.mlx);
}
