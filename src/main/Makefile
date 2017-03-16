GCC = g++
CFLAGS = -c
builderObjs = ys_build.o build.o
BIN_HOME = ../../bin

build:
	cd ../build; make
	${GCC} ${CFLAGS} build.cpp -o build.o
	${GCC} -o ${BIN_HOME}/build ${builderObjs}

clean:
	@rm *.o -f
	@rm ${BIN_HOME}/build -f
