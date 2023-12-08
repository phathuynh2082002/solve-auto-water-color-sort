#include<stdio.h>
#include<stdlib.h>
#define max 500
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
int max_bottle;

//cau truc bottle:mang a: luu tru mau sac ,capicity: luong nuoc co trong lo
typedef struct{
	int a[100];
	int capacity; 
}Bottle;

//cau truc state:n bottle
typedef struct{
	Bottle bottle[100];
}State;

void printState(State state){
	int i,j;
	for(i=0;i<=max_bottle;i++)
		printf("Bootle %d\t",i);
	printf("\n");
	for(i=max_bottle-1;i>=1;i--){
		for(j=0;j<=max_bottle;j++){
			if(state.bottle[j].a[i]!=0){
				if(state.bottle[j].a[i]==1)
					printf(RED"%5d\t\t"RESET,0);
				else if(state.bottle[j].a[i]==2)
					printf(BLUE"%5d\t\t"RESET,0);
				else if(state.bottle[j].a[i]==3)
					printf(YELLOW"%5d\t\t"RESET,0);
				else if(state.bottle[j].a[i]==4)
					printf(GREEN"%5d\t\t"RESET,0);
				else if(state.bottle[j].a[i]==5)
					printf(CYAN"%5d\t\t"RESET,0);
			}
			else printf("\t\t");
		}
		printf("\n");
	}
}

//so sanh 2 trang thai
int compare(State state1,State state2){
	int i,j;
	int count=0;
	for(i=0;i<=max_bottle;i++)
		for(j=1;j<max_bottle;j++){
			if(state1.bottle[i].a[j]!=state2.bottle[i].a[j])
				return 0;
		}
	return 1;
}

//kiem tra mau sac giua cac lo co khac nhau khong
int goalcheck(State state){
	int i,j;
	for(i=0;i<=max_bottle;i++){
		int capX=state.bottle[i].capacity;
		if(capX==max_bottle-1){
			for(j=max_bottle-1;j>1;j--)
				if(state.bottle[i].a[j]!=state.bottle[i].a[j-1])
					return 0;
		}
		else if(capX==0) continue;
		else return 0;
		
	}	
	return 1;
}

//dem so mau dau tien cua lo x
int cnt_color(State state,int x){
	int capx=state.bottle[x].capacity;
	int colorx=state.bottle[x].a[capx];
	int i,cnt=1;
	for(i=capx-1;i>=1;i--){
		if(state.bottle[x].a[i]==colorx)
			cnt++;
		else break;
	}
	return cnt;
}

//sao chep lo x qua lo y
void copy_bottle(State *state1,State state2){
	int i,j;
	for(i=0;i<=max_bottle;i++){
		for(j=1;j<max_bottle;j++)
			state1->bottle[i].a[j]=state2.bottle[i].a[j];
		state1->bottle[i].capacity=state2.bottle[i].capacity;
	}	
}

//kiem tra lo x co rong khong
int empty_bottle(State state,int x){
	return state.bottle[x].capacity==0;
}

//do 1 mau lo x qua lo y
void pour1color(State *state,int x,int y){
	int capx=state->bottle[x].capacity;
	int capy=state->bottle[y].capacity;
	state->bottle[y].a[capy+1]=state->bottle[x].a[capx];
	state->bottle[y].capacity++;
	state->bottle[x].a[capx]=0;
	state->bottle[x].capacity--;
}

//do lo x qua lo y
int pourXtoY(State state,State *result,int x,int y){
	int i;
	int capX=state.bottle[x].capacity;//dinh cua lo x
	int capY=state.bottle[y].capacity;//dinh cua lo y
	int colorX=state.bottle[x].a[capX];//mau cua dinh lo x
	int colorY=state.bottle[y].a[capY];//mau cua dinh lo y
	if(!empty_bottle(state,x)&&capY!=max_bottle-1){
		copy_bottle(result,state);
		int cnt=cnt_color(state,x);//dem tong so mau cua dinh lo x
		if(colorX==colorY||colorY==0){
			for(i=0;i<cnt;i++){
				capX=state.bottle[x].capacity;
				if(!empty_bottle(*result,x)&&(capY+i!=max_bottle-1)) pour1color(result,x,y);
			}
			return 1;		
		}
	}
	return 0;
}

const int array_action[7][7]={
0,1,2,3,4,5,6
,7,0,8,9,10,11,12
,13,14,0,15,16,17,18
,19,20,21,0,22,23,24
,25,26,27,28,0,29,30
,31,32,33,34,35,0,36
,37,38,39,40,41,42,0
};
const char* action[]={"Firs State"
,"POUR 0 TO 1","POUR 0 TO 2","POUR 0 TO 3","POUR 0 TO 4","POUR 0 TO 5","POUR 0 TO 6"
,"POUR 1 TO 0","POUR 1 TO 2","POUR 1 TO 3","POUR 1 TO 4","POUR 1 TO 5","POUR 1 TO 6"
,"POUR 2 TO 0","POUR 2 TO 1","POUR 2 TO 3","POUR 2 TO 4","POUR 2 TO 5","POUR 2 TO 6"
,"POUR 3 TO 0","POUR 3 TO 1","POUR 3 TO 2","POUR 3 TO 4","POUR 3 TO 5","POUR 3 TO 6"
,"POUR 4 TO 0","POUR 4 TO 1","POUR 4 TO 2","POUR 4 TO 3","POUR 4 TO 5","POUR 4 TO 6"
,"POUR 5 TO 0","POUR 5 TO 1","POUR 5 TO 2","POUR 5 TO 3","POUR 5 TO 4","POUR 5 TO 6"
,"POUR 6 TO 0","POUR 6 TO 1","POUR 6 TO 2","POUR 6 TO 3","POUR 6 TO 4","POUR 6 TO 5"
};

//ham heuristic dem so mau giong nhau trong cung 1 lo
int hrt1(State state){
	int i,j,count=0;
	for(i=0;i<=max_bottle;i++){
		int capi=state.bottle[i].capacity;
		for(j=1;j<capi;j++)
			if(state.bottle[i].a[j]!=0)//neu lo khong rong, dem so mau giong nhau trong lo
				if(state.bottle[i].a[j]==state.bottle[i].a[j+1])
					count++;
	}
	return count;		
} 

typedef struct Node{
	State state;
	struct Node* parent;
	int no_func;
	int hrt;
}Node;

typedef struct{
	Node * E[max];
	int size;
}List;

void nullList(List *list){
	list->size=0;
}

int emptyList(List list){
	return list.size==0;
}

int fullList(List list){
	return list.size==max;
}

Node* elm_at(int p,List list){
	return list.E[p-1];
}

void pushList(Node* x,int pos,List *list){
	if(!fullList(*list)){
		int q;
		for(q=list->size;q>=pos;q--)
			list->E[q]=list->E[q-1];
		list->E[pos-1]=x;
		list->size++;
	}
	else printf("List is full\n");
}

void deleteList(int pos,List *list){
	if(emptyList(*list))
		printf("List is empty\n");
	else if(pos<1||pos>list->size)
		printf("Position is not possible to delete\n");
	else{
		int i;
		for(i=pos-1;i<list->size;i++)
			list->E[i]=list->E[i+1];
		list->size--;
	}	
}

Node* findState(State state,List list,int *pos){
	int i;
	for(i=1;i<=list.size;i++)
		if(compare(elm_at(i,list)->state,state)){
			*pos=i;
			return elm_at(i,list);
		}
	return NULL;
}

void sortList(List *list){
	int i,j;
	for(i=0;i<list->size-1;i++)
		for(j=i+1;j<list->size;j++)
			if(list->E[i]->hrt<list->E[j]->hrt){
				Node* node=list->E[i];
				list->E[i]=list->E[j];
				list->E[j]=node;
			}
}

void print_Ways(Node* node){
	List listPrint;
	nullList(&listPrint);
	while(node->parent!=NULL){
		pushList(node,listPrint.size+1,&listPrint);
		node=node->parent;
	}
	pushList(node,listPrint.size+1,&listPrint);
	int no_action=0,i;
	for(i=listPrint.size;i>0;i--){
		printf("\nAction %d: %s\n",no_action,action[elm_at(i,listPrint)->no_func]);
		printState(elm_at(i,listPrint)->state);
		no_action++;
	}
}

Node* BFS(State state){
	List open,close;
	nullList(&open);
	nullList(&close);
	Node* root=(Node*)malloc(sizeof(Node));
	root->state=state;
	root->parent=NULL;
	root->no_func=0;
	root->hrt=hrt1(root->state);
	pushList(root,open.size+1,&open);
	while(!emptyList(open)){
		Node* node = elm_at(1,open);
		deleteList(1,&open);
		pushList(node,close.size+1,&close);
		if(goalcheck(node->state))
			return node;
		int i,j;
		for(i=0;i<=max_bottle;i++){
			for(j=0;j<=max_bottle;j++){
				if(i!=j){
					State newstate;
					newstate = node->state;
					if(pourXtoY(node->state,&newstate,i,j)){
						Node* newNode = (Node*)malloc(sizeof(Node));
						newNode->state=newstate;
						newNode->parent=node;
						newNode->hrt=hrt1(newstate);
						newNode->no_func=array_action[i][j];
						int posOpen,posClose;
						Node* nodeFoundOpen=findState(newstate,open,&posOpen);
						Node* nodeFoundClose=findState(newstate,close,&posClose);
						if(nodeFoundOpen==NULL&&nodeFoundClose==NULL){
							pushList(newNode,open.size+1,&open);
						}
						else if(nodeFoundOpen!=NULL&&nodeFoundOpen->hrt<newNode->hrt){
							deleteList(posOpen,&open);
							pushList(newNode,posOpen,&open);
						}
						else if(nodeFoundClose!=NULL&&nodeFoundClose->hrt<newNode->hrt){
							deleteList(posClose,&close);
							pushList(newNode,open.size+1,&open);
						}
						sortList(&open);
					}
				}
			}
		}
	}
	return NULL;
}

//random trong pham vi
int GetRandom(int min,int maxn){
    return min + (int)(rand()*(maxn-min+1.0)/(1.0+RAND_MAX));
}

//random trang thai voi so n mau va n+1 lo  
State random_state(int n){
	int m=n-1;
	State state;
	int i,j;
	int a[m];
	for(i=0;i<=n;i++)	
		state.bottle[i].capacity=0;//khoi tao luong nuoc trong n bang 0
	for(i=1;i<=m;i++)
		a[i]=m;//khoi tao mang a de luu gia toi da cua random
	state.bottle[0].capacity=0;
	state.bottle[1].capacity=0;
	for(i=1;i<=m;i++){//khoi tao lo 0,1 rong
		state.bottle[0].a[i]=0;
		state.bottle[1].a[i]=0;
	}
	for(i=2;i<=n;i++){//khoi tao mau cho lo 2 den lo n 
		j=1;
		do{
			int rand=GetRandom(1,m);
			if(a[rand]!=0){
				a[rand]--;
				state.bottle[i].a[j]=rand;
				j++;
				state.bottle[i].capacity++;
			}
			
		}while(state.bottle[i].capacity!=m);//random mau cho den khi day lo i
	}
	return state;
}

int main(){
	int i,j,k,f,g=0;
	printf("\nVui long chon do kho cho tro choi.\n");
	printf("\nDo kho 1: 3 lo day va 2 lo trong\n");
	printf("\nDo kho 2: 4 lo day va 2 lo trong\n");
	printf("\nDo kho 3: 5 lo day va 2 lo trong\n");
	printf("\nVui long chon do kho:");
	scanf("%d",&k);
	if(k==1) max_bottle=4;
	else if(k==2) max_bottle=5;
	else max_bottle=6;
	system("cls");
	State state[40],result;
	for(i=0;i<40;i++)
		state[i]=random_state(max_bottle);
	Node* root=(Node*)malloc(sizeof(Node));
	root->state=state[0];
	root->parent=NULL;
	root->no_func=0;
	root->hrt=hrt1(root->state);
	while(1){
		printf("Current State\n\n");
		printState(root->state);
		printf("\nVui long chon phuon an ma ban muon:\n");
		printf("\n0 :Thoat tro choi.\n");
		printf("\n1 :Loi giai tu dong.\n");
		printf("\n2 :Tu minh giai tro choi.\n");
		printf("\n3 :Doi trang thai ban co.\n");
		printf("\nPhuong an cua ban:");
		scanf("%d",&k);
		system("cls");
		if(k==0) break;
		else if(k==1){
			printf("\nLoi giai cua tro choi\n");
			Node* p=BFS(root->state);
			print_Ways(p);
			printf("\n----------WIN----------\n");
			printf("\nBan co muon tiep tuc khong!\n");
			printf("\n0: Khong tiep tuc va thoat khoi tro choi.\n");
			printf("\n1 :Tiep tuc voi trang thai moi.\n");
			printf("\nPhuong an cua ban:");
			scanf("%d",&f);
			if(f==0) break;
			else{
				root->state=state[g+1];
				root->parent=NULL;
				root->no_func=0;
				root->hrt=hrt1(root->state);
				g++;
				system("cls");
			}
		}
		else if(k==2){
			while(1){
				system("cls");
				if(root->parent!=NULL){
					printf("\nPrevious State\n\n");
					printState(root->parent->state);	
				}
				printf("\n%s\n\n",action[root->no_func]);
				printState(root->state);
				if(root->parent!=NULL){
					printf("\nBan co muon tiep tuc:\n");
					printf("\n0: Thoat tro choi.\n");
					printf("\n1: Tiep tuc.\n");
					printf("\n2: Tro ve trang thai truoc.\n");
					printf("\nMoi ban chon:");
					scanf("%d",&f);
				}
				if(f==0){
					system("cls");
					break;						
				} 
				else if(f==1);
				else if(f==2){
					while(1){
						system("cls");
						root=root->parent;
						printf("\nPrevious Current\n\n");
						printState(root->state);
						printf("\nBan co muon tip tuc quay lai trang thai truoc?\n");
						printf("\n0 :Choi tiep.\n");
						printf("\n1 :Tiep tuc trang thai truoc.\n");
						printf("\nMoi ban chon:");
						scanf("%d",&f);
						if(f==0) break;
						else if(f==1);
					}
				} 
				printf("\nBan muon do lo nao vao lo nao?\n");
				scanf("%d%d",&i,&j);
				State newstate;
				if(pourXtoY(root->state,&newstate,i,j)){
					Node* newNode = (Node*)malloc(sizeof(Node));
					newNode->state=newstate;
					newNode->parent=root;
					newNode->hrt=hrt1(newstate);
					newNode->no_func=array_action[i][j];
					root=newNode;
				}
				else continue;	
				if(goalcheck(root->state)){
					system("cls");
					printf("---WIN---");
					printf("\nBan co muon tiep tuc khong!\n");
					printf("\n0: Khong tiep tuc va thoat khoi tro choi.\n");
					printf("\n1 :Tiep tuc voi trang thai moi.\n");
					printf("\nMoi ban chon:");
					scanf("%d",&f);
					if(f==0) break;
					else if(f==1){
						root->state=state[g+1];
						root->parent=NULL;
						root->no_func=0;
						root->hrt=hrt1(root->state);
						g++;
					}
				}
			}
		}
		else if(k==3){
			root->state=state[g+1];
			root->parent=NULL;
			root->no_func=0;
			root->hrt=hrt1(root->state);
			g++;
		}
		else printf("\nKhong co phuong an ban da chon, vui long chon lai phuong an\n");
	}
	printf("\n---Chao tam biet---\n");
	return 0;
}
