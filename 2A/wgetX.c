/**
 *  Jiazi Yi
 *
 * LIX, Ecole Polytechnique
 * jiazi.yi@polytechnique.edu
 *
 * Updated by Pierre Pfister
 *
 * Cisco Systems
 * ppfister@cisco.com
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "url.h"
#include "wgetX.h"


// Inspired in some parts by beej.us/guide/bgnet (notably beej.us/guide/bgnet/examples/showip.c)
// and https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response


// Example urls for testing:
// Short file -> beej.us/guide/bgnet/examples/showip.c
// Long file -> beej.us/guide/bgnet/
// Very long file -> beej.us/guide/bgnet/html/
 
// Template netcat request that works:
// printf 'GET /guide/bgnet/examples/showip.c HTTP/1.1\r\nHost: beej.us\r\nConnection: close\r\n\r\n' | netcat beej.us 80


int main(int argc, char* argv[]) {
    char *file_name = "received_page";
    if (argc < 2) {
	fprintf(stderr, "Missing argument. Please enter URL.\n");
	return 1;
    }

    char *url = argv[1];

    // Get optional file name
    if (argc > 2) {
	file_name = argv[2];
    }
    
    int n = 5;
    
    int status = fetch(url, file_name, n);

    return status;
}


int fetch(char *url, char* file_name, int n) {
    
    if (n == 0) {
    	return 1;
    }
    
    url_info info;
    
    // First parse the URL
    int ret = parse_url(url, &info);
    if (ret) {
	fprintf(stderr, "Could not parse URL '%s': %s\n", url, parse_url_errstr[ret]);
	return 2;
    }

    //If needed for debug
    print_url_info(&info);
    printf("\n");

    // Download the page
    struct http_reply reply;

    ret = download_page(&info, &reply);
    if (ret) {
    printf("Download Failed\n");
	return 3;
    }

    // Now parse the responses
    char *response = read_http_reply(&reply, file_name, n);
    if (response == NULL) {
	fprintf(stderr, "Could not parse http reply\n");
	return 4;
    } else if (strcmp(response, "Redirect123456789") == 0) {
    	printf("Was redirected\n\n");
    	return 0;
    }

    
    int response_length = reply.reply_buffer + reply.reply_buffer_length - response;
    printf("\n\nParsed response: \n%.*s\n\n", response_length, response);
    printf("Parsed response length: %d\n\n", response_length);

    // Write response to a file
    write_data(file_name, response, response_length);

    // Free allocated memory
    free(reply.reply_buffer);

    // Just tell the user where is the file
    fprintf(stderr, "the file is saved in %s\n\n", file_name);
    
    return 0;
}

int download_page(url_info *info, http_reply *reply) {
     
     struct addrinfo hints;
     struct addrinfo *res;
     
     // Initializing hints
     memset(&hints, 0, sizeof(hints));
     hints.ai_family = PF_UNSPEC;
     hints.ai_socktype = SOCK_STREAM;
     
     //Getting the socket
     int status = getaddrinfo(info->host, info->protocol, &hints, &res);
     if (status != 0) {
     	printf("getaddrinfo failed\n\n");
     } else {
     	printf("Successfully ran getaddrinfo\n\n");
     }
     
     // Displays IP version
     if (res->ai_family == AF_INET) {
     	printf("IPv4\n\n");
     } else if (res->ai_family == AF_INET6) {
     //COULDN'T FIND IPV6 HOST NAME			// THAT'S PROBABLY THE POINT OF THE LOOP
     	printf("IPv6\n\n");
     } else {
     	printf("Unknown IP format\n\n");
     }
     
     // PROBABLY WON'T WORK ON IPV6 (sockaddr_in6)
     struct sockaddr_in *dest = (struct sockaddr_in *) res->ai_addr;
     struct in_addr *myinaddr = &(dest->sin_addr);
     char ipstr[50];
     
     inet_ntop(res->ai_family, myinaddr, ipstr, sizeof(ipstr));
     printf("IP adress: %s\n\n", ipstr);
     
     // Builds the socket
     int mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
     printf("Successfully built socket\n");
     printf("mysocket = %d\n\n", mysocket);
     
     /*
     printf("%d\n\n", res->ai_family);
     printf("%d\n\n", res->ai_socktype);
     printf("%d\n\n", res->ai_protocol);
     */
     
     // Prints whether it is a TCP socket
     if (res->ai_socktype == SOCK_STREAM) printf("TCP\n\n"); else printf("not TCP\n\n");
     
     // Connects to dest
     int connect_status = connect(mysocket, (struct sockaddr*) dest, sizeof(struct sockaddr_in));
     if (connect_status != 0) {
     	printf("Connection failed\n\n");
     	return 1;
     }
     printf("Successfully connected to %s (%s)\n\n", info->host, ipstr);
     
     // Gets the request
     char *request = http_get_request(info);
     printf("Sending the following request:\n%s", request);
     int request_length = strlen(request);
     printf("Request length: %d\n", request_length);
     
     // Preparing reply
     int base_size = 1000;
     reply->reply_buffer = malloc(base_size);
     reply->reply_buffer_length = base_size;
     
     // Sending the request
     int sent_length = send(mysocket, request, request_length, 0);
     printf("Sent length: %d\n\n", sent_length);
     if (sent_length == request_length) printf("Successfully sent the message\n\n"); 
     else printf("Message not sent entirely\n\n");
     
     
     // Receiving the answer
     int remaining_size = base_size;
     char *current_buffer = reply->reply_buffer;
     int received_length = 1;
     
     
     while (received_length > 0) {
     	
     	received_length = recv(mysocket, current_buffer, remaining_size, 0);
     	printf("Received: %d\tbytes\n", received_length);
     	current_buffer += received_length;
     	remaining_size -= received_length;
     	
     	// If the buffer has no more room left, we make it bigger
     	if (remaining_size == 0) {
     		reply->reply_buffer_length += base_size;
     		reply->reply_buffer = realloc(reply->reply_buffer, reply->reply_buffer_length);
     		remaining_size = base_size;
     	}
     }
     
     // Modifying reply->reply_buffer_length so it gives the size of the buffer that is filled
     reply->reply_buffer_length -= remaining_size;
     
     printf("\nReceived message: \n%.*s\n\n", reply->reply_buffer_length, reply->reply_buffer);
     printf("Received message length: %d\n\n", reply->reply_buffer_length);
     
     
     // Old version
     /*
     int received_length = recv(mysocket, reply->reply_buffer, reply->reply_buffer_length, 0);
     printf("Buffer length: %d\n", reply->reply_buffer_length);
     printf("Received length: %d\n", received_length);
     if (reply->reply_buffer_length > received_length) printf("Successfully received entirety of the message\n\n"); 
     else printf("Message might not have been received entirely\n\n");
     printf("Received message: \n%.*s\n\n", base_size, reply->reply_buffer);
     */
     
     
     
     // Closes the socket
     if (close(mysocket) == 0) printf("Successfully closed socket\n\n");
     else fprintf(stderr, "Failed to close the socket\n\n");
     
     // Frees the request buffer
     free(request);
     
     printf("Download Successful\n\n");
     return 0;
}

void write_data(const char *path, const char * data, int len) {
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Failed to open a file to write in\n\n");
	}
	fwrite(data, len, 1, file);
	fclose(file);
}

char* http_get_request(url_info *info) {
    char * request_buffer = (char *) malloc(100 + strlen(info->path) + strlen(info->host));
    snprintf(request_buffer, 1024, "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
	    info->path, info->host);
    return request_buffer;
}

char *next_line(char *buff, int len) {
    if (len == 0) {
	return NULL;
    }

    char *last = buff + len - 1;
    while (buff != last) {
	if (*buff == '\r' && *(buff+1) == '\n') {
	    return buff;
	}
	buff++;
    }
    return NULL;
}

char *read_http_reply(struct http_reply *reply, char *file_name, int n) {

    // Let's first isolate the first line of the reply
    char *status_line = next_line(reply->reply_buffer, reply->reply_buffer_length);
    if (status_line == NULL) {
	fprintf(stderr, "Could not find status\n");
	return NULL;
    }
    *status_line = '\0'; // Make the first line is a null-terminated string

    // Now let's read the status (parsing the first line)
    int status;
    double http_version;
    int rv = sscanf(reply->reply_buffer, "HTTP/%lf %d", &http_version, &status);
    if (rv != 2) {
	fprintf(stderr, "Could not parse http response first line (rv=%d, %s)\n", rv, reply->reply_buffer);
	return NULL;
    }
    
    if (status == 301 || status == 302) {
    	printf("Starting redirect...\n\n");
    	char *new_url = parse_redirect(status_line + 2);
    	int status = fetch(new_url, file_name, n - 1);
    	return "Redirect123456789";
    	
    } else if (status != 200) {
	fprintf(stderr, "Server returned status %d (should be 200)\n", status);
	return NULL;
    }
    
    int len = reply->reply_buffer_length - sizeof(reply->reply_buffer) - 2;
    char *buf = status_line + 2;
    
    while (*buf != '\r') {
    	status_line = next_line(buf, len);
    	if (status_line == NULL) {
		fprintf(stderr, "Error while parsing the response\n");
		return NULL;
    	}
    	*status_line = '\0';
    	len -= sizeof(buf) + 2;
    	buf = status_line + 2;
    }
    
    printf("I was not redirected\n");
    return buf + 2;
}


char *parse_redirect(char* buff) {
	char *location = strstr(buff, "Location");
	if (location == NULL) {printf("No redirect location\n\n");}
	location += 10;
	char *next_line = strchr(location, '\r');
	*next_line = '\0';
	printf("Redirecting to: %s\n\n", location);
	return location;
}
