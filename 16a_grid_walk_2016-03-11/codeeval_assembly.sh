#!/bin/bash

ASSEMBLY=codeeval_assembly.c

#cat macrodef.h readbuf.h challenge.h readbuf.c challenge.c main.c > $ASSEMBLY
cat macrodef.h challenge.h challenge.c main.c > $ASSEMBLY

sed -i -- 's/#define DBGFLAG/\/\/#define DBGFLAG/g' $ASSEMBLY
sed -i -- 's/#include \"/\/\/#include \"/g' $ASSEMBLY

gcc -Wall -g -std=c99 $ASSEMBLY -o challenge
