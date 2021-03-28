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


// Inspired in some parts by beej.us/guide/bgnet (notably beej.us/bgnet/examples/showip.c)
// and https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response


// Example urls for testing:
// Short file -> beej.us/bgnet/guide/examples/showip.c
// Long file -> beej.us/guide/bgnet/
// Very long file -> beej.us/guide/bgnet/html/
 
// Template netcat request that works:
// printf 'GET /guide/bgnet/examples/showip.c HTTP/1.1\r\nHost: beej.us\r\nConnection: close\r\n\r\n' | netcat beej.us 80


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
     int n = 3000;
     reply->reply_buffer = malloc(n);
     reply->reply_buffer_length = n;
     
     // Sending the request
     int sent_length = send(mysocket, request, request_length, 0);
     printf("Sent length: %d\n\n", sent_length);
     if (sent_length == request_length) printf("Successfully sent the message\n\n"); 
     else printf("Message not sent entirely\n\n");
     
     
     
     // Receiving the answer
     int remaining_size = n;
     char *current_buffer = reply->reply_buffer;
     int received_length = 1;
     
     while (received_length > 0) {
     	
     }
     
     
     // Old version
     /*
     int received_length = recv(mysocket, reply->reply_buffer, reply->reply_buffer_length, 0);
     printf("Buffer length: %d\n", reply->reply_buffer_length);
     printf("Received length: %d\n", received_length);
     if (reply->reply_buffer_length > received_length) printf("Successfully received entirety of the message\n\n"); 
     else printf("Message might not have been received entirely\n\n");
     printf("Received message: \n%.*s\n\n", n, reply->reply_buffer);
     */
     
     
     
     // Closes the socket
     if (close(mysocket) == 0) printf("Successfully closed socket\n\n");
     else fprintf(stderr, "Failed to close the socket\n\n");
     
     // Frees the request buffer
     free(request);
     
     
     printf("OK so far\n\n");
     

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

// with n = 3000: IhvcNAQcEoIIHqjCCB6YCAQExggEwMIIBLAIBADCBlDCBjjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb20CAQAwDQYJKoZIhvcNAQEBBQAEgYACzGjNdeobbyuFFRxWN4B7AyR3lFBKybAKc+HnQG4tBtRTx1BVzQuUTJRF2yLAT9CgaWQu/SLrAR1tNrysrFvqlXGkRe/0IUNp9wLE2flrpoRwf7Af3pRTNJdgy/JCiSti0Kaz9nR9jB8V8A8CU5gt+5fYZ5dpB0MhgUtYEeRICDELMAkGBSsOAwIaBQAwggE1BgkqhkiG9w0BBwEwFAYIKoZIhvcNAwcECDF+UJbmZdzHgIIBED/Kdg64miQoNja+JN1dLojdjTZocgYDS4ykmfBa3hmBkby3kMJWB/8oKkXN3nF8x8ZAjycNrAKejnIsq4oLYBt8/kD6KfeZmysyenw94CZhpkipGKmlvgzuJhygOw5k0wYXE/rV6k9/BVysJduTziLQJ3Yk/ye3KDV17Y8/drLAT0ueiL/sk7eWbEN6Drc1eEVL4FtfaayTWPCDF/McC6AuPLzv+RaoVX/QlOLsjuoibWrVk9P1Sq/76/bR8x5op//ZRDtHnQdy/EH86D0p6XD7EAMg2W1R01+zJ+cdYs6FyCeCFO5OH4d3Kfm1ihJ75JELAKSaomQweP0i4z68p/uoEFS1LMnDzRcwshxo3lmkoIIDhzCCA4MwggLsoAMCAQICAQAwDQYJKoZIhvcNAQEFBQAwgY4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsUCmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tMB4XDTA0MDIxMzEwMTMxNVoXDTM1MDIxMzEwMTMxNVowgY4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsUCmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDBR07d/ETMS1ycjtkpkvjXZe9k+6CieLuLsPumsJ7QC1odNz3sJiCbs2wC0nLE0uLGaEtXynIgRqIddYCHx88pb5HTXv4SZeuv0Rqq4+axW9PLAAATU8w04qqjaSXgbGLP3NmohqM6bV9kZZwZLR/klDaQGo1u9uDb9lr4Yn+rBQIDAQABo4HuMIHrMB0GA1UdDgQWBBSWn3y7xm8XvVk/UtcKG+wQ1mSUazCBuwYDVR0jBIGzMIGwgBSWn3y7xm8XvVk/UtcKG+wQ1mSUa6GBlKSBkTCBjjELMAkGA1UEBhMCVVMxCzAJB

