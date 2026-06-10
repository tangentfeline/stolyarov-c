#include <stdio.h>
#include <stdlib.h>

struct letter {
    int symbol;             
    struct letter *next;   
};

struct word {
	struct letter *first_letter;
	struct letter *print_cursor;
	int length;
	struct word *next_word;
};

struct sentence {
    struct word *first_word;  
    struct word *last_word;  
    int word_count;
    int max_word_length;    
};

struct word_builder {
    struct letter *first_letter; 
    struct letter *last_letter; 
    int current_length;        
};

void append_letter(int symbol, struct word_builder *builder)
{
	struct letter *new_letter;
	new_letter = malloc(sizeof(struct letter));
	if(!new_letter) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_letter->symbol = symbol;
	new_letter->next = NULL;

	if(!builder->first_letter) {
		builder->first_letter = new_letter;
	} else {
		builder->last_letter->next = new_letter;
	}
	builder->last_letter = new_letter;
	builder->current_length++;
}

void append_word(struct word_builder *builder, struct sentence *sent)
{
	struct word *new_word;
	if(!builder->first_letter) {
		return;
	}

	new_word = malloc(sizeof(struct word));
	if(!new_word) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	new_word->first_letter = builder->first_letter;
	new_word->print_cursor = builder->first_letter;
	new_word->length = builder->current_length;
	new_word->next_word = NULL;
	
	if(!sent->first_word) {
		sent->first_word = new_word;
		sent->max_word_length = new_word->length;
	} else {
		sent->last_word->next_word = new_word;
	}
	sent->last_word = new_word;
	if(sent->max_word_length < new_word->length) {
		sent->max_word_length = new_word->length;
	}
	sent->word_count++;

	builder->first_letter = NULL;
	builder->last_letter = NULL;
	builder->current_length = 0;
}

void print_sentence(struct sentence *sent)
{
	struct word *w;
	int depth = 0;

	while (depth < sent->max_word_length) {
		w = sent->first_word;
		while (w) {
			if (w->print_cursor) {
				putchar(w->print_cursor->symbol);
				w->print_cursor = w->print_cursor->next;
			} else {
				putchar(' ');
			}
			w = w->next_word;
		}
		depth++;
		putchar('\n');
	}
}

void free_sentence(struct sentence *sent)
{
	struct word *curr_word = sent->first_word, *next_word;
	struct letter *curr_letter, *next_letter;
	while(curr_word) {
		curr_letter = curr_word->first_letter;
		while(curr_letter) {
			next_letter = curr_letter->next;
			free(curr_letter);
			curr_letter = next_letter;
		}
		next_word = curr_word->next_word;
		free(curr_word);
		curr_word = next_word;
	}
	sent->first_word = NULL;
	sent->last_word = NULL;
	sent->word_count = 0;
	sent->max_word_length = 0;
}

int main()
{
	struct sentence sentence = {0};
	struct word_builder builder = {0};
	int symbol;
	int in_word = 0;

	while((symbol = getchar()) != EOF) {
		if((symbol == '\n') || (symbol == ' ')) {
			if(in_word) {
				append_word(&builder, &sentence);
				in_word = 0;
			}
			if(symbol == '\n') {
				print_sentence(&sentence);
				free_sentence(&sentence);
			}
		}
		else {
			append_letter(symbol, &builder);
			in_word = 1;
		}
	}
	if(in_word) {
		append_word(&builder, &sentence);
	}
	if(sentence.first_word) {
		putchar('\n');
		print_sentence(&sentence);
		free_sentence(&sentence);
	}
	return 0;
}
