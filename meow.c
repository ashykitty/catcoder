#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* meow translator 10/09/21 */

#define WORD_SIZE 8

static const char * const MEOWS[] = {
	"meow",
	"Meow",
	"meoww",
	"MEOW",
	"MEOWW",
	"m-meow",
	"MeOw",
	"mEoW",
	"meow?",
	"meowo",
	"mEow",
	"meOw",
	"meoW",
	"mEOW",
	"MeOW",
	"mEOw"
};

int invalid_character( char c)
{
	char *valid = "meowMEOW-?";
	int i;

	for( i = 0; i < strlen(valid); i++)
	{
		if( c == valid[i])
			return 0;
	}

	return 1;
}

int match_meow( char *word, size_t len)
{
	int i;
	for( i = 0; i < 16; i++)
		if( strncmp( word, MEOWS[i], len) == 0) return i;
	return -1;
}

int valid_meow( char *str, size_t len)
{
	int i, words = 0;
	char word[WORD_SIZE];
	char *ptr = word;

	memset( word, 0, WORD_SIZE);

	for( i = 0; i < len; i++)
	{
		if( str[i] == ' ')
		{
			
			if( match_meow( word, len) < 0)
				return 0;

			memset( word, 0, WORD_SIZE);
			ptr = word;
			words++;

		} else {

			if( invalid_character( str[i]))
				return 0;

			*(ptr++) = str[i];

			if( ptr >= word+WORD_SIZE)
				return 0;
		}	

	}
	return words % 2 == 0;
}

void decrypt( char *str, size_t len)
{
	int i, words = 0;
	
	char word[WORD_SIZE];
	char *ptr = word;
	char chr = 0;
	
	memset( word, 0, WORD_SIZE);
	
	for( i = 0; i < len; i++)
	{
		if( str[i] == ' ')
		{
			int meow = match_meow( word, len);
			
			if( words++ % 2)
			{
				printf("%c", meow+chr);
				chr = 0;
			} else {
				chr = meow << 4;
			}
			
			memset( word, 0, WORD_SIZE);
			ptr = word;
			
		} else {
			*(ptr++) = str[i];
		}
	}	
}

void encrypt( char *str, size_t len)
{
	int i;
		 
	for( i = 0; i < len; i++)
	{
		unsigned char c = (unsigned char)str[i];
		printf("%s %s ", MEOWS[c>>4], MEOWS[c&15]);
	}
}

int main()
{
	char *input = NULL;
	size_t alloc = 0;
	int len = 0;

	while(1)
	{
		len = getline( &input, &alloc, stdin);
		if( len <= 0) break;

		input[len] = ' ';
		
		valid_meow( input, len) ? decrypt( input, len) : encrypt( input, len);
	}
	
	free( input); input = NULL;
	return 0;
}

