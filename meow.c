#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* meow translator 10/09/21 */

#define WORD_SIZE 8
#define INVALID_MSG "INVALID MEOW TEXT OWO"

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

char *join( int argc, char ** argv, size_t *arg_len)
{
	int i;
	size_t full_len = 0;
	char *text = NULL;
	
	for( i = 0; i < argc; i++)
		full_len += strlen( argv[i]) + 1;
	
	text = malloc( full_len + 1);
	if( text == NULL) return NULL;
	
	text[full_len] = 0x00;
	
	for( i = 0; i < argc; i++)
	{
		size_t len = strlen( argv[i]);
		memcpy( text, argv[i], len);
		*(text + len) = ' ';
		text += len + 1;
	}
	
	text -= full_len;
	*arg_len = full_len;
	
	return text;
}

int match_meow( char *word)
{
	int i;
	for( i = 0; i < 16; i++)
		if( strcmp( word, MEOWS[i]) == 0) return i;
	return -1;
}

void decrypt( int argc, char ** argv)
{
	int i;
	char *text = NULL;
	size_t len = 0;
	
	char word[WORD_SIZE];
	char *ptr = word;
	char chr = 0;
	
	text = join( argc, argv, &len);
	memset( word, 0, WORD_SIZE);
	
	for( i = 0; i < len; i++)
	{
		if( text[i] == ' ')
		{
			int meow = match_meow( word);
			if( meow < 0)
			{
				printf(INVALID_MSG);
				free( text); text = NULL;
				return;
			}
			
			if( chr > 0)
			{
				chr += meow;
				printf("%c", chr);
				chr = 0;
			} else {
				chr += meow << 4;
			}
			
			memset( word, 0, WORD_SIZE);
			ptr = word;
			
		} else {
			*(ptr++) = text[i];
			if( ptr >= word + WORD_SIZE)
			{
				printf(INVALID_MSG);
				return;
			}			
		}
	}
	
	free( text); text = NULL;
}

void encrypt( int argc, char ** argv)
{
	unsigned char *text = NULL;
	size_t len = 0;
	int i;
	
	text = join( argc, argv, &len);
	
	for( i = 0; i < len-1; i++)
	{
		char c = text[i] < 128 ? text[i] : '?';
		printf("%s %s ", MEOWS[c>>4], MEOWS[c&15]);
	}
	
	free( text); text = NULL;
}

void parse_argument( int argc, char ** argv)
{
	if( strcmp( argv[1], "-e") == 0)
	{
		encrypt( argc - 2, argv + 2);
		return;
	}
	if( strcmp( argv[1], "-d") == 0)
	{
		decrypt( argc - 2, argv + 2);
		return;
	}
}

int main( int argc, char ** argv)
{
	if( argc > 2)
	{
		parse_argument( argc, argv);
	} else {
		printf("how to use:\n\n	-e [msg]\n\n-d [msg]\n");
	}
	return 0;
}

