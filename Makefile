# Evan Wide
#
# MAC CHORD Makefile
#
# LIBS
# pthread  	Threading
#
# Flags
# march=native
# mfpath=sse
# O3 optimization on release build
# g on debug build
# Wall on debug build
#
# Defines
# DEBUG	on debug build
# TEST on test build
#



EXEC 	= Zeke

# Commands
RM	= rm -f
RMD	= $(RM) -r
CC	= gcc
CXX	= g++
AR	= ar
TAR	= tar
MKDIR	= mkdir -p
CP	= cp

# Directories
D_SRC	= classes/
D_BUILD	= build/
D_BIN	= bin/
D_LIB	= libs/
D_INC	= classes/
D_TEST	= tests/
D_DOCS	= docs/


# Flags
CFLAGS	= -c -iquote $(D_INC)
LIBS	= -lBox2D -lsfml-graphics -lsfml-window -lsfml-system
CXFLAGS = $(CFLAGS) -std=c++11

# Generate Object file names
SRCS	= \
	  $(wildcard $(D_SRC)*.c) \
	  $(wildcard $(D_SRC)*.cpp) \
	  $(wildcard $(D_SRC)*.C) \
	  $(wildcard $(D_SRC)*.cc)

TEST_SRCS	= \
	  $(wildcard $(D_TEST)*.c) \
	  $(wildcard $(D_TEST)*.cpp) \
	  $(wildcard $(D_TEST)*.C) \
	  $(wildcard $(D_TEST)*.cc)

LIBSS		= \
	  $(wildcard $(D_LIB)*.a)

OBJS	= \
	  $(patsubst $(D_SRC)%.c, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.cpp, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.C, $(D_BUILD)%.o, \
	  $(patsubst $(D_SRC)%.cc, $(D_BUILD)%.o, $(SRCS)))))

TEST_OBJS	= \
	  $(patsubst $(D_TEST)%.c, $(D_BUILD)%.o, \
	  $(patsubst $(D_TEST)%.cpp, $(D_BUILD)%.o, \
	  $(patsubst $(D_TEST)%.C, $(D_BUILD)%.o, \
	  $(patsubst $(D_TEST)%.cc, $(D_BUILD)%.o, $(TEST_SRCS)))))

.PHONY:	all protocols release debug test clean rebuild docs
# No optimizations
all: $(D_BIN)$(EXEC)

# Optimizations
release: CFLAGS += -O3
release: all

# Debug flags, function names, and all errors reported
debug: CFLAGS += -g -DDEBUG -Wall
debug: all

# Unit testing enabled
test: CFLAGS += -Wall -DTEST
test: LIBS += -lcppunit -ldl
test: clean  $(TEST_OBJS) all

clean:
	$(RM) $(D_BUILD)*.o
	$(RM) $(D_BIN)$(EXEC)
	$(RMD) $(D_DOCS)html

rebuild: clean all

docs:
	doxygen $(D_DOCS)Doxyfile
# Create directories
$(D_TEST):
	$(MKDIR) $(D_TEST)

$(D_BUILD):
	$(MKDIR) $(D_BUILD)

$(D_BIN):
	$(MKDIR) $(D_BIN)


# Build files
# Files with headers
$(D_BUILD)%.o : $(D_SRC)%.c $(D_INC)%.h
	$(CC) $(CFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cpp $(D_INC)%.h
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cc $(D_INC)%.h
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.C $(D_INC)%.h
	$(CXX) $(CXFLAGS) $< -o $@

# Files with no headers
$(D_BUILD)%.o : $(D_SRC)%.c
	$(CC) $(CFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.C
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cpp
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_SRC)%.cc
	$(CXX) $(CXFLAGS) $< -o $@

# Build tests
$(D_BUILD)%.o : $(D_TEST)%.c
	$(CC) $(CFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_TEST)%.cc
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_TEST)%.cpp
	$(CXX) $(CXFLAGS) $< -o $@

$(D_BUILD)%.o : $(D_TEST)%.C
	$(CXX) $(CXFLAGS) $< -o $@


# Link Objects
$(D_BIN)$(EXEC): $(D_BIN) $(D_BUILD) $(LIBSS) $(OBJS)
	$(CXX) -o $(D_BIN)$(EXEC)  $(OBJS) $(LIBS)
