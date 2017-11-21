
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node *next;
}Node;

typedef struct{
    Node* rear;
}Queue;

int InintQueue(Queue *Q){
    Q->rear=(Node *)malloc(sizeof(Node));
    if(!Q->rear) return(0);
    Q->rear->next=Q->rear;
    return 1;
}

int EnQueue(Queue *Q,int *e){
    Node* p;
    p=(Node*)malloc(sizeof(Node));
    p->data=*e;
    p->next=Q->rear->next;
    Q->rear->next=p;    //
    Q->rear=p;   //
    return 1;
}
int DeQueue(Queue *Q,int *e){
    Node* p;
    if(Q->rear->next==Q->rear)
        return 0;
    p=Q->rear->next->next;
    *e=p->data;
    Q->rear->next->next=p->next;
    if(Q->rear==p){ // 如果去除的是最后一个有值的节点。
        Q->rear->next=Q->rear->next->next;
        Q->rear=Q->rear->next;
    }
    free(p);
    return 1;
}

int main(){
    Queue Q;
    int i,*e,n;
    e = (int *)malloc(sizeof(int));
    InintQueue(&Q);
    printf("需要入队列的数的个数为 :");
    scanf("%d",&n);
    for(i=0;i<n;i++){
        printf("输入一个数:");
        scanf("%d",e);
        EnQueue(&Q,e);
    }
    printf("需要出队列的数的个数:");
    scanf("%d",&n);
    for(i=0;i<n;i++){
        DeQueue(&Q,e);
        printf("%d\n",*e);
    }
    return 0;
    
}
