#include "Headers.h"


int main(void)
{
	list_parameters_t list_parameters;
	initialise_list_parameters(&list_parameters);

	scanf("I %d", &(list_parameters.block_size));

	while (TRUE)
	{
		if (feof(stdin) != 0)
			break;

		read_command(&list_parameters);
	}

	delete_list(&list_parameters);
	return 0;
}


void read_command(list_parameters_t* list_parameters)
{
	char command[MAX_COMMAND_LENGTH];
	scanf("%s", command);

	if (strcmp(command, ".A") == 0)
		parse_adding(list_parameters, BEFORE);
	if (strcmp(command, "A.") == 0)
		parse_adding(list_parameters, AFTER);
	if (strcmp(command, "R") == 0)
		parse_removing(list_parameters);
	if (strcmp(command, "P") == 0)
		parse_printing(list_parameters);
	if (strcmp(command, "i") == 0)
		parse_iterator_adding(list_parameters);
	if (strcmp(command, "+") == 0 || strcmp(command, "-") == 0)
		parse_iterator_moving(list_parameters, command[0]);
	if (strcmp(command, "K") == 0)
	{
		delete_list(list_parameters);
		exit(0);
	}
}


void parse_adding(list_parameters_t* list_parameters, char where)
{
	char position_string[MAX_ITERATOR_NAME_LENGTH], position;
	scanf("%s", position_string);

	position = get_position_from_string(position_string);

	unsigned long long int value_to_add;
	scanf("%llu", &value_to_add);

	continue_adding(list_parameters, position, value_to_add, where);
}

void parse_printing(list_parameters_t* list_parameters)
{
	char position_string[MAX_ITERATOR_NAME_LENGTH], position;
	scanf("%s", position_string);

	position = get_position_from_string(position_string);

	continue_printing(list_parameters, position);
}

void parse_removing(list_parameters_t* list_parameters)
{
	char position_string[MAX_ITERATOR_NAME_LENGTH], position;
	scanf("%s", position_string);

	position = get_position_from_string(position_string);

	continue_removing(list_parameters, position);
}

void parse_iterator_adding(list_parameters_t* list_parameters)
{
	int iterator_number;
	scanf("%d", &iterator_number);

	char position_string[MAX_ITERATOR_NAME_LENGTH], position;
	scanf("%s", position_string);

	position = get_position_from_string(position_string);

	continue_iterator_adding(list_parameters, (char)iterator_number, position);
}

void parse_iterator_moving(list_parameters_t* list_parameters, char movement_type)
{
	int iterator_number;
	scanf("%d", &iterator_number);

	continue_iterator_moving(list_parameters, (char)iterator_number, movement_type);
}


void continue_adding(list_parameters_t* list_parameters, char iterator_number, unsigned long long int value_to_add, char where)
{
	if (list_parameters->iterators[iterator_number].block == NULL) //if the list is empty
	{
		initialise_list(list_parameters, value_to_add);
		return;
	}

	if (is_place(list_parameters, iterator_number))
		add_to_block(list_parameters, iterator_number, value_to_add, where);
	else
	{
		add_to_new_block(list_parameters, iterator_number, value_to_add, where);
	}
}

void continue_printing(list_parameters_t* list_parameters, char position)
{
	if (position == ALL)
	{
		byte_t* current_block;
		current_block = list_parameters->iterators[BEG].block;

		if (current_block == NULL)
			return;

		while (TRUE)
		{
			char element_number;
	 		for (element_number = 0; element_number < read_elements_number_from_block(current_block); element_number++)
			{
				printf("%llu ", read_element_from_block(current_block, element_number));
			}
			
			byte_t* next;
			next = (byte_t*)read_address_from_block(current_block, NEXT);

			if (next != NULL)
				current_block = next;
			else
				break;
		}
	}
	else
	{
		byte_t* pointed_block;
		pointed_block = list_parameters->iterators[position].block;

		char pointed_element_number;
		pointed_element_number = list_parameters->iterators[position].element_number;

		printf("%llu", read_element_from_block(pointed_block, pointed_element_number));
	}

	putchar('\n');
}

void continue_removing(list_parameters_t* list_parameters, char position)
{
	byte_t* pointed_block;
	pointed_block = list_parameters->iterators[position].block;

	if (pointed_block == NULL)
		return;

	char pointed_element_number;
	pointed_element_number = list_parameters->iterators[position].element_number;

	set_element_in_block(pointed_block, pointed_element_number, NULL);
	change_iterators_removing(list_parameters, position);
	move_many_backwards(list_parameters, pointed_block, pointed_element_number + 1);
	change_elements_number(pointed_block, DECREMENT);

	check_structure(list_parameters, pointed_block);
}

void continue_iterator_adding(list_parameters_t* list_parameters, char iterator_number, char position)
{
	iterator copied_iterator;
	copied_iterator = list_parameters->iterators[position];

	list_parameters->iterators[iterator_number] = copied_iterator;
}

void continue_iterator_moving(list_parameters_t* list_parameters, char iterator_number, char movement_type)
{
	byte_t* pointed_block;
	pointed_block = list_parameters->iterators[iterator_number].block;

	char element_number, elements_number;
	element_number = list_parameters->iterators[iterator_number].element_number;
	elements_number = read_elements_number_from_block(pointed_block);

	if (movement_type == '+')
	{
		if (elements_number - 1 > element_number)
			(list_parameters->iterators[iterator_number].element_number)++;
		else
		{
			byte_t* next_block;
			next_block = (byte_t*)read_address_from_block(pointed_block, NEXT);

			if (next_block != NULL)
			{
				list_parameters->iterators[iterator_number].block = next_block;
				list_parameters->iterators[iterator_number].element_number = 0;
			}
		}
	}
	else
	{
		if(element_number > 0)
			(list_parameters->iterators[iterator_number].element_number)--;
		else
		{
			byte_t* prev_block;
			prev_block = (byte_t*)read_address_from_block(pointed_block, PREV);

			if (prev_block != NULL)
			{
				list_parameters->iterators[iterator_number].block = prev_block;
				list_parameters->iterators[iterator_number].element_number = read_elements_number_from_block(prev_block) - 1;
			}
		}
	}
}

void check_structure(list_parameters_t* list_parameters, byte_t* pointed_block)
{
	byte_t* prev, * next;
	prev = (byte_t*)read_address_from_block(pointed_block, PREV);
	next = (byte_t*)read_address_from_block(pointed_block, NEXT);

	char elements_number, min_elements_number, max_elements_number;
	elements_number = read_elements_number_from_block(pointed_block);
	min_elements_number = get_min_elements_number(list_parameters);
	max_elements_number = get_max_elements_number(list_parameters);

	if (elements_number == 0)
	{
		delete_block(list_parameters, pointed_block);
		return;
	}
	if (elements_number < min_elements_number)
	{
		if (prev != NULL)
			if (read_elements_number_from_block(prev) - 1 >= min_elements_number)
			{
				change_structure_shift_forward(list_parameters, pointed_block);
				return;
			}
		if (next != NULL)
			if (read_elements_number_from_block(next) - 1 >= min_elements_number)
			{
				change_structure_shift_backwards(list_parameters, pointed_block);
				return;
			}
		if(next != NULL)
			if (read_elements_number_from_block(next) + elements_number <= max_elements_number)
			{
				change_structure_shift_all_backwards(list_parameters, pointed_block);
				return;
			}
		if (prev != NULL)
			if (read_elements_number_from_block(prev) + elements_number <= max_elements_number)
			{
				change_structure_shift_all_backwards(list_parameters, prev);
				return;
			}
	}
}


void initialise_list(list_parameters_t* list_parameters, unsigned long long int value_to_add)
{
	byte_t* new_block;
	new_block = get_new_clear_block(list_parameters);

	change_elements_number(new_block, INCREMENT);
	set_element_in_block(new_block, 0, value_to_add);

	list_parameters->iterators[BEG].block = new_block;
	list_parameters->iterators[BEG].element_number = 0;
	list_parameters->iterators[END].block = new_block;
	list_parameters->iterators[END].element_number = 0;
}

void delete_list(list_parameters_t* list_parameters)
{
	byte_t* current_block;
	current_block = list_parameters->iterators[BEG].block;

	if (current_block == NULL)
		return;

	while (TRUE)
	{
		byte_t* next;
		next = (byte_t*)read_address_from_block(current_block, NEXT);

		free(current_block);

		if (next != NULL)
			current_block = next;
		else
			break;
	}
}

void add_to_block(list_parameters_t* list_parameters, char iterator_number, unsigned long long int value_to_add, char where)
{
	byte_t* pointed_block;
	pointed_block = list_parameters->iterators[iterator_number].block;

	char pointed_element_number;
	pointed_element_number = list_parameters->iterators[iterator_number].element_number;

	char position_of_new_element;
	if(where == BEFORE)
		position_of_new_element = list_parameters->iterators[iterator_number].element_number;
	if(where == AFTER)
		position_of_new_element = list_parameters->iterators[iterator_number].element_number + 1;

	move_many_forward(list_parameters, pointed_block, position_of_new_element);
	change_elements_number(pointed_block, INCREMENT);
	set_element_in_block(pointed_block, position_of_new_element, value_to_add);

	if (where == AFTER && pointed_block == list_parameters->iterators[END].block && pointed_element_number == list_parameters->iterators[END].element_number)
		//That's an only situation (in adding) when an iterator has to change despite its pointed element's not being moved
		(list_parameters->iterators[END].element_number)++;
}

void add_to_new_block(list_parameters_t* list_parameters, char iterator_number, unsigned long long int value_to_add, char where)
{
	byte_t* new_block;
	new_block = get_new_clear_block(list_parameters);
	
	byte_t* pointed_block;
	pointed_block = list_parameters->iterators[iterator_number].block;

	unsigned int past_pointed_next;
	past_pointed_next = read_address_from_block(pointed_block, NEXT);
	set_address_in_block(pointed_block, NEXT, (unsigned int)new_block);
	set_address_in_block(new_block, PREV, (unsigned int)pointed_block);
	set_address_in_block(new_block, NEXT, (unsigned int)past_pointed_next);
	
	if(past_pointed_next != NULL)
		set_address_in_block((byte_t*)past_pointed_next, PREV, (unsigned int)new_block);

	move_many_forward_outside(list_parameters, pointed_block);
	add_to_block(list_parameters, iterator_number, value_to_add, where);
}

void change_structure_shift_forward(list_parameters_t* list_parameters, byte_t* pointed_block)
{
	byte_t* prev;
	prev = (byte_t*)read_address_from_block(pointed_block, PREV);

	char prev_elements_number;
	prev_elements_number = read_elements_number_from_block(prev);

	move_many_forward(list_parameters, pointed_block, 0);
	
	set_element_in_block(pointed_block, 0, read_element_from_block(prev, prev_elements_number - 1));
	set_element_in_block(prev, prev_elements_number - 1, NULL);
	
	change_iterators_outside(list_parameters, prev, prev_elements_number - 1, pointed_block, 0);

	change_elements_number(prev, DECREMENT);
	change_elements_number(pointed_block, INCREMENT);
}

void change_structure_shift_backwards(list_parameters_t* list_parameters, byte_t* pointed_block)
{
	byte_t* next;
	next = (byte_t*)read_address_from_block(pointed_block, NEXT);

	char elements_number;
	elements_number = read_elements_number_from_block(pointed_block);

	set_element_in_block(pointed_block, elements_number, read_element_from_block(next, 0));
	set_element_in_block(next, 0, NULL);

	change_iterators_outside(list_parameters, next, 0, pointed_block, elements_number);

	move_many_backwards(list_parameters, next, 1); //1 is the number of the first element now present in the next block

	change_elements_number(next, DECREMENT);
	change_elements_number(pointed_block, INCREMENT);
}

void change_structure_shift_all_backwards(list_parameters_t* list_parameters, byte_t* pointed_block)
{
	byte_t* next;
	next = (byte_t*)read_address_from_block(pointed_block, NEXT);

	char elements_number, next_elements_number;
	elements_number = read_elements_number_from_block(pointed_block);
	next_elements_number = read_elements_number_from_block(next);

	char next_element_number;
	for (next_element_number = 0; next_element_number < next_elements_number; next_element_number++)
	{
		set_element_in_block(pointed_block, elements_number + next_element_number, read_element_from_block(next, next_element_number));
		set_element_in_block(next, next_element_number, NULL);

		change_iterators_outside(list_parameters, next, next_element_number, pointed_block, elements_number + next_element_number);

		change_elements_number(pointed_block, INCREMENT);
		change_elements_number(next, DECREMENT);
	}

	delete_block(list_parameters, next);
}


void move_many_forward(list_parameters_t* list_parameters, byte_t* block, char starting_element_number)
{
	char element_number;
	element_number = read_elements_number_from_block(block) - 1;

	while (element_number >= starting_element_number)
	{
		set_element_in_block(block, element_number + 1, read_element_from_block(block, element_number));
		change_iterators_inside_moving(list_parameters, block, element_number, FORWARD);

		element_number--;
	}
}

void move_many_forward_outside(list_parameters_t* list_parameters, byte_t* block)
{
	char elements_number;
	elements_number = read_elements_number_from_block(block);

	byte_t* next = (byte_t*)read_address_from_block(block, NEXT);

	char element_number;
	for (element_number = elements_number / 2; element_number < elements_number; element_number++)
	{
		set_element_in_block(next, element_number - elements_number / 2, read_element_from_block(block, element_number));
		set_element_in_block(block, element_number, NULL);
		
		change_elements_number(block, DECREMENT);
		change_elements_number(next, INCREMENT);

		change_iterators_outside(list_parameters, block, element_number, next, element_number - elements_number / 2);
	}
}

void move_many_backwards(list_parameters_t* list_parameters, byte_t* block, char starting_element_number)
{
	char element_number, elements_number_before_removing;
	elements_number_before_removing = read_elements_number_from_block(block);

	for (element_number = starting_element_number; element_number < elements_number_before_removing; element_number++) 
	{
		set_element_in_block(block, element_number - 1, read_element_from_block(block, element_number));
		set_element_in_block(block, element_number, NULL);
		change_iterators_inside_moving(list_parameters, block, element_number, BACKWARDS);
	}
}

void delete_block(list_parameters_t* list_parameters, byte_t* block)
{
	unsigned int next, prev;
	next = read_address_from_block(block, NEXT);
	prev = read_address_from_block(block, PREV);

	if (next != NULL)
		set_address_in_block((byte_t*)next, PREV, prev);
	if (prev != NULL)
		set_address_in_block((byte_t*)prev, NEXT, next);

	free(block);
}


byte_t* get_new_clear_block(list_parameters_t* list_parameters)
{
	byte_t* new_block;
	new_block = (byte_t*)malloc(list_parameters->block_size * sizeof(byte_t));

	clear_block(new_block, list_parameters->block_size);

	return new_block;
}

void set_address_in_block(byte_t* block, char address_type, unsigned int value)
{
	char offset;
	offset = (address_type == PREV) ? PREV_OFFSET : NEXT_OFFSET;

	int byte_number;
	for (byte_number = 0; byte_number < ADDRESS_SIZE; byte_number++)
	{
		unsigned char current_byte;
		current_byte = (unsigned char)(value >> ((ADDRESS_SIZE - 1 - byte_number) * BYTE_SIZE));

		block[offset + byte_number] = current_byte;
	}
}

void change_elements_number(byte_t* block, char option)
{
	unsigned char current_elements_number;
	current_elements_number = block[ELEMENTS_NUMBER_OFFSET];

	if (option == DECREMENT)
		block[ELEMENTS_NUMBER_OFFSET] = current_elements_number - 1;
	else
		block[ELEMENTS_NUMBER_OFFSET] = current_elements_number + 1;
}

void set_element_in_block(byte_t* block, char element_number, unsigned long long int value)
{
	int byte_number;
	for (byte_number = 0; byte_number < ELEMENT_SIZE; byte_number++)
	{
		unsigned char current_byte;
		current_byte = (unsigned char)(value >> ((ELEMENT_SIZE - 1 - byte_number) * BYTE_SIZE));

		block[FIRST_ELEMENT_OFFSET + element_number * ELEMENT_SIZE + byte_number] = current_byte;
	}
}

unsigned int read_address_from_block(byte_t* block, char address_type)
{
	unsigned int result;
	char offset;

	result = 0;
	offset = (address_type == PREV) ? PREV_OFFSET : NEXT_OFFSET;

	int byte_number;
	for (byte_number = 0; byte_number < ADDRESS_SIZE; byte_number++)
	{
		unsigned char current_byte;
		current_byte = block[offset + byte_number];

		result += (unsigned int)(current_byte << ((ADDRESS_SIZE - 1 - byte_number) * BYTE_SIZE));
	}

	return result;
}

char read_elements_number_from_block(byte_t* block)
{
	return block[ELEMENTS_NUMBER_OFFSET];
}

unsigned long long int read_element_from_block(byte_t* block, char element_number)
{
	unsigned long long int result;
	result = 0;

	int byte_number;
	for (byte_number = 0; byte_number < ELEMENT_SIZE; byte_number++)
	{
		unsigned long long int current_byte;
		current_byte = block[FIRST_ELEMENT_OFFSET + element_number * ELEMENT_SIZE + byte_number];

		result += (unsigned long long int)(current_byte << ((ELEMENT_SIZE - 1 - byte_number) * BYTE_SIZE));
	}

	return result;
}

void change_iterators_inside_moving(list_parameters_t* list_parameters, byte_t* block, char element_number, char direction)
{
	int iterator_number;
	for (iterator_number = 0; iterator_number <= BEG; iterator_number++)
	{
		if (list_parameters->iterators[iterator_number].block == block && list_parameters->iterators[iterator_number].element_number == element_number)
		{
			if(direction == FORWARD && iterator_number != BEG) //when moving forward BEG can't be moved, new element is about to appear there
				(list_parameters->iterators[iterator_number].element_number)++;
			if(direction == BACKWARDS)
				(list_parameters->iterators[iterator_number].element_number)--;
		}
	}
}

void change_iterators_outside(list_parameters_t* list_parameters, byte_t* past_block, char past_element_number, byte_t* new_block, char new_element_number)
{
	int iterator_number;
	for (iterator_number = 0; iterator_number <= BEG; iterator_number++)
	{
		if (list_parameters->iterators[iterator_number].block == past_block && list_parameters->iterators[iterator_number].element_number == past_element_number)
		{
			list_parameters->iterators[iterator_number].block = new_block;
			list_parameters->iterators[iterator_number].element_number = new_element_number;
		}
	}
}

void change_iterators_removing(list_parameters_t* list_parameters, char position)
{
	byte_t* pointed_block;
	pointed_block = list_parameters->iterators[position].block;

	char pointed_element_number;
	pointed_element_number = list_parameters->iterators[position].element_number;

	char elements_number_before_removing;
	elements_number_before_removing = read_elements_number_from_block(pointed_block);

	int iterator_number;
	for (iterator_number = 0; iterator_number <= BEG; iterator_number++)
	{
		if (list_parameters->iterators[iterator_number].block == pointed_block && list_parameters->iterators[iterator_number].element_number == pointed_element_number)
		{
			if (pointed_element_number < elements_number_before_removing - 1) //if there is an element to the right of the deleted one inside the same block
				(list_parameters->iterators[iterator_number].element_number)++;
			else
			{
				byte_t* next_block;
				next_block = (byte_t*)read_address_from_block(pointed_block, NEXT);

				if (next_block != NULL) //if there's anything inside the next block
				{
					list_parameters->iterators[iterator_number].block = next_block;
					list_parameters->iterators[iterator_number].element_number = 0;
				}
				else //if the "end" element was deleted
				{
					if (pointed_element_number > 0) //if there's anything to the left of the deleted element inside the same block
						(list_parameters->iterators[iterator_number].element_number)--;
					else
					{
						byte_t* previous_block;
						previous_block = (byte_t*)read_address_from_block(pointed_block, PREV);

						if (previous_block != NULL) //if there's anything inside the previous block
						{
							list_parameters->iterators[iterator_number].block = previous_block;
							list_parameters->iterators[iterator_number].element_number = read_elements_number_from_block(previous_block) - 1;
							
						}
						else //the list is now empty
						{
							list_parameters->iterators[iterator_number].block = NULL;
							list_parameters->iterators[iterator_number].element_number = TEST_INIT_VALUE;
						}
					}
				}
			}
		}
	}
}


void initialise_list_parameters(list_parameters_t* list_parameters)
{
	list_parameters->block_size = TEST_INIT_VALUE;

	int iterator_number;
	for (iterator_number = 0; iterator_number < MAX_ITERATORS_NUMBER + FIXED_ITERATORS_NUMBER; iterator_number++)
	{
		list_parameters->iterators[iterator_number].block = NULL;
		list_parameters->iterators[iterator_number].element_number = TEST_INIT_VALUE;
	}
}

void clear_block(byte_t* block, int block_size)
{
	int block_byte;
	for (block_byte = 0; block_byte < block_size; block_byte++)
	{
		block[block_byte] = NULL;
	}
}

char is_place(list_parameters_t* list_parameters, char iterator_number)
{
	char elements_number;
	elements_number = read_elements_number_from_block(list_parameters->iterators[iterator_number].block);

	char max_elements_number;
	max_elements_number = get_max_elements_number(list_parameters);

	if (elements_number < max_elements_number)
		return TRUE;
	return FALSE;
}

char get_min_elements_number(list_parameters_t* list_parameters)
{
	char max_elements_number;
	max_elements_number = get_max_elements_number(list_parameters);

	float result;
	result = (float)max_elements_number * MIN_FILL_RATIO;

	return (char)ceil(result);
}


char get_max_elements_number(list_parameters_t* list_parameters)
{
	return (char)((list_parameters->block_size - 2 * ADDRESS_SIZE - ELEMENTS_NUMBER_SIZE) / ELEMENT_SIZE);
}

char get_position_from_string(char* position_string)
{
	if (strcmp(position_string, "BEG") == 0)
		return BEG;
	if (strcmp(position_string, "END") == 0)
		return END;
	if (strcmp(position_string, "ALL") == 0)
		return ALL;
	return position_string[0] - '0'; //converting: ASCII -> number
}