#ifndef INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H
#define INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H

#define DBG

#ifdef DBG
#define DBGPREFIX "  ...debug  "
#define DBGPREFIXL2 "  ...debug      "
#define MACRODBGOUTPUT(fmt, s) printf(DBGPREFIX),   printf(fmt, s), printf("\n")
#define MACRODBGOUTPL2(fmt, s) printf(DBGPREFIXL2), printf(fmt, s), printf("\n")
#else
#define DBGPREFIX
#define DBGPREFIXL2
#define MACRODBGOUTPUT(fmt, s)
#define MACRODBGOUTPL2(fmt, s)
#endif

#define SUCCESSEXITCODE 0
#define ERREXITCODE 1

#endif //INC_09_C_CODE_LIKE_HUFFMAN_MACRODEF_H
