/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dduraku <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:07:15 by dduraku           #+#    #+#             */
/*   Updated: 2022/03/09 16:07:17 by dduraku          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

typedef struct s_game
{
	void	*mlx;
	void	*mlx_win;
	void	*right_hero;
	void	*left_hero;
	void	*floor;
	void	*wall;
	void	*portal;
	void	*locked_portal;
	void	*item;
	char	**map;
	int		sprite_x;
	int		sprite_y;
	int 	char_position_x;
	int		char_position_y;
	int		orientation;
	int		collection;
	int		item_max;
	int		item_x;
	int		item_y;
	int		eated;
	int		moves;
}				t_game;

#endif
