CC = g++
CFLAGS = -g -Wall

all: src/cDraw.o src/cDraws.o src/main.o
	$(CC) src/main.o src/cDraw.o src/cDraws.o $(CFLAGS) -lm -lcrypto -ljsoncpp -lpthread -lboost_log -lboost_thread -lboost_date_time -lboost_system -DBOOST_LOG_DYN_LINK -o main 
	

src/main.o: src/main.cpp src/cDraw.hpp
	$(CC) -c src/main.cpp $(CFLAGS) -DBOOST_LOG_DYN_LINK
	mv main.o src/main.o

src/cDraws.o: src/cDraws.cpp src/cDraw.hpp
	$(CC) -c src/cDraws.cpp $(CFLAGS) -DBOOST_LOG_DYN_LINK 
	mv cDraws.o src/cDraws.o 

src/cDraw.o: src/cDraw.cpp src/cDraw.hpp
	$(CC) -c src/cDraw.cpp $(CFLAGS) -DBOOST_LOG_DYN_LINK 
	mv cDraw.o src/cDraw.o

client: 
	$(CC) src/client.cpp $(CFLAGS) -lcrypto -lpthread -o client

clean:
	rm src/*.o
	rm -f main
	rm -f client

mrproper:
	rm -f src/*.o
	rm -f main
	rm -f client
	rm -f ./LOGS/*.log
	rm -f ./DRAWS/*.json
