#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern char* strdup(const char*);
struct Node {
	char* data;
	struct Node* next;
};
struct twoNodes {
	struct Node* node1;
	struct Node* node2;
};
struct Node* allocate(char*);

struct Node* allocate(char* dat){
	struct Node* temp=malloc(sizeof(struct Node));
	temp->data=strdup(dat);
	temp->next=0;
	return temp;
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

int countNodes(struct Node* z){
	int a=0;
	struct Node* helper = z;
	while(helper != 0){
		a++;
		helper=helper->next;
	}
	return a;
}

struct twoNodes* printBFS(struct Node* head, struct Node* toDo, struct Node* trash){
	struct twoNodes* nodes=malloc(sizeof(struct twoNodes));	
	struct Node* current=head;
	struct Node* tempTrash=trash;
	int a=0;
	while(current!=0){
		while(tempTrash!=0){
			if(strcmp(current->data,tempTrash->data)==0){
				a=1;
			}
			tempTrash=tempTrash->next;
		}
		if(a==0){
			printf("%s ", current->data);
			toDo=enqueueNode(toDo, current->data);
			trash=insertInOrder(trash, current->data);
		}
		a=0;
		current=current->next;
		tempTrash=trash;
	}
	nodes->node1=toDo;
	nodes->node2=trash;
	return nodes;
}

int main(int argc, char* argv[argc+1]) {
	struct Node* head=0;
	int verticesNum=0;
	int counter=0;
	char vertex[20];
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
	struct Node* toDo=0;
	struct Node* trash=0;
	struct twoNodes* ez=0;
	int z=0;
	while(fscanf(f2, "%s\n", c2) !=EOF){
		struct Node* key=findLinkedList(c2, verticesNum, graph);
		ez=printBFS(key, toDo, trash);
		toDo=ez->node1;
		trash=ez->node2;
		struct Node* tempDo=toDo;
		toDo=toDo->next;
		free(tempDo->data);
		free(tempDo);
		z=countNodes(trash);
		while(z<verticesNum){
			key=findLinkedList(toDo->data, verticesNum, graph);
			free(ez);
			ez=printBFS(key, toDo, trash);
			toDo=ez->node1;
			trash=ez->node2;
			struct Node* tempDo=toDo;
			toDo=toDo->next;
			free(tempDo->data);
			free(tempDo);
			z=countNodes(trash);
		}
		printf("\n");
		struct Node* tempal1=toDo;
		struct Node* tempal2=trash;
		toDo=0;
		trash=0;
		freeNodes(tempal1);
		freeNodes(tempal2);
		free(ez);
	}
	freeNodes(toDo);
	freeNodes(trash);
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
