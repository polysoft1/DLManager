CFLAGS_SHARED=-shared -fpic
CFLAGS=-ldl
SHARED_OUT=test.so
EXEC=test
OBJECTS=${SHARED_OUT} ${EXEC}

all: ${OBJECTS}

${SHARED_OUT}: test.cpp
	${CXX} ${CFLAGS_SHARED} test.cpp -o ${SHARED_OUT}

${EXEC}: main.cpp
	${CXX} main.cpp ${CFLAGS} -o ${EXEC}

clean:
	rm ${OBJECTS}
