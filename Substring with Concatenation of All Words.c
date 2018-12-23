/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */

int getSubStringIndex(char* s, int ssize, char* word, int start) {
    int i, j;
    int wordsize = strlen(word);
    for(i = start; i + wordsize <= ssize; i++) {
        for(j = 0; j < wordsize; j++) {
            if(s[i+j] != word[j]) {
                break;
            }
        }
        if(j == wordsize) {
            return i;
        }
    }
    return -1;
}

int *getHashArray (char* s, char** words, int wordsSize) {
    int i;
    int strl = strlen(s);
    int *ret = (int *)malloc ( sizeof(int) * strl );
    for(i = 0; i < strl; i++) 
        ret[i] = -1;
    for(i = 0; i < wordsSize; i++) {
        int start = 0;
        while ( true ) {
            int idx = getSubStringIndex(s, strl, words[i], start);
            if (idx == -1) {
                break;
            }else {
                if(ret[ idx ] == -1) {
                    ret[ idx ] = i;
                    start = idx + 1;
                }else {
                    break;
                }
            }
        }
    }
    return ret;
}

int *getAns(int *hashv, int len, char **words, int wordLen, int wordsSize, int *returnSize) {
    int i, j;
    int *filled = (int *) malloc( sizeof(int) * wordsSize);
    int *ret = (int *) malloc( sizeof(int) * len);
    int *maxCount = (int *) malloc( sizeof(int) * wordsSize);
    int retSize = 0;
    memset(ret, 0, sizeof(int)*len);
    memset(maxCount, 0, sizeof(int)*wordsSize);
    
    for(i = 0; i < wordsSize; ++i) {
        for(j = 0; j < i; ++j) {
            if( !strcmp(words[i], words[j]) ) {
                break;
            }
        }
        maxCount[j] ++;
    }
    
    /*for(i = 0; i < wordsSize; ++i) {
        printf("%d ", maxCount[i]);
    }
    puts("");
    for(i = 0; i < len; ++i) {
        printf("%d ", hashv[i]);
    }
    puts("");*/
    
    for(i = 0; i < len; ++i) {
        memset(filled, 0, sizeof(int)*wordsSize);
        for(j = 0; j < wordsSize; ++j) {
            int idx = i + j*wordLen;
            if (idx >= len) {
                break;
            }
            if(hashv[idx] == -1 || filled[ hashv[idx] ] >= maxCount[ hashv[idx] ] ) {
                break;
            }
            filled[ hashv[idx] ] ++;
        }
        if(j == wordsSize) {
            ret[ retSize++ ] = i;
        }
    }
    *returnSize = retSize;
    return ret;
}

int* findSubstring(char* s, char** words, int wordsSize, int* returnSize) {
    int *hashv;
    hashv = getHashArray (s, words, wordsSize);
    if (wordsSize == 0) {
        *returnSize = 0;
        return hashv;
    }
    int *ret = getAns(hashv, strlen(s), words, strlen(words[0]), wordsSize, returnSize);
    return ret;
}
