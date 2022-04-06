#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"

#define BUF_SIZE 10000

#define NO_ACCESS 1
#define BAD_ARGS 2

void print_cmdline(const char *proc_path, FILE *out)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/cmdline", proc_path);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, file);
    buf[len - 1] = 0;
    fprintf(out, "\n%s:\n%s\n", path, buf);

    fclose(file);
}

void print_cwd(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "%s/cwd", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);
    char buf[BUF_SIZE] = {'\0'};
    readlink(pathToOpen, buf, BUF_SIZE);
    fprintf(out, "%s\n", buf);
}

void print_comm(const char *proc_path, FILE *out)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/comm", proc_path);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, file);
    buf[len - 1] = 0;
    fprintf(out, "\n%s:\n%s\n", path, buf);

    fclose(file);
}

void print_environ(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "%s/environ", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);
    FILE *file = fopen(pathToOpen, "r");

    int len;
    char buf[BUF_SIZE];

    while ((len = fread(buf, 1, BUF_SIZE, file)) > 0)
    {
        for (int i = 0; i < len; i++)
            if (!buf[i])
                buf[i] = '\n';
        buf[len - 1] = '\n';
        fprintf(out, "%s", buf);
    }
    fclose(file);
}

void print_exe(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "%s/exe", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);

    char buf[BUF_SIZE] = {'\0'};
    readlink(pathToOpen, buf, BUF_SIZE);
    fprintf(out, "%s\n", buf);
}

void print_fd(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "%s/fd/", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);

    DIR *dir = opendir(pathToOpen);
    struct dirent *readDir;
    char string[PATH_MAX];
    char path[BUF_SIZE] = {'\0'};
    while ((readDir = readdir(dir)) != NULL)
    {
        if ((strcmp(readDir->d_name, ".") != 0) && (strcmp(readDir->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", pathToOpen, readDir->d_name);
            readlink(path, string, PATH_MAX);
            fprintf(out, "%s -> %s\n", readDir->d_name, string);
        }
    }

    closedir(dir);
}

void print_root(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX] = {'\0'};
    snprintf(pathToOpen, PATH_MAX, "%s/root", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);
    char buf[BUF_SIZE] = {'\0'};
    readlink(pathToOpen, buf, BUF_SIZE);
    fprintf(out, "%s\n", buf);
}

void print_stat(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX] = {'\0'};
    snprintf(pathToOpen, PATH_MAX, "%s/stat", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);

    char buf[BUF_SIZE];
    FILE *file = fopen(pathToOpen, "r");
    fread(buf, 1, BUF_SIZE, file);

    char *token = strtok(buf, " ");
    for (int i = 0; token != NULL; i++)
    {
        fprintf(out, WITH_DESCR[i], token);
        token = strtok(NULL, " \n");
    }

    fclose(file);
}

void print_statm(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "%s/statm", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);

    FILE *file = fopen(pathToOpen, "r");
    char buf[BUF_SIZE];
    fread(buf, 1, BUF_SIZE, file);

    char *token = strtok(buf, " ");
    for (int i = 0; token != NULL; i++)
    {
        fprintf(out, STATM_PATTERNS[i], token);
        token = strtok(NULL, " \n");
    }
    fclose(file);
}

void print_maps(const char *proc_path, FILE *out)
{
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "%s/maps", proc_path);
    fprintf(out, "\n%s:\n", pathToOpen);

    char buf[BUF_SIZE] = {'\0'};
    FILE *file = fopen(pathToOpen, "r");
    int lengthOfRead;
    while ((lengthOfRead = fread(buf, 1, BUF_SIZE, file)))
    {
        buf[lengthOfRead] = '\0';
        fprintf(out, "%s\n", buf);
    }
    fclose(file);
}

char* get_proc_path(int argc, char *argv[])
{
    static char pathToOpen[PATH_MAX];
    if (argc < 2)
    {
        snprintf(pathToOpen, PATH_MAX, "/proc/self");
    }
    else {
        int pid = atoi(argv[1]);
        snprintf(pathToOpen, PATH_MAX, "/proc/%d", pid);
    }

    if (access(pathToOpen, F_OK))
    {
        printf("Директория для введенного pid не доступна!");
        exit(NO_ACCESS);
    }
    return pathToOpen;
}

int main(int argc, char *argv[])
{
    char *proc_path = get_proc_path(argc, argv);

    FILE *out = fopen("result.txt", "a");

    print_cmdline(proc_path, out);
    print_cwd(proc_path, out);
    print_comm(proc_path, out);
    print_environ(proc_path, out);
    print_exe(proc_path, out);
    print_fd(proc_path, out);
    print_root(proc_path, out);
    print_stat(proc_path, out);
    print_statm(proc_path, out);
    print_maps(proc_path, out);

    fclose(out);

    return 0;
}
