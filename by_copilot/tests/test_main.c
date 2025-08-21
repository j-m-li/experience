#include <stdio.h>
#include "../src/utils.h"

int main(void) {
    int result = add(2, 3);
    if (result == 5) {
        printf("Test passed!\n");
        return 0;
    } else {
        printf("Test failed!\n");
        return 1;
    }
}