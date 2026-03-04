/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokeniser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:52:13 by kmooney           #+#    #+#             */
/*   Updated: 2026/02/23 12:40:50 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokeniser.hpp"

size_t Tokeniser::_line_count = 0;

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

const std::vector<tokeniser_s>& Tokeniser::getTokens() const {
	return _tokens;
}

bool Tokeniser::isSymbol(char c) const {
	return _context.symbol_chars.find(c) != std::string::npos;
}

void Tokeniser::tokenise() {
	while (true) {
		tokeniser_s t = nextToken();
		_tokens.push_back(t);
		if (t.type == T_EOF)
			break;
	}
}

tokeniser_s Tokeniser::nextToken() {
	char c;

	while (_stream.get(c)) {
		if (c == '\n')
			_line_count++;
		if (c == '#') {
			while (_stream.get(c) && c != '\n') {}
			continue;
		}

		if (std::isspace(c))
			continue;

		if (isSymbol(c))
			return tokeniser_s(T_SYMBOL, std::string(1, c), Tokeniser::getLineCount());

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
			return tokeniser_s(T_QWORD, value, Tokeniser::getLineCount());
		return tokeniser_s(T_WORD, value, Tokeniser::getLineCount());
	}
	return tokeniser_s(T_EOF, "", Tokeniser::getLineCount());
}

size_t	Tokeniser::getLineCount(){
	return _line_count;
}

void	Tokeniser::setLineCount(){
	_line_count = 0;
}