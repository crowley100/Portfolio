#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include "huffman.h" 

struct symbol{
	union u{
		struct children{
			struct symbol * left; // 0
			struct symbol * right; // 1		
		}child; // instance

		unsigned char c; 
	}union_member; // instance
	int frequency;	
	int is_leaf;
}huff_tree_root; 

struct bitfile{
	FILE * file;
	unsigned char buffer;
	int buffer_index;
};

struct bitfile * bitfile_new(char * filename)
{
	struct bitfile * result;
	result = malloc(sizeof(struct bitfile));
	// populate fields
	result->file = fopen(filename, "r"); // "r" and "w" passed as parameter for read/write
	result->buffer = 0;	
	result->buffer_index = -1; 
	return result;
}

int bitfile_read(struct bitfile * this)
{
	if(this->buffer_index == -1)
	{
		// refill buffer
		this->buffer = fgetc(this->file);
		this->buffer_index = 7; // 7 or 8??
	}
	int result = this->buffer >> (this->buffer_index)&1;
	this->buffer_index--;
	return result;
}

unsigned char get_next_char(struct symbol * root, struct bitfile * file)
{
	int bit;
	if(root->is_leaf == 1)
	{
		return root->union_member.c;
	}
	bit = bitfile_read(file);
	if(bit == 0)
	{
		return get_next_char(root->union_member.child.left, file);
	}
	else
	{
		return get_next_char(root->union_member.child.right, file);
	}
}

int count = 0;// global var
unsigned char ** huff_codes; 

void print_codes(struct symbol * s, int * path, int depth) 
{
	printf("check... \n");
	if(s->is_leaf)
	{
		printf("check1... count: %d \n", count);
		printf("%c \n", s->union_member.c);
		huff_codes[count] = malloc(sizeof(unsigned char*)*(depth+1));
		huff_codes[count][0] = s->union_member.c; 
		fprintf(stderr, "check2... \n");		
		for(int i = 0; i <= depth; i++)
		{
			printf("oops \n");
			if(i == depth)
			{
				huff_codes[count][i+1] = '\n';
			}
			else
			{
				if(path[i] == 0)
				{
					huff_codes[count][i+1] = '0';
				}
				else
				{
					huff_codes[count][i+1] = '1';
				}
			}
		}
		printf("hi \n");
		count++;
		printf("count: %d \n", count);	
	}
	else
	{
		path[depth] = 0;
		printf("path check1... \n");
		print_codes(s->union_member.child.left, path, depth+1); 
		path[depth] = 1;
		printf("path check2... \n");
		print_codes(s->union_member.child.right, path, depth+1); 
	}
}


struct symbol * get_smallest(struct symbol ** symbol_list, int N)
{
	struct symbol * result = symbol_list[0];
	int index = 0;
	for(int i = 0; i < N; i++)
	{
		if(result->frequency > symbol_list[i]->frequency)
		{
			result = symbol_list[i];
			index = i;
		}
	}
	while(index < (N-1))
	{
		symbol_list[index] = symbol_list[index+1];
		index++;
	}
	return result;
}

void build_huffman_tree(int * freqs, unsigned char * chars)
{
	printf("\n characters being mapped: \n");
	for(int j = 0; j < 95; j++)
	{

		printf("char: %c  frequency: %d  \n", chars[j], freqs[j]);	
	}
	printf("\n");

	int nitems = 95;
	// create array of pointers
	struct symbol ** list;
	struct symbol * new_symbol;
	list = malloc(sizeof(struct symbol *) * nitems);  
	for(int i = 0; i < nitems; i++)
	{
		list[i] = malloc(sizeof(struct symbol)); 
		struct	symbol * leaf;
		leaf = malloc(sizeof(struct symbol));
		leaf->union_member.c = chars[i];
		leaf->frequency = freqs[i];
		leaf->is_leaf = 1;	
		list[i] = leaf;	
	}
	
	// merge nitems-1 times
	while(nitems > 1)
	{
		struct symbol * smallest = get_smallest(list, nitems);
		struct symbol * next_smallest = get_smallest(list, (nitems-1)); 
		
		//create compound symbol
		new_symbol = malloc(sizeof(struct symbol));
		//populate symbol
		new_symbol->union_member.child.left = smallest;
		new_symbol->union_member.child.right = next_smallest; 
		new_symbol->frequency = smallest->frequency + next_smallest->frequency;
		list[nitems-2] = new_symbol; 
		nitems--;
	}
	huff_tree_root = * new_symbol; // pointer to root
	int * null_array;
	huff_codes = malloc(sizeof(unsigned char*)*96);
	print_codes(new_symbol, null_array, 0); 
}

void run(int argc, char ** argv)
{
	printf("argc?: %d \n", argc);
	printf("argv[1]?: %s \n", argv[1]);
	printf("argv[2]?: %s \n", argv[2]);
	printf("argv[3]?: %s \n", argv[3]);
	unsigned char c;
	FILE * file;
	
	unsigned char char_array[95];
	int freq_array[95];
	for(int i = 0; i < 95; i++)
	{
		char_array[i] = i + 32;	
		freq_array[i] = 1;	
	}

	file = fopen(argv[2], "r");
	assert( file != NULL );
	c = fgetc(file);
	int count;
	while( !feof(file) ) {
		count = 0;
		while(char_array[count] != c)
		{
			count++;
		}
		freq_array[count]++;
		c = fgetc(file);
	}
	fclose(file);

	//bug fixes
	char_array[60] = 92;
	char_array[61] = 93;

	

	build_huffman_tree(freq_array, char_array);
}

int main(int argc, char ** argv)
{
	run(argc, argv);
	printf("%c, %c, %c \n", huff_codes[0][0], huff_codes[1][1], huff_codes[2][2]);
	printf("hi there (main) \n");
	if(strcmp(argv[1], "huffcode") == 0)
	{
		//encode
		unsigned char encoding_buffer = 0;
		int encoding_buffer_index = 7;
		int bit;	 
		printf("%s \n", argv[3]);
		struct bitfile * new_bitfile = bitfile_new(argv[3]);

		// obtain number of chars in file and reset file pointer 
		fseek(new_bitfile->file, 0, SEEK_END); 
		int number_of_chars = ftell(new_bitfile->file);
		fseek(new_bitfile->file, 0, SEEK_SET);

		FILE * bit_output_file = fopen(argv[4], "w");  // encoded output file

		// reserve first 32 bits of file to indicate number of chars
		int *p = &number_of_chars;
		fwrite(p, sizeof(int), 1, bit_output_file);
		unsigned char new_char = fgetc(new_bitfile->file);
		while( !feof(new_bitfile->file) ) 
		{
			printf("cat \n");
			printf("new char: %c \n", new_char);
			int i = 0;
			int j = 1;
			while(new_char != huff_codes[i][0])
			{
				i++;
			}
			while(huff_codes[i][j] != '\n')
			{
				if(encoding_buffer_index == -1)
				{
					printf("buffer: %c \n", encoding_buffer);
					fputc(encoding_buffer, bit_output_file);
					number_of_chars++;
					encoding_buffer_index = 7;
				}
				if(huff_codes[i][j] == '1')
				{
					// set bit in buffer
					encoding_buffer = encoding_buffer | 1 << encoding_buffer_index; 
					encoding_buffer_index--;
				}
				if(huff_codes[i][j] == '0')
				{
					// clear bit in buffer
					encoding_buffer = encoding_buffer & ~(1 << encoding_buffer_index); 
					encoding_buffer_index--;
				}
				j++;
			}
			new_char = fgetc(new_bitfile->file);
			printf("end?? \n");
		}
	}
	else if(strcmp(argv[1], "huffdecode") == 0)
	{
		//decode
		struct bitfile * decoding_bitfile = bitfile_new(argv[2]);
		FILE * text_output_file = fopen(argv[3], "w");  // decoded output file	
		int char_count; 
		int *cp = &char_count; 
		fread(cp, sizeof(int), 1, decoding_bitfile->file);	
		for(int i = 0; i < *cp; i++) 
		{
			unsigned char c = get_next_char(&huff_tree_root, decoding_bitfile);
			fputc(c, text_output_file);
		}
	}	
	else
	{
		fprintf(stderr, "FATAL!\n");
		exit(1);
	}
	return 0;
}
