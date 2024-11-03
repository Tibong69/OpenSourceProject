#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

#define MAX_LEN 20

struct node{
	char *name;
	int mid;
	int fin;
};

int compare(const void* a, const void* b){
	return strcmp(((struct node*)a)->name, ((struct node*)b)->name);
}

void print_node(const void *, VISIT, int);

int main(void){
	char *nameTable;
	struct node *stud_table;
	struct node *root = NULL;
	int n;
	printf("최대학생 수: ");
	scanf("%d", &n);
	nameTable = (char *)malloc(n * sizeof(char) * MAX_LEN);
	stud_table = (struct node*)malloc(n * sizeof(struct node));

	char *name_ptr = nameTable;
	struct node *stud_ptr = stud_table;
	struct node **ret;
	int i = 0;

	while(scanf("%s %d %d", name_ptr, &stud_ptr->mid, &stud_ptr->fin) != EOF && i++ < n){
		stud_ptr->name = name_ptr;
		ret = (struct node **)tsearch( (void *) stud_ptr, (void **)&root, compare);
		printf("\"%s\"학생이 ", (*ret)->name);

		if(*ret == stud_ptr)
			printf("트리에 추가됨.\n");
		else
			printf("트리에 이미 존재함.\n");

		name_ptr += strlen(name_ptr)+1;
		stud_ptr++;
	}

	twalk((void *)root, print_node);
}

void print_node(const void *stud_ptr, VISIT order, int level){
	if(order == preorder || order == leaf)
		printf("이름 = %-20s, 중간고사 = %d, 기말고사 = %d\n", 
        (*(struct node**)stud_ptr)->name, (*(struct node**)stud_ptr)->mid, 
        (*(struct node**)stud_ptr)->fin);
}