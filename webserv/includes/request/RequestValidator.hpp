/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2026/03/06 14:08:45 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "CLocation.hpp"
#include "RequestTypes.hpp"
#include "core_def.hpp"
#include "utilityFunctions.hpp"

class Request;
class RequestParser;

// REQUEST LINE ERRORS
#define ERR_RL_METHOD_NOT_PERMITTED "Malformed Request Line: Method not permitted "//501 (VALID METHOD BUT NOT SUPPORTED ON ROUTE)
#define ERR_RL_METHOD_NOT_SUPPORTED "Malformed Request Line: Method not supported "//501 (VALID METHOD BUT NOT SUPPORTED ON SERVER)
#define ERR_RL_METHOD_CASE "Malformed Request Line: Method must be uppercase "//400 Bad Request
#define ERR_RL_VERSION_NOT_SUPPORTED "Malformed Request Line: Unsupported HTTP Version "//505 VALID VERSION, BUT NOT SUPPORTED ON SERVER
#define ERR_RL_VERSION_NOT_RECOGNISED "Malformed Request Line: Unrecognised HTTP Version "//400 INVALID HTTP VERSION
#define ERR_RL_MISSING_SCHEME "Malformed Request Line: Scheme element missing from Absolute Form URI" // 400 Bad Request
#define ERR_RL_UNSUPPORTED_SCHEME "Malformed Request Line: Unsupported scheme" // 400 Bad Request
#define ERR_RL_PERCENT_ENCODING "Malformed Request Line: Invalid Percent Encoding (User) " // 400 Bad Request
#define ERR_RL_UTF8_ENCODING "Malformed Request Line: Invalid UTF-8 Encoding (User)" // 400 Bad Request
#define ERR_RL_PERCENT_ENCODING_U "Malformed Request Line: Invalid Percent Encoding (User) " // 400 Bad Request
#define ERR_RL_UTF8_ENCODING_U "Malformed Request Line: Invalid UTF-8 Encoding (User)" // 400 Bad Request
#define ERR_RL_PERCENT_ENCODING_P "Malformed Request Line: Invalid Percent Encoding (Pass) " // 400 Bad Request
#define ERR_RL_UTF8_ENCODING_P "Malformed Request Line: Invalid UTF-8 Encoding (Pass)" // 400 Bad Request
#define ERR_RL_HOST_MISSING "Malformed Request Line: Missing Host Element " //400 Bad Request
#define ERR_RL_PERCENT_ENCODING_HOST "Malformed Request Line: Invalid Percent Encoding (Host)"//400 Bad Request
#define ERR_RL_UTF8_ENCODING_HOST "Malformed Request Line: Invalid UTF-8 Encoding (Host)"//400 Bad Request
#define ERR_RL_PERCENT_ENCODING_PATH "Malformed Request Line: Invalid Percent Encoding (Path)"//400 Bad Request
#define ERR_RL_UTF8_ENCODING_PATH "Malformed Request Line: Invalid UTF-8 Encoding (Path)"//400 Bad Request
#define ERR_RL_PERCENT_ENCODING_QUERY "Malformed Request Line: Invalid Percent Encoding (Query)"//400 Bad Request
#define ERR_RL_UTF8_ENCODING_QUERY "Malformed Request Line: Invalid UTF-8 Encoding (Query)"//400 Bad Request
#define ERR_RL_PERCENT_ENCODING_FRAG "Malformed Request Line: Invalid Percent Encoding (Frag)"//400 Bad Request
#define ERR_RL_UTF8_ENCODING_FRAG "Malformed Request Line: Invalid UTF-8 Encoding (Frag)"//400 Bad Request
#define ERR_RL_PATH_SYNTAX "Malformed Request Line: Path Syntax" //400 Bad Request

//MP HEADER ERRORS
#define ERR_NO_MATCHING_SERVER "Headers: No server matches 'Host' header value " //404?
#define ERR_MP_BOUNDARY_LENGTH "Multipart Boundary string is too long" //415
#define ERR_MP_BOUNDARY_MISSING "Missing multiPart boundary"  
#define ERR_MP_TYPE_NOT_SUPPORTED "MultiPart type not supported" //415 Unsupported Media Type

//MP PART ERRORS
#define ERR_MP_CONTENT_DISPOSITION_MISSING "Malformed Body: Invalid multipart header: missing content-disposition"
#define ERR_MP_CONTENT_DISPOSITION_MULTI_VALS "Malformed Body: Invalid multipart header: multiple content-disposition values"
#define ERR_MP_CONTENT_DISPOSITION_UNEXPECTED_VALUE "Malformed Body: Invalid Content-Disposition: unexpected content-disposition value"
#define ERR_MP_CONTENT_DISPOSITION_NO_NAME "Malformed Body: Invalid Content-Disposition: no name parameter" 
#define ERR_MP_CONTENT_DISPOSITION_DUPLICATE_PARAMS "Malformed Body: Invalid Content-Disposition: Multiple instances of same parameter" //415
#define	ERR_MP_CONTENT_DISPOSITION_FILENAME_MISSING "Malformed Body: Invalid Content-Disposition: Multiple instances of same parameter"
#define ERR_MP_CONTENT_TYPE_NOT_SUPPORTED "Malformed Body: Content-Type(s): not supported" //415 Unsupported Media Type

enum HeaderFlag
{
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

		void 							validate();

	private:

		Request*						_req;
		//std::map<std::string, 		int>	hdr_seen;
		//const HeaderFlags*					_hdr_flags;

		void							validateReqLine();

		bool							validateMethod();
		bool							isValidMethod(const std::string& str);

		bool							validateURI();
		bool							validateScheme();
		bool							validateUser();
		bool							validatePass();
		bool							validateHost();
		bool							validatePort();
		bool							validatePath();
		bool							validateQuery();
		bool							validateFrag();
		bool							validateVersion();

		void							validateHeaders();
		bool							validateHostHeader();
		bool							validateAbsoluteForm(fieldMap_t::const_iterator& it);
		bool							validateRepresentation();
		bool							identifyLengthHeader();
		bool							validateTransferEncoding();
		bool							validateContentLength();
		bool							convertContentLength();
		bool							validateContentEncoding();
		bool							validateMultiPartHeader(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);

		bool							validateContentType();
		bool							validateParamsForContentType(const std::string& type, const std::vector<std::pair<str_t, str_t> > params);
		//bool							validateCharset(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);
		bool							validateCharset(const std::vector< std::pair<std::string, std::string> >& options);

		bool							duplicateOptionCheck(const std::vector< std::pair<std::string, std::string> >& options);
		bool							duplicateValueCheck(const std::vector<field_t*>& values);

		bool							validateAllHeaders(const fieldMap_t& headers);
		//bool							validate_header(const std::string& name, const std::vector<HeaderField_t*>& fields, const HeaderFlags* meta);
		void							headerDefinitions();

	//	const HeaderFlags*				findHeaderFlags(const std::string& name);
		int								compute_received_flags(const std::string& name, std::vector<field_t*> values);

		bool							validateMethodHeaders(void);

		void							validateBody(void);

		bool							validateMPheaders(void);
		bool							validateMPContentDisposition(mpPart_t& part, const std::vector<field_t*>& header);
		bool							validateMPContentType(mpPart_t& part, const std::vector<field_t*>& header);
		bool							updateMPparams(mpPart_t& part, const std::vector<std::pair< str_t, str_t> >& params);
		bool							validateMultiPartBody(void);

		void							validateTrailers(void);

		int								countHeaderValues (const fieldMap_t& headers, const std::string& headerName);
		int								countOptions(const std::vector<std::pair<std::string, std::string> >& options);
		bool							percentDecode( str_t& encoded);

		bool							testHeaderPresent(std::string& name);
		const std::vector<field_t*>*	getHeader(const fieldMap_t& headers, const str_t& name);
		const std::vector<field_t*>*	getHeader(const fieldMap_t* headers, const str_t& name);

	};

bool	temporaryIsValidContentType(const std::string& name);
