#ifndef INC_09_C_CODE_LIKE_HUFFMAN_READBUF_H
#define INC_09_C_CODE_LIKE_HUFFMAN_READBUF_H

typedef struct readbuf_struct {
    char *mp_buf;
    char *mp_cur;
    size_t mn_size;
} readbuf_t;

inline int readbuf_init(readbuf_t *);

inline void readbuf_reset(readbuf_t *);

inline void readbuf_destroy(readbuf_t *);

inline size_t readbuf_datalen(readbuf_t *);

inline int readbuf_extend(readbuf_t *);

inline int readbuf_putc(readbuf_t *, char);

#endif //INC_09_C_CODE_LIKE_HUFFMAN_READBUF_H
