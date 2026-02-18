.PHONY:all
all:udpclient udpserver

udpclient:UdpClient.cpp
	g++ -o $@ $^ -std=c++17 -static
udpserver:UdpServer.cpp
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f udpclient udpserver
