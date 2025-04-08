#ifndef __logfind_h__
#define __logfind_h__

#include <stdio.h>
#include <stdbool.h>

/**
 * Do the and search on a given file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 * operator - OR or AND search to be performed on files
 */
void logfind (char **keywordsv, int keywordsc, bool operator);
/**
 * Do the and search on a given file
 * pathfile - Complete path file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 */
void searchAND(char *pathfile, char **keywordsv, int keywordsc);
/**
 * Do the or search on a given file
 * pathfile - Complete path file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 */
void searchOR(char *pathfile, char **keywordsv, int keywordsc);

#endif
