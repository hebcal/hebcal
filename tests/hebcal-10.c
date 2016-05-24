/* vi: set sw=2 ai sm et: */
/* Tests for city.c - Written by Ambrose Li */

/*
   TODO: POSSIBLE EDGE CASES THAT MIGHT NEED TO BE TESTED:
   - Two strings completely identical except there are trailing blanks in one
   - Two cities that have almost the same name but one has an apostrophe at the end
 */

#include <stdio.h>
#include <stdlib.h>

#include "city.h"

int result_code = 0;

void test_compare_city( const char *a, const char *b, int expected ) {
    int actual = compare_city(a, b);
    printf("%s%s%s vs %s%s%s, expect %d, got %d: ",
            (a? "\"": ""), a, (a? "\"": ""),
            (b? "\"": ""), b, (b? "\"": ""),
            expected, actual);

    if (actual == expected) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        result_code = 1;
    }
}

void main() {
    /* Invalid strings */
    test_compare_city(NULL, NULL, 0);
    test_compare_city(NULL, "", 0);
    test_compare_city(NULL, "a", -1);
    test_compare_city("", NULL, 0);
    test_compare_city("a", NULL, 1);

    /* Minimal strings */
    test_compare_city("", "", 0);
    test_compare_city("a", "", 1);
    test_compare_city("", "a", -1);
    test_compare_city("a", "a", 0);
    test_compare_city("a", "aa", -1);
    test_compare_city("aa", "a", 1);
    test_compare_city("aa", "ab", -1);
    test_compare_city("ab", "aa", 1);
    test_compare_city("a", "a'", -1);

    /* Let's try some real cities */
    test_compare_city("Toronto", "Toronto", 0);
    test_compare_city("Toronto", "TORONTO", 0);
    test_compare_city("Toronto", "Markham", 1);
    test_compare_city("Toronto", "Vaughan", -1);
    test_compare_city("Kitchener", "Kitchener Waterloo", -1);
    test_compare_city("Kitchener-Waterloo", "Kitchener", 1);
    test_compare_city("Kitchener-Waterloo", "Kitchener Waterloo", 0);
    test_compare_city("Saint John", "Saint John's", -1);

    /* Bug 30 - This must pass because this is why the comparator was written in the first place */
    test_compare_city("Winston-Salem NC", "Winston Salem NC", 0);
    exit(result_code);
}
