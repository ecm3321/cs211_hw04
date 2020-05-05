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
    //
     size_t i= 0;

    ballot_t result= malloc(sizeof(struct ballot));
    if (!result){
        exit(2);
    }
    result->length=0;

    return result;
    // //

}


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
        ballot->entries[len].active = true;
        ballot->length+=1;
        ballot->entries[len].name=name;

    } else {
        exit(3);
    }

}




const char* ballot_leader(ballot_t ballot)
{
    size_t i = 0;
    size_t len = ballot->length;
    while(i < len){
        if(ballot->entries[i].active){
            return ballot->entries[i].name;
        }
        i = i + 1;
    }
    return NULL;
}



void ballot_eliminate(ballot_t ballot, const char* name)
{
    size_t i = 0;
    size_t len = ballot->length;
    while(i < len && ballot->entries[i].name != NULL && name!= NULL){
        if(strcmp(name, ballot->entries[i].name)== 0){
                ballot->entries[i].active = false;
        }
        i = i + 1;
    }
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    size_t i = 0;
    size_t len = ballot->length;
    while(i < len){
        if(ballot->entries[i].active){
            size_t* first = vc_update(vc, ballot->entries[i].name);
            if (first == NULL){
                exit(4);
            }
            *first = *first + 1;
        }
        i = i + 1;
    }
}

ballot_t read_ballot(FILE* inf)
{
   ballot_t bal = ballot_create();
    char* line;
    while ((line = fread_line(inf)) != NULL){
        if(*line == '%'){
            free(line);
        } else {
            clean_name(line);
            ballot_insert(bal, line);
            free(line);
        }
    }
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
