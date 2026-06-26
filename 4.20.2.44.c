#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct digit_sequence {
	char *data;
	int capacity;
	int size;
};

struct longest_sequence {
	struct digit_sequence *items;
	int capacity;
	int size;
	int max_len;
};

void longest_sequence_reset(struct longest_sequence *ls)
{
	int i;
	for(i = 0; i < ls->size; i++) {
		free(ls->items[i].data);
	}
	free(ls->items);

	ls->items = NULL;
	ls->size = 0;
	ls->capacity = 0;
	ls->max_len = 0;
}

void longest_sequence_append(const struct digit_sequence *seq,
				struct longest_sequence *ls)
{
	int i;
	struct digit_sequence *new_seq;
	if(ls->size == ls->capacity) {
		ls->capacity = (ls->capacity == 0) ? 1 : ls->capacity * 2;
		new_seq = malloc(ls->capacity * sizeof(struct digit_sequence));
		if(!new_seq) {
			perror("Allocation failed for sentence list");
			exit(EXIT_FAILURE);
		}

		for(i = 0; i < ls->size; i++) {
			new_seq[i] = ls->items[i];
		}
		free(ls->items);
		ls->items = new_seq;
	}
	ls->items[ls->size] = *seq;
	ls->size++;
}

void longest_sequence_push(struct digit_sequence *seq,
				struct longest_sequence *ls)
{
	if(ls->max_len < seq->size) {
		longest_sequence_reset(ls);
		ls->max_len = seq->size;
		longest_sequence_append(seq, ls);
	} else if(ls->max_len == seq->size) {
		longest_sequence_append(seq, ls);
	} else {
		free(seq->data);
	}

	seq->data = NULL;
	seq->size = 0;
	seq->capacity = 0;
}

void seq_add_char(const char ch, struct digit_sequence *seq)
{
	int i;
	char *new_data;
	if(seq->size == seq->capacity) {
		seq->capacity = (seq->capacity == 0) ? 1 : seq->capacity * 2;
		new_data = malloc(seq->capacity * sizeof(char));
		if(!new_data) {
			perror("Allocation failed for digit sequence buffer");
			exit(EXIT_FAILURE);
		}

		for(i = 0; i < seq->size; i++) {
			new_data[i] = seq->data[i];
		}
		free(seq->data);
		seq->data = new_data;
	}

	seq->data[seq->size] = ch;
	seq->size++;
}

void longest_sequence_print(const struct longest_sequence *ls)
{
	int i;
	int any_printed = 0;
	for(i = 0; i < ls->size; i++) {
		if(any_printed) {
			putchar(' ');
		}
		fputs(ls->items[i].data, stdout);
		any_printed = 1;
	}
	putchar('\n');
}

int main()
{
	struct longest_sequence longest_list = {NULL, 0, 0, 0};
	struct digit_sequence curr_seq = {NULL, 0, 0};
	int ch;
	int in_seq = 0;
	
	while((ch = getchar()) != EOF) {
		if(ch == '\n') {
			if(in_seq) {
				seq_add_char('\0', &curr_seq);
				longest_sequence_push(&curr_seq, &longest_list);
				in_seq = 0;
			}
			longest_sequence_print(&longest_list);
			longest_sequence_reset(&longest_list);
		} else if(!isdigit(ch) && in_seq) {
			seq_add_char('\0', &curr_seq);
			longest_sequence_push(&curr_seq, &longest_list);
			in_seq = 0;
		} else if(isdigit(ch)) {
			seq_add_char(ch, &curr_seq);
			in_seq = 1;
		}
	}

	if(in_seq) {
		seq_add_char('\0', &curr_seq);
		longest_sequence_push(&curr_seq, &longest_list);
	}
	if(longest_list.size > 0) {
		longest_sequence_print(&longest_list);
		longest_sequence_reset(&longest_list);
	}

	return 0;
}
