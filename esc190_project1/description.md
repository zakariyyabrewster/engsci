# Project 1: Autocomplete

In this project, you will write a fast implementation of the autocomplete functionality. <br>
You will work with files that contain a large number of terms, along with the importance weight
associated with those terms. <br><br>
The autocomplete task is to quickly retrieve the top terms that match a query string. For
example, if the query string is "Eng", your matches might be "English", "Engineering", and
"EngSci". In the project, matches are case-sensitive, and only the beginning of the string is
matched. "Eng" matches "EngSci" but not "engaged" and "Sci" matches "Science" but not
"EngSci".<br>

To accomplish the task, you will:
<ul>
  <li>Read in the entire file of terms, and sort the terms in lexicographic ordering. This sorted list will be reused for multiple queries</li>
  <li>Use binary search to find the location of the first term in lexicographic ordering that matches
the query string, as well as the last term in lexicographic ordering that matches the query
string</li>
  <li>Extract the match the query string, and sort them by weight</li>
  <li>Extract the top matching terms by weight</li>
</ul>

Throughout, you will use the following ```struct```:

```
typedef struct term{
  char term[200];
  double weight;
} term;
```

## Part 1

Write a function with the signature
```void read_in_terms(term **terms, int *pnterms, char *filename);```<br>
The function takes in a pointer to a pointer ```term```, a pointer to an int ```int```, and the name of a file that is formatted like ```cities.txt```.<br>

The function allocates memory for all the terms in the file and stores a pointer to the block in
```*terms```. The function stores the number of terms in ```*pnterms```. The function reads in all the terms
from ```filename```, and places them in the block pointed to by ```*terms```.<br>

The terms should be sorted in non-descending lexicographic order. You must use ```qsort``` to
accomplish this. You can assume that ```strcmp``` correctly returns a negative number if the first
argument comes before the second number in lexicographic order, and a positive number if the
first argument comes after the second argument in lexicographic order. (Note that ```strcmp``` assumes
that the inputs are C strings that consist of one-byte characters; the actual input you are given is
encoded using UTF-8, but you can ignore this). <br>

## Part 2
Write a function with the signature ```int lowest_match(term *terms, int nterms, char *substr);```<br>
The function returns the index in ```terms``` of the first term in lexicographic ordering that matches the string ```substr```.<br>

The function must run in O(log(nterms)) time, where ```nterms``` is the number of terms in ```terms```.<br>
You can assume that ```terms``` is sorted in ascending lexicographic order.

## Part 3
Write a function with the signature ```int highest_match(term *terms, int nterms, char *substr);```<br>
The function returns the index in terms of the last terms in lexicographic order that matches the string ```substr```.<br>

This function must run in O(log(nterms)) time, where ```nterms``` is the number of terms in ```terms```.<br> You can assume that ```terms``` is sorted in increasing lexicographic order.

## Part 4
Write a function with the signature ```void autocomplete(term **answer, int *n_answer, term *terms, int nterm, char *substr);```<br>
The function takes terms (assume it is sorted lexicographically in increasing order), the number of terms ```nterms```, and the query string ```substr```, and places the answers in ```answer```, with ```*n_answer``` being the number of answers. The answers should be sorted by weight in non-increasing order.<br>
You <b>must</b> use ```qsort``` for this question.
