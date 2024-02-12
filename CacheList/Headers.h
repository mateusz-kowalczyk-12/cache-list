#include "Definitions.h"

//--> Main dispatcher:

	//Proceeds to user's entering command
	void read_command(list_parameters_t* list_parameters);


//--> Commands parameters parsers:

	//Continues parsing the command input and proceeds to its execution controller
	void parse_adding(list_parameters_t* list_parameters, char where);

	//Continues parsing the command input and proceeds to its execution controller
	void parse_printing(list_parameters_t* list_parameters);

	//Continues parsing the command input and proceeds to its execution controller
	void parse_removing(list_parameters_t* list_parameters);

	//Continues parsing the command input and proceeds to its execution controller
	void parse_iterator_adding(list_parameters_t* list_parameters);

	//Continues parsing the command input and proceeds to its execution controller
	void parse_iterator_moving(list_parameters_t* list_parameters, char movement_type);


//--> General list operations controllers:

	//Controlls adding an element to the list
	void continue_adding(list_parameters_t* list_parameters, char iterator_number, unsigned long long int value_to_add, char where);

	//Controlls printing
	void continue_printing(list_parameters_t* list_parameters, char position);

	//Controlls removing an element from the list
	void continue_removing(list_parameters_t* list_parameters, char position);

	//Controlls initialising the given iterator with the given position
	void continue_iterator_adding(list_parameters_t* list_parameters, char iterator_number, char position);

	//Controlls moving the given iterator in the given direction
	void continue_iterator_moving(list_parameters_t* list_parameters, char iterator_number, char movement_type);

	//Checks if the list's structure needs to be changes due to the removal of an element from the pointed block
	void check_structure(list_parameters_t* list_parameters, byte_t* pointed_block);


//--> Specific operations executors:

	//Creates the first block and sets the given value there
	void initialise_list(list_parameters_t* list_parameters, unsigned long long int value_to_add);

	//Frees all allocated memory
	void delete_list(list_parameters_t* list_parameters);

	//Adds the given element to the block pointed by the given iterator
	void add_to_block(list_parameters_t* list_parameters, char iterator_number, unsigned long long int value_to_add, char where);

	//Adds the given element to a new block after the one pointer by the given iterator
	void add_to_new_block(list_parameters_t* list_parameters, char iterator_number, unsigned long long int value_to_add, char where);

	//Shifts an element from the previous block forward due to not efficient list structure
	void change_structure_shift_forward(list_parameters_t* list_parameters, byte_t* pointed_block);

	//Shifts an element from the next block backwards due to not efficient list structure
	void change_structure_shift_backwards(list_parameters_t* list_parameters, byte_t* pointed_block);

	//Shifts all the elements from the next block backwards due to not efficient list structure
	void change_structure_shift_all_backwards(list_parameters_t* list_parameters, byte_t* pointed_block);


//--> Moving executors:

	//Copies all the elements is the given block starting with the specified (last) one forward (no element is actually deleted!)
	void move_many_forward(list_parameters_t* list_parameters, byte_t* block, char starting_element_number);

	//Moves half of the elements is the given block to the next block. Note: only to use if the next block is a new clear block
	void move_many_forward_outside(list_parameters_t* list_parameters, byte_t* block);

	//Moves all the elements is the given block starting with the specified (first) one backwards
	void move_many_backwards(list_parameters_t* list_parameters, byte_t* block, char starting_element_number);

	//Deletes the given block
	void delete_block(list_parameters_t* list_parameters, byte_t* block);


//--> Specific sub-operations executors

	//Returns a pointer to a new cleared block:
	byte_t* get_new_clear_block(list_parameters_t* list_parameters);
	
	//Sets the given address as a given address type in the given block 
	void set_address_in_block(byte_t* block, char address_type, unsigned int value);

	//Increments or decrements the value indicating the number or elements in the block
	void change_elements_number(byte_t* block, char option);

	//Sets the given value in the given block on the given position 
	void set_element_in_block(byte_t* block, char element_number, unsigned long long int value);
	
	//Returns the given address from the given block
	unsigned int read_address_from_block(byte_t* block, char address_type);

	//Returns the number of elements present in the given block
	char read_elements_number_from_block(byte_t* block);
	
	//Returns the value of the given element from the given block
	unsigned long long int read_element_from_block(byte_t* block, char element_number);

	//Checks if any iterator needs to be changed due to the pointed element's being moved in the given direction inside the block
	void change_iterators_inside_moving(list_parameters_t* list_parameters, byte_t* block, char element_number, char direction);

	//Checks if any iterator needs to be changed due to the pointed element's being moved outside of the block
	void change_iterators_outside(list_parameters_t* list_parameters, byte_t* past_block, char past_element_number, byte_t* new_block, char new_element_number);

	//Checks if any iterator needs to be changed due to the pointed element's being removed
	void change_iterators_removing(list_parameters_t* list_parameters, char position);


//--> Minifunctions:

	//Initialises the list parameters with NULLs and TEST_INIT_VALUEs
	void initialise_list_parameters(list_parameters_t* list_parameters);

	//Sets every byte in the block equal to NULL
	void clear_block(byte_t* block, int block_size);

	//Checks if there is a place for another element in the given block
	char is_place(list_parameters_t* list_parameters, char iterator_number);

	//Returns the minimum elements number in the block according to the set mininal fill ratio
	char get_min_elements_number(list_parameters_t* list_parameters);


//--> Additinal calculating functions:

	//Returns the maximum number of elements in a block
	char get_max_elements_number(list_parameters_t* list_parameters);

	//Returns the iterator number or the special ALL value
	char get_position_from_string(char* position_string);