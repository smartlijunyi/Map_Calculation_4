#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Graph data structure

#define MAXSIZE 4000							
typedef int Vertex;
typedef int LinkType;
typedef double Weight;
typedef struct ArcNode{							
	int adjvex;									
	struct ArcNode *nextarc;					
	Weight length;								
	LinkType linkid;
}ArcNode;

typedef struct VNode{							
	Vertex id;
	ArcNode *firstarc;							
}VNode,AdjList[MAXSIZE]; 

typedef struct{
	AdjList verlist;							
	int nodenum,linknum;						
}ALGraph;

// Search link list data structure

struct Cor{
	int node_id;
	double lat;
	double lon;
}cor;

struct node{
	struct Cor data;
	struct node *next;
};

struct node *head;
struct node *curr;

// Functions about dealing with files

char *find_next_string(char *source, char *x){
	char *result;
	result = (char *)malloc(sizeof(char)*1000000);
	char *str = strstr(source,x);
	int length = strlen(x);
	int j = 0;
	for(int i = length; str[i] != '\0'; i++){
		result[j] = str[i];
		j++;
	}
	result[j] = '\0';
	return result;
}

int split_the_string_int(char source[], char x[], char y[]){
	char *result,*s0;
	result = (char *)malloc(sizeof(char)*1000000);
	s0 = (char *)malloc(sizeof(char)*1000000);
	s0 = find_next_string(source,x);
	int L0 = strlen(s0);
	char *newc = strstr(source,y);
	int L1 = strlen(newc);
	int index = L0 - L1;
	s0[index] = '\0';
	strcpy(result,s0);
	free(s0);
	return atoi(result);
}

double split_the_string_double(char source[], char x[], char y[]){
	char *result,*s0;
	result = (char *)malloc(sizeof(char)*1000000);
	s0 = (char *)malloc(sizeof(char)*1000000);
	s0 = find_next_string(source,x);
	int L0 = strlen(s0);
	char *newc = strstr(source,y);
	int L1 = strlen(newc);
	int index = L0 - L1;
	s0[index] = '\0';
	strcpy(result,s0);
	free(s0);
	return atof(result);
}

int find_the_first_place(char source[],char ch[]){
	int l = strlen(source);
	int position = 0;
	for(int i = 0; i < l; i++){
		if(ch[0] == source[i]){
			position = i;
			break;
		}
	}
	return position;
}

int find_the_second_place(char source[],char ch[]){
	int l = find_the_first_place(source,ch);
	int position = 0;
	for(int i = l + 1; source[i] != '\0'; i++){
		if(ch[0] == source[i]){
			position = i ;
			break;
		}
	}
	return position;
}

int take_int(char source[], int index1, int index2){
	char *result;
	result = (char *)malloc(sizeof(char)*1000000);
	int k = 0;
	for(int i = index1 + 1; i < index2; i++){
		result[k] = source[i];
		k++;
	}
	result[k] = '\0';
	return atoi(result);
}

int take_next_int(char source[], int index, char s[]){
	int k = 0;
	int index0 = 0;
	for(int i = index + 1; source[i] != '\0'; i++){
		if(s[0] == source[i]){
			index0 = i;
			break;
		}
	}
	int r = take_int(source,index,index0);
	return r;
}

void read_map_file(){
	FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6,*fp7;
	
	fp1 = fopen("Final_Map.map","r");
	fp2 = fopen("node.dat","w");
	
	fp3 = fopen("nodeinfo.txt","w");
	fp4 = fopen("id.txt","w");
	fp5 = fopen("length.txt","w");
	fp6 = fopen("link_id.txt","w");
	fp7 = fopen("allthever.txt","w");
	
	char buf[200];
	while(fgets(buf,sizeof(buf),fp1)){
		if(buf[0] == '<' && buf[1] == 'n' && buf[2] == 'o' && buf[3] == 'd'){
			
			double b = split_the_string_double(buf,"lat="," lon=");
			double c = split_the_string_double(buf,"lon="," /node>");
			fprintf(fp2,"%lf  %lf  \n",c,b);
			int a = split_the_string_int(buf,"id="," lat=");
			fprintf(fp4,"%i   %lf   %lf \n",a,c,b);
			fprintf(fp7,"%i \n",a);
		}
		if(buf[0] == '<' && buf[1] == 'l' && buf[2] == 'i' && buf[3] == 'n'){
			
			double l = split_the_string_double(buf,"length="," veg=");
			fprintf(fp5,"%lf \n",l);
			int f0 = find_the_first_place(buf,"o");
			int f1 = find_the_second_place(buf,"o");
			 
			int n1 = take_int(buf,f0+3,f1-2);
			int n2 = take_next_int(buf,f1+3," way=");
			fprintf(fp3,"%i  %i \n",n1,n2);
			
			int name = split_the_string_int(buf,"id="," node");
			fprintf(fp6,"%i \n",name);
		}
	}
	
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
	fclose(fp7);
}

int get_node_num(){
	int num;
	char s;
	FILE *fp;
	fp = fopen("node.dat","r");
	while((s = fgetc(fp)) != EOF){
		if(s == '\n')
			num++;
	}
	return num;
}

int get_link_num(){
	int num;
	char s;
	FILE *fp;
	fp = fopen("link_id.txt","r");
	while((s = fgetc(fp)) != EOF){
		if(s == '\n')
			num++;
	}
	return num;
}

void load_cor_info(){
	FILE *fp;
	fp = fopen("id.txt","r");
	struct node *p;
	head = NULL;
	while(!feof(fp)){
		fscanf(fp,"%i%lf%lf",cor.node_id,cor.lat,cor.lon);
		if(head == NULL){
			head = (struct node*)malloc(sizeof(struct node));
			head->data = cor;
			head->next = NULL;
			curr = head;
		}
		else{
			p = (struct node*)malloc(sizeof(struct node));
			p->data = cor;
			p->next = NULL;
			curr->next = p;
			curr = p;
		}
	}
	fclose(fp);
}

double search_lat(int nodeid){
	curr = head;
	double lat;
	while(curr != NULL){
		if(nodeid == curr->data.node_id){
			lat = curr->data.lat;
			break;
		}
		curr = curr->next;
	}
	return lat;
}

double search_lon(int nodeid){
	curr = head;
	double lon;
	while(curr != NULL){
		if(nodeid == curr->data.node_id){
			lon = curr->data.lon;
			break;
		}
		curr = curr->next;
	}
	return lon;
}

void draw_link(){
	FILE *fp1,*fp2;
	fp1 = fopen("Final_Map.map","r");
	fp2 = fopen("link.dat","w");
	char buf[200];
	while(fgets(buf,sizeof(buf),fp1)){
		if(buf[0] == '<' && buf[1] == 'l' && buf[2] == 'i' && buf[3] == 'n'){
			int f0 = find_the_first_place(buf,"o");
			int f1 = find_the_second_place(buf,"o");
			 
			int n1 = take_int(buf,f0+3,f1-2);
			int n2 = take_next_int(buf,f1+3," way=");
			
			double x1 = search_lon(n1);
			double y1 = search_lat(n1);
			double x2 = search_lon(n2);
			double y2 = search_lat(n2);
			fprintf(fp2,"%lf  %lf  \n",x1,y1);
			fprintf(fp2,"%lf  %lf  \n\n\n",x2,y2);
		}
	}
	fclose(fp1);
	fclose(fp2);
}

// Functions about graph;

void CreateALGraph(ALGraph *G){
	
}

// Main function

int main(int argc, char **argv){
	printf("Welcome to Map Calculation System!\n");
	ALGraph *G;
	read_map_file();
	load_cor_info();
	draw_link();
	return 0;
}
