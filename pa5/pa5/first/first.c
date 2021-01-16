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
	if(argc!=6){
		printf("error\n");
		return 0;
	}
	int cacheSize=atoi(argv[1]);
	char assoc[50];
	strcpy(assoc, argv[2]);
	char policy[50];
	strcpy(policy, argv[3]);
	int blockSize=atoi(argv[4]);
	int test=1;
	int validCache=0;
	int validBlock=0;
	int assocNum=0;
	int setNum=0;
	char *token="hi";
	long int memread=0;
	long int memwrite=0;
	long int cachehit=0;
	int tagbits=0;
	int setbits=0;
	int blockbits=0;
	for(int i=0; i<64; i++){
		if(cacheSize==test){
			validCache=1;
		}
		if(blockSize==test){
			validBlock=1;
		}
		test=test*2;
	}
	if(validCache==0 || validBlock==0){
		printf("error\n");
		return 0;
	}
	if(access(argv[5],F_OK )==-1){
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
	//printf("%d %d\n", setNum, assocNum);
	//printf("%d %d %s %s %d %s %d %d\n", argc, cacheSize, assoc, policy, blockSize, token, setNum, assocNum);
	setbits=log10(setNum)/log10(2);
	blockbits=log10(blockSize)/log10(2);
	tagbits=48-setbits-blockbits;
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
	//printf("%d %d %d\n", tagbits, setbits, blockbits);
	//printf("%lu %lu %lu\n", tagmask, setmask, bitmask);
	struct cachedata **cache=malloc(setNum*sizeof(struct cachedata *));
	for(int i=0; i<setNum; i++){
		cache[i]=malloc(assocNum*sizeof(struct cachedata));
		for(int j=0; j<assocNum; j++){
			cache[i][j].data=NULL;
			cache[i][j].lastVisited=2147483647;
			cache[i][j].lru=2147483647;
			cache[i][j].valid=0;
		}
	}
	FILE* f1=fopen(argv[5], "r");
	char rw[50];
	unsigned long memadd;
	unsigned long setindex;
	//unsigned long blockoffset;
	unsigned long temptag;
	int hit=0;
	unsigned long visitCounter=0;
	int isFull=0;
	unsigned long tempVisited=2147483647;
	unsigned long tempIndex=0;
	while(fscanf(f1, "%s %lx\n", rw, &memadd) !=EOF){
		setindex=memadd & setmask;
		setindex=setindex >> blockbits;
		//blockoffset=memadd & bitmask;
		temptag=memadd & tagmask;
		temptag=temptag >> (blockbits+setbits);
		if(strcmp(rw,"R")==0){
			for(int j=0; j<assocNum; j++){
				if(cache[setindex][j].tag==temptag && cache[setindex][j].valid==1){
					hit=1;
					visitCounter++;
					cache[setindex][j].lru=visitCounter;
					break;
				}
			}
			if(hit==1){
				cachehit++;
			}
			else{
				memread++;
				for(int j=0; j<assocNum; j++){
					if(cache[setindex][j].valid==0){
						cache[setindex][j].tag=temptag;
						visitCounter++;
						cache[setindex][j].lastVisited=visitCounter;
						cache[setindex][j].valid=1;
						cache[setindex][j].data="HIT";
						cache[setindex][j].lru=visitCounter;
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
					cache[setindex][tempIndex].tag=temptag;
					visitCounter++;
					cache[setindex][tempIndex].lastVisited=visitCounter;
					cache[setindex][tempIndex].lru=visitCounter;
					cache[setindex][tempIndex].valid=1;
					cache[setindex][tempIndex].data="HIT";
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
			}
			if(hit==1){
				cachehit++;
			}
			else{
				memread++;
				for(int j=0; j<assocNum; j++){
					if(cache[setindex][j].valid==0){
						cache[setindex][j].tag=temptag;
						visitCounter++;
						cache[setindex][j].lastVisited=visitCounter;
						cache[setindex][j].valid=1;
						cache[setindex][j].data="HIT";
						cache[setindex][j].lru=visitCounter;
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
					cache[setindex][tempIndex].tag=temptag;
					visitCounter++;
					cache[setindex][tempIndex].lastVisited=visitCounter;
					cache[setindex][tempIndex].lru=visitCounter;
					cache[setindex][tempIndex].valid=1;
					cache[setindex][tempIndex].data="HIT";
				}
			}
		}
		hit=0;
		//printf("%d\n", isFull);
		isFull=0;
		tempVisited=2147483647;
		tempIndex=0;
	}
	printf("memread:%li\n",memread);
	printf("memwrite:%li\n",memwrite);
	printf("cachehit:%li\n",cachehit);
	printf("cachemiss:%li\n",memread);
	fclose(f1);
	freeCache(cache, setNum, assocNum);
	return 0;
}
