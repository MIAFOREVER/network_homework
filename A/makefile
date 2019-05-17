all:server client udpserver udpclient
server:server.cpp
	g++ server.cpp -o server
client:client.cpp
	g++ client.cpp -o client
udpclient:udpclient.cpp
	g++ $^ -o $@
udpserver:udpserver.cpp
	g++ $^ -o $@