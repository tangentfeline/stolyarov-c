#include <stdio.h>
#include <stdlib.h>

struct digit {
    int value;             
    struct digit *next;   
};

struct digit_sequence {
	struct digit *first_digit;
	int length;
	struct digit_sequence *next_sequence;
};

struct line {
    struct digit_sequence *first_sequence;  
    struct digit_sequence *last_sequence;  
    int max_sequence_length;    
};

struct sequence_builder {
    struct digit *first_digit; 
    struct digit *last_digit; 
    int current_length;        
};

void append_digit(int value, struct sequence_builder *builder)
{
	struct digit *new_digit;
	new_digit = malloc(sizeof(struct digit));
	if(!new_digit) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_digit->value = value;
	new_digit->next = NULL;

	if(!builder->first_digit) {
		builder->first_digit = new_digit;
	} else {
		builder->last_digit->next = new_digit;
	}
	builder->last_digit = new_digit;
	builder->current_length++;
}

void append_sequence(struct sequence_builder *builder, struct line *line)
{
	struct digit_sequence *new_sequence;
	if(!builder->first_digit) {
		return;
	}

	new_sequence = malloc(sizeof(struct digit_sequence));
	if(!new_sequence) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	new_sequence->first_digit = builder->first_digit;
	new_sequence->length = builder->current_length;
	new_sequence->next_sequence = NULL;

	if(!line->first_sequence) {
		line->first_sequence = new_sequence;
		line->max_sequence_length = new_sequence->length;
	} else {
		line->last_sequence->next_sequence = new_sequence;
	}
	line->last_sequence = new_sequence;
	if(line->max_sequence_length < new_sequence->length) {
		line->max_sequence_length = new_sequence->length;
	}

	builder->first_digit = NULL;
	builder->last_digit = NULL;
	builder->current_length = 0;
}

void print_longest_sequences(struct line *line)
{
	struct digit_sequence *curr_seq = line->first_sequence;
	struct digit *curr_digit;
	int printed_any = 0;
	while(curr_seq) {
		if(curr_seq->length == line->max_sequence_length) {
			if(printed_any) {
				putchar(' ');
			}
			curr_digit = curr_seq->first_digit;
			while(curr_digit) {
				putchar(curr_digit->value);
				curr_digit = curr_digit->next;
			}
			printed_any = 1;
		}
		curr_seq = curr_seq->next_sequence;
	}
	putchar('\n');
}

void free_line(struct line *line)
{
	struct digit_sequence *curr_seq = line->first_sequence, *next_seq;
	struct digit *curr_digit, *next_digit;
	while(curr_seq) {
		curr_digit = curr_seq->first_digit;
		while(curr_digit) {
			next_digit = curr_digit->next;
			free(curr_digit);
			curr_digit = next_digit;
		}
		next_seq = curr_seq->next_sequence;
		free(curr_seq);
		curr_seq = next_seq;
	}
	line->first_sequence = NULL;
	line->last_sequence = NULL;
	line->max_sequence_length = 0;
}

int main()
{
	struct line line = {0};
	struct sequence_builder builder = {0};
	int symbol;
	int in_sequence = 0;

	while((symbol = getchar()) != EOF) {
		if(symbol >= '0' && symbol <= '9') {
			append_digit(symbol, &builder);
			in_sequence = 1;
		}
		else {
			if(in_sequence) {
				append_sequence(&builder, &line);
				in_sequence = 0;
			}
			if(symbol == '\n') {
				print_longest_sequences(&line);
				free_line(&line);
			}
		}
	}
	if(in_sequence) {
		append_sequence(&builder, &line);
	}
	if(line.first_sequence) {
		// putchar('\n');
		print_longest_sequences(&line);
		free_line(&line);
	}
	return 0;
}
