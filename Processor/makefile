CC = g++
ASS_INCDIR = inc
ASS_SRCDIR = src
ASS_OBJDIR = obj
OBJCOMPFLAGS = -c -g -Wall -I $(ASS_INCDIR)
ASSCOMPFLAGS = -g -Wall -I ASS_INCDIR
ASS_EXECUTABLE = processor.asm
ASS_OBJECTS = $(ASS_OBJDIR)/Lexer.o $(ASS_OBJDIR)/Assembling.o $(ASS_OBJDIR)/AssemblingMain.o
ASS_SRC = $(ASS_OBJDIR)/Lexer.cpp $(ASS_OBJDIR)/Assembling.cpp $(ASS_OBJDIR)/AssemblingMain.cpp
ASM: $(ASS_EXECUTABLE)

$(ASS_OBJDIR)/Lexer.o: $(ASS_SRCDIR)/Lexer.cpp
	$(CC) $(OBJCOMPFLAGS) $^ -o $@

$(ASS_OBJDIR)/Assembling.o: $(ASS_SRCDIR)/Assembling.cpp
	$(CC) $(OBJCOMPFLAGS) $^ -o $@

$(ASS_OBJDIR)/AssemblingMain.o: $(ASS_SRCDIR)/AssemblingMain.cpp
	$(CC) $(OBJCOMPFLAGS) $^ -o $@

$(ASS_EXECUTABLE): $(ASS_OBJECTS)
	$(CC) $(ASSCOMPFLAGS) $^ -o $@

PHONY.:clean

	clean:
		rm -f obj/*
		rm -f processor.asm
