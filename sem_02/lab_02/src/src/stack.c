#include "stack.h"

typedef struct node {
    void        *entry;
    struct node *next;
} node_t;

typedef struct dirstack
{
    node_t *head;

    void (*el_free)(void **);
} dirstack_t;

dirstack_t *init(void (*el_free)(void **))
{
    dirstack_t *stack = malloc(sizeof(dirstack_t));
    if (!stack)
    {
        fprintf(stderr, "Error while allocation.");
        exit(ALLOCATION_ERROR);
    }

    stack->head = NULL;
    stack->el_free = el_free;

    return stack;
}

void push(dirstack_t *const stack, void *elem)
{
    if (!stack)
        exit(STACK_ERROR);

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

void *pop(dirstack_t *const stack)
{
    if (!stack->head)
    {
        fprintf(stderr, "Stack is empty.");
        exit(STACK_ERROR);
    }

    node_t *temp = stack->head;
    stack->head = stack->head->next;

    void *res = temp->entry;
    free(temp);
    return res;
}

uint8_t is_empty(const dirstack_t *const stack)
{
    if (!stack)
        exit(STACK_ERROR);

    return !(stack->head);
}

void free_stack(dirstack_t **stack)
{
    if (!stack ||!(*stack))
        exit(STACK_ERROR);

    while (!is_empty(*stack)) {
        (*stack)->el_free(pop((dirstack_t *const)(*stack)));
    }

    free(*stack);
}
