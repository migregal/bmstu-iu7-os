#include <buf.h>

#include <string.h>

int init_buf(cbuf_t *const buf) {
    if (!buf)
        return -1;

    memset(buf, 0, sizeof(cbuf_t));

    return 0;
}

int write_buf(cbuf_t *const buf, const char c) {
    if (!buf)
        return -1;

    buf->data[buf->w_pos++] = c;
    buf->w_pos %= N;

    return 0;
}

int read_buf(cbuf_t *const buf, char *const dst) {
    if (!buf)
        return -1;

    *dst = buf->data[buf->r_pos++];
    buf->r_pos %= N;

    return 0;
}
