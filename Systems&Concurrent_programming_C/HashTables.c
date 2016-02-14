#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <assert.h>
#include <stdio.h>

struct node {
	char * data;		
	struct node * next;
};

struct hashtable {
	unsigned int size;
	int nEntries;
	struct node ** table;
};

// construct a new hash table with !!size elements!!
struct hashtable * hashtable_new(unsigned int size)
{
	// populate fields
	struct hashtable * result;
	result = malloc(sizeof(struct hashtable));
	result->size = size;
	result->nEntries = 0;
	result->table = malloc(sizeof(struct node *) * size); 
	return result;
}

unsigned hash_string(char * str)
{
	unsigned hash = 0;
	for ( int i = 0; str[i] != '\0'; i++ ) 
	{
		hash = hash * 37 + str[i];
	}
	return hash;
}

// free the memory for all parts of the hashtable
void hashtable_free(struct hashtable * this)
{
	// free each row of table ...
	for(int i = 0; i < this->size; i++)
	{
		free(this->table[i]);
	}
	// then free top level pointer
	free(this->table);
	// then free struct
	free(this);
}

// return 1 if string is already in table, 0 otherwise
int lookup(struct hashtable * this, char * item)
{
	printf("item: %s \n", item);
	unsigned int index;
	index = hash_string(item);
	index = index % 9;
	struct node * lookup_node;
	lookup_node = this->table[index];
	if(lookup_node != NULL)  
	{
		while(lookup_node != NULL)
		{
			printf("string lookup: %s \n", lookup_node->data);
			if(strcmp(lookup_node->data, item) == 0)  
			{
				return 1;
			}
			lookup_node = lookup_node->next;
		}	
	}
	return 0;
}

// insert string into the hash table, no effect if it's already there
void insert(struct hashtable * this, char * item)
{
	printf("inserting: %s \n", item);
	// check if its already there using lookup
	if(lookup(this, item) == 0)
	{
		unsigned int index;
		struct node * new_node;
		index = hash_string(item);
		index = index % 9;
		new_node = malloc(sizeof(struct node));
		// now populate new node
		new_node->next = this->table[index]; // pointer updated.
		new_node->data = item;
		this->table[index] = new_node; // insert at start of list
		this->nEntries++;
	}
}

// remove string from the hash table, no effect if not in table
void remove_string(struct hashtable * this, char * item)
{
	if(lookup(this, item) == 1)
	{
		printf("removing...\n");
		unsigned int index;
		index = hash_string(item);
		index = index % 9;
		struct node * removal_node;
		removal_node = this->table[index];
		while(strcmp(removal_node->next->data, item) != 0)
		{
			removal_node = removal_node->next;
		}
		removal_node->next = (removal_node->next)->next;
		this->nEntries--;
	}
}

// print out each entry in the hash table and the values
// stored at that entry
void print(struct hashtable * this)
{
	struct node * current_node;
	for(int i = 0; i < this->size; i++)
	{
		current_node = &this->table[i][0];
		if(current_node == NULL)
		{
			printf("%d: <empty>\n", i);
		}
		else
		{
			printf("%d: ", i); 
			do
			{
				printf("%s ", current_node->data);
				current_node = current_node->next;
			} while(current_node != NULL);
			printf("\n");
		}
	}
}

int main(int argc, char * argv[])
{
	int table_size = 9;
	struct hashtable * my_hashtable = hashtable_new(table_size);
	
	for(int i = 1; i < argc; i++)
	{
		printf("string: %s \n", argv[i]);
		insert(my_hashtable, argv[i]);
		printf("done inserting element \n");
	}
	printf("finished user input \n");
	print(my_hashtable);
	char * str1 = "fish";
	
	char * str2 = "porridge";
	
	insert(my_hashtable, str1);
	printf("finished fish input \n");
	print(my_hashtable);
	remove_string(my_hashtable, str2);
	printf("finished porridge removal \n");	

	print(my_hashtable);

	hashtable_free(my_hashtable);

	return 0;
}
