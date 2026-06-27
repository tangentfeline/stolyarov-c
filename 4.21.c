#include <stdio.h>
#include <stdlib.h>

#define STACK_INIT_CAP 4

struct stack_dbl {
	double *data;
	int capacity;
	int size;
};

void stackdbl_init(struct stack_dbl *stack)
{
	stack->capacity = STACK_INIT_CAP;
	stack->size = 0;
	stack->data = malloc(stack->capacity * sizeof(double));
	if(!stack->data) {
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
}

void stackdbl_destroy(struct stack_dbl *stack)
{
	free(stack->data);
	
	stack->data = NULL;
	stack->size = 0;
	stack->capacity = 0;
}

void stackdbl_push(const double value, struct stack_dbl *stack)
{
	double *new_data;
	int i;
	if(stack->size == stack->capacity) {
		stack->capacity *= 2;
		new_data = malloc(stack->capacity * sizeof(double));
		if(!new_data) {
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}

		for(i = 0; i < stack->size; i++) {
			new_data[i] = stack->data[i];
		}
		free(stack->data);
		stack->data = new_data;
	}
	stack->data[stack->size] = value;
	stack->size++;
}

int stackdbl_empty(const struct stack_dbl *stack)
{
	return stack->size == 0;
}

double stackdbl_pop(struct stack_dbl *stack)
{
	if(stackdbl_empty(stack)) {
		fprintf(stderr, "Error: stackdbl_pop: stack is empty\n");
		exit(EXIT_FAILURE);
	}
	
	stack->size--;
	return stack->data[stack->size];
}

int main()
{
	return 0;
}
