/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:04:19 by kmooney           #+#    #+#             */
/*   Updated: 2025/06/22 16:57:11 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef TESTUTILS_HPP
#	define TESTUTILS_HPP

# ifdef _TEST
# define test(x) ((x))
# else
# define test(x)
# endif

# ifdef _CLASS
# 	define classText(x) ((x))
# else
#	define classText(x)
# endif

#include <iostream>

# define DEF "\033[0m"
# define RED "\033[31m"
# define GRE "\033[32m" 
# define YEL "\033[33m"
# define BLU "\033[34m"
# define MAG "\033[35m"
# define CYA "\033[36m"
# define WHI "\033[37m"
# define RY "\033[31;43m"
# define RW "\033[31;47m"
# define YR "\033[33;41m"
# define WG "\033[38;2;255;255;255;48;2;0;128;0m"
# define WB "\033[96;44m"
# define WM "\033[37;45m"

# define GRN 1
# define DFLT 2
# define WBWG 3
# define WBG 4
# define RYYR 5

std::string iToStr(const int i);
void		mainHeading(const std::string text,  bool lead_nl, bool trail_nl, const char *colour);
void		textColoured(const std::string text, bool lead_nl, bool trail_nl, const char* colour);
void		textTwoFields(const std::string p1, const std::string p2, bool lead_nl, bool trail_nl, const char* col1, const char* col2);
void		newLine( void );
#endif