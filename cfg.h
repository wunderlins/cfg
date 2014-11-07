#ifndef __CFG_H__
#define __CFG_H__

typedef struct {
	char* name;
	char* value;
} node_simple_t;

typedef struct {
	char* name;
	char* value;
	int length;
	void** children; // this is actually of type node_u, but it is declared further down
} node_list_t;

typedef struct {
	int length;
	void** children; // this is actually of type node_u, but it is declared further down
} node_root_t;

typedef union {
	node_simple_t node;
	node_list_t list; // WTF: does root have to be listed here? might cause nasty problems but it works now. Strange.
} node_u;

typedef struct {
	int nodeid;
	int has_children;
	enum {
		UNDEF,   // 0
		ROOT,    // 1
		NODE,    // 2
		NODELIST // 3
	} type;
	node_u* data;
} node_t;

node_t* init_node(char* name, char* value);
node_t* init_nodelist(char* name, char* value);
node_t* init_root();
int node_append(node_t* nodelist, node_t* node);
char* node_to_str(char* buffer, node_t* node);
int node_dump(node_t* root, int indent);
//void test_createnode();
int parse_config_file(const char* filename);


#endif
