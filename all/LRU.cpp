#include <iostream>
using namespace std;
struct Node{
    int value;
    Node*next;
}*head,*tail;    
void AddNode(int d)
{
    Node*temp=new Node;
    temp->value=d;
    temp->next=NULL;
    tail->next=temp; 
    tail=temp;   
//    cout<<"Add "<<d<<endl;
}    
void Del(Node*prev,Node*current)
{
    if (current==head)
    {
        head=current->next;
    }    
    else
    {
        prev->next=current->next;
    }    
 //   cout<<"Delete "<<current->value<<endl;
    delete current; 
}    
void replace(int stackSize,int d,int& replaceCnt)
{
    Node*temp=head;
    Node*prev;
    int cnt=0;
    while(temp)
    {
        if (temp->value==d)
        {
             Del(prev,temp);
             AddNode(d);
             break;       
        }    
        cnt++;
        prev=temp;
        temp=temp->next;
    }    
    if (temp==NULL)
    {
        if (cnt==stackSize) //replace page
        {
            Del(prev,head);
            replaceCnt++;
        }    
        AddNode(d);
    }    
}    
void printStack(Node*head)
{
    Node*temp=head;
    while(temp)
    {
        cout<<temp->value<<" ";
        temp=temp->next;
    }    
    cout<<endl;
}    
int main()
{
    int stackSize=4;
    int data[]={0,1,2,1,3,5,2,1,4,2,0,4,2,3,1,5,2,3,0};
    int n=19;
    int replaceCnt=0;
    for (int i=0;i<n;i++)
    {
        if (i==0)
        {
            head=new Node;
            head->value=data[i];
            head->next=NULL;
            tail=head;
        }    
        else
        {
             replace(stackSize,data[i],replaceCnt);         
        }
        cout<<data[i]<<":";           
        printStack(head); 
   
     }    
     cout<<"Replace Count:"<<replaceCnt<<endl;
    system("pause");
 
       return 0;
}    
