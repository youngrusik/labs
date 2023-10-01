#include "machine_word.h"

void readMachWord(machineWord* data) {
    scanf("%llu", data);
}

void writeMachWord(machineWord* data) {
    printf("%llu\t", *data);
}