#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
extern char* strdup(const char*);
struct Node {
	char* data;
	struct Node* next;
	int weight;
};
struct twoNodes {
	struct Node* node1;
	struct Node* node2;
	struct Node* topo;
};
struct Node* allocate(char*);

struct Node* allocate(char* dat){
	struct Node* temp=malloc(sizeof(struct Node));
	temp->data=strdup(dat);
	temp->next=0;
	return temp;
}

void printLinkedList(struct Node* head){
	struct Node* helper = head;
	while(helper != 0){
		printf("%s ", helper->data);
		helper=helper->next;
	}
	printf("\n");
	return;
}

struct Node* enqueueNode(struct Node* head, char* a){
	struct Node* newNode=allocate(a);
	struct Node* current;
	current=head;
	if(head==0){
		head=newNode;
		return head;
	}
	while(current->next!=0){
		current=current->next;
	}
	current->next=newNode;
	return head;
}

struct Node* stackNode(struct Node* head, char* a){
	struct Node* newNode=allocate(a);
	newNode->next=head;
	return newNode;
}

void freeNodes(struct Node* head){
	struct Node* temp;
	while(head!=0){
		temp=head;
		head=head->next;
		free(temp->data);
		free(temp);
	}
}

struct Node* insertInOrder(struct Node* head, char* a){
	struct Node* newNode=allocate(a);
	struct Node* current;
	if(head==0){
		head=newNode;
	}
	else if(head->next==0){
		head->next=newNode;
	}
	else{
		current=head;
		while(current->next != 0 && strcmp(current->next->data, newNode->data)<=0){
			current=current->next;
		}
		newNode->next = current->next;
		current->next=newNode;
	}
	return head;
}

int findNumOfAdjVertices(struct Node* head){
	int count=0;
	while(head->next!=0){
		head=head->next;
		count++;
	}
	return count;
}

struct Node* findLinkedList(char* c, int n, struct Node* graph[]){
	for(int i=0; i<=n-1; i++){
		if(strcmp(c,graph[i]->data)==0){
			return graph[i];
		}
	}
	return graph[0];
}

int countNodes(struct Node* z){
	int a=0;
	struct Node* helper = z;
	while(helper != 0){
		a++;
		helper=helper->next;
	}
	return a;
}

struct twoNodes* printBFS(struct Node* head, struct Node* toDo, struct Node* trash, struct Node* edgeList[], int counter, struct Node* topoSorted, int y){
	struct twoNodes* nodes=malloc(sizeof(struct twoNodes));	
	struct Node* current=head;
	struct Node* tempTrash=trash;
	struct Node* temptoDo=0;
	int a=0;
	int z=0;
	int c=0;
	while(tempTrash!=0){
		if(strcmp(current->data,tempTrash->data)==0){
			nodes->node1=toDo;
			nodes->node2=trash;
			nodes->topo=topoSorted;
			return nodes;
		}
		tempTrash=tempTrash->next;
	}
	tempTrash=trash;
	while(current!=0){
		while(tempTrash!=0){
			if(strcmp(current->data,tempTrash->data)==0){
				a=1;
			}
			tempTrash=tempTrash->next;
		}
		if(a==0){
			for(int i=0; i<counter; i++){
				if(current==head && z==0){
					temptoDo=enqueueNode(temptoDo, current->data);
					trash=insertInOrder(trash, current->data);
					z=1;
					if(y==1){
						topoSorted=enqueueNode(topoSorted, current->data);
					}
					else{
						topoSorted=enqueueNode(topoSorted, current->data);
					}
				}
				else if(strcmp(edgeList[i]->data, head->data)==0 && strcmp(edgeList[i]->next->data, current->data)==0 && c==0 && z==0){
					c=1;
					temptoDo=enqueueNode(temptoDo, current->data);
					trash=insertInOrder(trash, current->data);
					if(y==1){
						topoSorted=enqueueNode(topoSorted, current->data);
					}
					else{
						topoSorted=enqueueNode(topoSorted, current->data);
					}
				}
				else if(strcmp(edgeList[i]->data, head->data)==0 && strcmp(edgeList[i]->next->data, current->data)==0){
					temptoDo=enqueueNode(temptoDo, current->data);
				}
			}
		}
		a=0;
		current=current->next;
		tempTrash=trash;
	}
	struct Node* temptemptoDo=temptoDo;
	while(temptemptoDo->next!=0){
		temptemptoDo=temptemptoDo->next;
	}
	temptemptoDo->next=toDo;
	nodes->topo=topoSorted;
	nodes->node1=temptoDo;
	nodes->node2=trash;
	return nodes;
}

struct Node* addunvisitedtoDo(struct Node* trash, struct Node* graph[], int b){
	struct Node* toDo=0;
	struct Node* temptrash=trash;
	int a=0;
	for(int i=0;i<b;i++){
		while(temptrash!=0){
			if(strcmp(temptrash->data,graph[i]->data)==0){
				a=1;
			}
			temptrash=temptrash->next;
		}
		if(a==0){
			toDo=insertInOrder(toDo, graph[i]->data);
		}
		a=0;
	}
	return toDo;
}

int findWeight(struct Node* edgeList[], int counter, struct Node* first, struct Node* second){
	for(int i=0; i<counter; i++){
		if(strcmp(edgeList[i]->data,first->data)==0 && strcmp(edgeList[i]->next->data,second->data)==0){
			return edgeList[i]->weight;
		}
	} 
	return INT_MAX;
}

int main(int argc, char* argv[argc+1]) {
	struct Node* head=0;
	int verticesNum=0;
	int counter=0;
	char vertex[20];
	char c2[20];
	char c3[20];
	FILE* f1=fopen(argv[1], "r");
	fscanf(f1, "%d\n", &verticesNum);
	struct Node* *graph=malloc(verticesNum*sizeof(struct Node));
	for(int i=0; i<verticesNum; i++){
		fscanf(f1, "%s\n", vertex);
		head=insertInOrder(head, vertex);
	}
	struct Node* temphead=head;
	for(int i=0; i<verticesNum; i++){
		graph[i]=allocate(temphead->data);
		temphead=temphead->next;
	}
	if(head!=0){
		freeNodes(head);
	}
	head=0;
	int wt=0;
	while(fscanf(f1, "%s %s %d\n", c3, c2, &wt) !=EOF){
		counter++;
	}
	fclose(f1);
	FILE* f3=fopen(argv[1], "r");
	fscanf(f3, "%d\n", &verticesNum);
	for(int i=0; i<verticesNum; i++){
		fscanf(f3, "%s\n", vertex);
	}
	struct Node* *edgeList=malloc(counter*sizeof(struct Node));
	counter=0;
	while(fscanf(f3, "%s %s %d\n", c3, c2, &wt) !=EOF){
		head=insertInOrder(head, c3);
		head->next=allocate(c2);
		head->weight=wt;
		edgeList[counter]=head;
		head=0;
		counter++;
	}
	for(int i=0; i<verticesNum; i++){
		for(int j=0; j<counter; j++){
			if(strcmp(graph[i]->data, edgeList[j]->data)==0){
				graph[i]=insertInOrder(graph[i],edgeList[j]->next->data);	
			}
			else if(strcmp(graph[i]->data, edgeList[j]->next->data)==0){
				graph[i]=insertInOrder(graph[i],edgeList[j]->data);
			}
		}
	}
	struct Node* toDo=0;
	struct Node* trash=0;
	struct twoNodes* ez=0;
	struct Node* topoSorted=0;
	struct Node* tempSorted=0;
	int z=0;
	int y=0;
	struct Node* key=findLinkedList(graph[0]->data, verticesNum, graph);
	ez=printBFS(key, toDo, trash, edgeList, counter, topoSorted, y);
	toDo=ez->node1;
	trash=ez->node2;
	topoSorted=ez->topo;
	struct Node* tempDo=toDo;
	toDo=toDo->next;
	free(tempDo->data);
	free(tempDo);
	z=countNodes(trash);
	while(z<verticesNum){
		if(toDo==0){
			tempSorted=topoSorted;
			topoSorted=0;
			toDo=addunvisitedtoDo(trash, graph, verticesNum);
			y=1;
		}
		key=findLinkedList(toDo->data, verticesNum, graph);
		free(ez);
		ez=printBFS(key, toDo, trash, edgeList, counter, topoSorted, y);
		toDo=ez->node1;
		trash=ez->node2;
		topoSorted=ez->topo;
		struct Node* tempDo=toDo;
		toDo=toDo->next;
		free(tempDo->data);
		free(tempDo);
		z=countNodes(trash);
	}
	struct Node* topocnt=topoSorted;
	if(topocnt!=0){
		while(topocnt->next!=0){
			topocnt=topocnt->next;
		}
		topocnt->next=tempSorted;
	}
	struct Node* tempal1=toDo;
	struct Node* tempal2=trash;
	toDo=0;
	trash=0;
	freeNodes(tempal1);
	freeNodes(tempal2);
	free(ez);
	freeNodes(toDo);
	freeNodes(trash);
	fclose(f3);
	char c5 [20];
	int cwt=0;
	int ccc=0;
	int bbb=ccc;
	struct Node* temptp=0;
	struct Node* tempkey=0;
	FILE* f6=fopen(argv[2], "r");
	/*FILE* f7=fopen(argv[3], "w");
	fprintf(f7, "\n");*/
	while(fscanf(f6, "%s\n", c5) !=EOF){
		temptp=topoSorted;
		for(int i=0; i<verticesNum; i++){
			if(strcmp(graph[i]->data, c5)==0){
				graph[i]->weight=0;
			}
			else{
				graph[i]->weight=INT_MAX;
			}
		}
		do{
			ccc=bbb;
			while(temptp!=0){
				key=findLinkedList(temptp->data, verticesNum, graph);
				tempkey=key;
				if(key->next!=0){
					tempkey=tempkey->next;
					while(tempkey!=0){
						struct Node* key2=findLinkedList(tempkey->data, verticesNum, graph);
						cwt=findWeight(edgeList, counter, key, key2);
						if(key->weight==INT_MAX || cwt==INT_MAX){	
						}
						else if(key2->weight>cwt+key->weight){
							key2->weight=cwt+key->weight;
							bbb++;
						}
						tempkey=tempkey->next;
					}
				}
				temptp=temptp->next;
			}
			temptp=topoSorted;
		}while(bbb!=ccc);
		for(int i=0; i<verticesNum; i++){
			if(graph[i]->weight==INT_MAX){
				printf("%s %s\n", graph[i]->data, "INF");
			}
			else{
				printf("%s %d\n", graph[i]->data, graph[i]->weight);
			}
		}
		/*for(int i=0; i<verticesNum; i++){
			if(graph[i]->weight==INT_MAX){
				fprintf(f7, "%s %s\n", graph[i]->data, "INF");
			}
			else{
				fprintf(f7, "%s %d\n", graph[i]->data, graph[i]->weight);
			}
		}
		fprintf(f7, "\n");*/
		printf("\n");
		ccc=0;
	}
	fclose(f6);
	/*fclose(f7);*/
	freeNodes(topoSorted);
	for(int i=0; i<verticesNum; i++){
		if(graph[i]!=0){
			freeNodes(graph[i]);
		}
	}
	for(int i=0; i<counter; i++){
		if(edgeList[i]!=0){
			freeNodes(edgeList[i]);
		}
	}
	free(graph);
	free(edgeList);
	return 0;
}
