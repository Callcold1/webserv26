/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 22:59:34 by kmooney           #+#    #+#             */
/*   Updated: 2025/06/10 18:29:41 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/TestClasses/testUtils.hpp"
#include <sstream>
#include <string>

std::string	iToStr(const int i){
	std::stringstream ss;

	ss << i ;
	std::string str = ss.str();
	return (str);
}

void	textTwoFields(const std::string p1, const std::string p2, bool lead_nl, bool trail_nl, const char* col1, const char* col2){
	if (p1 != "" || p2 != "")
	{
		if (lead_nl == true)
			newLine();
		std::cout << col1 << p1 << DEF << col2 << p2 << DEF << std::endl;
		if (trail_nl == true)
			newLine();
	}
}

void	textColoured(const std::string text, bool lead_nl, bool trail_nl, const char* colour){
	if (text != "")
	{
		if (lead_nl == true)
			newLine();
		std::cout << colour << text << DEF << std::endl;
		if (trail_nl == true)
			newLine();
	}
}

void	mainHeading(const std::string text,  bool lead_nl, bool trail_nl, const char* colour){
	std::string	stars;
	int			len;

	if (text != "")
	{
		if (lead_nl == true)
			newLine();
		len = text.length();
		for (int i = 0; i < len; i++)
		{
			stars += "*";
		}	
		std::cout << std::endl;
		std::cout << colour << stars << DEF << std::endl;
		std::cout << colour << text << DEF << std::endl;
		std::cout << colour << stars << DEF << std::endl;
		if (trail_nl == true)
			newLine();
	}
}

void	newLine(){
	std::cout << std::endl;
}

