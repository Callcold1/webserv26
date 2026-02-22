/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokeniser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:22 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 09:37:52 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

enum TokenType_e {
	T_WORD,
	T_QWORD,
	T_SYMBOL,
	T_EOF
};

struct token_t {
	TokenType_e type;
	std::string value;

	token_t(TokenType_e k, const std::string& v)
		: type(k), value(v) {}
};

struct tokenContext_t {
	std::string symbol_chars;
	bool        canBeEscaped;
	bool        canBeQuoted;

	tokenContext_t(const std::string& symbols, bool escaped, bool quoted)
		: symbol_chars(symbols), canBeEscaped(escaped), canBeQuoted(quoted) {}
};

class Tokeniser {

public:

	Tokeniser(std::istream& input, const tokenContext_t& context);
	Tokeniser(const Tokeniser& other);
	Tokeniser&	operator=(const Tokeniser& other);
	~Tokeniser();

	const std::vector<token_t>&	getTokens() const;

private:

	tokenContext_t          _context;
	std::stringstream       _stream;
	std::vector<token_t>   _tokens;

	void	tokenise();
	token_t	nextToken();

	bool	isSymbol(char c) const;
};
