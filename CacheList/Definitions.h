#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define TEST_INIT_VALUE -1
#define MIN_FILL_RATIO 1 / 3
#define BYTE_SIZE 8 //bits

//boolean values:
#define FALSE 0
#define TRUE 1

//fixed array lengths:
#define MAX_COMMAND_LENGTH 3 //characters, including '\0' at the end
#define MAX_ITERATOR_NAME_LENGTH 4 //characters, including '\0' at the end
#define FIXED_ITERATORS_NUMBER 2
#define MAX_ITERATORS_NUMBER 10

//possible adding places:
#define BEFORE 0
#define AFTER 1

//directions:
#define FORWARD 0
#define BACKWARDS 1

//fixed iterators indexes:
#define END 10
#define BEG 11

//special position value:
#define ALL 12

//addresses types:
#define PREV 0
#define NEXT 1

//way of change:
#define DECREMENT 0
#define INCREMENT 1

//size of data in blocks:
#define ADDRESS_SIZE 4 //bytes
#define ELEMENTS_NUMBER_SIZE 1 //bytes
#define ELEMENT_SIZE 8 //bytes

//offsets in block
#define PREV_OFFSET 0  //bytes
#define NEXT_OFFSET 4 //bytes
#define ELEMENTS_NUMBER_OFFSET 8 //bytes
#define FIRST_ELEMENT_OFFSET 9 //bytes


typedef unsigned char byte_t;

typedef struct iterator
{
	byte_t* block;
	char element_number;
}
iterator;

typedef struct list_parameters_t
{
	int block_size;
	iterator iterators[MAX_ITERATORS_NUMBER + FIXED_ITERATORS_NUMBER];
}
list_parameters_t;