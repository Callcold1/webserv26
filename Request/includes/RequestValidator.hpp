/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2025/06/23 17:09:49 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "RequestTypes.hpp"
#include "core_def.hpp"
#include "utility_functions.hpp"

class Request;
class RequestParser;
class Config;
class Route;

enum HeaderFlag {
	HDR_MANDATORY        = 1 << 1, // TRUE IF HEADER MUST BE PRESENT IN MESSAGE
	HDR_ONLY_ONCE	 	 = 1 << 2, // TRUE IF HEADER MUST ONLY APPEAR ONCE IN HEADERS
	HDR_ONLY_ONE_VAL_	 = 1 << 3, // TRUE IF HEADER MUST ONLY CONTAIN A SINGLE VALUE
	HDR_CASE_SENS_VALUE  = 1 << 4, // TRUE IF HEADER IS CASE SENSITIVE (i.e must)
	HDR_MUST_BE_UPPER	 = 1 << 5, // TRUE IF HEADER IS CASE SENSITIVE & MUST BE UPPERCASE
	HDR_MUST_BE_LOWER    = 1 << 6, // TRUE IF HEADER IS CASE SENSITIVE & MUST BE LOWERCASE
	HDR_VALUE_IS_FORMAT  = 1 << 7, // TRUE IF VALUE MUST BELONG TO A KNOWN GROUP OF VALUES
	HDR_PARAM_IS_FORMAT  = 1 << 8, // TRUE IF PARAMETERS MUST BELONG TO A KNOWN GROUP OF PARAMETERS
	HDR_SERVER_CAPABLE   = 1 << 9, // TRUE IF SERVER IS CAPABLE OF HANDLING THIS HEADER
	HDR_ONE_OR_OTHER	 = 1 << 10 // TRUE IF THE HEADER IF THIS HEADER AND OTHERS ARE MUTUALLY EXCLUSIVE (ONLY ONE OF THE TWO CAN BE PRESENT)
};

// struct header_context {
// 	std::string					name;
// 	std::vector<std::string> 	accepted_values;
// 	uint64_t					flags;

// 	header_context(std::string n, std::vector<std::string> av, uint64_t flags):
// 	name(n), accepted_values(), flags(){}
// };

#define TRANSFER_ENCODING 1111000000 //ORDER IS FROM RIGHT TO LEFT (LEFTMOST VALUE IS BOTTOM OF LIST)
// HDR_MANDATORY       	0
// HDR_ONE_INST_ONLY 	0
// HDR_ONE_VAL_ONLY		0
// HDR_VALUE_CASE_SENS 	0
// HDR_MUST_BE_UPPER	0	
// HDR_MUST_BE_LOWER   	0
// HDR_VALUE_CHECK     	1
// HDR_OPTION_CHECK    	1
// HDR_SERVER_CAPABLE  	1
// HDR_ONE_OR_OTHER		1
// HDR_

class RequestValidator {
	
	public:
							RequestValidator(Request* request);
							~RequestValidator();

		void 				validate();

	private:

		Request*			_req;
		Route*				_route;
		Config*				_config;
		
		//std::map<std::string, int>	hdr_seen;
		//const HeaderFlags*			_hdr_flags;

		void				validateReqLine();
		bool				validateMethod();
		void				setRoute();	
		bool				serverSupportsMethod(const str_t& method);
		bool				validateURI();
		bool				validateScheme();
		bool				validateUser();
		bool				validatePass();
		bool				validateHost();
		bool				validatePort();
		bool				validatePath();
		bool				validateQuery();
		bool				validateFrag();
		bool				validateVersion();

		void				validateHeaders();
		bool				validateHostHeader();

		bool				validateRepresentation();
		bool				identifyLengthHeader();
		bool				validateTransferEncoding();
		bool				validateContentLength();
		bool				convertContentLength();
		bool				validateContentEncoding();

		bool				validateContentType();
		bool				validateMultiPart(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);
		//bool				validateCharset(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);
		bool				validateCharset(const std::vector< std::pair<std::string, std::string> >& options);
		
		bool				duplicateOptionCheck(const std::vector< std::pair<std::string, std::string> >& options);
		bool				duplicateValueCheck(const std::vector<field_t*>& values);
		
		bool				validateAllHeaders(const fieldMap_t& headers);
		bool				validate_all_headers(const fieldMap_t& headers);
		//bool				validate_header(const std::string& name, const std::vector<HeaderField_t*>& fields, const HeaderFlags* meta);
		void				headerDefinitions();
	
	//	const HeaderFlags*	findHeaderFlags(const std::string& name);
		int					compute_received_flags(const std::string& name, std::vector<field_t*> values);
		
		bool				validateMethodHeaders();
		
		void				validateBody();
		
		void				validateTrailers();

		int					countHeaderValues (const fieldMap_t& headers, const std::string& headerName);
		int					countOptions(const std::vector<std::pair<std::string, std::string> >& options);
		bool				percentDecode( str_t& encoded);

		bool				testHeaderPresent(std::string& name);
};

bool	temporaryIsValidContentType(const std::string& name);
