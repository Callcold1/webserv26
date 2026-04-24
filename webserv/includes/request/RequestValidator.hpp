/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:37:52 by kmooney           #+#    #+#             */
/*   Updated: 2026/04/24 17:43:38 by kmooney          ###   ########.fr       */
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
#define ERR_RL_METHOD_NOT_PERMITTED "Malformed Request Line: Method not permitted "
#define ERR_HEADERS_METHOD_NOT_PERMITTED "Location error: Method not permitted "
#define ERR_RL_METHOD_NOT_SUPPORTED "Malformed Request Line: Method not supported "
#define ERR_RL_METHOD_CASE "Malformed Request Line: Method must be uppercase "
#define ERR_RL_VERSION_NOT_SUPPORTED "Malformed Request Line: Unsupported HTTP Version "
#define ERR_RL_VERSION_NOT_RECOGNISED "Malformed Request Line: Unrecognised HTTP Version "
#define ERR_RL_MISSING_SCHEME "Malformed Request Line: Scheme element missing from Absolute Form URI"
#define ERR_RL_UNSUPPORTED_SCHEME "Malformed Request Line: Unsupported scheme"
#define ERR_RL_PERCENT_ENCODING "Malformed Request Line: Invalid Percent Encoding (User) "
#define ERR_RL_UTF8_ENCODING "Malformed Request Line: Invalid UTF-8 Encoding (User)"
#define ERR_RL_PERCENT_ENCODING_U "Malformed Request Line: Invalid Percent Encoding (User) "
#define ERR_RL_UTF8_ENCODING_U "Malformed Request Line: Invalid UTF-8 Encoding (User)"
#define ERR_RL_PERCENT_ENCODING_P "Malformed Request Line: Invalid Percent Encoding (Pass) "
#define ERR_RL_UTF8_ENCODING_P "Malformed Request Line: Invalid UTF-8 Encoding (Pass)"
#define ERR_RL_HOST_MISSING "Malformed Request Line: Missing Host Element "
#define ERR_RL_PERCENT_ENCODING_HOST "Malformed Request Line: Invalid Percent Encoding (Host)"
#define ERR_RL_UTF8_ENCODING_HOST "Malformed Request Line: Invalid UTF-8 Encoding (Host)"
#define ERR_RL_PERCENT_ENCODING_PATH "Malformed Request Line: Invalid Percent Encoding (Path)"
#define ERR_RL_UTF8_ENCODING_PATH "Malformed Request Line: Invalid UTF-8 Encoding (Path)"
#define ERR_RL_PERCENT_ENCODING_QUERY "Malformed Request Line: Invalid Percent Encoding (Query)"
#define ERR_RL_UTF8_ENCODING_QUERY "Malformed Request Line: Invalid UTF-8 Encoding (Query)"
#define ERR_RL_PERCENT_ENCODING_FRAG "Malformed Request Line: Invalid Percent Encoding (Frag)"
#define ERR_RL_UTF8_ENCODING_FRAG "Malformed Request Line: Invalid UTF-8 Encoding (Frag)"
#define ERR_RL_PATH_SYNTAX "Malformed Request Line: Path Syntax"

//MP HEADER ERRORS
#define ERR_NO_MATCHING_SERVER "Headers: No server matches 'Host' header value "
#define ERR_NO_MATCHING_LOCATION "Headers: No location match found on matched server"
#define ERR_MP_BOUNDARY_LENGTH "Multipart Boundary string is too long"
#define ERR_MP_BOUNDARY_MISSING "Missing multiPart boundary"
#define ERR_MP_TYPE_NOT_SUPPORTED "MultiPart type not supported"

//MP PART ERRORS
#define ERR_MP_CONTENT_DISPOSITION_MISSING "Malformed Body: Invalid multipart header: missing content-disposition"
#define ERR_MP_CONTENT_DISPOSITION_MULTI_VALS "Malformed Body: Invalid multipart header: multiple content-disposition values"
#define ERR_MP_CONTENT_DISPOSITION_NO_NAME "Malformed Body: Invalid Content-Disposition: no name parameter"
#define ERR_MP_CONTENT_DISPOSITION_NAME_MULTI "Malformed Body: Invalid multipart header: multiple filename values" //400?
#define ERR_MP_CONTENT_DISPOSITION_FILENAME_MULTI "Malformed Body: Invalid multipart header: multiple filename values" //400?
#define ERR_MP_CONTENT_DISPOSITION_FILENAME_STAR_MULTI "Malformed Body: Invalid multipart header: multiple filename* values" //400?
#define ERR_MP_CONTENT_DISPOSITION_UNEXPECTED_VALUE "Malformed Body: Invalid Content-Disposition: unexpected content-disposition value"
#define ERR_MP_CONTENT_DISPOSITION_DUPLICATE_PARAMS "Malformed Body: Invalid Content-Disposition: Multiple instances of same parameter" //415
#define	ERR_MP_CONTENT_DISPOSITION_FILENAME_MISSING "Malformed Body: Invalid Content-Disposition: No filename parameter provided"
#define	ERR_MP_CONTENT_DISPOSITION_FILENAME_ERROR "Malformed Body: Invalid Content-Disposition: Unsafe filename"
#define	ERR_MP_CONTENT_DISPOSITION_FILENAME_DUP "Malformed Body: Invalid Content-Disposition: Duplicate filename is forbidden"
#define ERR_MP_CONTENT_TYPE_NOT_SUPPORTED "Malformed Body: Content-Type(s): not supported" //415 Unsupported Media Type

class RequestValidator {

	public:
										RequestValidator(Request* request);
										~RequestValidator();

		void 							validate();

	private:

		Request*						_req;

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
		bool							validateConnectionHeader();
		bool							validateRepresentation();
		bool							identifyLengthHeader();
		bool							validateTransferEncoding();
		bool							validateContentLength();
		bool							convertContentLength();
		bool							validateContentEncoding();
		bool							validateMultiPartHeader(const std::string& val, const std::vector< std::pair<std::string, std::string> >& options);

		bool							validateContentType();
		bool							validateParamsForContentType(const std::string& type, const std::vector<std::pair<str_t, str_t> > params);
		bool							validateCharset(const std::vector< std::pair<std::string, std::string> >& options);

		bool							duplicateOptionCheck(const std::vector< std::pair<std::string, std::string> >& options);
		bool							duplicateValueCheck(const std::vector<field_t*>& values);

		bool							validateMethodHeaders(void);

		void							validateBody(void);

		bool							validateMPheaders(void);
		bool							validateMPContentDisposition(mpPart_t& part, const std::vector<field_t*>& header);
		bool							validateMPContentType(mpPart_t& part, const std::vector<field_t*>& header);
		bool							updateMPparams(mpPart_t& part, const std::vector<std::pair< str_t, str_t> >& params);
		bool							validateMultiPartBody(void);

		bool							sanitiseFilename(std::string& name);
		bool							sanitiseFilenameStar(std::string& name);
		bool							splitFilename(std::string& name, std::vector<std::string>& vec);
		void							validateTrailers(void);

		int								countHeaderValues (const fieldMap_t& headers, const std::string& headerName);
		bool							percentDecode( str_t& encoded);

		const std::vector<field_t*>*	getHeader(const fieldMap_t& headers, const str_t& name);
		const std::vector<field_t*>*	getHeader(const fieldMap_t* headers, const str_t& name);

		bool							isValidMediaType(const std::string& value);
		bool							isSupportedMediaType(const std::string& type);

		bool							isValidCharset(std::string& value);
		bool							isValidToken(std::string& value);

										/* CHECK IF SUPPORTED ON MATCHED SERVER / LOCATION */
		bool							isSupportedCharset(const std::string& value);

		bool							validateFileFormat(std::string& path);

};
