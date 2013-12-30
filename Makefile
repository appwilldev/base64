uname_M := $(shell sh -c 'uname -m 2>/dev/null || echo not')
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
CXXFLAGS = -std=c++11 -Wall -Wextra -stdlib=libc++ -g -O2
# LDLIBS = -lboost_regex-mt -lboost_system-mt -lboost_thread-mt
OBJS := base64.o test.o
LDLIBS =
ifeq ($(uname_S),Darwin)
CC = clang
CXX = clang++
endif

ifeq ($(uname_S),Linux)
CC = gcc
CXX = g++
endif

-include $(OBJS:.o=.d)

%.o: %.c Makefile
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	$(CC) -MM -MP $(CFLAGS) $*.c > $*.d
%.o: %.cpp Makefile
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) -MM -MP $(CXXFLAGS) $*.cpp > $*.d
%.o: %.cc Makefile
	$(CXX) -g -c $(CXXFLAGS) $*.cc -o $*.o
	$(CXX) -MM -MP $(CXXFLAGS) $*.cc > $*.d
%.s: %.cc Makefile
	$(CXX) -S $(CXXFLAGS) $*.cc -o $*.s
	$(CXX) -MM -MP $(CXXFLAGS) $*.cc > $*.d
# %.out: $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $*.out $(LDLIBS) $(OBJS)
# 	$(CXX) -MM -MP $(CXXFLAGS) $*.cc > $*.d
test.out: $(OBJS) Makefile
	$(CXX) $(CXXFLAGS) -o $*.out $(LDLIBS) $(OBJS)
	$(CXX) -MM -MP $(CXXFLAGS) $*.cc > $*.d
all:test.out
.PHONY:clean
clean:
	rm -f *.o *.s *.i $(ELFS) *.d *.out