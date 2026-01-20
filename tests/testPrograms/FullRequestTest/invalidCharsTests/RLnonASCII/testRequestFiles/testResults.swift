PROBLEMS
5 : ACTION : INVESTIGATE : Read Size of Zero : Incomplete message
16: ACTION : INVESTIGATE : Read Size of Zero : Incomplete message
21: ACTION : INVESTIGATE : Read Size of Zero : Incomplete message
29: ACTION : INVESTIGATE : Read Size of Zero : Incomplete message
9 : ACTION : INVESTIGATE INVALID CHAR IN THE URI 
18. ACTION : INVESTIGATE INVALID CHAR IN THE URI 

// 3. Invalid char in request line

ACTION : NONE 

	const std::string str3 =
		"POST\x01 /upload HTTP/1.1\r\n"
		"Host: test.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"4\r\nWiki\r\n"
		"5\r\npedia\r\n"
		"0\r\n\r\n";
	strings.push_back(str3);


// 4. Invalid char in request line

ACTION : NONE 

	const std::string str4 =
		"POST /empt\x7Fy HTTP/1.1\r\n"
		"Host: test.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"0\r\n\r\n";
	strings.push_back(str4);

// 5. Invalid char in request line

ACTION : INVESTIGATE : Read Size of Zero : Incomplete message 

	const std::string str5 =
		"POST /da\x00ta HTTP/1.1\r\n"
		"Host: server.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"A;ext=value\r\nHelloWorld\r\n"
		"0\r\n\r\n";
	strings.push_back(str5);

// 6. Invalid char in request line

ACTION : NONE 

	const std::string str6 =
		"POS\x19T /log HTTP/1.1\r\n"
		"Host: myhost.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"3\r\nabc\r\n"
		"3\r\ndef\r\n"
		"0\r\nTrailer-Header: value\r\n\r\n";
	strings.push_back(str6);

// 7. Invalid char in request line

ACTION : NONE 

	const std::string str7 =
		"POST /smal\x1Bl HTTP/1.1\r\n"
		"Host: test.org\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"1\r\na\r\n"
		"1\r\nb\r\n"
		"1\r\nc\r\n"
		"0\r\n\r\n";
	strings.push_back(str7);

// 8. Invalid char in request line
	
ACTION : NONE 

const std::string str8 =
		"PO\x7FST /bad HTTP/1.1\r\n"
		"Host: badhost\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"5\r\nhello\r\n";
	strings.push_back(str8);

// 9. Invalid char in request line
	
ACTION : INVESTIGATE INVALID CHAR IN THE URI 

	const std::string str9 =
		"POST /ba\x80g HTTP/1.1\r\n"
		"Host: test\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"ZZ\r\nfail\r\n"
		"0\r\n\r\n";
	strings.push_back(str9);

// 10. Invalid char in request line

ACTION : NONE 

	const std::string str10 =
		"POST /b\x19had3 HTTP/1.1\r\n"
		"Host: err\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"4hello\r\n"
		"0\r\n\r\n";
	strings.push_back(str10);

// 11. Invalid char in request line

ACTION : NONE 

	const std::string str11 =
		"POST /bad\x004 HTTP/1.1\r\n"
		"Host: bug\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"5\r\nhel\r\n"
		"0\r\n\r\n";
	strings.push_back(str11);

// 12. Invalid char in request line
	
ACTION : NONE 

const std::string str12 =
		"POST /bad5\x10 HTTP/1.1\r\n"
		"Host: test\r\n"
		"\r\n"
		"4\r\ndata\r\n"
		"0\r\n\r\n";
	strings.push_back(str12);

// 13. Invalid char in request line

ACTION : NONE 

	const std::string str13 =
		"POST /up\x7Fper HTTP/1.1\r\n"
		"Host: site.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"A\r\n0123456789\r\n"
		"0\r\n\r\n";
	strings.push_back(str13);

// 14. Invalid char in request line
	
ACTION : NONE 

const std::string str14 =
		"POST /trai\x1Fler HTTP/1.1\r\n"
		"Host: host.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"0\r\nX-Debug: true\r\n\r\n";
	strings.push_back(str14);

// 15. Invalid char in request line

ACTION : NONE 

	const std::string str15 =
		"POST /cr\x7Flf HTTP/1.1\r\n"
		"Host: data\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"B\r\nLine1\r\nLine2\r\n"
		"0\r\n\r\n";
	strings.push_back(str15);

// 16. Invalid char in request line
	
ACTION : INVESTIGATE : Read Size of Zero : Incomplete message 

const std::string str16 =
		"POST /bi\x00g HTTP/1.1\r\n"
		"Host: x\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"10\r\n0123456789ABCDEF\r\n"
		"0\r\n\r\n";
	strings.push_back(str16);

// 17. Invalid char in request line
	const std::string str17 =

ACTION : NONE 

		"POST /t\x19 HTTP/1.1\r\n"
		"Host: t.com\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"1\r\nx\r\n"
		"0\r\nT:1\r\n\r\n";
	strings.push_back(str17);

// 18. Invalid char in request line
	
  ACTION : INVESTIGATE INVALID CHAR IN THE URI 
  
  const std::string str18 =
		"POST /ne\x80g HTTP/1.1\r\n"
		"Host: bad\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"-5\r\nnope\r\n"
		"0\r\n\r\n";
	strings.push_back(str18);

// 19. Invalid char in request line

ACTION : NONE 

	const std::string str19 =
		"POST /nod\x01ata HTTP/1.1\r\n"
		"Host: h\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"3\r\n\r\n"
		"0\r\n\r\n";
	strings.push_back(str19);

// 20. Invalid char in request line

ACTION : NONE 

	const std::string str20 =
		"POST /bad\x7Ftrailer HTTP/1.1\r\n"
		"Host: h\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"3\r\nfoo\r\n"
		"0\r\nInvalidHeader\r\n\r\n";
	strings.push_back(str20);

// 21. Invalid char in request line

ACTION : INVESTIGATE : Read Size of Zero : Incomplete message 

	const std::string str21 =
		"POST /sp\x00tace HTTP/1.1\r\n"
		"Host: host\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"3 \r\nabc\r\n"
		"0\r\n\r\n";
	strings.push_back(str21);

// 22. Invalid char in request line
	
ACTION : NONE 

const std::string str22 =
		"POST /ju\x7Fnk HTTP/1.1\r\n"
		"Host: host\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"1\r\nx\r\n"
		"0\r\n\r\nGARBAGE";
	strings.push_back(str22);

// 23. Invalid char in request line
	  
ACTION : NONE 

const std::string str23 =
		"POST /ca\x19sed HTTP/1.1\r\n"
		"Host: site\r\n"
		"Transfer-Encoding: Chunked\r\n"
		"\r\n"
		"2\r\nhi\r\n"
		"0\r\n\r\n";
	strings.push_back(str23);

// 24. Invalid char in request line
	
ACTION : NONE 

const std::string str24 =
		"POST /q?\x7Fx=1 HTTP/1.1\r\n"
		"Host: s\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"6\r\nquery!\r\n"
		"0\r\n\r\n";
	strings.push_back(str24);

// 25. Invalid char in request line

ACTION : NONE 

	const std::string str25 =
		"POST /l\x00f HTTP/1.1\r\n"
		"Host: h\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"2\r\nok\n\r\n"
		"0\r\n\r\n";
	strings.push_back(str25);

// 26. Invalid char in request line

ACTION : NONE 

	const std::string str26 =
		"POST /ut\x1g9f8 HTTP/1.1\r\n"
		"Host: h\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"4\r\nåßçø\r\n"
		"0\r\n\r\n";
	strings.push_back(str26);

// 27. Invalid char in request line
	
ACTION : NONE 

const std::string str27 =
		"POST /cus\x7Ftom HTTP/1.1\r\n"
		"Host: test\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"4\r\ntest\r\n"
		"0\r\nX-Trailer: yes\r\n\r\n";
	strings.push_back(str27);

// 28. Invalid char in request line
	
ACTION : NONE 

const std::string str28 =
		"POST /mis\x7Fscrlf HTTP/1.1\r\n"
		"Host: x\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"1\r\na\r\n"
		"0\r\n";
	strings.push_back(str28);

// 29. Invalid char in request line

ACTION : INVESTIGATE : Read Size of Zero : Incomplete message 

	const std::string str29 =
		"POST /e\x00of HTTP/1.1\r\n"
		"Host: x\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"5\r\nhello\r\n";
	strings.push_back(str29);

// 30. Invalid char in request line
	
ACTION : NONE 

const std::string str30 =
		"POST /ex\x19tra HTTP/1.1\r\n"
		"Host: y\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"5z\r\nwrong\r\n"
		"0\r\n\r\n";
	strings.push_back(str30);

// 31. Invalid char in request line
	
ACTION : NONE 

const std::string str31 =
		"POST /oo\x7Fps HTTP/1.1\r\n"
		"Host: z\r\n"
		"Transfer-Encoding: chunked\r\n"
		"Content-Length: 10\r\n"
		"\r\n"
		"2\r\nhi\r\n"
		"0\r\n\r\n";
	strings.push_back(str31);

// 32. Invalid char in request line

ACTION : NONE 

	const std::string str32 =
		"POST /tr\x00ail HTTP/1.1\r\n"
		"Host: site\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"2\r\nok\r\n"
		"X-Bad: oops\r\n";
	strings.push_back(str32);
