#include <stdio.h>
#include <stdlib.h>

struct number {
	int value;
	struct number *next;
};

struct number_list {
	struct number *first;
	struct number *last;
};

void append_number(const int value, struct number_list *list)
{
	struct number *new_num;
	new_num = malloc(sizeof(struct number_list));
	if(!new_num) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	new_num->value = value;
	new_num->next = NULL;

	if(!list->first) {
		list->first = new_num;
	} else {
		list->last->next = new_num;
	}
	list->last = new_num;
}

void print_pairs(const struct number_list *list)
{
	struct number *curr, *next;
	if(!list->first) {
		return;
	}

	curr = list->first;
	while(curr->next) {
		next = curr->next;
		if(abs(curr->value - next->value) <= 5) {
			printf("%d %d\n", curr->value, next->value);
		}
		curr = curr->next;
	}
}

void free_list(struct number_list *list)
{
	struct number *curr, *next;
	curr = list->first;
	while(curr) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	return;
}

/*
void free_nodes_recursive(struct number *num)
{
	if(!num) {
		return;
	}
	free_node_recursive(num->next);
	free(num);
}
*/

int main()
{
	struct number_list list = {0};
	int value;

	while(1 == scanf("%d", &value)) {
		append_number(value, &list);
	}
	print_pairs(&list);
	free_list(&list);

	return(0);
}
