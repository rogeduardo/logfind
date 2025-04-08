#ifndef __logfind_h__
#define __logfind_h__

#include <stdio.h>
#include <stdbool.h>

/**
 * Do the and search on a given file, return 0 on succeded, anything else on error.
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 * operator - OR or AND search to be performed on files
 */
int logfind (char **keywordsv, int keywordsc, bool operator);

/**
 * Do the and search on a given file, return 0 on succeded, anything else on error.
 * pathfile - Complete path file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 */
int searchAND(char *pathfile, char **keywordsv, int keywordsc);

/**
 * Do the or search on a given file, return 0 on succeded, anything else on error.
 * pathfile - Complete path file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 */
int searchOR(char *pathfile, char **keywordsv, int keywordsc);

#endif
