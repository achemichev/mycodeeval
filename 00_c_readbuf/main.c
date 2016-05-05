#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG

#ifdef DBG
#define MACRODBGOUTPUT(fmt, s) printf("  ...debug  "),     printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf("  ...debug      "), printf(fmt, s), printf("\n")
#else
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

#define SUCCESSEXITCODE 0
#define ERREXITCODE 1

////////////////////////////////////////////////////////////////////////////////
// readbuf.h

typedef struct readbuf_struct {
    char *mp_buf;
    char *mp_cur;
    size_t mn_size;
} readbuf_t;

int readbuf_init(readbuf_t *);

void readbuf_reset(readbuf_t *);

void readbuf_destroy(readbuf_t *);

size_t readbuf_datalen(readbuf_t *);

int readbuf_extend(readbuf_t *);

int readbuf_putc(readbuf_t *, char);

// readbuf.h
////////////////////////////////////////////////////////////////////////////////
// readbuf.c

#define INITSIZE 16
#define EXTENTSIZE 4

int readbuf_init(readbuf_t *pp_readbuf) {
    pp_readbuf->mn_size = INITSIZE;
    pp_readbuf->mp_buf = malloc((size_t) pp_readbuf->mn_size * sizeof(char));
    if (!pp_readbuf->mp_buf) return ERREXITCODE;
    pp_readbuf->mp_cur = pp_readbuf->mp_buf;
    *(pp_readbuf->mp_cur) = '\0';
    return SUCCESSEXITCODE;
}

void readbuf_reset(readbuf_t *pp_readbuf) {
    pp_readbuf->mp_cur = pp_readbuf->mp_buf;
}

void readbuf_destroy(readbuf_t *pp_readbuf) {
    free(pp_readbuf->mp_buf);
    pp_readbuf->mp_buf = pp_readbuf->mp_cur = NULL;
    pp_readbuf->mn_size = 0;
}

size_t readbuf_datalen(readbuf_t *pp_readbuf) {
    return pp_readbuf->mp_cur - pp_readbuf->mp_buf;
}

int readbuf_extend(readbuf_t *pp_readbuf) {
    size_t ln_datalen;
    char *lp_newbuf;

    pp_readbuf->mn_size += EXTENTSIZE;
    lp_newbuf = malloc((size_t) pp_readbuf->mn_size * sizeof(char));
    if (!lp_newbuf) return ERREXITCODE;
    strcpy(lp_newbuf, pp_readbuf->mp_buf);
    ln_datalen = readbuf_datalen(pp_readbuf);
    free(pp_readbuf->mp_buf);
    pp_readbuf->mp_buf = lp_newbuf;
    pp_readbuf->mp_cur = pp_readbuf->mp_buf + ln_datalen;

    return SUCCESSEXITCODE;
}

int readbuf_putc(readbuf_t *pp_readbuf, char pn_char) {
    if (readbuf_datalen(pp_readbuf) + 1 == pp_readbuf->mn_size) {
        if (readbuf_extend(pp_readbuf) == ERREXITCODE) return ERREXITCODE;
    }
    *(pp_readbuf->mp_cur++) = pn_char;
    *(pp_readbuf->mp_cur) = '\0';
    return SUCCESSEXITCODE;
}

// readbuf.c
////////////////////////////////////////////////////////////////////////////////

void process_line(char *pp_line) {
    MACRODBGOUTPL2("*pp_line =  %s", pp_line);
}

////////////////////////////////////////////////////////////////////////////////

int main(int pn_argc, const char *pp_argv[]) {

    FILE *lp_file;
    int ln_char;
    readbuf_t lo_readbuf;

    if (readbuf_init(&lo_readbuf) == ERREXITCODE) return ERREXITCODE;
    lp_file = fopen(pp_argv[1], "r");
    if (lp_file == NULL) return ERREXITCODE;

    do {
        ln_char = fgetc(lp_file);
        if (feof(lp_file) || ln_char == '\n') {
            process_line(lo_readbuf.mp_buf);
            readbuf_reset(&lo_readbuf);
        }
        else {
            if (readbuf_putc(&lo_readbuf, ln_char) == ERREXITCODE) {
                readbuf_destroy(&lo_readbuf);
                fclose(lp_file);
                return ERREXITCODE;
            }
        }
        if (feof(lp_file)) break;
    } while (1);

    readbuf_destroy(&lo_readbuf);
    fclose(lp_file);
    return SUCCESSEXITCODE;
} // main()

////////////////////////////////////////////////////////////////////////////////