all:server client
server:server.cpp
	g++ -o $@ $^ -std=c++17 -lpthread
client:client.cpp
	g++ -o $@ $^ -std=c++17 -lpthread
.PHONY:clean
clean:
	rm -rf server client

