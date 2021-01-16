#include <stdlib.h>
#include <stdio.h>
#define _SVID_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <string.h>
extern char* strdup(const char*);
struct Node {
	char* data;
	struct Node* next;
};
struct Node* allocate(char*);

struct Node* allocate(char* dat){
	struct Node* temp=malloc(sizeof(struct Node));
	temp->data=strdup(dat);
	temp->next=0;
	return temp;
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

void printLinkedList(struct Node* head){
	struct Node* helper = head;
	while(helper != 0){
		printf("%s ", helper->data);
		helper=helper->next;
	}
	printf("\n");
	return;
}

int main(int argc, char* argv[argc+1]) {
	struct Node* head=0;
	int verticesNum=0;
	int counter=0;
	char vertex[20];
	char c1;
	char c2[20];
	char c3[20];
	FILE* f1=fopen(argv[1], "r");
	FILE* f2=fopen(argv[2], "r");
	fscanf(f1, "%d\n", &verticesNum);
	struct Node* *graph=malloc(verticesNum*sizeof(struct Node));
	for(int i=0; i<verticesNum; i++){
		fscanf(f1, "%s\n", vertex);
		head=insertInOrder(head, vertex);
		graph[i]=head;
		head=0;
	}
	while(fscanf(f1, "%s %s\n", c3, c2) !=EOF){
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
	while(fscanf(f3, "%s %s\n", c3, c2) !=EOF){
		head=insertInOrder(head, c3);
		head->next=allocate(c2);
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

	while(fscanf(f2, "%c %s\n", &c1, c2) !=EOF){
		struct Node* key=findLinkedList(c2, verticesNum, graph);
		if(c1=='d'){
			int adjVerNum=findNumOfAdjVertices(key);
			printf("%d\n", adjVerNum);
		}
		else{
			printLinkedList(key->next);
		}
	}
	fclose(f2);
	fclose(f3);
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
