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

int match_meow( char *word)
{
	int i;
	for( i = 0; i < 16; i++)
		if( strcmp( word, MEOWS[i]) == 0) return i;
	return -1;
}

int valid_meow( char *str)
{
	int i, words = 0;
	char word[WORD_SIZE];
	char *ptr = word;

	memset( word, 0, WORD_SIZE);

	for( i = 0; i < strlen( str); i++)
	{
		if( str[i] == ' ')
		{
			
			if( match_meow( word) < 0)
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

void decrypt( char *str)
{
	int i;
	
	char word[WORD_SIZE];
	char *ptr = word;
	char chr = 0;
	
	memset( word, 0, WORD_SIZE);
	
	for( i = 0; i < strlen( str); i++)
	{
		if( str[i] == ' ')
		{
			int meow = match_meow( word);
			
			if( chr)
			{
				printf("%c", chr+meow);
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

void encrypt( char *str)
{
	int i;
		 
	for( i = 0; i < strlen( str); i++)
	{
		char c = str[i] < 128 ? str[i] : '?';
		printf("%s %s ", MEOWS[c>>4], MEOWS[c&15]);
	}
}

void remove_newline( char *str)
{
	int i;
	for( i = 0; i < strlen( str); i++)
		if( str[i] == '\n')
			str[i] = ' ';
}

int main()
{
	char *input = NULL;
	size_t len = 0;

	while( getline( &input, &len, stdin) > 0)
	{
		remove_newline( input);
		
		valid_meow( input) ? decrypt( input) : encrypt( input);
	}
	
	free( input); input = NULL;
	return 0;
}

