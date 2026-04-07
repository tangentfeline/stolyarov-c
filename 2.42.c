#include <stdio.h>
#include <stdlib.h>

struct char_node {
	int ch;
	struct char_node *next;
};

struct char_list {
	struct char_node *head;
	struct char_node *tail;
};

struct word_node {
	struct char_node *chars;
	struct word_node *next;
};

void append_char(int ch, struct char_list *word)
{
	struct char_node *new_node;
	new_node = malloc(sizeof(struct char_node));
	if(!new_node) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_node->ch = ch;
	new_node->next = NULL;
	if(!word->head) {
		word->head = new_node;
		word->tail = new_node;
		return;
	}
	word->tail->next = new_node;
	word->tail = new_node;
}

void prepend_word(struct char_list *word, struct word_node **sentence)
{
	struct word_node *new_node;
	new_node = malloc(sizeof(struct word_node));
	if(!new_node) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_node->chars = word->head;
	new_node->next = *sentence;
	*sentence = new_node;
	word->head = NULL;
	word->tail = NULL;
}

void print_string(struct word_node *sentence)
{
	struct word_node *word_ptr;
	struct char_node *char_ptr;
	for(word_ptr = sentence; word_ptr; word_ptr = word_ptr->next) {
		for(char_ptr = word_ptr->chars; char_ptr; char_ptr = char_ptr->next) {
			putchar(char_ptr->ch);
		}
		if(word_ptr->next) {
			putchar(' ');
		}
	}
	if(sentence) {
		putchar('\n');
	}
}

void free_string(struct word_node **head)
{
	struct word_node *word_ptr = *head, *word_ptr_next;
	struct char_node *char_ptr, *char_ptr_next;
	while(word_ptr) {
		char_ptr = word_ptr->chars;
		while(char_ptr) {
			char_ptr_next = char_ptr->next;
			free(char_ptr);
			char_ptr = char_ptr_next;
		}
		word_ptr_next = word_ptr->next;
		free(word_ptr);
		word_ptr = word_ptr_next;
	}
	*head = NULL;
}

int main()
{
	struct word_node *sentence = NULL;
	struct char_list word = {NULL, NULL};
	int ch;
	int in_word = 0;
	while((ch = getchar()) != EOF) {
		if((ch == '\n') || (ch == ' ')) {
			if(in_word) {
				prepend_word(&word, &sentence);
				in_word = 0;
			}
			if(ch == '\n') {
				print_string(sentence);
				free_string(&sentence);
			}
		}
		else {
			append_char(ch, &word);
			in_word = 1;
		}
	}
	if(in_word) {
		prepend_word(&word, &sentence);
	}
	if(sentence) {
		putchar('\n');
		print_string(sentence);
		free_string(&sentence);
	}
	return 0;
}
