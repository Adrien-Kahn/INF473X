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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"url.h"

/**
 * parse a URL and store the information in info.
 * return 0 on success, or an integer on failure.
 *
 * Note that this function will heavily modify the 'url' string
 * by adding end-of-string delimiters '\0' into it, hence chunking it into smaller pieces.
 * In general this would be avoided, but it will be simpler this way.
 */

int parse_url(char* url, url_info *info)
{
	// url format: [http://]<hostname>[:<port>]/<path>
	// e.g. https://www.polytechnique.edu:80/index.php

	char *column_slash_slash, *host_name_path, *protocol;

	column_slash_slash = strstr(url, "://");
	// it searches :// in the string. if exists,
	//return the pointer to the first char of the match string

	if(column_slash_slash != NULL){ //protocol type exists
		*column_slash_slash = '\0'; //end of the protocol type
		host_name_path = column_slash_slash + 3; //jump to the host name
		protocol = url;
	} else {	//no protocol type: using http by default.
		host_name_path = url;
		protocol = "http";
	}
	
	info->protocol = protocol;
	
	//Return an error (PARSE_URL_PROTOCOL_UNKNOWN) if the protocol is not 'http' using strcmp.
	if (strcmp(protocol, "http") != 0) return 3;

	/*
	 * To be completed:
	 * 	 Look for the first '/' in host_name_path using strchr
	 * 	 Return an error (PARSE_URL_NO_SLASH) if there is no such '/'
	 * 	 Store the hostname and the queried path in the structure.
	 *
	 * 	 Note: The path is stored WITHOUT the first '/'.
	 * 	       It simplifies this function, but I'll let you understand how. :-)
	 */
	
	char *slash, *hostname_port, *path; 
	slash = strchr(host_name_path, '/');
		
	if (slash != NULL) {
		*slash = '\0';
		hostname_port = host_name_path;
		path = slash + 1;
	} else {
		return 1;
	}
	
	info->path = path;
	
	/*
	 * To be completed:
	 * 	 Look for ':' in the hostname
	 * 	 If ':' is not found, the port is 80 (store it)
	 * 	 If ':' is found, split the string and use sscanf to parse the port.
	 * 	 Return an error if the port is not a number, and store it otherwise.
	 */
	
	char *two_points, *hostname, *port;
	two_points = strchr(hostname_port, ':');
	
	if (two_points != NULL) {
		*two_points = '\0';
		hostname = hostname_port;
		port = two_points + 1;
	} else {
		hostname = hostname_port;
		port = "80";
	}
	
	info->host = hostname;
	
	int port_int;
	
	// EVEN IF port STARTS WITH A SINGLE INT AND AFTER SOMETHING ELSE, sscanf WILL REPORT 1 !
	
	if (sscanf(port, "%d", &port_int) == 1) {
		info->port = port_int;
	} else {
		return 2;
	}
	
	// If everything went well, return 0.
	return 0;
}

/**
 * print the url info to std output
 */
void print_url_info(url_info *info){
	printf("The URL contains following information: \n");
	printf("Protocol:\t%s\n", info->protocol);
	printf("Host name:\t%s\n", info->host);
	printf("Port No.:\t%d\n", info->port);
	printf("Path:\t\t/%s\n", info->path);
}
