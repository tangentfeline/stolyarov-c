#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct sentence {
	char **words;
	int capacity;
	int size;
};

struct word {
	char *data;
	int capacity;
	int size;
};

void sentence_add_word(struct word *w, struct sentence *sent)
{
	int i;
	char **new_words;
	if(sent->size == sent->capacity) {
		sent->capacity = (sent->capacity == 0) ? 1 : sent->capacity * 2;
		new_words = malloc(sent->capacity * sizeof(char*));
		if(!new_words) {
			perror("Allocation failed for sentence words");
			exit(EXIT_FAILURE);
		}

		for(i = 0; i < sent->size; i++) {
			new_words[i] = sent->words[i];
		}
		free(sent->words);
		sent->words = new_words;
	}
	sent->words[sent->size] = w->data;
	sent->size++;
	
	w->data = NULL;
	w->size = 0;
	w->capacity = 0;
}

void word_add_char(char ch, struct word *w)
{
	int i;
	char *new_data;
	if(w->size == w->capacity) {
		w->capacity = (w->capacity == 0) ? 1 : w->capacity * 2;
		new_data = malloc(w->capacity * sizeof(char));
		if(!new_data) {
			perror("Allocation failed for word data");
			exit(EXIT_FAILURE);
		}

		for(i = 0; i < w->size; i++) {
			new_data[i] = w->data[i];
		}
		free(w->data);
		w->data = new_data;
	}

	w->data[w->size] = ch;
	w->size++;
}

void sentence_print_reverse(const struct sentence *sent)
{
	int i;
	int any_printed = 0;
	for(i = sent->size - 1; i >= 0; i--) {
		if(any_printed) {
			putchar(' ');
		}
		printf("%s", sent->words[i]);
		any_printed = 1;
	}
	putchar('\n');
}

void free_sentence(struct sentence *sent)
{
	int i;
	if(sent->words) {
		for(i = 0; i < sent->size; i++) {
			free(sent->words[i]);
		}
		free(sent->words);
	}
	sent->words = NULL;
	sent->size = 0;
	sent->capacity = 0;
}

int main()
{
	struct sentence sent = {NULL, 0};
	struct word w = {NULL, 0, 0};
	int ch;
	int in_word = 0;
	
	while((ch = getchar()) != EOF) {
		if(ch == '\n') {
			if(in_word) {
				word_add_char('\0', &w);
				sentence_add_word(&w, &sent);
				in_word = 0;
			}
			sentence_print_reverse(&sent);
			free_sentence(&sent);
		} else if (isspace(ch)) {
			if(in_word) {
				word_add_char('\0', &w);
				sentence_add_word(&w, &sent);
				in_word = 0;
			}
		} else {
			word_add_char(ch, &w);
			in_word = 1;
		}
	}

	if(in_word) {
		word_add_char('\0', &w);
		sentence_add_word(&w, &sent);
	}
	if(sent.size > 0) {
		sentence_print_reverse(&sent);
		free_sentence(&sent);
	}

	return 0;
}
