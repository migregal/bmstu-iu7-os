#include "stack.h"

typedef struct node {
    entry_t entry;
    struct node *next;
} node_t;

typedef struct dirstack
{
    node_t *head;
} dirstack_t;


dirstack_t *init()
{
    dirstack_t *stack = malloc(sizeof(dirstack_t));
    stack->head = NULL;

    return stack;
}

void push(dirstack_t *const stack, entry_t elem)
{
    node_t *node = malloc(sizeof(node_t));
    if (!node) {
        free_stack((dirstack_t **)&stack);

        fprintf(stderr, "Error while allocation.");
        exit(ALLOCATION_ERROR);
    }

    node->entry = elem;

    node->next = stack->head;
    stack->head = node;
}

entry_t pop(dirstack_t *const stack)
{
    if (stack->head)
    {
        node_t* temp = stack->head;
        stack->head = stack->head->next;

        entry_t res = temp->entry;
        free(temp);
        return res;
    }

    fprintf(stderr, "Stack is empty.");
    exit(STACK_ERROR);
}

uint8_t is_empty(const dirstack_t *const stack)
{
    return !(stack->head);
}

void free_entry(entry_t *entry) {
    if (!entry)
        return;

    free(entry->symbolic_name);
    free(entry->path);
}

void free_stack(dirstack_t ** stack)
{
    if (!stack ||!(*stack))
        exit(STACK_ERROR);

    while (!is_empty(*stack)) {
        pop((dirstack_t *const)(*stack));
    }
    free(*stack);
}
