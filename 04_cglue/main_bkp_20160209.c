#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DBG

#ifdef DBG
#define MACRODBGOUTPUT(fmt, s) printf("  ...debug  "),     printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf("  ...debug      "), printf(fmt, s), printf("\n")
#else
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

#define MAXLINELENGTH 32767
#define DELIMITERSTR "|"
#define DELIMITERCHAR '|'
#define HASHBUCKETS 127

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// globs

unsigned short N = 0;
unsigned short n = 0;
unsigned short t = 0;
int hot[1024];
unsigned short global_prev_len = 0;
char **P = NULL;
char *S = NULL;
struct node *H[HASHBUCKETS];
struct node *H2[HASHBUCKETS];
struct node *R[HASHBUCKETS];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct node {
    char *_p;
    struct node *_next;
};

void init(struct node *pnode) {
    pnode->_p = NULL;
    pnode->_next = NULL;
}

struct node *push(struct node *pnode, char *piece) {
    struct node *p;
    p = (struct node *) malloc(sizeof(struct node));
    init(p);
    p->_p = piece;
    if (pnode != NULL)
        p->_next = pnode;
    return p;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline char *truncate_input(char *d) {
    size_t len;
    char *c;
    //MACRODBGOUTPUT("%s", "truncate_input func call:");
    if (*d == DELIMITERCHAR) d++;
    len = strlen(d);
    for (c = &d[len - 1]; *c != DELIMITERCHAR; c--)
        *c = '\0';
    if (*c == DELIMITERCHAR) *c = '\0';
    MACRODBGOUTPUT("input string shifted and truncated, d = \"%s\"", d);
    //MACRODBGOUTPUT("%s", "truncate_input func call finished...\n");
    return d;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline unsigned short count_pieces(char *d) {
    unsigned short k;
    char *c;
    //MACRODBGOUTPUT("%s", "count_pieces func call:");
    for (c = d, k = 0; *c; c++)
        if (*c == DELIMITERCHAR)
            k++;
    MACRODBGOUTPUT("number of pieces, N = %d", k + 1);
    //MACRODBGOUTPUT("%s", "count_pieces func call finished...\n");
    return (unsigned short) (k + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void alloc_pieces_pointers() {
    size_t mem2alloc;
    //MACRODBGOUTPUT("%s", "alloc_pieces_pointers func call:");
    mem2alloc = (size_t) (N * sizeof(char *));
    if (P == NULL) P = (char **) malloc(mem2alloc);
    else P = (char **) realloc(P, mem2alloc);
    MACRODBGOUTPUT("pieces pointer, P = 0x%x", (unsigned int) P);
    MACRODBGOUTPUT("memory allocated for P (bytes), mem2alloc = %d", mem2alloc);
    //MACRODBGOUTPUT("%s", "alloc_pieces_pointers func call finished...\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void set_pieces_pointers(char *d) {
    unsigned short i;
    char *p;
    //MACRODBGOUTPUT("%s", "pieces, P:");
    for (i = 0, p = strtok(d, DELIMITERSTR); p != NULL; p = strtok(NULL, DELIMITERSTR)) {
        P[i] = p;
        // MACRODBGOUTPL2("\"%s\"", P[i]);
        i++;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline size_t count_piece_length() {
    MACRODBGOUTPUT("piece length, n = %d", strlen(P[0]));
    return strlen(P[0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline size_t count_solution_text_length() {
    MACRODBGOUTPUT("solution text length, t = %d", N + n - 1);
    return (size_t) (N + n - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void alloc_solution_text_string() {
    //MACRODBGOUTPUT("%s", "alloc_solution_text_string func call:");
    if (S == NULL) S = (char *) malloc((size_t) (t + 1));
    else S = (char *) realloc(S, (size_t) (t + 1));
    S[0] = '\0';
    MACRODBGOUTPUT("solution text pointer, S = 0x%x", (unsigned int) S);
    MACRODBGOUTPUT("memory allocated for S (bytes), mem2alloc = %d", (t + 1));
    //MACRODBGOUTPUT("%s", "alloc_solution_text_string func call finished...\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned short hash_func(char *c) {
    unsigned short i, s;
    for (i = 0, s = 0; i < n - 1; i++)
        s += *(c + i);
    return (unsigned short) (s % HASHBUCKETS);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned short right_hash_func(char *c) {
    unsigned short i, s;
    for (i = 0, s = 0; i < n - 1; i++)
        s += *(c + i + 1);
    return (unsigned short) (s % HASHBUCKETS);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void free_hashes() {
    unsigned short h;
    struct node *pnode, *p;
    //MACRODBGOUTPUT("%s", "free_hashes func call:");
    for (h = 0; h < HASHBUCKETS; h++) {
        if ((pnode = H[h]) != NULL)
            while (pnode != NULL) {
                p = pnode;
                pnode = pnode->_next;
                free(p);
            }

        if ((pnode = H2[h]) != NULL)
            while (pnode != NULL) {
                p = pnode;
                pnode = pnode->_next;
                free(p);
            }

        if ((pnode = R[h]) != NULL)
            while (pnode != NULL) {
                p = pnode;
                pnode = pnode->_next;
                free(p);
            }
        H[h] = NULL;
        H2[h] = NULL;
        R[h] = NULL;
    }
    //MACRODBGOUTPUT("%s", "free_hashes func call finished...\n");
} // free_hashes()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void build_simple_hashes() {
    unsigned short i, h, r;
    struct node *pnode;

    //MACRODBGOUTPUT("%s", "build_simple_hash func call:");

    for (i = 0; i < N; i++) {
        h = hash_func(P[i]);
        r = right_hash_func(P[i]);
        H[h] = push(H[h], P[i]);
        H2[h] = push(H2[h], P[i]);
        R[r] = push(R[r], P[i]);
    }

    //unsigned short hot[1024];
    for (i = 0; i < 1024; i++)
        hot[i] = 0;


#ifdef DBG

    MACRODBGOUTPUT("%s", "hashes, H:");
    for (h = 0; h < HASHBUCKETS; h++)
        if (H[h] != NULL) {
            MACRODBGOUTPUT("hash, h = %d, list:", h);
            for (pnode = H[h]; pnode != NULL; pnode = pnode->_next)
                MACRODBGOUTPL2("pnode->_p = %s", pnode->_p);
        };

    MACRODBGOUTPUT("%s", "hashes, R:");
    for (r = 0; r < HASHBUCKETS; r++)
        if (R[r] != NULL) {
            MACRODBGOUTPUT("hash, r = %d, list:", r);
            for (pnode = R[r]; pnode != NULL; pnode = pnode->_next)
                MACRODBGOUTPL2("pnode->_p = %s", pnode->_p);
        };
#endif

    //MACRODBGOUTPUT("%s", "build_simple_hash func call finished...\n");

} // build_simple_hashes()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline char *detect_start_left_piece() {
    unsigned short h;
    struct node *pnode, *pnode_r;
    char *start_left_piece;
    //MACRODBGOUTPUT("%s", "detect_start_left_piece func call:");
    for (h = 0; h < HASHBUCKETS; h++)
        if (H[h] != NULL)
            for (pnode = H[h]; pnode != NULL; pnode = pnode->_next) {
                start_left_piece = pnode->_p;
                if (R[h] != NULL)
                    for (pnode_r = R[h]; pnode_r != NULL; pnode_r = pnode_r->_next)
                        if (pnode_r->_p != NULL && !strncmp(start_left_piece, pnode_r->_p + 1, (size_t) (n - 1))) {
                            start_left_piece = NULL;
                            pnode_r->_p = NULL; // exclude
                            break;
                        }
                if (start_left_piece != NULL) {
                    MACRODBGOUTPUT("start left piece found (use only first n-1 chars), start_left_piece = %s",
                                   start_left_piece);
                    return start_left_piece;
                }
            } // for pnode
    return NULL;
    //MACRODBGOUTPUT("%s", "detect_start_left_piece func call finished...\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void glue_recur(char *prev, char *new2set) {
    unsigned short h, bucket_len, hcounts[HASHBUCKETS], h2, iii;
    struct node *pnode, *pnode_exlude, *pnode2, *pnode3;
    char *key, *p_exclude, *new_, *best, *prev_plus_char;
    size_t prev_len, best_len, new_len;

    // MACRODBGOUTPUT("%s", "glue_recur func call:");

    prev_len = strlen(prev);

    hot[prev_len]++;
    if ( hot[prev_len] >= 100 )
        iii = 0;

    if (prev_len > global_prev_len)
        global_prev_len = prev_len;

    if (prev_len == t) {
        MACRODBGOUTPUT("previous solution of size t found, prev = %s", prev);
        strcpy(new2set, prev);
        return;
    }

    if (prev_len == 417)
        prev_len = 417;

    new_ = (char *) malloc((size_t) (t + 1));
    new_[0] = '\0';
    new_len = 0;
    best = (char *) malloc((size_t) (t + 1));
    best[0] = '\0';
    best_len = 0;
    prev_plus_char = (char *) malloc((size_t) (prev_len + 2));

    key = prev + prev_len - n + 1;
    //MACRODBGOUTPUT("key used, key = %s", key);

    bucket_len = 0;
    h = hash_func(key);

    if (H[h] != NULL)
        for (pnode = H[h]; pnode != NULL; pnode = pnode->_next) {
            if (pnode->_p != NULL && !strncmp(key, pnode->_p, (size_t) (n - 1))) {
                /*
                bucket_len++;
                if (bucket_len == 2) {
                    bucket_len = 2;
                    for (h2 = 0; h2 < HASHBUCKETS; h2++) {
                        hcounts[h2] = 0;
                        if ((pnode2 = H[h2]) != NULL)
                            while (pnode2 != NULL) {
                                hcounts[h2]++;
                                pnode2 = pnode2->_next;
                            }
                    }
                    for (h2 = 0; h2 < HASHBUCKETS; h2++)
                        if (H2[h2] != NULL) {
                            for (pnode2 = H2[h2], pnode3 = H[h2]; pnode2 != NULL; pnode2 = pnode2->_next,pnode3 = pnode3->_next)
                                    if(pnode3->_p && pnode2->_p != pnode3->_p)
                                        iii = 0;
                        };

                }
                */
                if (prev_len == 361)
                    prev_len = 361;
                strcpy(prev_plus_char, prev);
                prev_plus_char[prev_len] = pnode->_p[strlen(pnode->_p) - 1];
                prev_plus_char[prev_len + 1] = '\0';
                // exclude:
                p_exclude = pnode->_p;
                pnode_exlude = pnode;
                pnode->_p = NULL;
                // recur:
                if (prev_len == 361)
                    prev_len = 361;
                glue_recur(prev_plus_char, new_);
                // return back excluded:
                if (pnode_exlude->_p != NULL)
                    iii = 3;
                pnode_exlude->_p = p_exclude;
                // choose longest solution:
                if (new_[0] == '\0')
                    new_[0] = '\0';
                new_len = strlen(new_);
                if (new_len > best_len) {
                    strcpy(best, new_);
                    best_len = new_len;
                }
                if (best_len == t)
                    break;
            }
        } // for


    if (best_len == 0)
        strcpy(new2set, prev);
    else
        strcpy(new2set, best);

    free(new_);
    free(best);
    free(prev_plus_char);

    // MACRODBGOUTPUT("%s", "glue_recur func call finished...\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void glue(char *d) {
    //MACRODBGOUTPUT("%s", "glue func call:");
    char *start_left, *start_left_p;
    d = truncate_input(d);
    N = count_pieces(d);
    alloc_pieces_pointers();
    set_pieces_pointers(d);
    n = (unsigned short) count_piece_length();
    t = (unsigned short) count_solution_text_length();
    alloc_solution_text_string();
    free_hashes();
    build_simple_hashes();

    global_prev_len = 0;
    start_left_p = detect_start_left_piece();
    if (start_left_p != NULL) {
        start_left = (char *) malloc((size_t) n);
        strncpy(start_left, start_left_p, (size_t) (n - 1));
        start_left[n - 1] = '\0';
        glue_recur(start_left, S);
        free(start_left);
        printf("%s\n", S); // release output
    }
    else
            MACRODBGOUTPUT("%s", "start_left not detected, key ring possible...");

    MACRODBGOUTPUT("%s", "\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[MAXLINELENGTH];
    while (fgets(line, MAXLINELENGTH, file))
        glue(line);
    if (P != NULL)free(P);
    if (S != NULL)free(S);
    free_hashes();
    return 0;
}