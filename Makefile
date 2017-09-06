TARGET = expr
C_SRCFILES=${TARGET}_parser.cpp ${TARGET}_lexer.cpp ast.cpp main.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}
TOKENS=tokens.h
.PHONY:	clean

all: ${TARGET}

$(TARGET):	$(OBJ_FILES)
	g++ -g -o $@ $(OBJ_FILES)

${TARGET}_lexer.cpp: ${TARGET}.l ast.h
	flex -i -o $@ $<

${TARGET}_parser.cpp: ${TARGET}.y ast.h
	bison -Werror --report=all --defines=${TOKENS} -o $@ $<

%.o: %.cpp ${TOKENS}
	g++ -c -g -o $@ $<

clean:
	rm -f *.o
	rm -f ${TARGET}_lexer.cpp
	rm -f ${TARGET}_parser.cpp
	rm -f ${TOKENS}
	rm -f ${TARGET}
	rm -f ${TARGET}_parser.output