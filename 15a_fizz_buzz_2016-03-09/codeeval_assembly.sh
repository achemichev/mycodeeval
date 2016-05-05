#!/bin/bash

ASSEMBLY=codeeval_assembly.txt

cat macrodef.h readbuf.h challenge.h readbuf.c challenge.c main.c > $ASSEMBLY

sed -i -- 's/#define DBGFLAG/\/\/#define DBGFLAG/g' $ASSEMBLY
sed -i -- 's/#include \"/\/\/#include \"/g' $ASSEMBLY
