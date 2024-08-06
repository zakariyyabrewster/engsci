#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
    printf("Lowest match: %d\n", lowest_match(terms, nterms, "New"));
    printf("Highest match: %d\n", highest_match(terms, nterms, "New"));
    
    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "New");
    for (int i = 0; i < n_answer; i++)
    {
        printf("%s\n", (answer + i)->term);
    }
    //free allocated blocks here -- not required for the project, but good practice
    free(terms);
    terms = NULL; // set pointer to NULL after freeing
    free(answer);
    answer = NULL; // set pointer to NULL after freeing
    if (terms != NULL)
    {
        printf("Memory not freed\n");
    }
    if (answer != NULL)
    {
        printf("Memory not freed\n");
    }
    return 0;
}
