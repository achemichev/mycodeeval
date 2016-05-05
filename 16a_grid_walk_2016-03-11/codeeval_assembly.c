////////////////////////////////////////////////////////////////////////////////
// macrodef.h

#ifndef INC_MACRODEF_H
#define INC_MACRODEF_H

////////////////////////////////////////////////////////////////////////////////

//#define DBGFLAG

////////////////////////////////////////////////////////////////////////////////

#ifdef DBGFLAG

#include <stdio.h>

#define DBGPREFIX "  ...debug  "
#define DBGPREFIXL2 "  ...debug      "
#define MACRODBGOUTPUT(fmt, s) printf(DBGPREFIX),   printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf(DBGPREFIXL2), printf(fmt, s), printf("\n")

#else //DBGFLAG

#define DBGPREFIX
#define DBGPREFIXL2
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)

#endif //DBGFLAG

////////////////////////////////////////////////////////////////////////////////

#define SUCCESSEXITCODE 0
#define ERREXITCODE 1

////////////////////////////////////////////////////////////////////////////////

#endif //INC_MACRODEF_H

// macrodef.h
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// challenge.h

#ifndef INC_CHALLENGE_H
#define INC_CHALLENGE_H

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    false, true
} bool;

////////////////////////////////////////////////////////////////////////////////

int challenge_main();

////////////////////////////////////////////////////////////////////////////////

#endif //INC_CHALLENGE_H

// challenge.h
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// challenge.c

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h> // for release output
//#include "macrodef.h"
//#include "challenge.h"

////////////////////////////////////////////////////////////////////////////////

#define N 512 
#define NINIT 298
#define DSUMMAX 19
#define ITERLIMIT 100
#define BITMAPSIZE N * N / CHAR_BIT

////////////////////////////////////////////////////////////////////////////////

unsigned long int dsum(unsigned long int pn_x, unsigned long int pn_y){
    unsigned long int ln_dsum;

    if(pn_x < 0) pn_x = 0 - pn_x;
    if(pn_y < 0) pn_y = 0 - pn_y;

    ln_dsum = 0;
    while(pn_x) { ln_dsum += pn_x % 10; pn_x /= 10; }
    while(pn_y) { ln_dsum += pn_y % 10; pn_y /= 10; }

    return ln_dsum;

} // dsum()

////////////////////////////////////////////////////////////////////////////////

void set_bit( unsigned long int pn_x,
              unsigned long int pn_y,
              unsigned char * pp_bitmap) {
    unsigned long int ln_byte = (pn_x * N + pn_y)  / CHAR_BIT;
    unsigned long int ln_bit_shift = (pn_x * N + pn_y) % CHAR_BIT;
    pp_bitmap[ln_byte] |= (1 << ln_bit_shift);

} // set_bit()

////////////////////////////////////////////////////////////////////////////////


bool get_bit( unsigned long int pn_x,
              unsigned long int pn_y,
              unsigned char * pp_bitmap) {

    unsigned long int ln_byte = (pn_x * N + pn_y)  / CHAR_BIT;
    unsigned long int ln_bit_shift = (pn_x * N + pn_y) % CHAR_BIT;
    return (bool)(pp_bitmap[ln_byte] & (1 << ln_bit_shift));

} // get_bit()

////////////////////////////////////////////////////////////////////////////////

void init_bitmap(unsigned char * pp_bitmap){
    unsigned long int i, j;

    #ifdef DBGFLAG
    unsigned long int ln_count_bits, ln_count_bits_border;
    ln_count_bits = 0;
    ln_count_bits_border = 0;
    #endif
    for (i = 0; i < NINIT; i++)
        for (j = 0; j < NINIT; j++){
            if (dsum(i, j) > DSUMMAX){
                #ifdef DBGFLAG
                ln_count_bits_border += j;
                #endif
                break;
            }
            else {
                set_bit(i, j, pp_bitmap);
                #ifdef DBGFLAG
                ln_count_bits++;
                #endif
            }
            #ifdef DBGFLAG
            if ( j == (NINIT - 1)) ln_count_bits_border += NINIT;
            #endif
        }

    MACRODBGOUTPUT("init_bitmap() ln_count_bits = %d", (int)ln_count_bits);
    MACRODBGOUTPUT("init_bitmap() ln_count_bits_border = %d", (int)ln_count_bits_border);

} // init_bitmap()

////////////////////////////////////////////////////////////////////////////////

unsigned long int count_bits( unsigned char * pp_bitmap,
                              unsigned long int pn_bitmap_size){

    unsigned long int ln_count, i;

    static const unsigned char la_bits_set_table256[256] = {
        #define B2(n) n,     n+1,     n+1,     n+2
        #define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
        #define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
        B6(0), B6(1), B6(1), B6(2)
    };

    ln_count = 0;
    for(i = 0; i < pn_bitmap_size; i++)
        ln_count += la_bits_set_table256[pp_bitmap[i]];
    
    return ln_count;

} // count_bits()

////////////////////////////////////////////////////////////////////////////////

bool filling_iteration_changed( unsigned char * pp_bitmap,
                                unsigned long int pn_bitmap_size){

    bool lb_changed;
    unsigned long int i, j;

    lb_changed = false;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (dsum(i, j) > DSUMMAX || get_bit(i, j, pp_bitmap) ) continue;
            else if (
                        (i > 0     && get_bit(i - 1, j, pp_bitmap) ) ||
                        (i < N - 1 && get_bit(i + 1, j, pp_bitmap) ) ||
                        (j > 0     && get_bit(i, j - 1, pp_bitmap) ) ||
                        (j < N - 1 && get_bit(i, j + 1, pp_bitmap) )
                    ) {    
                set_bit(i, j, pp_bitmap);
                if ( !lb_changed ) lb_changed |= true;
            }

    return lb_changed;

} // filling_iteration_changed()

////////////////////////////////////////////////////////////////////////////////

int challenge_main() {
    unsigned char la_bitmap[BITMAPSIZE];
    unsigned long int ln_count, ln_iter_count;

    #ifdef DBGFLAG
    unsigned long int i, j;
    #endif

    MACRODBGOUTPUT("%s", "challenge_main() ...");

    memset((void*)la_bitmap, 0, BITMAPSIZE);

    init_bitmap(la_bitmap);

    #ifdef DBGFLAG
    ln_count = count_bits(la_bitmap, BITMAPSIZE);
    #endif

    MACRODBGOUTPUT("ln_count = %d", (int)ln_count);

    for (ln_iter_count = 0; ln_iter_count < ITERLIMIT; ln_iter_count++)
        if (!filling_iteration_changed(la_bitmap, BITMAPSIZE)) break;

    MACRODBGOUTPUT("ln_iter_count = %d", (int)ln_iter_count);

    #ifdef DBGFLAG
    if (ln_iter_count == ITERLIMIT) MACRODBGOUTPUT("%s", "iteration limit reached ...");
    #endif

    ln_count = count_bits(la_bitmap, (int)BITMAPSIZE);

    MACRODBGOUTPUT("after filling iterations, ln_count = %d", (int)ln_count );

    // release output:
    printf( "%d\n", (int)(4 * (ln_count - NINIT - 1) + 1) );

    #ifdef DBGFLAG
    ln_count = 0;
    for (i = 0; i < N; i++)
        if ( get_bit(i, 0, la_bitmap) )
            ln_count++;
    MACRODBGOUTPUT("j = 0 bits, ln_count = %d", (int)ln_count );

    ln_count = 0;
    for (j = 0; j < N; j++)
        if ( get_bit(0, j, la_bitmap) )
            ln_count++;
    MACRODBGOUTPUT("i = 0 bits, ln_count = %d", (int)ln_count );
    #endif

    return SUCCESSEXITCODE;

} // challenge_main()

// challenge.c
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// main.c

//#include "macrodef.h"
//#include "challenge.h"

////////////////////////////////////////////////////////////////////////////////

int main(void) {
    MACRODBGOUTPUT("%s", "grid walk main() ...");
    return challenge_main();
} // main()

// main.c
////////////////////////////////////////////////////////////////////////////////
