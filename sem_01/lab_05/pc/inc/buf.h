#pragma once

#include <common.h>

#include <stdlib.h>

typedef char data_t[N];

typedef struct
{
    size_t r_pos;
    size_t w_pos;
    data_t data;
} cbuf_t;

int init_buf(cbuf_t *const buf);

int write_buf(cbuf_t *const buf, const char c);

int read_buf(cbuf_t *const buf, char *const dst);
