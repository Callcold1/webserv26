/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                              	        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 10:47:44 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/21 16:02:30 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(void) : _pos(0) {}
Parser::Parser(const std::vector<tokeniser_s>& tokens) : _tokens(tokens), _pos(0) {}
Parser::~Parser(){}

bool Parser::eof() const {
	return peek().type == T_EOF;
}

const tokeniser_s& Parser::peek(size_t offset) const {
	if (_pos + offset >= _tokens.size())
		throw std::runtime_error("Parser: peek out of range");
	return _tokens[_pos + offset];
}

const tokeniser_s& Parser::consume() {
	if (_pos >= _tokens.size())
		throw std::runtime_error("Parser: consume past EOF");
	return _tokens[_pos++];
}

bool Parser::check(TokenType_e type, const std::string& value) const {

	const tokeniser_s& t = peek();

	if (t.type != type)
		return false;
	if (!value.empty() && t.value != value)
		return false;
	return true;
}

void Parser::expect(TokenType_e type, const std::string& value) {

	const tokeniser_s& t = consume();

	if (t.type != type || (!value.empty() && t.value != value)) {
		throw std::runtime_error("Parser error: unexpected token '" + t.value + "'");
	}
}
