#include "tree.h"

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry);

void process_dir(dirstack_t *const stack, const entry_t cur_entry, DIR *dp, struct dirent *dirp);

void dirtree(char *path)
{
    struct stat statbuf;
    if (lstat(path, &statbuf) < 0)
        return;

    struct dirent *dirp = NULL;
    DIR *dp = NULL;
    entry_t cur_entry = {path, realpath(path, NULL), 1, statbuf.st_ino, 0};
    dirstack_t stack = init();

    push(&stack, cur_entry);

    while (stack.size)
    {
        cur_entry = pop(&stack);

        process_dir(&stack, cur_entry, dp, dirp);

        if (dp != NULL && closedir(dp) < 0)
            fprintf(stderr, "Can't close directory %s\n", cur_entry.symbolic_name);
    }

    free_stack(&stack);
}

void process_dir(dirstack_t *const stack, const entry_t cur_entry, DIR *dp, struct dirent *dirp)
{
    log_entry(cur_entry);

    if ((dp = opendir(cur_entry.path)) == NULL)
        return;

    chdir(cur_entry.path);

    while ((dirp = readdir(dp)) != NULL)
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
            process_entry(stack, cur_entry.level + 1, dirp->d_name);
}

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry)
{
    struct stat statbuf;

    if (lstat(cur_entry, &statbuf) < 0)
        return;

    entry_t element = {cur_entry, realpath(cur_entry, NULL), 0, statbuf.st_ino, level};

    if (S_ISDIR(statbuf.st_mode) != 0)
    {
        element.is_dir = 1;
        push(stack, element);
        return;
    }


    log_entry(element);
}
