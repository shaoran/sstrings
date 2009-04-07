TESTS=test
check_PROGRAMS = test
test_SOURCES = main.c 
INCLUDE = -I@top_srcdir@/sstrings

check-am:
	gcc -otest $(test_SOURCES) ../sstrings.c $(INCLUDE)

#check: check-am
#	./test
