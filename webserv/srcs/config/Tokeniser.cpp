/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokeniser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/08 16:07:02 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokeniser.hpp"

Tokeniser::Tokeniser(std::istream& input, const tokenContext_t& context) : _context(context){
	_stream << input.rdbuf();
	tokenise();
}

Tokeniser::~Tokeniser(void){}

Tokeniser::Tokeniser(const Tokeniser& other) : _context(other._context), _tokens(other._tokens){}

Tokeniser&	Tokeniser::operator=(const Tokeniser& other){
	if (this != &other){
		_context = other._context;
		_tokens = other._tokens;
	}
	return *this;
}

const std::vector<token_t>& Tokeniser::getTokens() const {
	return _tokens;
}

bool Tokeniser::isSymbol(char c) const {
	return _context.symbol_chars.find(c) != std::string::npos;
}

void Tokeniser::tokenise() {
	while (true) {
		token_t t = nextToken();
		_tokens.push_back(t);
		if (t.type == T_EOF)
			break;
	}
}

token_t Tokeniser::nextToken() {
	char c;

	while (_stream.get(c)) {

		if (c == '#') {
			while (_stream.get(c) && c != '\n') {}
			continue;
		}

		if (std::isspace(c))
			continue;

		if (isSymbol(c))
			return token_t(T_SYMBOL, std::string(1, c));

		std::string value;
		bool quoted = false;

		if (c == '"' && _context.canBeQuoted)
			quoted = true;
		else
			value += c;

		while (_stream.get(c)) {
			if (quoted) {
				if (c == '"')
					break;
			}
			else if (std::isspace(c) || isSymbol(c)) {
				_stream.unget();
				break;
			}
			value += c;
		}
		if (quoted)
			return token_t(T_QWORD, value);
		return token_t(T_WORD, value);
	}
	return token_t(T_EOF, "");
}
