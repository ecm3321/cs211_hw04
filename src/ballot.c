#include "ballot.h"
#include "helpers.h"

#include <lib211.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A `ballot_t` (defined in `ballot.h`) is be a pointer to a
// heap-allocated `struct ballot`, with the following invariant:
//
//  - `length <= MAX_CANDIDATES`
//
//  - the first `length` elements of `entries` are initialized
//
//  - the `name` field of each of the first `length` entries is non-NULL
//    and points to an OWNED, heap-allocated string containing only
//    letters, all of which are uppercase (a la isupper(3)).
//
// The remaining elements of `entries` (`MAX_CANDIDATES - length`)
// should be considered uninitialized.

struct entry
{
    char* name;
    bool active;
};

struct ballot
{
    size_t length;
    struct entry entries[MAX_CANDIDATES];
};


ballot_t ballot_create(void)
{
    ballot_t result = malloc(sizeof(struct ballot));
    if (!result) exit(2);
    return result;
}

//probably not correct idk why i decided to free name and nothing else...but its closer?
void ballot_destroy(ballot_t ballot)
{
    if(ballot == NULL)
    {
        free(ballot);
    } else {
        size_t i=0;
        while (i < ballot->length) {
            free(ballot->entries[i].name);
        i = i + 1;
        }
        free(ballot);
    }

    //need to free individual names
}


void ballot_insert(ballot_t ballot, char* name)
{
    size_t len= ballot->length;
    if (len < MAX_CANDIDATES)
    {
        clean_name(name);

        ballot->entries[len].name=name;

    } else {
        exit(3);
    }
}



//probs won't run
const char* ballot_leader(ballot_t ballot)
{
    size_t i=0;
    while(i< ballot->length)
    {
        if(ballot->entries[i].active)
        {
        }
    }
    return NULL;
}


void ballot_eliminate(ballot_t ballot, const char* name)
{
    //
    // TODO: your code here
    //
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    //
    // TODO: your code here
    //
}

ballot_t read_ballot(FILE* inf)
{
    //
    // TODO: replace with your code:
    //
    return NULL;
}

void clean_name(char* name)
{
    size_t src = 0;
    size_t dst = 0;
    while (name[src] != 0){
        if (isalph(name[src]) != 0){
            name[dst] = toupper(name[src]);
            dst = dst + 1;
            src = src + 1;
        } else
            src = src + 1;
        }
    name[dst] = 0;
}

void print_ballot(FILE* outf, ballot_t ballot)
{
    for (size_t i = 0; i < ballot->length; ++i) {
        bool active = ballot->entries[i].active;
        fprintf(outf, "%c%s%s\n",
                active? ' ' : '[',
                ballot->entries[i].name,
                active? "" : "]");
    }
}
