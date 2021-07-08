CFLAGS = -c -O3
EXEC = tslink2
OBJS = utils.o main.o 

tslink2 : main.o utils.o
	g++ main.o utils.o -o $(EXEC)

main.o : main.cpp
	g++ $(CFLAGS) main.cpp

utils.o : utils.cpp
	g++ $(CFLAGS) utils.cpp

clean	:
	rm -f $(OBJS) $(EXEC)