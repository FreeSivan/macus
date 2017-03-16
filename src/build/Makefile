GCC = g++
CFLAGS = -c 
OBJS = ys_build.o 

all:${OBJS}

clean:
	@rm -rf ${OBJS}

${OBJS} : %.o:%.cpp
	${GCC} ${CFLAGS} $< -o $@
	cp $@ ../main/
	@rm *.o -f 

