#pragma once
#include <stdbool.h>

typedef struct {
    char surname[30];
    char initials[3];
    char gender;
    int school_num;
    bool have_medal;
    int math;
    int russian;
    int informatics;
    bool essay;
} Entrant;

