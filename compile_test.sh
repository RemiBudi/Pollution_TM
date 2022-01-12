#!/bin/bash

gcc -Wall -c controlleur.c -lcunit


gcc -Wall -L/home/paul/Documents/tests_sim/CUnit-2.1-3/CUnit/Sources/.libs -I/home/paul/Documents/tests_sim/CUnit-2.1-3/CUnit/Headers -o test_controlleur test_controlleur.c controlleur.o -lcunit


