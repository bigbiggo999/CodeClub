#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include <sys/types.h>
#include<sys/syscall.h>
/*本次实验模拟实现操作系统中进程调度算法,模拟进程在不同时刻到达的情况*/
#define PNUM  5 //进程的数量
#define TIMER 10 //定时器,最长CPU区间时间
#define SLICE 2//轮转算法的时间片
 int timenow=0;     //当前时刻
typedef struct node{
   int pid;   //进程号
   int priority;//进程优先级,1~3,数字越小优先级越高
   int arrival; //到达时间
   int burst;  //CPU区间时间
   int rest;  //剩余时间
   char state;//进程状态,'N'新建,'R'运行,'W'等待CPU(就绪),'T'终止
   struct node *next;
}PCB;

int gantt[TIMER*PNUM]={0}; //用一个gantt数组记录调度过程,每个时刻调度的进程号 

PCB *job;//所有作业的序列,带头节点(为简化编程)
PCB *ready=NULL; //进程就绪队列,不带头节点
PCB *tail=NULL;  //记录就绪队列的尾节点
PCB *run=NULL;//正在运行中的进程,不带头结点
PCB *finish=NULL;//已经结束的程序,不带头结点
PCB *cur=NULL;
void InitialJob()
{   
    int i=0;
    PCB *p,*tail;
    job=(PCB *)malloc(sizeof(PCB));//生成头节点,其它域无意义
    job->next=NULL;
    tail=job;
    
 for(i=0;i<PNUM;i++)
 { p=(PCB *)malloc(sizeof(PCB));//生成新节点(新进程)
   p->pid=i+1;
   p->priority=rand()%3+1;//随机生成优先级:1~3
   p->arrival=rand()%TIMER;//随机生成到达时刻0-9,(预计到达就绪队列的时间) 
   p->burst=rand()%TIMER+1;//随机生成CPU区间时间:1~10;(估计运行时间)
   p->rest=p->burst;
   p->state='N';//初始化进程的状态为'新建'
   p->next=NULL; 
   tail->next=p; 
   tail=p;  //带头结点
  }
}
void DisplayPCB(PCB *pcb) //显示队列
{
  struct node *p=pcb;
  if(pcb==NULL) {printf("XXXXXX\n");return;}
  printf("进程号 优先级 到达时刻 区间时间 剩余时间 进程状态\n");
  do{
    printf("P%-3d\t",p->pid);
    printf("%3d\t",p->priority);
    printf("%3d\t",p->arrival);
    printf("%3d\t",p->burst);
	printf("%3d\t",p->rest);
	printf("%3c\t",p->state);
    printf("\n");
    p=p->next;
  }while(p!=NULL);   
}

void DisplayGantt() //显示甘特数组
{
  int i=0;
  for(i=0;i<timenow;i++)
  {
   if(gantt[i]==0) printf("空闲,");
   else
     printf("P%d,",gantt[i]);
  }
  printf("\n");
}

/*注:关于周转时间,等待时间与响应时间的概念释疑:
  在课程教材<操作系统概念第7版>中(P141),上述三个时间是从进程到达的时间开始的.
  在辅助教材<现代操作系统第4版>中(P89),上述三个时间时从进程提交的时刻(0时刻)开始的.
  国内普遍接受前一种理解,本程序以课程教材中的解释为准来计算时间.
*/
void DisplayTime() //显示周转时间t,等待时间w和响应时间r
{
    int t=0,w=0,r=0;
    float t_avg=0,w_avg=0,r_avg=0; 
    int i,j;
    PCB *p; //用p遍历finish队列,查找进程Pi的到达时间,调用该函数时所有进程都已放入finish队列
    if(finish==NULL) {return;}
    printf("进程号    周转时间    等待时间    响应时间\n");
    for(i=1;i<=PNUM;i++)
     { p=finish;
       while(p->pid!=i) p=p->next;
       j=0;
       while(gantt[j]!=i) j++; //遍历甘特数组,求进程Pi的响应时间
        r=j;  //响应时刻
        t=j+1;        
       for(j=r+1;j<timenow;j++) //继续遍历,求周转时间
       { if(i==gantt[j]) t=j+1;}//结束时刻
       r=r-p->arrival;  //响应时间=响应时刻-到达时刻
       t=t-p->arrival; //周转时间=结束时刻-到达时刻
       w=t-p->burst; //等待时间=周转时间-运行时间
       r_avg+=(float)r/PNUM; //平均响应时间
       w_avg+=(float)w/PNUM;  //平均等待时间
       t_avg+=(float)t/PNUM;   //平均周转时间
       
       printf("P%d       %4d       %4d       %4d\n",i,t,w,r);
     }
     printf("平均周转时间:%.2f,平均等待时间%.2f,平均响应时间%.2f\n",t_avg,w_avg,r_avg);
}
void ReadyQueue(char * algo,int t) //根据作业队列构造就绪队列,algo:算法,t:当前时刻
{   
   struct node *jpre,*jcur,*rpre,* rcur; 
   int j,r,a=0;         
   if(strcmp(algo,"FCFS")==0||strcmp(algo,"RR")==0)//FCFS和RR的就绪队列的构造方式相同
      a=0;
   else if(strcmp(algo,"SJF")==0)  //非抢占SJF
      a=1;
   else if(strcmp(algo,"SRTF")==0)  //抢占式SJF,最短剩余时间优先
      a=2;
   else if(strcmp(algo,"Priority")==0||strcmp(algo,"NonPriority")==0)//抢占和非抢占优先级 
      a=3;
   else {printf("ReadyQueue()函数调用参数错误!\n");exit(0);}
    if(job->next==NULL) {printf("作业队列为空!\n");return;}  
    jpre=job;
    jcur=job->next;
    while(jcur!=NULL) //遍历作业序列中选择已到达进程,将其从作业队列移入就绪队列,直到作业队列为空 	  
    {  
      if(jcur->arrival<=t) //如果当前时刻进程已经到达,则将其插入到就绪队列的合适位置
      {
         printf("P%d到达.\n",jcur->pid);
	     jpre->next=jcur->next;  //将jcur从作业队列移除
         jcur->state='W';//将进程状态设置为就绪
 	     if(ready==NULL) //就绪队列为空
         {jcur->next=NULL;ready=jcur;tail=ready;}
         else  //就绪队列不为空,遍历就绪队列,将jcur插入到合适位置
         {	 rpre=ready;
             rcur=ready;
              switch (a){ //遍历就绪队列查找插入点
                case 0:    //FCFS,RR.根据到达时间arrival查找合适插入点
                        while((rcur!=NULL)&&(jcur->arrival>=rcur->arrival)) 
                             {rpre=rcur;rcur=rcur->next;}
						break;
                case 1: //SJF,根据区间时间burst查找合适插入点 
                         while((rcur!=NULL)&&(jcur->burst>=rcur->burst))
                               {rpre=rcur;rcur=rcur->next;}
						 break;
                case 2:  //STRF,根据剩余时间rest查找合适插入点
						while((rcur!=NULL)&&(jcur->rest>=rcur->rest))
                               {rpre=rcur;rcur=rcur->next;}
						 break;
                case 3:  //Priority, Non-Priority,根据优先级查找合适插入点
						while((rcur!=NULL)&&(jcur->priority>=rcur->priority)) 
                             {rpre=rcur;rcur=rcur->next;}
                       
                        break;
                default: break;
              }
             if(rcur==NULL)// 插入点在就绪队列尾部
             { 
               jcur->next=NULL;
               rpre->next=jcur;
               tail=jcur;
			 }
			 else if(rcur==ready) //插入点在头部
             {
                jcur->next=rcur;
                ready=jcur; 
             }
             else //插入到rpre和rcur之间
             {   
                jcur->next=rcur;
                rpre->next=jcur;  
             }
          }
          jcur=jpre->next;  //下一个作业
       }else   //当前作业未达到
       {jpre=jcur;jcur=jcur->next;} //下一个作业
     }
        printf("\n作业队列:\n");
        DisplayPCB(job->next); 
}
void FCFS( )
{  
   timenow=0;
   while(true){
     printf("\n当前时刻:%d\n",timenow);
     ReadyQueue("FCFS",timenow);//刷新就绪队列
     printf("就绪队列:\n");
     DisplayPCB(ready);
     if(job->next==NULL&&ready==NULL&&run==NULL) break; //没有进程,结束循环   
     if(ready!=NULL||run!=NULL) //有进程处于就绪或者运行状态
     {
        if(run==NULL)//若CPU空闲
		    {        
		      run=ready;      //将CPU分配给ready队列的第一个进程
     	    ready=ready->next; 
     	    run->next=NULL;
          printf("\nP%d被调度程序分派CPU!\n",run->pid);        
        }
     	run->rest--;    //修改进程PCB
     	run->state='R';
        printf("\nP%d正在运行.......\n",run->pid);
    	printf("运行进程:\n");
     	DisplayPCB(run);
        gantt[timenow]=run->pid; //记录当前时刻调度进程的ID号
     	if(run->rest==0)
     	{   printf("\nP%d结束!\n",run->pid);
            run->state='T';   
       		run->next=finish;   //新完成的节点插入到finish的头结点,简单一点
       		finish=run;
       		run=NULL;
            printf("结束进程队列:\n");
      		DisplayPCB(finish);           
      	}           	
      }
      timenow++; //下一时刻继续扫描作业队列  
   }
}
void RR(int slice)  //时间片作为参数
{
   timenow=0;
   int count=0; //时间片计数,count==slice表示进程已经运行一个时间片
   while(true){
     printf("\n当前时刻:%d\n",timenow);
     ReadyQueue("RR",timenow);//刷新就绪队列
     printf("就绪队列:\n");
     DisplayPCB(ready);
    
     if(job->next==NULL&&ready==NULL&&run==NULL) {break;} //没有进程,结束循环 
     if(ready==NULL) {tail=NULL;}
     if(tail!=NULL) printf("就绪队列尾节点:P%d\n",tail->pid);  
     if(ready!=NULL||run!=NULL) //有进程处于就绪或者运行状态
     {
		 if(run==NULL)//若CPU空闲
		{        
		 run=ready;      //将CPU分配给ready队列的第一个进程
     	 ready=ready->next; 
     	 run->next=NULL;
         printf("\nP%d被调度程序分派CPU!\n",run->pid);        
        }
		count++;
		run->rest--;
		run->state='R';
		printf("\nP%d正在运行.......\n",run->pid);
		printf("运行进程:\n");
		DisplayPCB(run);
		gantt[timenow]=run->pid; //记录当前时刻调度进程的ID号
		if(run->rest==0){
			printf("\nP%d结束!\n",run->pid);
            run->state='T';   
       		run->next=finish;   //新完成的节点插入到finish的头结点,简单一点
       		finish=run;
       		run=NULL;
            printf("结束进程队列:\n");
      		DisplayPCB(finish);
		}
		else if(count%slice==0){
			if(ready==NULL){
				ready=run;
				tail=run;
				run->state='W';
				run=NULL;
			}
			else{
				tail->next=run;
				tail=run;
				tail->next=NULL;
				run->state='W';
				run=NULL;
			}
		}
       //这里加入RR调度的代码
      }     	 
      timenow++; //下一时刻继续扫描作业队列  
   }
}
void SJF() //课后完成,构造就绪队列时按照短作业排序
{
	timenow=0;
	while(true){
		printf("\n当前时刻:%d\n",timenow);
		ReadyQueue("SJF",timenow);//刷新就绪队列
		printf("就绪队列:\n");
		DisplayPCB(ready);
		if(job->next==NULL&&ready==NULL&&run==NULL) break; //没有进程,结束循环   
		if(ready!=NULL||run!=NULL) //有进程处于就绪或者运行状态
		{
			if(run==NULL)//若CPU空闲
			{        
			run=ready;      //将CPU分配给ready队列的第一个进程
			ready=ready->next; 
			run->next=NULL;
			printf("\nP%d被调度程序分派CPU!\n",run->pid);        
			}
			run->rest--;    //修改进程PCB
			run->state='R';
			printf("\nP%d正在运行.......\n",run->pid);
			printf("运行进程:\n");
			DisplayPCB(run);
			gantt[timenow]=run->pid; //记录当前时刻调度进程的ID号
			if(run->rest==0)
			{   printf("\nP%d结束!\n",run->pid);
				run->state='T';   
				run->next=finish;   //新完成的节点插入到finish的头结点,简单一点
				finish=run;
				run=NULL;
				printf("结束进程队列:\n");
				DisplayPCB(finish);           
			}           	
		}
		timenow++; //下一时刻继续扫描作业队列  
	}
}
void SRTF() //最少剩余时间优先,构造就绪队列时按照最短剩余时间作业排序
{
	timenow=0;
	while(true){
		printf("\n当前时刻:%d\n",timenow);
		ReadyQueue("SRTF",timenow);//刷新就绪队列
		printf("就绪队列:\n");
		DisplayPCB(ready);
		if(job->next==NULL&&ready==NULL&&run==NULL) break; //没有进程,结束循环   
		if(ready!=NULL||run!=NULL) //有进程处于就绪或者运行状态
		{
			if(run==NULL)//若CPU空闲
			{        
				run=ready;      //将CPU分配给ready队列的第一个进程
				ready=ready->next; 
				run->next=NULL;
				printf("\nP%d被调度程序分派CPU!\n",run->pid);        
			}
			else if(ready!=NULL&&ready->rest < run->rest){
					tail=ready;
                    if(ready->next!=NULL){
                        while(tail->next->rest < run->rest&&tail->next!=NULL){
                        tail=tail->next;
                        }
                    }
					run->next=tail->next;
					tail->next=run;
					run=ready;
          run->next=NULL;
          ready=ready->next;
          run->state='W';
				}
			run->rest--;    //修改进程PCB
			run->state='R';
			printf("\nP%d正在运行.......\n",run->pid);
			printf("运行进程:\n");
			DisplayPCB(run);
			gantt[timenow]=run->pid; //记录当前时刻调度进程的ID号
			if(run->rest==0)
			{   printf("\nP%d结束!\n",run->pid);
				run->state='T';   
				run->next=finish;   //新完成的节点插入到finish的头结点,简单一点
				finish=run;
				run=NULL;
				printf("结束进程队列:\n");
				DisplayPCB(finish);           
			}			
		}
		timenow++; //下一时刻继续扫描作业队列  
	}
}
void NonPriority() //非抢占优先级
{  
   timenow=0;
   while(true){
     printf("\n当前时刻:%d\n",timenow);
     ReadyQueue("NonPriority",timenow);//刷新就绪队列
     printf("就绪队列:\n");
     DisplayPCB(ready);
     if(job->next==NULL&&ready==NULL&&run==NULL) break; //没有进程,结束循环   
     if(ready!=NULL||run!=NULL) //有进程处于就绪或者运行状态
     {
        if(run==NULL)//若CPU空闲
		{        
		 run=ready;      //将CPU分配给ready队列的第一个进程
     	 ready=ready->next; 
     	 run->next=NULL;
         printf("\nP%d被调度程序分派CPU!\n",run->pid);        
        }
     	run->rest--;    //修改进程PCB
     	run->state='R';
        printf("\nP%d正在运行.......\n",run->pid);
    	printf("运行进程:\n");
     	DisplayPCB(run);
        gantt[timenow]=run->pid; //记录当前时刻调度进程的ID号
     	if(run->rest==0)
     	{   printf("\nP%d结束!\n",run->pid);
            run->state='T';   
       		run->next=finish;   //新完成的节点插入到finish的头结点,简单一点
       		finish=run;
       		run=NULL;
            printf("结束进程队列:\n");
      		DisplayPCB(finish);           
      	}           	
      }
      timenow++; //下一时刻继续扫描作业队列  
   }
}
void Priority()//抢占式优先级
{
	timenow=0;
	while(true){
		printf("\n当前时刻:%d\n",timenow);
		ReadyQueue("Priority",timenow);//刷新就绪队列
		printf("就绪队列:\n");
		DisplayPCB(ready);
		if(job->next==NULL&&ready==NULL&&run==NULL) break; //没有进程,结束循环   
		if(ready!=NULL||run!=NULL) //有进程处于就绪或者运行状态
		{
			if(run==NULL)//若CPU空闲
			{        
				run=ready;      //将CPU分配给ready队列的第一个进程
				ready=ready->next; 
				run->next=NULL;
				printf("\nP%d被调度程序分派CPU!\n",run->pid);        
			}
			else if(ready!=NULL&&ready->priority < run->priority){
					tail=ready;
                    if(ready->next==NULL){
                    }
                    else {
                        while(tail->next->priority < run->priority&&tail->next!=NULL){
                        tail=tail->next;
                        }
                    }
					run->next=tail->next;
					tail->next=run;
					run=ready;
                    ready=ready->next;
                    run->next=NULL;
                    run->state='W';
				}
			run->rest--;    //修改进程PCB
			run->state='R';
			printf("\nP%d正在运行.......\n",run->pid);
			printf("运行进程:\n");
			DisplayPCB(run);
			gantt[timenow]=run->pid; //记录当前时刻调度进程的ID号
			if(run->rest==0)
			{
                printf("\nP%d结束!\n",run->pid);
				run->state='T';   
				run->next=finish;   //新完成的节点插入到finish的头结点,简单一点
				finish=run;
				run=NULL;
				printf("结束进程队列:\n");
				DisplayPCB(finish);           
			}			
		}
		timenow++; //下一时刻继续扫描作业队列  
	}
}
int main()
{    srand((int)time(NULL)); //随机数种子
     //srand(0); 
     InitialJob();
     DisplayPCB(job->next);
     //RR(SLICE);
     //FCFS();
     //SJF();
     //SRTF(); 
     //NonPriority();
     Priority();      
     DisplayGantt();
     DisplayTime();
     return EXIT_SUCCESS;;
}
