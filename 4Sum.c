/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */

#define HASHSIZE (1<<18)

typedef struct _hashNodeData {
    int a, b;
    int aIdx, bIdx;
}hashNodeData;

typedef struct _hashNode {
    hashNodeData data;
    struct _hashNode *next;
}hashNode;

typedef struct _four {
    int v[4];
}four;


bool isEqualFour(four a, four b) {
    int i;
    for(i = 0; i < 4; ++i) {
        if (a.v[i] != b.v[i]) {
            return false;
        }
    }
    return true;
}

void resetHashNodeData(hashNodeData *nd, int _aIdx, int _bIdx, int _a, int _b) {
    nd->aIdx = _aIdx;
    nd->bIdx = _bIdx;
    nd->a = _a;
    nd->b = _b;
}

void resetHashNode(hashNode *hn, hashNodeData nd) {
    hn->data = nd;
    hn->next = NULL;
}

int getHashIndex(hashNodeData *nd) {
    return (nd->a + nd->b) & (HASHSIZE-1);
}

hashNode *HT[HASHSIZE];
four tmpRet[400000];

int **malloc2DArray(int r, int c) {
    int i, j;
    int **p = (int **)malloc(sizeof(int *) * r);
    for (i = 0; i < r; ++i) {
        p[i] = (int *)malloc( sizeof(int) * c );
    }
    return p;
}

void initHashTable() {
    int i = 0;
    for (i = 0; i < HASHSIZE; ++i) {
        HT[i] = NULL;
    }
}

void addNodeToHashTable(hashNodeData nd) {
    int idx = getHashIndex(&nd);
    hashNode *p = (hashNode *) malloc ( sizeof(hashNode) );
    resetHashNode(p, nd);
    
    if ( HT[idx] == NULL ) {
        HT[ idx ] = p;
    }else {
        hashNode *tmpNext = HT[idx]->next;
        HT[idx]->next = p;
        p->next = tmpNext;
        
    }
}

void addToHashTable(int *nums, int numsSize) {
    int i, j;
    for (i = 0; i < numsSize; ++i) {
        for(j = i+1; j < numsSize; ++j) {
            /* 初始化一个结点 */
            hashNodeData nd;
            resetHashNodeData(&nd, i, j, nums[i], nums[j]);
            addNodeToHashTable(nd);
        }
    }
}

int sortIntCmp (const void *a, const void *b) {
    int va = *(int *)a;
    int vb = *(int *)b;
    return va < vb ? -1 : 1;
}

int sort4IntCmp (const void *a, const void *b) {
    four va = *(four *)a;
    four vb = *(four *)b;
    int i;
    for (i = 0; i < 4; ++i) {
        if (va.v[i] != vb.v[i]) {
            return va.v[i] < vb.v[i] ? -1 : 1;
        }
    }
    return 0;
}

int getStatistics(int* nums, int numsSize, int target) {
    int i, j, k;
    int len = 0;
    int array4[4];
    
    for (i = 0; i < numsSize; ++i) {
        for(j = i+1; j < numsSize; ++j) {
            int v = (target - nums[i] - nums[j]);
            int idx = v & (HASHSIZE-1);
            if ( HT[idx] ) {
                hashNode *p = HT[idx];
                while (p) {
                    hashNodeData nd = p->data; 
                    if( v == nd.a + nd.b && i > nd.bIdx ) {
                        array4[0] = nd.a, array4[1] = nd.b, array4[2] = nums[i], array4[3] = nums[j];
                        qsort(array4, 4, sizeof(int), sortIntCmp);
                        
                        for(k = 0; k < 4; ++k) tmpRet[len].v[k] = array4[k];
                        len ++;
                    }
                    p = p->next;
                }
            }
        }
    }
    return len;
}

int fillTmp(int **p, int len) {
    int i, j, nlen = 0;
    qsort(tmpRet, len, sizeof(four), sort4IntCmp);
    
    for(i = 0; i < len; ++i) {
        if (i && isEqualFour( tmpRet[i], tmpRet[i-1] ) ) {
            continue;
        }
        for(j = 0; j < 4; ++j) {
            p[nlen][j] = tmpRet[i].v[j];
        }
        nlen ++;
    }
    return nlen;
}

int** fourSum(int* nums, int numsSize, int target, int* returnSize) {
    initHashTable();
    addToHashTable(nums, numsSize);
    int len = getStatistics(nums, numsSize, target);
    *returnSize = len;
    int **p = malloc2DArray(len, 4);
    *returnSize = fillTmp(p, len);
    return p;
}
