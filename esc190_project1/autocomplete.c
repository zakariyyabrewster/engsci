#include <stdio.h>
#include "autocomplete.h"
#include <stdlib.h>
#include <string.h>


void read_in_terms(term **terms, int *pnterms, char *filename)
{
    FILE *term_file = fopen(filename, "r"); // open file for reading
    if (term_file == NULL) // check if file was opened successfully
    {
        printf("Error: File not found\n"); // print error message
        exit(1); // exit program
    }
    char line[200]; 
    int nterms = 0; 
    
    int c = 0;
    fgets(line, sizeof(line), term_file); // grab the first line of the file
    while (line[c] >= '0' && line[c] <= '9'){ // while the character is a digit
        nterms = nterms * 10 + (line[c] - '0'); // convert the character to an integer and add it to nterms
        c++; // move to the next character
    }

    *pnterms = nterms; // set the value of pnterms to nterms

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    *terms = (term *)malloc(nterms * sizeof(term)); // allocate memory for nterms terms

    int term_i = 0;
    while(fgets(line, sizeof(line), term_file)) // while there are lines to read
    {
        int i = 0;
        double term_weight = 0.0;
        while (line[i] != '\n' && line[i] != '\0') // while the character is not a newline or null character
        {
            if (line[i] == '\t' || line [i] == ' ') // if the character is a tab or space
            {
                i++;
                continue;
            }

            while(line[i] >= '0' && line[i] <= '9')
            {
                term_weight = term_weight * 10 + (line[i] - '0'); // convert the character to an integer and add it to term_weight
                i++;
            }
            
            if (line[i] == '\t' || line [i] == ' ') // if the character is a tab or space
            {
                i++;
                continue;
            }
            int j = 0;
            while (line[i] != '\t' && line[i] != '\n' && line[i] != '\0')
            {
                (*terms + term_i)->term[j] = line[i]; // set the value of the term at index term_i to the character
                i++;
                j++; 
            }
            (*terms + term_i)->term[j] = '\0'; // set the last character of the term to the null character
            (*terms + term_i)->weight = term_weight; // set the value of the weight at index term_i to term_weight
        }
        term_i++; // move to the next term
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int cmpfcn_alpha(const void *a, const void *b) // compare function
    {
        term *term_a = (term *)a;
        term *term_b = (term *)b;
        return strcmp(term_a->term, term_b->term); // compare the terms
    }

    qsort(*terms, nterms, sizeof(term), cmpfcn_alpha); // sort the terms

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fclose(term_file); // close the file

}

int lowest_match (term *terms, int nterms, char *substr)
{
    // implement binary search to find first element that matched the substring

    int low = 0;
    int high = nterms - 1;

    while (low <= high)
    {
        int mid = low + ((high - low) / 2);
        if (strncmp((terms + mid)->term, substr, strlen(substr)) < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return low;
}

int highest_match(term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;

    while (low <= high)
    {
        int mid = low + ((high - low) / 2);
        if (strncmp((terms + mid)->term, substr, strlen(substr)) > 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return high;
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);
    *n_answer = last - first + 1;
    *answer = (term *)malloc(*n_answer * sizeof(term));
    for (int i = first; i <= last; i++)
    {
        (*answer - first + i)->weight = (terms + i)->weight;
        strcpy((*answer + i - first)->term, (terms + i)->term);
    }

    int cmpfcn_num_increase(const void *a, const void *b) // compare function
    {
        term *term_a = (term *)a;
        term *term_b = (term *)b;
        return -(term_a->weight - term_b->weight); // compare the weights
    }
    qsort(*answer, *n_answer, sizeof(term), cmpfcn_num_increase);
}

