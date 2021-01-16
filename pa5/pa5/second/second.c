#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <unistd.h>
struct cachedata {
	unsigned long tag;
	char *data;
	unsigned long lastVisited;
	unsigned long lru;
	int valid;
	unsigned long memadd;
};

void freeCache(struct cachedata** cache, int setNum, int assocNum){
	for(int i=0; i<setNum; i++){
		/*for(int j=0; j<assocNum; i++){
			printf("worked1\n");
			free(cache[i][j].data);
		}*/
		free(cache[i]);
	}
	free(cache);
}

int main(int argc, char* argv[argc+1]) {
	if(argc!=9){
		printf("error\n");
		return 0;
	}
	int cacheSize=atoi(argv[1]);
	char assoc[50];
	strcpy(assoc, argv[2]);
	char policy[50];
	strcpy(policy, argv[3]);
	int blockSize=atoi(argv[4]);
	int cache2Size=atoi(argv[5]);
	char assoc2[50];
	strcpy(assoc2, argv[6]);
	char policy2[50];
	strcpy(policy2, argv[7]);
	int test=1;
	int validCache=0;
	int validCache2=0;
	int validBlock=0;
	int assocNum=0;
	int setNum=0;
	int assoc2Num=0;
	int set2Num=0;
	char *token="hi";
	long int memread=0;
	long int memwrite=0;
	long int l1cachehit=0;
	long int l1cachemiss=0;
	long int l2cachehit=0;
	int tagbits=0;
	int setbits=0;
	int tag2bits=0;
	int set2bits=0;
	int blockbits=0;
	for(int i=0; i<64; i++){
		if(cacheSize==test){
			validCache=1;
		}
		if(cache2Size==test){
			validCache2=1;
		}
		if(blockSize==test){
			validBlock=1;
		}
		test=test*2;
	}
	if(validCache==0 || validBlock==0 || validCache2==0){
		printf("error\n");
		return 0;
	}
	if(access(argv[8],F_OK )==-1){
		printf("error\n");
		return 0;
	}
	if(strcmp(assoc, "direct")==0){
		assocNum=1;
		setNum=cacheSize/blockSize;
	}
	else if(strcmp(assoc, "assoc")==0){
		setNum=1;
		assocNum=cacheSize/blockSize;
	}
	else{
		token=strtok(assoc, ":");
		token=strtok(NULL, ":");
		assocNum=atoi(token);
		setNum=(cacheSize/blockSize)/assocNum;
	}
	token="hi";
	if(strcmp(assoc2, "direct")==0){
		assoc2Num=1;
		set2Num=cache2Size/blockSize;
	}
	else if(strcmp(assoc2, "assoc")==0){
		set2Num=1;
		assoc2Num=cache2Size/blockSize;
	}
	else{
		token=strtok(assoc2, ":");
		token=strtok(NULL, ":");
		assoc2Num=atoi(token);
		set2Num=(cache2Size/blockSize)/assoc2Num;
	}
	//printf("%d %d\n", set2Num, assoc2Num);
	//printf("%d %d %s %s %d %s %d %d\n", argc, cacheSize, assoc, policy, blockSize, token, setNum, assocNum);
	setbits=log10(setNum)/log10(2);
	set2bits=log10(set2Num)/log10(2);
	blockbits=log10(blockSize)/log10(2);
	tagbits=48-setbits-blockbits;
	tag2bits=48-set2bits-blockbits;
	unsigned long temp=1;
	unsigned long bitmask=0;
	unsigned long setmask=0;
	unsigned long tagmask=0;
	for(int i=0; i<blockbits; i++){
		bitmask=bitmask+temp;
		temp=temp*2;
	}
	for(int i=0; i<setbits; i++){
		setmask=setmask+temp;
		temp=temp*2;
	}
	for(int i=0; i<tagbits; i++){
		tagmask=tagmask+temp;
		temp=temp*2;
	}
	temp=1;
	unsigned long bit2mask=0;
	unsigned long set2mask=0;
	unsigned long tag2mask=0;
	for(int i=0; i<blockbits; i++){
		bit2mask=bit2mask+temp;
		temp=temp*2;
	}
	for(int i=0; i<set2bits; i++){
		set2mask=set2mask+temp;
		temp=temp*2;
	}
	for(int i=0; i<tag2bits; i++){
		tag2mask=tag2mask+temp;
		temp=temp*2;
	}
	//printf("%d %d %d\n", tagbits, setbits, blockbits);
	//printf("%lu %lu %lu\n", tagmask, setmask, bitmask);
	struct cachedata **cache=malloc(setNum*sizeof(struct cachedata *));
	for(int i=0; i<setNum; i++){
		cache[i]=malloc(assocNum*sizeof(struct cachedata));
		for(int j=0; j<assocNum; j++){
			//cache[i][j].data=NULL;
			//cache[i][j].lastVisited=2147483647;
			//cache[i][j].lru=2147483647;
			cache[i][j].valid=0;
		}
	}
	struct cachedata **cache2=malloc(set2Num*sizeof(struct cachedata *));
	for(int i=0; i<set2Num; i++){
		cache2[i]=malloc(assoc2Num*sizeof(struct cachedata));
		for(int j=0; j<assoc2Num; j++){
			//cache2[i][j].data=NULL;
			//cache2[i][j].lastVisited=2147483647;
			//cache2[i][j].lru=2147483647;
			cache2[i][j].valid=0;
		}
	}
	FILE* f1=fopen(argv[8], "r");
	char rw[50];
	unsigned long memadd;
	unsigned long setindex;
	unsigned long set2index;
	//unsigned long blockoffset;
	unsigned long temptag;
	unsigned long temp2tag;
	int hit=0;
	unsigned long visitCounter=0;
	int isFull=0;
	int is2Full=0;
	unsigned long tempVisited=2147483647;
	unsigned long temp2Visited=2147483647;
	unsigned long tempIndex=0;
	unsigned long temp2Index=0;
	unsigned long temp3Index=0;
	while(fscanf(f1, "%s %lx\n", rw, &memadd) !=EOF){
		setindex=memadd & setmask;
		setindex=setindex >> blockbits;
		//blockoffset=memadd & bitmask;
		temptag=memadd & tagmask;
		temptag=temptag >> (blockbits+setbits);
		set2index=memadd & set2mask;
		set2index=set2index >> blockbits;
		//blockoffset=memadd & bitmask;
		temp2tag=memadd & tag2mask;
		temp2tag=temp2tag >> (blockbits+set2bits);
		if(strcmp(rw,"R")==0){
			for(int j=0; j<assocNum; j++){
				if(cache[setindex][j].tag==temptag && cache[setindex][j].valid==1){
					hit=1;
					visitCounter++;
					cache[setindex][j].lru=visitCounter;
					break;
				}
				if(j==assocNum-1){
					hit=0;
				}
			}
			if(hit==1){
				l1cachehit++;
			}
			else{
				l1cachemiss++;
				for(int j=0; j<assoc2Num; j++){
					if(cache2[set2index][j].tag==temp2tag && cache2[set2index][j].valid==1){
						hit=2;
						temp2Index=j;
						//visitCounter++;
						//cache2[set2index][j].lru=visitCounter;
						break;
					}
					if(j==assoc2Num-1){
						hit=0;
					}
				}
				if(hit==2){
					l2cachehit++;
					/*for(int j=0; j<assoc2Num; j++){
						if(cache2[set2index][j].valid==0){
							break;
						}
						if(j==assoc2Num-1){
							is2Full=1;
						}
					}*/
					unsigned long tmemadd2=cache2[set2index][temp2Index].memadd;
					//unsigned long tempmem2=tmemadd;
					tmemadd2=tmemadd2 & setmask;
					tmemadd2=tmemadd2 >> blockbits;
					for(int j=0; j<assocNum; j++){
						if(cache[setindex][j].valid==0){
							tempIndex=j;
							break;
						}
						if(j==assocNum-1){
							isFull=1;
						}
					}
					if(isFull==1){
						for(int j=0; j<assocNum; j++){
							if(cache[setindex][j].lastVisited<tempVisited && strcmp(policy, "fifo")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lastVisited;
								tempIndex=j;
							}
							else if(cache[setindex][j].lru<tempVisited && strcmp(policy, "lru")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lru;
								tempIndex=j;
							}
						}
						unsigned long tmemadd=cache[setindex][tempIndex].memadd;
						unsigned long tempmem=tmemadd;
						unsigned long shindex=cache[setindex][tempIndex].memadd;
						//unsigned long tempsetindex=tmemadd & set2mask;
						//tempsetindex=tempsetindex >> blockbits;
						tmemadd=tmemadd & tag2mask;
						tmemadd=tmemadd >> (blockbits+set2bits);
						shindex=shindex & set2mask;
						shindex=shindex >> blockbits;
						//printf("%d\n", is2Full);
						cache2[set2index][temp2Index].tag=0;
						visitCounter++;
						cache2[set2index][temp2Index].lastVisited=2147483647;
						cache2[set2index][temp2Index].lru=2147483647;
						cache2[set2index][temp2Index].valid=0;
						cache2[set2index][temp2Index].memadd=0;
						for(int j=0; j<assoc2Num; j++){
							if(cache2[shindex][j].valid==0){
								temp3Index=j;
								break;
							}
							if(j==assoc2Num-1){
								is2Full=1;
							}
						}
						if(is2Full==1){
							for(int j=0; j<assoc2Num; j++){
								if(cache2[shindex][j].lastVisited<temp2Visited && strcmp(policy2, "fifo")==0 && cache2[shindex][j].valid==1){
									temp2Visited=cache2[shindex][j].lastVisited;
									temp3Index=j;
								}
								else if(cache2[shindex][j].lru<temp2Visited && strcmp(policy2, "lru")==0 && cache2[shindex][j].valid==1){
									temp2Visited=cache2[shindex][j].lru;
									temp3Index=j;
								}
							}
							cache2[shindex][temp3Index].tag=tmemadd;
							visitCounter++;
							cache2[shindex][temp3Index].lastVisited=visitCounter;
							cache2[shindex][temp3Index].lru=visitCounter;
							cache2[shindex][temp3Index].valid=1;
							cache2[shindex][temp3Index].memadd=tempmem;
						}
						else{
							cache2[shindex][temp3Index].tag=tmemadd;
							visitCounter++;
							cache2[shindex][temp3Index].lastVisited=visitCounter;
							cache2[shindex][temp3Index].lru=visitCounter;
							cache2[shindex][temp3Index].valid=1;
							cache2[shindex][temp3Index].memadd=tempmem;
						}
						//
						cache[setindex][tempIndex].tag=temptag;
						visitCounter++;
						cache[setindex][tempIndex].lastVisited=visitCounter;
						cache[setindex][tempIndex].lru=visitCounter;
						cache[setindex][tempIndex].valid=1;
						cache[setindex][tempIndex].memadd=memadd;
						/*cache2[set2index][temp2Index].tag=0;
						visitCounter++;
						cache2[set2index][temp2Index].lastVisited=2147483647;
						cache2[set2index][temp2Index].lru=2147483647;
						cache2[set2index][temp2Index].valid=0;
						cache2[set2index][temp2Index].memadd=0;
						is2Full=0;*/
					}
					else{
						cache[setindex][tempIndex].tag=temptag;
						visitCounter++;
						cache[setindex][tempIndex].lastVisited=visitCounter;
						cache[setindex][tempIndex].lru=visitCounter;
						cache[setindex][tempIndex].valid=1;
						cache[setindex][tempIndex].memadd=memadd;
						cache2[set2index][temp2Index].tag=0;
						visitCounter++;
						cache2[set2index][temp2Index].lastVisited=2147483647;
						cache2[set2index][temp2Index].lru=2147483647;
						cache2[set2index][temp2Index].valid=0;
						cache2[set2index][temp2Index].memadd=0;
					}
				}
				else{
					memread++;
					isFull=0;
					for(int j=0; j<assocNum; j++){
						if(cache[setindex][j].valid==0){
							cache[setindex][j].tag=temptag;
							visitCounter++;
							cache[setindex][j].lastVisited=visitCounter;
							cache[setindex][j].valid=1;
							cache[setindex][j].data="HIT";
							cache[setindex][j].lru=visitCounter;
							cache[setindex][j].memadd=memadd;
							break;
						}
						if(j==assocNum-1){
							isFull=1;
						}
					}
					/*for(int j=0; j<assoc2Num; j++){
						if(cache2[set2index][j].valid==0){
							temp2Index=j;
							break;
						}
						if(j==assoc2Num-1){
							is2Full=1;
						}
					}*/
					tempVisited=2147483647;
					if(isFull==1){
						for(int j=0; j<assocNum; j++){
							if(cache[setindex][j].lastVisited<tempVisited && strcmp(policy, "fifo")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lastVisited;
								tempIndex=j;
							}
							else if(cache[setindex][j].lru<tempVisited && strcmp(policy, "lru")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lru;
								tempIndex=j;
							}
						}
						unsigned long tmemadd=cache[setindex][tempIndex].memadd;
						unsigned long tempmem=tmemadd;
						unsigned long tempsetindex=tmemadd & set2mask;
						tempsetindex=tempsetindex >> blockbits;
						tmemadd=tmemadd & tag2mask;
						tmemadd=tmemadd >> (blockbits+set2bits);
						is2Full=0;
						for(int j=0; j<assoc2Num; j++){
							if(cache2[tempsetindex][j].valid==0){
								temp2Index=j;
								break;
							}
							if(j==assoc2Num-1){
								is2Full=1;
							}
						}
						temp2Visited=2147483647;
						if(is2Full==1){
							//printf(" 2Full ");
							for(int j=0; j<assoc2Num; j++){
								if(cache2[tempsetindex][j].lastVisited<temp2Visited && strcmp(policy2, "fifo")==0 && cache2[tempsetindex][j].valid==1){
									temp2Visited=cache2[tempsetindex][j].lastVisited;
									temp2Index=j;
								}
								else if(cache2[tempsetindex][j].lru<temp2Visited && strcmp(policy2, "lru")==0 && cache2[tempsetindex][j].valid==1){
									temp2Visited=cache2[tempsetindex][j].lru;
									temp2Index=j;
								}
							}
							//printf(" %lu ", temp2Index);
							cache2[tempsetindex][temp2Index].tag=tmemadd;
							visitCounter++;
							cache2[tempsetindex][temp2Index].lastVisited=visitCounter;
							cache2[tempsetindex][temp2Index].lru=visitCounter;
							cache2[tempsetindex][temp2Index].valid=1;
							cache2[tempsetindex][temp2Index].memadd=tempmem;
							cache[setindex][tempIndex].tag=temptag;
							visitCounter++;
							cache[setindex][tempIndex].lastVisited=visitCounter;
							cache[setindex][tempIndex].lru=visitCounter;
							cache[setindex][tempIndex].valid=1;
							cache[setindex][tempIndex].data="HIT";
							cache[setindex][tempIndex].memadd=memadd;
						}
						else{
							cache2[tempsetindex][temp2Index].tag=tmemadd;
							visitCounter++;
							cache2[tempsetindex][temp2Index].lastVisited=visitCounter;
							cache2[tempsetindex][temp2Index].lru=visitCounter;
							cache2[tempsetindex][temp2Index].valid=1;
							cache2[tempsetindex][temp2Index].memadd=tempmem;
							cache[setindex][tempIndex].tag=temptag;
							visitCounter++;
							cache[setindex][tempIndex].lastVisited=visitCounter;
							cache[setindex][tempIndex].lru=visitCounter;
							cache[setindex][tempIndex].valid=1;
							cache[setindex][tempIndex].data="HIT";
							cache[setindex][tempIndex].memadd=memadd;
						}
					}
				}
			}
		}
		else{
			memwrite++;
			for(int j=0; j<assocNum; j++){
				if(cache[setindex][j].tag==temptag && cache[setindex][j].valid==1){
					hit=1;
					visitCounter++;
					cache[setindex][j].lru=visitCounter;
					break;
				}
				if(j==assocNum-1){
					hit=0;
				}
			}
			if(hit==1){
				l1cachehit++;
			}
			else{
				l1cachemiss++;
				for(int j=0; j<assoc2Num; j++){
					if(cache2[set2index][j].tag==temp2tag && cache2[set2index][j].valid==1){
						hit=2;
						temp2Index=j;
						//visitCounter++;
						//cache2[set2index][j].lru=visitCounter;
						break;
					}
					if(j==assoc2Num-1){
						hit=0;
					}
				}
				if(hit==2){
					l2cachehit++;
					/*for(int j=0; j<assoc2Num; j++){
						if(cache2[set2index][j].valid==0){
							break;
						}
						if(j==assoc2Num-1){
							is2Full=1;
						}
					}*/
					unsigned long tmemadd2=cache2[set2index][temp2Index].memadd;
					//unsigned long tempmem2=tmemadd;
					tmemadd2=tmemadd2 & setmask;
					tmemadd2=tmemadd2 >> blockbits;
					for(int j=0; j<assocNum; j++){
						if(cache[setindex][j].valid==0){
							tempIndex=j;
							break;
						}
						if(j==assocNum-1){
							isFull=1;
						}
					}
					if(isFull==1){
						for(int j=0; j<assocNum; j++){
							if(cache[setindex][j].lastVisited<tempVisited && strcmp(policy, "fifo")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lastVisited;
								tempIndex=j;
							}
							else if(cache[setindex][j].lru<tempVisited && strcmp(policy, "lru")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lru;
								tempIndex=j;
							}
						}
						unsigned long tmemadd=cache[setindex][tempIndex].memadd;
						unsigned long tempmem=tmemadd;
						unsigned long shindex=cache[setindex][tempIndex].memadd;
						//unsigned long tempsetindex=tmemadd & set2mask;
						//tempsetindex=tempsetindex >> blockbits;
						tmemadd=tmemadd & tag2mask;
						tmemadd=tmemadd >> (blockbits+set2bits);
						shindex=shindex & set2mask;
						shindex=shindex >> blockbits;
						cache2[set2index][temp2Index].tag=0;
						visitCounter++;
						cache2[set2index][temp2Index].lastVisited=2147483647;
						cache2[set2index][temp2Index].lru=2147483647;
						cache2[set2index][temp2Index].valid=0;
						cache2[set2index][temp2Index].memadd=0;
						for(int j=0; j<assoc2Num; j++){
							if(cache2[shindex][j].valid==0){
								temp3Index=j;
								break;
							}
							if(j==assoc2Num-1){
								is2Full=1;
							}
						}
						if(is2Full==1){
							for(int j=0; j<assoc2Num; j++){
								if(cache2[shindex][j].lastVisited<temp2Visited && strcmp(policy2, "fifo")==0 && cache2[shindex][j].valid==1){
									temp2Visited=cache2[shindex][j].lastVisited;
									temp3Index=j;
								}
								else if(cache2[shindex][j].lru<temp2Visited && strcmp(policy2, "lru")==0 && cache2[shindex][j].valid==1){
									temp2Visited=cache2[shindex][j].lru;
									temp3Index=j;
								}
							}
							cache2[shindex][temp3Index].tag=tmemadd;
							visitCounter++;
							cache2[shindex][temp3Index].lastVisited=visitCounter;
							cache2[shindex][temp3Index].lru=visitCounter;
							cache2[shindex][temp3Index].valid=1;
							cache2[shindex][temp3Index].memadd=tempmem;
						}
						else{
							cache2[shindex][temp3Index].tag=tmemadd;
							visitCounter++;
							cache2[shindex][temp3Index].lastVisited=visitCounter;
							cache2[shindex][temp3Index].lru=visitCounter;
							cache2[shindex][temp3Index].valid=1;
							cache2[shindex][temp3Index].memadd=tempmem;
						}
						//
						cache[setindex][tempIndex].tag=temptag;
						visitCounter++;
						cache[setindex][tempIndex].lastVisited=visitCounter;
						cache[setindex][tempIndex].lru=visitCounter;
						cache[setindex][tempIndex].valid=1;
						cache[setindex][tempIndex].memadd=memadd;
						/*cache2[set2index][temp2Index].tag=0;
						visitCounter++;
						cache2[set2index][temp2Index].lastVisited=2147483647;
						cache2[set2index][temp2Index].lru=2147483647;
						cache2[set2index][temp2Index].valid=0;
						cache2[set2index][temp2Index].memadd=0;
						is2Full=0;*/
					}
					else{
						cache[setindex][tempIndex].tag=temptag;
						visitCounter++;
						cache[setindex][tempIndex].lastVisited=visitCounter;
						cache[setindex][tempIndex].lru=visitCounter;
						cache[setindex][tempIndex].valid=1;
						cache[setindex][tempIndex].memadd=memadd;
						cache2[set2index][temp2Index].tag=0;
						visitCounter++;
						cache2[set2index][temp2Index].lastVisited=2147483647;
						cache2[set2index][temp2Index].lru=2147483647;
						cache2[set2index][temp2Index].valid=0;
						cache2[set2index][temp2Index].memadd=0;
					}
				}
				else{
					memread++;
					isFull=0;
					for(int j=0; j<assocNum; j++){
						if(cache[setindex][j].valid==0){
							cache[setindex][j].tag=temptag;
							visitCounter++;
							cache[setindex][j].lastVisited=visitCounter;
							cache[setindex][j].valid=1;
							cache[setindex][j].data="HIT";
							cache[setindex][j].lru=visitCounter;
							cache[setindex][j].memadd=memadd;
							break;
						}
						if(j==assocNum-1){
							isFull=1;
						}
					}
					/*for(int j=0; j<assoc2Num; j++){
						if(cache2[set2index][j].valid==0){
							temp2Index=j;
							break;
						}
						if(j==assoc2Num-1){
							is2Full=1;
						}
					}*/
					tempVisited=2147483647;
					if(isFull==1){
						for(int j=0; j<assocNum; j++){
							if(cache[setindex][j].lastVisited<tempVisited && strcmp(policy, "fifo")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lastVisited;
								tempIndex=j;
							}
							else if(cache[setindex][j].lru<tempVisited && strcmp(policy, "lru")==0 && cache[setindex][j].valid==1){
								tempVisited=cache[setindex][j].lru;
								tempIndex=j;
							}
						}
						unsigned long tmemadd=cache[setindex][tempIndex].memadd;
						unsigned long tempmem=tmemadd;
						unsigned long tempsetindex=tmemadd & set2mask;
						tempsetindex=tempsetindex >> blockbits;
						tmemadd=tmemadd & tag2mask;
						tmemadd=tmemadd >> (blockbits+set2bits);
						is2Full=0;
						for(int j=0; j<assoc2Num; j++){
							if(cache2[tempsetindex][j].valid==0){
								temp2Index=j;
								break;
							}
							if(j==assoc2Num-1){
								is2Full=1;
							}
						}
						if(is2Full==1){
							for(int j=0; j<assoc2Num; j++){
								if(cache2[tempsetindex][j].lastVisited<temp2Visited && strcmp(policy2, "fifo")==0 && cache2[tempsetindex][j].valid==1){
									temp2Visited=cache2[tempsetindex][j].lastVisited;
									temp2Index=j;
								}
								else if(cache2[tempsetindex][j].lru<temp2Visited && strcmp(policy2, "lru")==0 && cache2[tempsetindex][j].valid==1){
									temp2Visited=cache2[tempsetindex][j].lru;
									temp2Index=j;
								}
							}
							cache2[tempsetindex][temp2Index].tag=tmemadd;
							visitCounter++;
							cache2[tempsetindex][temp2Index].lastVisited=visitCounter;
							cache2[tempsetindex][temp2Index].lru=visitCounter;
							cache2[tempsetindex][temp2Index].valid=1;
							cache2[tempsetindex][temp2Index].memadd=tempmem;
							cache[setindex][tempIndex].tag=temptag;
							visitCounter++;
							cache[setindex][tempIndex].lastVisited=visitCounter;
							cache[setindex][tempIndex].lru=visitCounter;
							cache[setindex][tempIndex].valid=1;
							cache[setindex][tempIndex].data="HIT";
							cache[setindex][tempIndex].memadd=memadd;
						}
						else{
							cache2[tempsetindex][temp2Index].tag=tmemadd;
							visitCounter++;
							cache2[tempsetindex][temp2Index].lastVisited=visitCounter;
							cache2[tempsetindex][temp2Index].lru=visitCounter;
							cache2[tempsetindex][temp2Index].valid=1;
							cache2[tempsetindex][temp2Index].memadd=tempmem;
							cache[setindex][tempIndex].tag=temptag;
							visitCounter++;
							cache[setindex][tempIndex].lastVisited=visitCounter;
							cache[setindex][tempIndex].lru=visitCounter;
							cache[setindex][tempIndex].valid=1;
							cache[setindex][tempIndex].data="HIT";
							cache[setindex][tempIndex].memadd=memadd;
						}
					}
				}
			}
		}
		/*for(int i=0; i<setNum; i++){
			for(int j=0; j<assocNum; j++){
				printf("%lx ", cache[i][j].memadd);
				printf("%lu ", cache[i][j].lastVisited);
			}
		}
		printf("\n");
		for(int i=0; i<set2Num; i++){
			for(int j=0; j<assoc2Num; j++){
				printf("%lx ", cache2[i][j].memadd);
				printf("%lu ", cache2[i][j].lastVisited);
			}
		}
		printf("\n");*/
		hit=0;
		//printf("%d\n", isFull);
		isFull=0;
		is2Full=0;
		tempVisited=2147483647;
		temp2Visited=2147483647;
		tempIndex=0;
		temp2Index=0;
		temp3Index=0;
	}
	printf("memread:%li\n",memread);
	printf("memwrite:%li\n",memwrite);
	printf("l1cachehit:%li\n",l1cachehit);
	printf("l1cachemiss:%li\n",l1cachemiss);
	printf("l2cachehit:%li\n",l2cachehit);
	printf("l2cachemiss:%li\n",memread);
	fclose(f1);
	freeCache(cache, setNum, assocNum);
	freeCache(cache2, set2Num, assoc2Num);
	return 0;
}
