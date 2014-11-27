/**
 * dynamically growing pointer array
 *
 * GOAL:
 * create a set of functions which will allocate a pointer array. if
 * methods add items, the main array should grow to the desired size.
 *
 *
 * (c) 2014, Simon Wunderlin <simonATwunderlinDTnet>
 */


#ifndef __PARRAY_H__
#define __PARRAY_H__

#define PARRAY_ERR_MAX 6
#define PARRAY_ERR_LENGTH 50

/**
 * This structure holds an array of pointers including the number of items
 */
typedef struct {
	size_t _psize;    // size of pointer
	size_t length;    // number of elements
	size_t allocated; // allocated memory
	size_t expand;    // number of elements to add if the array is full
	void** elements;  // actual data
} parray;

parray* parray_init(size_t psize, size_t expand);
size_t parray_set(parray* p, void* element, size_t pos); // if pos == 0, append
const char* parray_errstr();
size_t parray_free(parray* p);

#endif