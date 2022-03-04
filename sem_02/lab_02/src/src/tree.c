#include "tree.h"
#include "tree_struct.h"

#include <string.h>

tree_entry_t *new_entry(char *path, uint8_t is_dir, unsigned long ino, size_t level);

void free_entry(void **entry);

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry);

void process_dir(dirstack_t *const stack, tree_entry_t *const cur_entry, struct dirent *dirp);

void dirtree(char *path)
{
    struct stat statbuf;
    if (lstat(path, &statbuf) < 0)
        return;

    struct dirent *dirp = NULL;

    tree_entry_t *cur_entry = new_entry(path, 1, statbuf.st_ino, 0);

    dirstack_t *stack = init(free_entry);

    push(stack, cur_entry);

    while (!is_empty(stack))
    {
        cur_entry = pop(stack);

        process_dir(stack, cur_entry, dirp);

        free_entry((void**)&cur_entry);
    }

    free_stack(&stack);
}

void process_dir(dirstack_t *const stack, tree_entry_t *const cur_entry, struct dirent *dirp)
{
    log_entry(cur_entry);

    DIR *dp = NULL;
    if ((dp = opendir(cur_entry->path)) == NULL)
        return;

    chdir(cur_entry->path);

    while ((dirp = readdir(dp)) != NULL)
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
            process_entry(stack, cur_entry->level + 1, dirp->d_name);

    if (dp != NULL && closedir(dp) < 0)
        fprintf(stderr, "Can't close directory %s\n", cur_entry->symbolic_name);
}

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry)
{
    struct stat statbuf;

    if (lstat(cur_entry, &statbuf) < 0)
        return;

    tree_entry_t *element = new_entry(cur_entry, 0, statbuf.st_ino, level);

    if (S_ISDIR(statbuf.st_mode) != 0)
    {
        element->is_dir = 1;
        push(stack, element);
        return;
    }


    log_entry(element);
    free_entry((void**)&element);
}

tree_entry_t *new_entry(char *path, uint8_t is_dir, unsigned long ino, size_t level)
{
    tree_entry_t *temp = malloc(sizeof(tree_entry_t));
    if (!temp)
    {
        fprintf(stderr, "Error while allocation.");
        exit(ALLOCATION_ERROR);
    }

    temp->symbolic_name = strdup(path);
    temp->path = realpath(temp->symbolic_name, NULL);
    temp->is_dir = is_dir;
    temp->ino = ino;
    temp->level = level;

    return temp;
}

void free_entry(void **entry)
{
    if (!entry || !*entry)
        return;

    tree_entry_t **entry_t = (tree_entry_t **)entry;
    free((*entry_t)->symbolic_name);
    free((*entry_t)->path);
    free(*entry_t);
}
