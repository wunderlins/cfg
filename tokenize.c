//
//  tokenize.c
//  pkg
//
//  Created by Simon Wunderlin on 13/09/14.
//  Copyright (c) 2014 ___HACKERSINC___. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parray.h"

#ifndef TOKENIZE_TESTCASE
	#define TOKENIZE_TESTCASE 0
#endif

/**
 * Split string by characters defined in whitespace
 * 
 * TODO: if a line ends with a '\\' the directive is continued on the next line
 * TODO: find a way to preserve whitespace
 * TODO: concatenate comments beginning with '#' and ending at the end of 
 *       the line into one token 
 * 
 * returns the number of elements
 */
int tokenize(parray* result, char* string, char* whitespace) {
	
	char current;
	int i = 0;
	int word = 0;
	int length = strlen(string);
	int buffer_l = 0;
	char buffer[100] = "";
	int whitespace_length = strlen(whitespace);
	
	// loop over the array character by character
	//printf("str: %s\n", string);
	for (i=0; i<length; i++) {
		current = string[i];
		
		// check if the current character is a delimiter
		int n = 0;
		int is_whitespace = 0;
		for(n=0; n<whitespace_length; n++) {
			if (current == whitespace[n]) {
				is_whitespace = 1;
				break;
			}
		}
		
		if (is_whitespace && buffer_l == 0)
			continue;
		
		if (is_whitespace && buffer_l) {
			// FIXME check for malloc errors
			//printf("A\n");
			char* str = malloc(sizeof(char)*101);
			strcpy(str, buffer);
			//printf("B\n");
			
			size_t r = parray_set(result, str, 0);
			if(r == 0)
				return -1;
			//printf("B1\n");
			
			// clear buffer
			buffer[0] = '\0';
			buffer_l = 0;
			word++;
			
			continue;
		}
		
		// remember character
		// FIXME: check fo buffer overruns
		buffer[buffer_l++] = current;
		buffer[buffer_l] = '\0';
	}
	
	// copy last buffer into the result
	if (buffer[0] != '\0') {
		//strcpy(result[word++], buffer);
		// FIXME check for malloc errors
		//printf("C\n");
		char* str = malloc(sizeof(char)*101);
		strcpy(str, buffer);
		//printf("D\n");
		
		size_t r = parray_set(result, str, 0);
		if(r == 0)
			return -1;
		word++;
	}
	
	return word;
}

#if TOKENIZE_TESTCASE == 1
int main() {
	
	char *string = "module-name      @1.2.sdef				mod/submod";
	parray* result = parray_init(sizeof(char), 10);
	
	tokenize(result, string, " \t");
	
	//printf("count %d, length %ld\n", count, result->length);
	char** e = (char**) result->elements;
	int i = 0;
	for(i=0; i<result->length; i++)
		printf("%d %s\n", i, e[i]);
	
	/*
	printf("%s\n", result[0]);
	printf("%s\n", result[1]);
	printf("%s\n", result[2]);
	*/
	
	return 0;
}
#endif
