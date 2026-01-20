# TESTS



## FILES AND FOLDERS

- Makefile : The makefile in the root of this folder can be used to compile all tests added to it.

- testPrograms : this folder contains test programs

- the test programs are categorized by type (e.g. FullRequestTest)

    each test has it's own folder that contains :

        testRequestFiles : a copy of the request message from each test is generated and stored here.    
                             (I was also sending the test output to a file here, so that it can be analysed/compared to previous tests)

        Makefile : called by the makefile in the root of the test folder (needs to include the following)
                
                SRCS:= Request/Request.cpp \
                       Request/RequestParser.cpp \
                       Request/RequestValidator.cpp \
                       Request/utility_functions.cpp \
                       Request/TestClasses/testUtils.cpp \
                       Config/Config.cpp \
                       Config/Route.cpp \
                       Config/ConfigParser.cpp \

                       tests/testPrograms/testMain/tests.cpp\
                       tests/testPrograms/testMain/testFunctions.cpp \
                       
                       tests/testPrograms/FullRequestTest/chunkedRequests/testCases.cpp **This file should be the path to the test being added**

        testCases.cpp : contains the test cases in char array format, and contains the corresponding testProfile (a char array and testProfile needs to be created for each test)

        testConfig.conf : file containing server config

### Adding a test to the main Makefile (tests/Makefile)

For TESTX, TESTNAMEX replace the X with the next number (based on the number of tests already in the makefile)

1.Add TESTX to the end of the list of tests

    TESTX = $(MK_DIR)testPrograms/testFolderPath

    e.g. TEST9 = $(MK_DIR)testPrograms/FullRequestTest/myTest

2. Add TESTNAMEX = "test" (X must be the same as in 1.)

    TESTNAME9 = myTest

3. Add command to "maketests" (Compiles all tests in makefile)

Add the following 2 lines to "maketests:" as follows: (NOTE WHEN MAKE IS RUN, ALL TESTS WILL BE CREATED)

    maketests:

        @$(MAKE) -C $(TESTX) --no-print-directory
        @echo $(TESTCREATED)$(TESTNAMEX)

    e.g.

    maketests:

        @$(MAKE) -C $(TEST9) --no-print-directory
        @echo $(TESTCREATED)$(TESTNAME9)
    
4. Create new test definition "myTest" (So that it can be compiled in isolation)

Add the following (if the name of the name of the test is to be "myTest")

    myTest:
        @$(MAKE) -C $(TEST9)
        @echo $(TESTCREATED)$(TESTNAME9)

5. Create new test definition "myTest" (So that it can be compiled in isolation)

Add the following to fclean:

    fclean: clean
    
        @$(MAKE) fclean -C $(TESTX) --no-print-directory
        @echo $(TESTDESTROY)$(TESTNAMEX)

    e.g. 

            fclean: clean
    
                @$(MAKE) fclean -C $(TEST9) --no-print-directory
                @echo $(TESTDESTROY)$(TESTNAME9)

## Creating a new test

To create a new test :

- copy one of the existing test folders in the testPrograms folder
- change the name of the NEWFOLDER as appropriate
- change the Makefile in NEWFOLDER so that the last source file is 'tests/testPrograms/NEWFOLDERNAME/testCases.cpp'
- change the Makefile program name to reflect the new test name'
- add the new test to the main test makefile (see above "### Adding a test to the main Makefile (tests/Makefile)")
- delete the contents of 'tests/testPrograms/NEWFOLDERNAME/testRequestFiles'
- alter testCases.cpp to reflect the tests to be run (the char array and testProfiles can be created/generated - see generateTestFormats folder/subfolders)

## Other information

The files in testMain are required for compilation and contain the testing logic (testFunctions.cpp/hpp and tests.cpp/hpp)
The file testCases.cpp contain the Request Message string, the tests to be run, and the expected results (in testProfile function)