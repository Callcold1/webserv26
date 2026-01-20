#pragma once

#define TEST_BUFFER_SIZE 30

#include "Request.hpp"
#include "RequestParser.hpp"
#include "ConfigParser.hpp"
#include "core_def.hpp"
#include "TestClasses/testUtils.hpp"
#include "testFunctions.hpp"
#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

void		populateTestStringsVector(std::vector<std::string>& strings);
void		testProfileCreation(std::vector<testProfile *>& testProfiles);

template <typename T>
std::string toString(const T val);

void		writeStringToFile(const std::string& name, const std::string& filename);
void		printEscaped(const std::string& data);
void		printErrorString(int i);
void		printMessage(int fd, std::vector<std::string>& strings);
void		printHeaders(fieldMap_t headers);
void		printRequest(Request* request);
size_t  	len(const char* str);
void		populateMap(std::map<int, Request* >& map, Config* config);
void		cleanUpMap(int fd, Request* request);
void		server(Config* config, std::vector<std::string>& strings);
