///
/// Tests for functions in ../src/ballot_box.c.
///

#include "ballot_box.h"
#include "helpers.h"

#include <lib211.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Takes the expected winner, followed by all the votes. Each ballot is
// given as successive string arguments, terminated by "%". The final
// argument, after the final ballot's "%", must be NULL.
static void assert_election(const char* expected_winner, ...);

// Test case functions (you need more!):
static void three_candidates_tied(void),
            win_on_second_round(void),
            example_from_wikipedia(void),
    win_on_third_round(void);
ballot_box_t test_read_file(const char*);

///
/// MAIN FUNCTION
///

int main(void)
{
    three_candidates_tied();
    win_on_second_round();
    example_from_wikipedia();
    win_on_third_round();   
}


///
/// TEST CASE FUNCTIONS
///

static void three_candidates_tied(void)
{
    assert_election("A",
            "a", "b", "c", "%",
            "a", "b", "c", "%",
            NULL);
}

static void win_on_second_round(void)
{
    assert_election("BIDEN",
            "Bloomberg", "Biden", "%",
            "Yang", "Biden", "%",
            "Warren", "Biden", "%",
            "Sanders", "Biden", "%",
            "Biden", "Sanders", "%",
            "Biden", "Gabbard", "%",
            NULL);
}

static void example_from_wikipedia(void)
{
    assert_election("SUE",
            "bob", "bill", "s u e", "%",
            "Sue", "Bob", "Bill", "%",
            "Bill!", "Sue!", "BoB!", "%",
            "bob", "bill", "sue", "%",
            "sue", "bob", "bill", "%",
            NULL);
            assert_election("CAMPBELL",
                    "abbott", "Borden", "Campbell", "%",
                    "campbell", "abbott", "borden", "%",
                    "borden", "campbell", "abbott", "%",
                    "abbott", "Borden", "campbell", "%",
                    "Campbell", "abbotT", "borden", "%",
                    NULL);
    assert_election("B",
                    "a", "b", "c", "%",
                    "a", "b", "c", "%",
                    "b", "c", "%",
            NULL);
}

static void win_on_third_round(void)
{
    assert_election("CAMPBELL",
                    "abbott", "Borden", "Campbell", "%",
                    "campbell", "abbott", "borden", "%",
                    "borden", "campbell", "abbott", "%",
                    "abbott", "Borden", "campbell", "%",
                    "Campbell", "abbotT", "borden", "%",
                    NULL);
    assert_election("B",
                    "a", "b", "c", "%",
                    "a", "b", "c", "%",
                    "b", "c", "%",
            NULL);
//    ballot_box_t test = test_read_file("Resources/ballot_new_test");          
    //  assert_election("CAMPBELL", test);                                      
    // bb_destroy(test);                                                        
}

ballot_box_t test_read_file(const char* file)
{
    FILE * fp = fopen(file,"r");
    ballot_box_t result = read_ballot_box(fp);
    fclose(fp);
    return result;
}


///
/// HELPER FUNCTIONS
///

static void assert_election(const char* expected, ...)
{
    ballot_box_t bb = empty_ballot_box;

    {
        ballot_t ballot = ballot_create();

        va_list ap;
        va_start(ap, expected);

        char* name;
        while ((name = va_arg(ap, char*))) {
            if (strcmp(name, "%") == 0) {
                bb_insert(&bb, ballot);
                ballot = ballot_create();
            } else {
                ballot_insert(ballot, strdupb(name, "assert_election"));
            }
        }

        va_end(ap);

        ballot_destroy(ballot);
    }

    char* actual = get_irv_winner(bb);
    if (expected)
        CHECK_STRING(actual, expected);
    else
        CHECK_POINTER(actual, expected);
    free(actual);

    bb_destroy(bb);
}
