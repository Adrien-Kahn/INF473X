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


// Inspired in some parts by beej.us/guide/bgnet (notably /exemples/showip.c)
// and https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response


int main(int argc, char* argv[]) {
    url_info info;
    const char * file_name = "received_page";
    if (argc < 2) {
	fprintf(stderr, "Missing argument. Please enter URL.\n");
	return 1;
    }

    char *url = argv[1];

    // Get optional file name
    if (argc > 2) {
	file_name = argv[2];
    }

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
    char *response = read_http_reply(&reply);
    if (response == NULL) {
	fprintf(stderr, "Could not parse http reply\n");
	return 4;
    }

    // Write response to a file
    write_data(file_name, response, reply.reply_buffer + reply.reply_buffer_length - response);

    // Free allocated memory
    free(reply.reply_buffer);

    // Just tell the user where is the file
    fprintf(stderr, "the file is saved in %s.", file_name);
    return 0;
}

int download_page(url_info *info, http_reply *reply) {

    /*
     * To be completed:
     *   You will first need to resolve the hostname into an IP address.
     *
     *   Option 1: Simplistic
     *     Use gethostbyname function.
     *
     *   Option 2: Challenge
     *     Use getaddrinfo and implement a function that works for both IPv4 and IPv6.
     *
     */
     
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
     	printf("Successfully built socket\n\n");
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
     int n = 2000;
     reply->reply_buffer = malloc(n);
     reply->reply_buffer_length = n - 100;
     
     // Sending the request
     int sent_length = send(mysocket, request, request_length, 0);
     printf("Sent length: %d\n\n", sent_length);
     if (sent_length == request_length) printf("Successfully sent the message\n\n"); 
     else printf("Message not sent entirely\n\n");
     
     // Receiving the answer
     int received_length = recv(mysocket, reply->reply_buffer, reply->reply_buffer_length, 0);
     printf("Received length: %d\n\n", received_length);
     printf("Received message: %.1900s\n\n", reply->reply_buffer);
     
     printf("OK so far\n\n");
     

    /*
     * To be completed:
     *   Next, you will need to send the HTTP request.
     *   Use the http_get_request function given to you below.
     *   It uses malloc to allocate memory, and snprintf to format the request as a string.
     *
     *   Use 'write' function to send the request into the socket.
     *
     *   Note: You do not need to send the end-of-string \0 character.
     *   Note2: It is good practice to test if the function returned an error or not.
     *   Note3: Call the shutdown function with SHUT_WR flag after sending the request
     *          to inform the server you have nothing left to send.
     *   Note4: Free the request buffer returned by http_get_request by calling the 'free' function.
     *
     */



    /*
     * To be completed:
     *   Now you will need to read the response from the server.
     *   The response must be stored in a buffer allocated with malloc, and its address must be save in reply->reply_buffer.
     *   The length of the reply (not the length of the buffer), must be saved in reply->reply_buffer_length.
     *
     *   Important: calling recv only once might only give you a fragment of the response.
     *              in order to support large file transfers, you have to keep calling 'recv' until it returns 0.
     *
     *   Option 1: Simplistic
     *     Only call recv once and give up on receiving large files.
     *     BUT: Your program must still be able to store the beginning of the file and
     *          display an error message stating the response was truncated, if it was.
     *
     *   Option 2: Challenge
     *     Do it the proper way by calling recv multiple times.
     *     Whenever the allocated reply->reply_buffer is not large enough, use realloc to increase its size:
     *        reply->reply_buffer = realloc(reply->reply_buffer, new_size);
     *
     *
     */



    return 1;
}

void write_data(const char *path, const char * data, int len) {
    /*
     * To be completed:
     *   Use fopen, fwrite and fclose functions.
     */
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

char *read_http_reply(struct http_reply *reply) {

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

    if (status != 200) {
	fprintf(stderr, "Server returned status %d (should be 200)\n", status);
	return NULL;
    }

    char *buf = status_line + 2;

    /*
     * To be completed:
     *   The previous code only detects and parses the first line of the reply.
     *   But servers typically send additional header lines:
     *     Date: Mon, 05 Aug 2019 12:54:36 GMT<CR><LF>
     *     Content-type: text/css<CR><LF>
     *     Content-Length: 684<CR><LF>
     *     Last-Modified: Mon, 03 Jun 2019 22:46:31 GMT<CR><LF>
     *     <CR><LF>
     *
     *   Keep calling next_line until you read an empty line, and return only what remains (without the empty line).
     *
     *   Difficul challenge:
     *     If you feel like having a real challenge, go on and implement HTTP redirect support for your client.
     *
     */




    return buf;
}
