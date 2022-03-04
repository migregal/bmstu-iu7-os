#include "utils.h"

void log_entry(tree_entry_t *entry)
{
    printf(" ");

    for (size_t i = 0; i < entry->level; i++)
        printf("── ");

    if (!entry->is_dir)
    {
        printf("├─┤ %s [%lu]\n", entry->symbolic_name, entry->ino);
        return;
    }

    if (entry->symbolic_name[strlen(entry->symbolic_name) - 1] == '/')
        printf("> %s [%lu]\n", entry->symbolic_name, entry->ino);
    else
        printf("> %s/ [%lu]\n", entry->symbolic_name, entry->ino);
}
