#include <stdio.h>
#include <sys/socket.h> // Core socket functions
#include <netinet/in.h>  // Internet address family constants (e.g., AF_INET)
#include <unistd.h>      // Provides close()
#include <string.h>      // For string manipulation

int main() {
    	// 1. Create a Socket
    	int server_file_descriptor_id = socket(AF_INET, SOCK_STREAM, 0);
	// server_file_descriptor_id is holding some integer if kernel successfully created struct sock in its memory ?? only from deepseek so , need actual harward debugging

	if(server_file_descriptor_id < 0){
		perror("socket failed");
		printf("%s" , "end of line\n");
		return 1;
	}

	//conigure server address
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	address.sin_family = AF_INET;          // IPv4
    	address.sin_addr.s_addr = INADDR_ANY;  // Bind to any available interface
    	address.sin_port = htons(8080);        // Listen on port 8080
	
	if(bind(server_file_descriptor_id , (struct sockaddr *) &address , addrlen) < 0 ){
		perror("bind failed");
		//if this program has more sockets then kernel memory should be reclaimed so that they can have 
		close(server_file_descriptor_id);
		printf("%s" , "bind failed");
		return 1;
	}
	if(listen(server_file_descriptor_id , 3) < 0){//3 means ??
		perror("listen error");
		close(server_file_descriptor_id);
		printf("%s" , "listen error");
		return 1;
	}
	printf("%s" , "server listening on port 8080...\n");
	/* network stack is not stack like memory ,
	 it will receive data from outside , then ip layer 
	asks it ip address if they have one of our opened socket 
	then it store that received packets to DMA then 
	tcp layer make connection , then  links that
	to specific server_file_descriptor_id these things are 
	done by kernel



	 */
	while(1){
		//if anything is received then 
		int new_socket = accept(
		server_file_descriptor_id ,
		(struct sockaddr *)&address  ,
		 (socklen_t*)&addrlen
		);
		if(new_socket < 0 ) {
			perror("accept");
			printf("%s" , "accepted");
			continue;
		}
		//now we have received packets in memory [where]??
		char buffer[30000] = {0};
		read(new_socket , buffer , 30000);
		char *response =
        	 "HTTP/1.1 200 OK\r\n"
            	"Content-Type: text/plain\r\n"
            	"Connection: close\r\n"
            	"\r\n"
            	"Hello from a server written in C!\n"; // Response body
        	write(new_socket, response, strlen(response));

        	// 7. Close the client connection
        	close(new_socket);
		//printf("%s" , "end of line\n");
		continue;
		return 0;
	}
}
