#include <HsFFI.h>
#ifdef __GLASGOW_HASKELL__
#include "solver_stub.h"
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *query;
    char *expected;
} test;

int doTest(test t) {
    char *ans = rpn(t.query);
    int result = strcmp(ans, t.expected);
    fprintf(stderr, "\tResult: '%s'\n", ans);
    free(ans);

    return result;
}

#define testSize 15
test allTests[] = {
    {"", "Empty"},
    {"20 20 +", "40.0"},
    {"50 30 -", "20.0"},
    {"50 20 /", "2.5"},
    {"5 5 + 2 2 + *", "40.0"},
    {"10 2 ^", "100.0"},
    {"100 10 * log", "3.0"},
    {"2.71 2.71 2.71 * * ln", "2.990846"},
    {"10 5 + 6 * 2 * 3 / 45 + 5 - log", "2.0"},
    {"           ", "Empty"},
    {"10 10 * 0 /", "Division by 0"},
    {"10 -1.5 * ln", "Invalid Logarithm (ln)"},
    {"25 -12 10 + * log", "Invalid Logarithm (log)"},
    {" +  +  + ", "Invalid Symbol: \"+\""},
    {" 10 10 * /", "Invalid Symbol: \"/\""}
};

int main(int argc, char *argv[]) {
    hs_init(&argc, &argv);
    
    for(int i = 0; i < testSize; i++){
        fprintf(stderr, "Test %d:\n\tQuery : '%s'\n\tExpect: '%s'\n", i+1, allTests[i].query, allTests[i].expected);
        if(doTest(allTests[i]) != 0){
            fprintf(stderr, "Test failed\n");
            return 1;
        }
    }

    hs_exit();
    return 0;
}