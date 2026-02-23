/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp        	     	                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:55:28 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:02:39 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <stdexcept>
#include <string>

#include "Tokeniser.hpp"

struct node_t {
	
	std::string				    type;
	std::vector<std::string>	args;
	std::vector<node_t*>	    children;

	node_t(){}
	node_t(std::string s, std::vector<std::string> a, std::vector<node_t*> c) : type(s), args(a), children(c){}
};

class Parser {

public:
	Parser( void );
	Parser(const std::vector<tokeniser_s>& tokens);

	virtual node_t* parse() = 0;

	const tokeniser_s&  peek(size_t offset = 0) const;
	const tokeniser_s&  consume();
	bool            eof() const;

	void            expect(TokenType_e type, const std::string& value = "");
	bool            check(TokenType_e type, const std::string& value = "") const;

protected:
	std::vector<tokeniser_s>   _tokens;
	size_t                 _pos;
};
