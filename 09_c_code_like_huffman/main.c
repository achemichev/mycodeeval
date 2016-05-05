#include <stdio.h>
#include <stdlib.h>
#include "macrodef.h"
#include "readbuf.h"
#include "huffman.h"

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
            huffman_entry(lo_readbuf.mp_buf);
            readbuf_reset(&lo_readbuf);
        }
        else {
            if (readbuf_putc(&lo_readbuf, (char) ln_char) == ERREXITCODE) {
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