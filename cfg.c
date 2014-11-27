/**
 * cfg.c
 *
 *  Created on: Nov 6, 2014
 *      Author: wus
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parray.h"
#include "tokenize.h"
#include "cfg.h"

static int nodeid = 0;

node_t* init_node(char* name, char* value) {
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->has_children = 0;
	n->type = NODE;
	n->data = malloc(sizeof(node_u));
	n->data->node.name = name;
	n->data->node.value = value;
	return n;
}

node_t* init_nodelist(char* name, char* value) {
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->has_children = 1;
	n->type = NODELIST;
	n->data = malloc(sizeof(node_u));
	n->data->list.name = name;
	n->data->list.value = value;
	n->data->list.length = 0;
	n->data->list.children = NULL;
	return n;
}

node_t* init_root() {
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->has_children = 1;
	n->type = ROOT;
	n->data = malloc(sizeof(node_u));
	n->data->list.length = 0;
	n->data->list.children = NULL;
	return n;
}

int node_append(node_t* nodelist, node_t* node) {
	
	// check if nodelist accepts children
	if (nodelist->type != NODELIST && nodelist->type != ROOT)
		return 1;
	
	// allocate memory for children
	if (nodelist->data->list.length == 0) {
		// malloc
		nodelist->data->list.children = malloc(sizeof(node_t));
		if (nodelist->data->list.children == NULL)
			return 2;
	} else {
		// realloc
		// FIXME: length + 1? ordo length++ before allocating!
		nodelist->data->list.children = 
				realloc(nodelist->data->list.children, 
				sizeof(node_t) * nodelist->data->list.length);
		if (nodelist->data->list.children == NULL)
			return 2;
	}
	
	node_t** children = (node_t**) nodelist->data->list.children;
	children[nodelist->data->list.length] = node;
	nodelist->data->list.length++;
	
	return 0; // success
}

char* node_to_str(char* buffer, node_t* node) {
	//char* buffer = malloc(sizeof(char)*101);
	buffer[0] = '\0';
	char l[20];
	sprintf(buffer, "%p %d ", node, node->nodeid);
	
	switch(node->type) {
		case UNDEF:
			strcat(buffer, "UNDEF");
			break;
		
		case ROOT:
			strcat(buffer, "ROOT     ");
			strcat(buffer, "length: ");
			sprintf(l, "%d", node->data->list.length);
			strcat(buffer, l);
			break;
		
		case NODE:
			strcat(buffer, "NODE     ");
			strcat(buffer, "name: ");
			strcat(buffer, (const char*) node->data->node.name);
			strcat(buffer, ", value: ");
			if (node->data->node.value == NULL)
				strcat(buffer, "(null)");
			else
				strcat(buffer, (const char*) node->data->node.value);
			break;
		
		case NODELIST:
			strcat(buffer, "NODELIST ");
			strcat(buffer, "name: ");
			strcat(buffer, node->data->list.name);
			strcat(buffer, ", value: ");
			if (node->data->list.value == NULL)
				strcat(buffer, "(null)");
			else
				strcat(buffer, (const char*) node->data->list.value);
			strcat(buffer, ", length: ");
			sprintf(l, "%d", node->data->list.length);
			strcat(buffer, l);
			break;
	}
	
	return buffer;
}

// recursive dump
int node_dump(node_t* root, int indent) {
	
	// handle indent
	char ind[21] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	if (indent > 20)
		ind[20] = '\0';
	else
		ind[indent] = '\0';
	
	// no children? simple key/value pair
	if (root->has_children == 0) {
		printf("%s%s %s\n", ind, root->data->node.name, root->data->node.value);
		return 0;
	}
	
	// TODO: check if a type node is undefined
	
	// print start tag
	if (root->type != ROOT) {
		printf("%s<%s", ind, root->data->list.name);
		if (root->data->list.value != NULL)
			printf(" %s", root->data->list.value);
		printf(">");
		
		if (root->data->list.length > 0)
			printf("\n");
	}
	
	int i = 0;
	node_t** children = (node_t**) root->data->list.children;
	for (i=0; i < root->data->list.length; i++) {
		/*
		printf("%d %p ", children[i]->nodeid, children[i]);
		if (children[i]->has_children)
			printf(" %d", children[i]->data->list.length);
		printf("\n");
		*/
		if (root->type == ROOT)
			node_dump(children[i], 0);
		else
			node_dump(children[i], indent+1);
	}
	
	if (root->type != ROOT)
		printf("%s</%s>\n", ind, root->data->list.name);
	
	return 0;
}

void test_createnode() {
	
	//printf("%d\n", nodeid);
	
	node_t* root = init_root(); 
	
	node_t* n = malloc(sizeof(node_t));
	n->nodeid = nodeid++;
	n->type = NODE;
	n->data = malloc(sizeof(node_u));
	n->data->node.name = "name";
	n->data->node.value = "val";
	
	// printf("%p %d: name: %s, value: %s\n", n, n->type, n->data->node.name, n->data->node.value);
	node_append(root, n);
	
	node_t* n1 = init_node("name1", NULL);
	//printf("%p %d: name: %s, value: %s\n", n1, n1->type, n1->data->node.name, n1->data->node.value);
	node_append(root, n1);
	
	node_t* nl1 = init_nodelist("nl1", "val1");
	//printf("%p %d: name: %s, value: %s\n", nl1, nl1->type, nl1->data->list.name, nl1->data->list.value);
	node_append(root, nl1);
	
	node_append(nl1, n1);
	
	/*
	node_t** children = (node_t**) nl1->data->list.children;
	//nl1->data->list.children[0]->data.node->name
	printf("%p %d: length: %d, type: %d, name: %s, value: %s\n", 
			                                 nl1,
			                                 nl1->type, 
			                                 nl1->data->list.length, 
			                                 children[0]->type,
			                                 children[0]->data->node.name,
			                                 children[0]->data->node.value);
	node_t* child = children[0];
	printf("%p %d: length: %d, type: %d, name: %s, value: %s\n", 
			                                 nl1, 
			                                 nl1->type, 
			                                 nl1->data->list.length, 
			                                 child->type,
			                                 child->data->node.name,
			                                 child->data->node.value);
	
	printf("%p %d: name: %s, value: %s\n", 
	                                    child,
	                                    child->type, 
	                                    child->data->node.name, 
	                                    child->data->node.value);
	
	//char nstr[100] = "";
	//node_to_str(nstr, n1);
	*/
	
	/*
	printf("==============================================\n\n");
	char buffer[101] = "";
	printf("%s\n", node_to_str(buffer, root));
	printf("%s\n", node_to_str(buffer, n));
	printf("%s\n", node_to_str(buffer, n1));
	printf("%s\n", node_to_str(buffer, nl1));
	*/
	
	node_dump(root, 0);
	
}

int parse_config_file(const char* filename) {
	
	printf("Parsing %s\n\n", filename);
	
	// get a file handle
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "%s : ", filename);
		perror("");
		return 2;
	}
	
	node_t* root = init_root(); // the root node
	
	// parse file
	// 
	// loop over the file character by character
	// '<' is the beginning of a directive name
	// "</" is the beginning of the end of a directive name
	// '>' tag end
	// name/value pairs are separated by 1 or more spaces or tabs
	
	
	// TODO: handle utf8 characters
	// see explanation here: 
	// http://stackoverflow.com/questions/21737906/how-to-read-write-utf8-text-files-in-c
	
	char c;
	char line[1024] = "";
	int line_pos = 0;
	
	// pointer to the current node in the parser queue
	node_t* current = root;
	
	// create stack of nodes for tags. Whenever a new opening
	// tag is encountered add the new tag at the end of the stack.
	// when a tag is closed, remove the last tag (null) and update the 
	// stack length.
	struct {
		int length;
		node_t* items[1024];
	} stack;
	stack.length = 0;
	
	while ((c = fgetc(fp)) != EOF) {
		
		// scan line by line
		// FIXME: also take into account '\r' and "\r\n"
		if (c != '\n') {
			//printf("%d %c ", line_pos, c);
			line[line_pos] = c;
			line[line_pos+1] = '\0';
			line_pos++;
			
			continue;
		}
		
		// split string by ' ' and '\t' and store it in a pointer array
		// pointer type is char*
		parray* tokens = parray_init(sizeof(char), 10);
		tokenize(tokens, line, " \t");
		
		line_pos = 0;
		line[0] = '\0';
		
		if (tokens->length == 0)
			continue;
		
		// TODO: generate tokens also for white space
		
		// parse line and create node
		char** t = (char**) tokens->elements;
		//printf("[%ld] ", tokens->length);
		if (t[0][0] == '<' && t[0][1] == '/') {
			printf(" close ");
			
			// check for syntax errors
#define PARSER_ERR_TOOMANYCLOSE 128 
			if (stack.length == 0)
				return PARSER_ERR_TOOMANYCLOSE;
			
#define PARSER_ERR_TAGMISMATCH 129
			// check for tag mismatch, case sensitive
			if (strcmp(current->data->list.name, 
					       stack.items[stack.length-1]->data->list.name) != 0)
				return PARSER_ERR_TAGMISMATCH;
			
			// "remove" (forget) last tag on the stack
			stack.length--;
			
			
		} else if (t[0][0] == '<') {
			printf(" open ");
			
			// new nodelist
			//printf("Length: %ld ", tokens->length);
			char** e = (char**) tokens->elements;
			node_t* n = init_nodelist(e[0], e[1]);
			//printf("nodelist.name %s %s ", n->data->list.name, n->data->list.value);
			//printf("%s %s\n", e[0], e[1]);

			// FIXME: check for malloc errors
			stack.items[stack.length] = n;
			stack.length++;
			current = n;
			/*
			*/
			printf("end open " );
		} else {
			printf(" node ");
			
			// check that node can have children
			// TODO: just add a new node to the end of children
		}
		
		
		// debug output of all tokens
		if (tokens->length) {
			printf("%ld", tokens->length);
			int i=0;
			char** e = (char**) tokens->elements;
			for (i=0; i<tokens->length; i++)
				printf("|%s", e[i]);
			printf("\n");
		}
		
		
		// printf("%c", c);
	}
	
	// FIXME: free allocated memory from tokenizer properly

	// close file handle
	int ret = fclose(fp);
	if (ret != 0) {
		fprintf(stderr, "Failed to close %s : ", filename);
		perror("");
		return 2;
	}
	
	return 0;
}

int main(int argc, char* argv[]) {
	
	//test_createnode();
	
	// we expect command line parameter #1 to be a config file to parse
	if (argc != 2) {
		fprintf(stderr, "usage: %s [configfile]\n", argv[0]);
		exit(1);
	}
	
	int ret = parse_config_file(argv[1]);
	
	return ret;
}
