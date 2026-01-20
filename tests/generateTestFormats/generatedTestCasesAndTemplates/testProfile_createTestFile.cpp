/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testProfile_createTestFile.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:56:36 by kmooney           #+#    #+#             */
/*   Updated: 2026/01/20 14:31:25 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

This file is for use with createTestFile.cpp

For new tests, copy the template below, populate it for each test required and paste it into 
the main of createTestFile.cpp before compiling and redirecting the output to a testfile.

The output can be moved from the testfile to the appropriate test in tests/testPrograms/xxx relevantTestProgramFolder xx/testCases.cpp

The following fields must be populated :

    TestSpec t3;
    t3.method = "POST";
    t3.uri = "/upload";
    t3.version = "HTTP/1.1";
    t3.headers.push_back(std::make_pair("Host", "example.com"));
    t3.headers.push_back(std::make_pair("Transfer-Encoding", "chunked"));
    t3.body = "Wikipedia";
    t3.chunked = true;

    t3.uri_type = ORIGIN_FORM;
    t3.port_int = 80;
    t3.path = "/upload";

    t3.errNum = 200;
    t3.errMess = "Parsing Complete";
    t3.p_state = "P_DONE";

    tests.push_back(t3);

For each test, a unique TestSpec must be created.
multiple headers can be added.

errNum - expected HTTP response code
errMess - expected message (see )

*/

/*	TestSpec t3 
	Valid 
*/
TestSpec t3;
t3.method = "POST";
t3.uri = "/upload";
t3.version = "HTTP/1.1";
t3.headers.push_back(std::make_pair("Host", "example.com"));
t3.headers.push_back(std::make_pair("Transfer-Encoding", "chunked"));
t3.body = "Wikipedia";
t3.chunked = true;

t3.uri_type = ORIGIN_FORM;
t3.port_int = 80;
t3.path = "/upload";

t3.errNum = 200;
t3.errMess = "Parsing Complete";
t3.p_state = "P_DONE";

tests.push_back(t3);

TestSpec t3;
t3.method = "POST";
t3.uri = "/upload";
t3.version = "HTTP/1.1";
t3.headers.push_back(std::make_pair("Host", "example.com"));
t3.headers.push_back(std::make_pair("Transfer-Encoding", "chunked"));
t3.body = "Wikipedia";
t3.chunked = true;

t3.uri_type = ORIGIN_FORM;
t3.port_int = 80;
t3.path = "/upload";

t3.errNum = 200;
t3.errMess = "Parsing Complete";
t3.p_state = "P_DONE";

tests.push_back(t3);