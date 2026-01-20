MK_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

TEST1	=	$(MK_DIR)testPrograms/FullRequestTest/chunkedRequests/
TEST2	=	$(MK_DIR)testPrograms/FullRequestTest/invalidCharsTests/Header/
TEST3	=	$(MK_DIR)testPrograms/FullRequestTest/invalidCharsTests/URI/
TEST4	=	$(MK_DIR)testPrograms/FullRequestTest/invalidCharsTests/RLnonASCII/
TEST5	=	$(MK_DIR)testPrograms/FullRequestTest/serverRequest/
TEST6	=	$(MK_DIR)testPrograms/FullRequestTest/singleMessageTest/
TEST7	=	$(MK_DIR)testPrograms/RequestParserTests/RequestLineParserTests/methodParsing/
TEST8	=	$(MK_DIR)testPrograms/RequestParserTests/RequestLineParserTests/versionParsing/

TESTCREATED = "Test program created : "
TESTDESTROY = "Test program cleanup : "
TESTNAME1 = "chunkedTests"
TESTNAME2 = "invalidCharsTestsHeader"
TESTNAME3 = "invalidCharsTestsURI"
TESTNAME4 = "invalidCharsTestsRLnonASCII"
TESTNAME5 = "serverTest"
TESTNAME6 = "singleMessageTest"
TESTNAME7 = "methodTest"
TESTNAME8 = "versionTest"

maketests:
	@$(MAKE) -C $(TEST1) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME1)
	@$(MAKE) -C $(TEST2) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME2)
	@$(MAKE) -C $(TEST3) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME3)
	@$(MAKE) -C $(TEST4) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME4)
	@$(MAKE) -C $(TEST5) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME5)
	@$(MAKE) -C $(TEST6) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME6)
	@$(MAKE) -C $(TEST7) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME7)
	@$(MAKE) -C $(TEST8) --no-print-directory
	@echo $(TESTCREATED)$(TESTNAME8)

chunkedRequests:
	@$(MAKE) -C $(TEST1)
	@echo $(TESTCREATED)$(TESTNAME1)

invalidCharsTestsURI:
	@$(MAKE) -C $(TEST2)
	@echo $(TESTCREATED)$(TESTNAME2)

invalidCharsTestsRLnonASCII:
	@$(MAKE) -C $(TEST3)
	@echo $(TESTCREATED)$(TESTNAME3)

invalidCharsTestsHeader:
	@$(MAKE) -C $(TEST4)
	@echo $(TESTCREATED)$(TESTNAME4)

serverRequest:
	@$(MAKE) -C $(TEST5)
	@echo $(TESTCREATED)$(TESTNAME5)	

singleMessageTest:
	@$(MAKE) -C $(TEST6)
	@echo $(TESTCREATED)$(TESTNAME6)

methodParsing:
	@$(MAKE) -C $(TEST7)
	@echo $(TESTCREATED)$(TESTNAME7)

versionParsing:
	@$(MAKE) -C $(TEST8)
	@echo $(TESTCREATED)$(TESTNAME8)

fclean: clean
	@$(MAKE) fclean -C $(TEST1) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME1)
	@$(MAKE) fclean -C $(TEST2) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME2)
	@$(MAKE) fclean -C $(TEST3) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME3)
	@$(MAKE) fclean -C $(TEST4) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME4)
	@$(MAKE) fclean -C $(TEST5) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME5)
	@$(MAKE) fclean -C $(TEST6) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME6)
	@$(MAKE) fclean -C $(TEST7) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME7)
	@$(MAKE) fclean -C $(TEST8) --no-print-directory
	@echo $(TESTDESTROY)$(TESTNAME8)
	@echo "Clean up complete"
	
re: fclean all
.PHONY: all clean fclean re