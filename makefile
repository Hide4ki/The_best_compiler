CC=g++
CFLAGS=-c -Wall

SOURCES=main.cpp

DIRRUN=debug

OBJECTS=$(SOURCES:%.cpp=$(DIRRUN)\\%.o)
EXECUTABLE=$(DIRRUN)\Compiler

all: $(SOURCES) $(EXECUTABLE) clean
	echo ####RUN TASK       ####
	$(DIRRUN)\Compiler

$(EXECUTABLE): $(OBJECTS) 
	echo ####LINKING START  ####
	if not exist $(DIRRUN) mkdir $(DIRRUN)
	$(CC) $(OBJECTS) -o $@
	echo ####LINKING DONE   ####

$(DIRRUN)\\%.o: %.cpp
	echo ####COMPILING START####
	if not exist $(DIRRUN) mkdir $(DIRRUN)
	$(CC) $(CFLAGS) $< -o $@
	echo ####COMPILING DONE ####

clean: 
	echo ####CLEAN START    ####
	del $(OBJECTS)
	echo ####CLEAN DONE     ####
