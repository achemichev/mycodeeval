#ifndef INC_09_C_CODE_LIKE_HUFFMAN_HUFFMAN_H
#define INC_09_C_CODE_LIKE_HUFFMAN_HUFFMAN_H

////////////////////////////////////////////////////////////////////////////////

#define CHAROFFSET 'a'
#define MAXALPHABETSIZE 'z' - CHAROFFSET + 1

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////

typedef struct node_struct node_t;

////////////////////////////////////////////////////////////////////////////////

struct node_struct {
    char *mp_chars;
    unsigned char mn_prio;
    bool mb_has_parent;
    unsigned char mn_code;
    node_t *mp_left_child;
    node_t *mp_right_child;
};

////////////////////////////////////////////////////////////////////////////////

inline void huffman_entry(char *pp_line);

////////////////////////////////////////////////////////////////////////////////

#endif //INC_09_C_CODE_LIKE_HUFFMAN_HUFFMAN_H
