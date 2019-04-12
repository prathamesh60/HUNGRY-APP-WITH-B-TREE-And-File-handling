#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#define MAX 3
#define degree 2
typedef enum{FAILURE,SUCCESS} statuscode;
typedef enum{FALSE,TRUE} boolean;
typedef struct item_name_tag
{
	char name[50];
	int no_of_time;
	struct item_name_tag *next;
}item_name;
typedef struct item_tag
{ char itemname[100];
  float price;
}items;
typedef struct menu_tag
{
   int no_of_items;
   items item[100]; 
}menu;
typedef struct location_tag
{  char name[50];
   char address[100];
   char zone[50];
   int no_of_seats;
   menu *res_menu;
   int category;     //1 for restaurant 2 for cafe 3 for pub
   int cuis_category;//1 for north indian 2 for south indian 3 for continental
}location;
int agent_id_allocator=114;
typedef struct agent_tag
{ int  id;
  char name[50];
  char phone_no[11];
  char area[50];
  boolean is_available;
  int ord_id[21];
  float curr_accu_commi;
  //struct agent_tag *next;
}agent;
typedef struct user_tag
{// char us_id[21];
  char name[50];
  char address[50];
  char phone_no[11];
  //orders *ord;
  item_name *ordered_items;
  //struct user_tag *next;
}user;
int order_id_allocator=1002;
typedef struct order_tag
{ char res_name[50];
  char res_address[50];
  int ord_id;
  int no_of_items;
  int item_index[100];
  int quantity_index[100];
  int ord_time;
  agent *allocated_agent;
  user *username;
  float total_price;
  location *ordered_restaurant;
 // struct order_tag *next;
}orders;
typedef struct location_tree
{
	location all_eatspots[MAX];
	int count;
	boolean isLeaf;
	struct location_tree *child[MAX+1];
}loc_tree;
typedef struct ag_tree
{
	agent all_agents[MAX];
	int count;
	boolean isLeaf;
	struct ag_tree *child[MAX+1];
}agent_tree;
typedef struct us_tree
{
	user all_users[MAX];
	int count;
	boolean isLeaf;
	struct us_tree *child[MAX+1];
}user_tree;
typedef struct or_tree
{
	orders all_orders[MAX];
	int count;
	boolean isLeaf;
	struct or_tree *child[MAX+1];
}order_tree;
menu* create_menu()
{
	menu *lptr=(menu*)malloc(sizeof(menu));
	int n;char name[100];float price;
	printf("Enter the number of items\t:\n");
    scanf("%d",&n);
	lptr->no_of_items=n;
	int i;
	for(i=0;i<n;i++)
	{
		printf("Enter item name\t:\n");
		scanf("%s",name);
		printf("Enter the pricw\t:\n");
		scanf("%f",&price);
		strcpy(lptr->item[i].itemname,name);
		lptr->item[i].price=price;	
	}
	return lptr;
}
void visit_menu(menu *lptr)
{
	int i=0;
	printf("-------------MENU-------------\n");
	printf("No.of items\t:%d\n",lptr->no_of_items);
	while(i<(lptr->no_of_items))
	{
		printf("%d.%s\n",i+1,lptr->item[i].itemname);
		printf("  Price\t:%f\n",lptr->item[i].price);
		i++;
	}
}
loc_tree* makeNode()
{
	loc_tree* nptr=(loc_tree*)malloc(sizeof(loc_tree));
	if(nptr!=NULL)
	{
		nptr->count=0;
		nptr->isLeaf=TRUE;
	}
	return nptr;
}
agent_tree* makeagent()
{
	agent_tree* nptr=(agent_tree*)malloc(sizeof(agent_tree));
	if(nptr!=NULL)
	{
		nptr->count=0;
		nptr->isLeaf=TRUE;
	}
	return nptr;
}
int insert_loc_into_specific_node(loc_tree *nptr,location kptr)
{
	int i,j,ans,temp,x=nptr->count;
	i=0;
	temp=0;
	while(i<x&&temp==0)
	{   
		if((strcmp(kptr.name,nptr->all_eatspots[i].name)>0) || ( strcmp(kptr.name,nptr->all_eatspots[i].name)==0 && strcmp(kptr.address,nptr->all_eatspots[i].address)>0))
			i++;
		else
		{
			temp=1;
			
		}
	}
	ans=i;
	(nptr->count)++;
	if(temp==0)
	{
		nptr->all_eatspots[i]=kptr;
	}
	else
	{
		j=(nptr->count)-1;
		while(j>i)
		{
			nptr->all_eatspots[j]=nptr->all_eatspots[j-1];
			j--;
		}
		nptr->all_eatspots[j]=kptr;
	
	}
	return ans;	
}

loc_tree * split(loc_tree **parent,loc_tree *childr,location value)
{   //printf("SPLIT IS RUNNING\n");
	if(*parent==NULL)
	{ //printf("SPLIT IS RUNNING\n");
	 loc_tree *xptr=makeNode();
     loc_tree *yptr=makeNode();
     loc_tree *zptr=makeNode();
     int x=(childr->count)/2;
     //int cont;
    // printf("count= %d",x);
    // scanf("%d",&cont);
     int i,j,temp,k;
     yptr->count=0;
     for(i=0;i<x;i++)
     { yptr->all_eatspots[i]=childr->all_eatspots[i];
       if(childr->isLeaf==FALSE) 
	   yptr->child[i]=childr->child[i];
       (yptr->count)++;
	 }
	 if(childr->isLeaf==FALSE) 
	 yptr->child[i]=childr->child[i];
	 zptr->count=0;
	 k=i+1,i=0;
	 for(j=x+1;j<childr->count;j++)
	 {zptr->all_eatspots[i]=childr->all_eatspots[j];
	  if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	  i++;
	  k++;
	  (zptr->count)++;
	 }
	 if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	 xptr->all_eatspots[0]=childr->all_eatspots[x];
	 xptr->count=1;
	 xptr->child[0]=yptr;
	 xptr->child[1]=zptr;
	 xptr->isLeaf=FALSE;
	 free(childr);
	 *parent=xptr;
	// printf("Running ");
    // scanf("%d",&cont);
	 if(strcmp(value.name,xptr->all_eatspots[0].name)>0 ||( strcmp(value.name,xptr->all_eatspots[0].name)==0 && strcmp(value.address,xptr->all_eatspots[0].address)>0) )
	  return zptr;
	 else
	  return yptr;
	  
	}
	else
	{loc_tree *yptr=makeNode();
     loc_tree *zptr=makeNode();
	 int x=(childr->count)/2;
	 int i,j,temp,k,k1;
	 yptr->count=0;
     for(i=0;i<x;i++)
     { yptr->all_eatspots[i]=childr->all_eatspots[i];
       if(childr->isLeaf==FALSE); 
       yptr->child[i]=childr->child[i];
       (yptr->count)++;
	 }
	 if(childr->isLeaf==FALSE) 
	 yptr->child[i]=childr->child[i];
	 zptr->count=0;
	 k=i+1,i=0;
	 for(j=x+1;j<childr->count;j++)
	 {zptr->all_eatspots[i]=childr->all_eatspots[j];
	  if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	  i++;
	  k++;
	  (zptr->count)++;
	 }
	 if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	 k1=insert_loc_into_specific_node(*parent,childr->all_eatspots[x]);
	 (*parent)->child[k1]=yptr;
	 
	   j=(*parent)->count;
	   while(j>k1+1)
	   {
	   	(*parent)->child[j]=(*parent)->child[j-1];
	   	j--;
	   }
	   (*parent)->child[j]=zptr;
      free(childr);
	if(strcmp(value.name,(*parent)->all_eatspots[k1].name)>0 || (strcmp(value.name,(*parent)->all_eatspots[k1].name)>0 && strcmp(value.address,(*parent)->all_eatspots[k1].address)>0) )
	  return zptr;
	else
	  return yptr;	
	}	
}


void insert(loc_tree **prev,loc_tree **root,location value)
{
	if(*root==NULL)
	{
		loc_tree* nptr=makeNode();
		int k;
		k=insert_loc_into_specific_node(nptr,value);
		//return nptr;
		*root=nptr;
	}
	else if((*root)->count==MAX)
	{
		if(*prev==NULL)
		{ //printf("SPLIT IS RUNNING\n");
		 *root=split(prev,*root,value);
		 *root=*prev;
		 *prev=NULL;
		 insert(prev,root,value);
	    }
	    else
	     { *root=split(prev,*root,value);
	     	insert(prev,root,value);
		 }
	    
	}
	else if((*root)->isLeaf==TRUE)
	{
		int k=insert_loc_into_specific_node(*root,value);
	}
	else
	{ //prev=root;
	  int i=0,temp=0,x=(*root)->count;
	  while(i<x && temp==0)
	  { if(strcmp(value.name,(*root)->all_eatspots[i].name)>0 || ( strcmp(value.name,(*root)->all_eatspots[i].name)>0 && strcmp(value.address,(*root)->all_eatspots[i].address)>0))
	  	i++;
	  	else
	  	temp=1;
	  }
	  if(temp==1)
	  { 
	    insert(root,&((*root)->child[i]),value);	
	  }
	  else
	  { insert(root,&((*root)->child[(*root)->count]),value);
	  }
		
	}
}

void print_location(location *nptr)
{
	printf("\n*************************************\n");
	printf("Eating spot name:%s\n",nptr->name);
	printf("Address\t\t:%s\n",nptr->address);
	printf("Zone\t\t:%s\n",nptr->zone);
	if(nptr->category==1)
	{
		printf("Category\t:Restaurant\n");
	}
	else if(nptr->category==2)
	{
		printf("Category\t:Cafe\n");
	}
	else if(nptr->category==3)
	{
		printf("Category\t:Pub\n");
	}
		if(nptr->cuis_category==1)
	{
		printf("Cuisine\t\t:NORTH INDIAN\n");
	}
	else if(nptr->cuis_category==2)
	{
		printf("Cuisine\t\t:SOUTH INDIAN\n");
	}
	else if(nptr->cuis_category==3)
	{
		printf("Cuisine\t\t:CONTINENTAL\n");
	}
	printf("No. of seats\t:%d\n",nptr->no_of_seats);
	menu *lptr=nptr->res_menu;
	visit_menu(lptr);
	printf("*************************************\n");
}
statuscode insert_eating_location(loc_tree **root,char res_zone[50],char res_name[50],char res_address[50],int res_no_of_seats,int res_category,int res_cuis_category, menu *res_res_menu)
{
    printf("Enter the name of your restaurant\t:\n");
    scanf("%s",res_name);
    printf("Enter the address of your restaurant\t:\n");
    //strcpy(res_address,"NAGPUR");
    scanf("%s",res_address);
    printf("Enter the zone of your restaurant\t:\n");
    //strcpy(res_zone,"EAST");
    scanf("%s",res_zone);
   // res_no_of_seats=100;
    printf("Enter the number of seats in your restaurant\t:\n");
	scanf("%d",&res_no_of_seats);
    res_res_menu=create_menu();
    printf("Enter the category of your restaurant\t:\n");
    printf("PRESS 1 FOR RESTAURANT\nPRESS 2 FOR CAFE\nPRESS 3 FOR PUB\n");
    scanf("%d",&res_category);
    printf("Enter the cuisine of your restaurant\t:\n");
    printf("PRESS 1 FOR NORTH INDIAN\nPRESS 2 FOR SOUTH INDIAN\nPRESS 3 FOR CONTINENTAL\n");
    scanf("%d",&res_cuis_category);
    
    location *nptr=	(location*)malloc(sizeof(location));
	statuscode sc=FAILURE;
	if(nptr!=NULL)
	{
		sc=SUCCESS;
		strcpy(nptr->name,res_name);
		strcpy(nptr->address,res_address);
		strcpy(nptr->zone,res_zone);
		nptr->no_of_seats=res_no_of_seats;
		nptr->res_menu=res_res_menu;
		nptr->category=res_category;
		nptr->cuis_category=res_cuis_category;
	    loc_tree *prev=NULL;
	    //print_location(nptr);
		insert(&prev,root,*nptr);
	    sc=SUCCESS;
	}
  return sc;
}

void Traverse(loc_tree *root)
{ if(root==NULL)
  printf("NO restaurants:\n");
  else
  {
  int i=0,x=root->count;
  while(i<x)
   { if(root->isLeaf==FALSE)
      { Traverse(root->child[i]);
         
	  }
	 print_location(&(root->all_eatspots[i]));
	 i++; 
   }
   if(root->isLeaf==FALSE)
   Traverse(root->child[i]);
  }
  // printf("node size=%d",x);
}

void search_by_Category_Util(loc_tree *root,int category)
{
   if(root==NULL)
   printf("NOT Found:\n");
   else
   {
   int i=0,x=root->count;
   	while(i<x)
   { if(root->isLeaf==FALSE)
      { 
	  	search_by_Category_Util(root->child[i],category);
	  }
	  if(category==root->all_eatspots[i].category)
	  {
	 	printf("Name\t:%s\tAddress\t:%s",root->all_eatspots[i].name,root->all_eatspots[i].address);
	  }
	 i++; 
    }
   	if(root->isLeaf==FALSE)
  	search_by_Category_Util(root->child[i],category);
    }
  // printf("node size=%d",x);
}
void search_by_Category(loc_tree *root)
{
	int category;
	printf("Enter the category you are searching for\t:\nPRESS 1 FOR RESTAURANT\nPRESS 2 FOR CAFE\nPRESS 3 FOR PUB\n");
	scanf("%d",&category);
	search_by_Category_Util(root,category);
}
void search_by_Cuisine_Util(loc_tree *root,int cuisine,int *x1)
{  if(root==NULL)
   printf("NOT FOUND:\n");
   else
	{int i=0,x=root->count;
   	while(i<x)
    { if(root->isLeaf==FALSE)
      { 
	  	search_by_Cuisine_Util(root->child[i],cuisine,x1);
	  }
	  if(cuisine==root->all_eatspots[i].cuis_category)
	  { if(*x1==0)
	    printf("FOLLOWING ARE THE EATING SPOTS:\n");
	 	printf("Name:%s\nAddress:%s\n",root->all_eatspots[i].name,root->all_eatspots[i].address);
	 	(*x1)++;
	  }
	 i++; 
    }
   	if(root->isLeaf==FALSE)
  	search_by_Cuisine_Util(root->child[i],cuisine,x1);
    }
  // printf("node size=%d",x);
}
void search_by_Cuisine(loc_tree *root,int *x)
{   *x=0;
	int cuisine;
	printf("Enter the cuisine you are searching for\t:\nPRESS 1 FOR NORTH INDIAN\nPRESS 2 FOR SOUTH INDIAN\nPRESS 3 FOR CONTINENTAL\n");
	scanf("%d",&cuisine);
	search_by_Cuisine_Util(root,cuisine,x);
	if(*x==0)
	printf("NO SPOTS FOUND:");
}
void search_by_Area_Util(loc_tree *root,char area[],char zone[])
{ if(root==NULL)
   printf("NOT FOUND:\n");
  else 
  {
	int i=0,x=root->count;
   	while(i<x)
   { if(root->isLeaf==FALSE)
      { 
	  	search_by_Area_Util(root->child[i],area,zone);
	  }
	  if(strcmp(area,root->all_eatspots[i].address)==0||(strcmp(area,root->all_eatspots[i].address)!=0&&strcmp(zone,root->all_eatspots[i].zone)==0))
	  {
	 	print_location(&(root->all_eatspots[i]));
	  }
	 i++; 
    }
   	if(root->isLeaf==FALSE)
  	search_by_Area_Util(root->child[i],area,zone);
  // printf("node size=%d",x);
  }
}
void search_by_Area(loc_tree *root)
{
	char area[50],zone[50];
	printf("Enter the area you are searching for\t:");
	scanf("%s",area);
	printf("Enter the zone where you want to search for\t:");
	scanf("%s",zone);
	search_by_Area_Util(root,area,zone);
}
int insert_agent_into(agent_tree *nptr,agent value)
{  int i,j,ans,temp,x=nptr->count;
   i=0;
   temp=0;
   while(i<x && temp==0 )
    {
       if(value.id,nptr->all_agents[i].id>0)
        i++;
       else
         temp=1;
    }
   ans=i;
   (nptr->count)++;
   if(temp==0)
     nptr->all_agents[i]=value;
   else
     { j=(nptr->count)-1;
       while(j>i)
        { nptr->all_agents[j]=nptr->all_agents[j-1];
          j--;
        }
       nptr->all_agents[j]=value;    
	 }
     
   return ans;
}
agent_tree *split_agent(agent_tree **parent,agent_tree *childr,agent value)
{ if(*parent==NULL)
   { agent_tree *xptr=makeagent();
     agent_tree *yptr=makeagent();
     agent_tree *zptr=makeagent();
     int x=(childr->count)/2;
     int i,j,temp,k;
     yptr->count=0;
     for(i=0;i<x;i++)
      { yptr->all_agents[i]=childr->all_agents[i];     
        if(childr->isLeaf==FALSE) 
	   yptr->child[i]=childr->child[i];
       (yptr->count)++;
      }
     if(childr->isLeaf==FALSE) 
      yptr->child[i]=childr->child[i];
      zptr->count=0;
     k=i+1,i=0;
     for(j=x+1;j<childr->count;j++)
      { zptr->all_agents[i]=childr->all_agents[j];
        if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	 i++;
	 k++;
	 (zptr->count)++;
     
      }
     if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
     xptr->all_agents[0]=childr->all_agents[x];
     xptr->count=1;
     xptr->child[0]=yptr;
     xptr->child[1]=zptr;
     xptr->isLeaf=FALSE;
     free(childr);
     *parent=xptr;
     if(value.id>xptr->all_agents[0].id)
      return zptr;
     else
      return yptr;
   }
  else
   { agent_tree *yptr=makeagent();
     agent_tree *zptr=makeagent();
     int x=(childr->count)/2;
     int i,j,temp,k,k1;
     yptr->count=0;
    for(i=0;i<x;i++)
     { yptr->all_agents[i]=childr->all_agents[i];
       if(childr->isLeaf==FALSE); 
       yptr->child[i]=childr->child[i];
       (yptr->count)++;
     }
    if(childr->isLeaf==FALSE) 
    yptr->child[i]=childr->child[i];
    zptr->count=0;
    k=i+1,i=0;
    for(j=x+1;j<childr->count;j++)
    { zptr->all_agents[i]=childr->all_agents[j];
       if(childr->isLeaf==FALSE) 
	zptr->child[i]=childr->child[k];
	i++;
	k++;
      (zptr->count)++;

    }
    if(childr->isLeaf==FALSE) 
     zptr->child[i]=childr->child[k];    
    k1= insert_agent_into(*parent,childr->all_agents[x]);      
    (*parent)->child[k1]=yptr;
    j=(*parent)->count;
    while(j>k1+1)
     {
      (*parent)->child[j]=(*parent)->child[j-1];
       j--;
     }
    (*parent)->child[j]=zptr;
    free(childr);
    if(value.id>(*parent)->all_agents[k1].id)
     return zptr;
    else
     return yptr;
   }
}
void insert_agent(agent_tree **prev,agent_tree **root,agent value)
{
	if(*root==NULL)
	{
		agent_tree* nptr=makeagent();
		int k;
		k=insert_agent_into(nptr,value);
		//return nptr;
		*root=nptr;
	}
	else if((*root)->count==MAX)
	{
		if(*prev==NULL)
		{ //printf("SPLIT IS RUNNING\n");
		 *root=split_agent(prev,*root,value);
		 *root=*prev;
		 *prev=NULL;
		 insert_agent(prev,root,value);
	    }
	    else
	     { *root=split_agent(prev,*root,value);
	     	insert_agent(prev,root,value);
		 }
	    
	}
	else if((*root)->isLeaf==TRUE)
	{
		int k=insert_agent_into(*root,value);
	}
	else
	{ //prev=root;
	  int i=0,temp=0,x=(*root)->count;
	  while(i<x && temp==0)
	  { if(value.id>(*root)->all_agents[i].id)
	  	i++;
	  	else
	  	temp=1;
	  }
	  if(temp==1)
	  { 
	    insert_agent(root,&((*root)->child[i]),value);	
	  }
	  else
	  { insert_agent(root,&((*root)->child[(*root)->count]),value);
	  }
		
	}
	
}

int search(loc_tree *nptr,char name[],char address[])
{ int temp=0;
  int i=0,x=nptr->count;
  while(i<x && temp==0)
  { if(strcmp(name,nptr->all_eatspots[i].name)==0 && strcmp(address,nptr->all_eatspots[i].address)==0)
  	 temp=1;
  	else
  	 i++;
  }
  if(temp==0)
   return -1;
  else
   return i;

}
loc_tree* take_from_right(loc_tree **root,int i)
{   //printf("right\n");
    int cont;
    //scanf("%d",&cont);
	(*root)->child[i]->all_eatspots[(*root)->child[i]->count]=(*root)->all_eatspots[i];
	((*root)->child[i]->count)++;
	(*root)->all_eatspots[i]=(*root)->child[i+1]->all_eatspots[0];
	if((*root)->child[i]->isLeaf==FALSE)
	(*root)->child[i]->child[(*root)->child[i]->count]=(*root)->child[i+1]->child[0];
	int j;
	for(j=1;j< (*root)->child[i+1]->count;j++)
	{
		(*root)->child[i+1]->all_eatspots[j-1]=(*root)->child[i+1]->all_eatspots[j];
		if((*root)->child[i+1]->isLeaf==FALSE)
		(*root)->child[i+1]->child[j-1]=(*root)->child[i+1]->child[j];
	}
	if((*root)->child[i+1]->isLeaf==FALSE)
	(*root)->child[i+1]->child[j-1]=(*root)->child[i+1]->child[j];
	((*root)->child[i+1]->count)--;
  
   return (*root)->child[i];
}
loc_tree* take_from_left(loc_tree **root,int i)
{  // printf("left=%d\n",(*root)->child[i-1]->count);
    int cont;
	//scanf("%d",&cont);
	int j=(*root)->child[i]->count;
	while(j>=1)
	{
		(*root)->child[i]->all_eatspots[j]=(*root)->child[i]->all_eatspots[j-1];
	    if((*root)->child[i]->isLeaf==FALSE)
		(*root)->child[i]->child[j+1]=(*root)->child[i]->child[j];
		j--;
	}
	if((*root)->child[i]->isLeaf==FALSE)
		(*root)->child[i]->child[j+1]=(*root)->child[i]->child[j];
	//printf("Running\n");
	//scanf("%d",&cont);
	((*root)->child[i]->count)++;
	(*root)->child[i]->all_eatspots[0]=(*root)->all_eatspots[i-1];
	//printf("Running\n");
	//scanf("%d",&cont);
	(*root)->all_eatspots[i-1]=(*root)->child[i-1]->all_eatspots[(*root)->child[i-1]->count -1];
	//printf("Running\n");
	//scanf("%d",&cont);
	if((*root)->child[i]->isLeaf==FALSE)
	(*root)->child[i]->child[0]=(*root)->child[i-1]->child[(*root)->child[i-1]->count];
	((*root)->child[i-1]->count)--;
	return (*root)->child[i];
}
loc_tree* merge(loc_tree **rooti,loc_tree **root,int i)
{   /*printf("merge\n");
    int cont;
	scanf("%d",&cont);*/
	loc_tree *new_loc=(loc_tree*)malloc(sizeof(loc_tree));
	int i1,x=(*root)->count;
	if(i!=x)
	i1=i;
	else
	i1=i;
	//printf("i=%d i1=%d=\n",i,i1);
	//scanf("%d",&cont);
	int j;
	for(j=0;j< (*root)->child[i1]->count;j++)
	{	
		new_loc->all_eatspots[j]=(*root)->child[i1]->all_eatspots[j];
		if((*root)->child[i1]->isLeaf==FALSE)
		new_loc->child[j]=(*root)->child[i1]->child[j];
		
	}
	//printf("Running\n");
	//scanf("%d",&cont);
	if((*root)->child[i1]->isLeaf==FALSE)
	new_loc->child[j]=(*root)->child[i1]->child[j];
	new_loc->all_eatspots[j]=(*root)->all_eatspots[i];
	j++;
	int k;
	//printf("j =%d\n",j);
	//scanf("%d",&cont);
	for(k=0;k< (*root)->child[i1+1]->count;k++)
	{ new_loc->all_eatspots[j]=(*root)->child[i1+1]->all_eatspots[k];
	  if((*root)->child[i1+1]->isLeaf==FALSE)
	  new_loc->child[j]=(*root)->child[i1+1]->child[k];
	  j++;
	}
	//printf("Running\n");
	//scanf("%d",&cont);
	if((*root)->child[i1+1]->isLeaf==FALSE)
	new_loc->child[j]=(*root)->child[i1+1]->child[k];
	new_loc->count=j;
	if((*root)->count==1)
	{   //printf("conditions encountered:\n");
	    //scanf("%d",&cont);
		*rooti=new_loc;
		if((*root)->child[i]->isLeaf==TRUE)
		new_loc->isLeaf=TRUE;
		free(*root);
		*root=NULL;
			
	}
    else
    {   if(i==0)
	     { for(j=1;j<x;j++)
	        {
	          (*root)->all_eatspots[j-1]=(*root)->all_eatspots[j];	
			}
			(*root)->child[0]=new_loc;
			for(j=2;j<=x;j++)
			 (*root)->child[j-1]=(*root)->child[j];
		  }
		else if(i==x)
		 {
		 	 (*root)->child[(*root)->count -1]=new_loc;
		 }
		else
		 { for(j=i+1;j< x;j++)
		    {
		    	(*root)->all_eatspots[j-1]=(*root)->all_eatspots[j-1];
			}
		   (*root)->child[i]=new_loc;
		   for(j=i+2;j<=x;j++)
		   {
		   	 (*root)->child[j-1]=(*root)->child[j-1];
		   }
	     }
        (*root)->count--;
	}
   return new_loc;	
}
loc_tree *remove_value(loc_tree **rooti,loc_tree *root,char name[],char address[])
{ int i=0,j;
  while(strcmp(name,root->all_eatspots[i].name)!=0 || strcmp(address,root->all_eatspots[i].address)!=0)
	i++;
  for(j=i+1;j< root->count;j++)
   root->all_eatspots[j-1]=root->all_eatspots[j];
   (root->count)--;
  if(root->count==0)
  {
  	free(root);
  	root=NULL;
  	*rooti=NULL;
  }
  return root;
}
void delete_restaurant(loc_tree **rooti,loc_tree **root,loc_tree **prev,char name[],char address[])
{ 
 if(*root==NULL)
  {printf("root is NULL:\n");
  printf("restaurant not found\n"); 
  }
 else
 {
 int k=search(*root,name,address);
  if(k!=-1)
  { if(*prev==NULL && (*root)->isLeaf==TRUE) 
  	 { *root=remove_value(rooti,*root,name,address);
	 }
	else if((*root)->isLeaf==TRUE)
	 {
	 	remove_value(rooti,*root,name,address);
	 }
    else if((*root)->isLeaf==FALSE)
     { if((*root)->child[k]->count >=degree)
        {
        	(*root)->all_eatspots[k]=(*root)->child[k]->all_eatspots[(*root)->child[k]->count -1];
        	prev=root;
        	*root=(*root)->child[k];
        	delete_restaurant(rooti,root,prev,(*root)->child[k]->all_eatspots[(*root)->child[k]->count -1].name,(*root)->child[k]->all_eatspots[(*root)->child[k]->count -1].address);
        	
		}
	   else
	    { int x=(*root)->count;
		  if((*root)->child[k+1]->count >=degree)
		   {
		   	(*root)->child[k]=take_from_right(root,k);
		   	(*root)->all_eatspots[k]=(*root)->child[k]->all_eatspots[(*root)->child[k]->count -1];
        	prev=root;
        	*root=(*root)->child[k];
            delete_restaurant(rooti,root,prev,(*root)->all_eatspots[(*root)->count -1].name,(*root)->all_eatspots[(*root)->count -1].address);
		  }
		  else if(k!=0 && (*root)->child[k-1]->count >=degree)
		  {
		  	 (*root)->child[k]=take_from_left(root,k);
		     (*root)->all_eatspots[k]=(*root)->child[k]->all_eatspots[(*root)->child[k]->count -1];
            	prev=root;
        	*root=(*root)->child[k];
        	delete_restaurant(rooti,root,prev,(*root)->all_eatspots[(*root)->count -1].name,(*root)->all_eatspots[(*root)->count -1].address);
          }
		  else 
		  {  loc_tree *nptr;
		  	 nptr=merge(rooti,root,k);
		  	 if(*root!=NULL)
		  	 {(*root)->child[k]=nptr;
		  	  prev=root;
		  	  *root=(*root)->child[k];
		     }
		     else
		     {
		     	*prev=NULL;
		     	*root=nptr;
			 }
		  	 delete_restaurant(rooti,root,prev,name,address);
		  }
		}
	         	
	 }
	 
   }
  else
   {  if((*root)->isLeaf==TRUE)    
       {
       	printf("Restauarant not found\n");
	   }
	  else
	   { int i=0,x=(*root)->count,temp=0;
	   	 while(i<x && temp==0)
	   	  {
	   	  	if(strcmp(name,(*root)->all_eatspots[i].name)>0 || (strcmp(name,(*root)->all_eatspots[i].name)==0 && strcmp(address,(*root)->all_eatspots[i].address)>0) )
	   	  	i++;
	   	  	else
	   	  	temp=1;
		  }
		  //printf("x=%d i=%d\n",x,i);
		  //int cont;
		 // scanf("%d",&cont);
		 if((*root)->child[i]->count <degree)
		  { if(i!=x && (*root)->child[i+1]->count >=degree)
		    (*root)->child[i]=take_from_right(root,i);
		  	else if(i!=0 && (*root)->child[i-1]->count >=degree)
		  	(*root)->child[i]=take_from_left(root,i);
		  	else
		  	 {  
			  loc_tree *nptr;
		  	  nptr=merge(rooti,root,i);
		  	 /* int cont;
		  	  printf("merge returned successfully\n");
		  	  scanf("%d",&cont);
		  	  */
		      if(*root!=NULL)
		      (*root)->child[i]=nptr;
		      
			 }
		  }
		 if(*root!=NULL)
		 {
		 prev=root;
		 *root=(*root)->child[i];
         }
         else
         { *prev=NULL;
         	root=rooti;
		 }
		 delete_restaurant(rooti,root,prev,name,address);
	   }
	
   }
 }
}
void delete_eating_spot(loc_tree **root)
{ char name[50],address[100];
  printf("Enter name= ");
  scanf("%s",name);
  printf("Enter address= ");
  scanf("%s",address);
  loc_tree *prev=NULL,*rooti=*root;
  delete_restaurant(root,&rooti,&prev,name,address);
}
int count_no_of_restaurants(loc_tree *root)
{ if(root==NULL)
   return 0;
  else
   { int i,x=root->count;
     int no=0;    
     for(i=0;i<x;i++)
      {
      	 no++;
      	 if(root->isLeaf==FALSE)
      	 no+=count_no_of_restaurants(root->child[i]);
	  }
	   if(root->isLeaf==FALSE)
   	   no+=count_no_of_restaurants(root->child[i]);
   	   return no;
   }
}
void write_into_file(location value,FILE *fp)
{   fprintf(fp,"%s\n%s\n%s\n%d\n",value.name,value.address,value.zone,value.no_of_seats);
	int i=0;
	fprintf(fp,"%d\n",value.res_menu->no_of_items);
	while(i< (value.res_menu->no_of_items))
	 { fprintf(fp,"%s\n%f\n",value.res_menu->item[i].itemname,value.res_menu->item[i].price);
	 	i++;
	 }
	fprintf(fp,"%d\n%d\n",value.category,value.cuis_category);

}
void Write_data_into_file(loc_tree *root,FILE *fp)
{ 
	int i,x=root->count;
	for(i=0;i<x;i++)
	{ if(root->isLeaf==FALSE)
		Write_data_into_file(root->child[i],fp);
	  write_into_file(root->all_eatspots[i],fp);
	  
	}
   if(root->isLeaf==FALSE)
	Write_data_into_file(root->child[i],fp);	
	
}

void insert_new_agent(agent_tree **agent_list)
{ agent value;
  printf("Enter agents name: ");
  scanf("%s",value.name);
  printf("Enter agents phone no: ");
  scanf("%s",value.phone_no);
  printf("Enter area: ");
  scanf("%s",value.area);
  value.id=agent_id_allocator;
  value.is_available=TRUE;
  (agent_id_allocator)++;
  value.curr_accu_commi=0;
  agent_tree *prev=NULL;
  insert_agent(&prev,agent_list,value);
}
void print_agent(agent value)
{
		printf("\n***************************\nAgent Id\t:%d\n",value.id);
		printf("Agent Name\t:%s\n",value.name);
		printf("Agent Phone No.\t:%s\n",value.phone_no);
		printf("Agent Area\t:%s\n",value.area);
		printf("Accumulated Commission\t:%f\n",value.curr_accu_commi);
}
void agent_details(agent_tree *agent_list)
{
	if(agent_list==NULL)
	printf("Empty list:");
	else
	{ int i,x=agent_list->count;
	  for(i=0;i< x;i++)
	  { if(agent_list->isLeaf==FALSE)
	     agent_details(agent_list->child[i]);
	    if(agent_list->all_agents[i].is_available==TRUE)
	  	print_agent(agent_list->all_agents[i]);
	  }
	 if(agent_list->isLeaf==FALSE)
	  agent_details(agent_list->child[i]);
	}
}
void agent_busy_details(agent_tree *agent_list)
{
	if(agent_list==NULL)
	printf("Empty list:");
	else
	{ int i,x=agent_list->count;
	  for(i=0;i< x;i++)
	  { if(agent_list->isLeaf==FALSE)
	     agent_details(agent_list->child[i]);
	    if(agent_list->all_agents[i].is_available==FALSE)
	  	print_agent(agent_list->all_agents[i]);
	  }
	 if(agent_list->isLeaf==FALSE)
	  agent_details(agent_list->child[i]);
	}
}
void agent_area_details(agent_tree *agent_list,char area[])
{
	if(agent_list==NULL)
	printf("Empty list:");
	else
	{ int i,x=agent_list->count;
	  for(i=0;i< x;i++)
	  { if(agent_list->isLeaf==FALSE)
	     agent_details(agent_list->child[i]);
	    if(strcmp(area,agent_list->all_agents[i].area)==0)
	  	print_agent(agent_list->all_agents[i]);
	  }
	 if(agent_list->isLeaf==FALSE)
	  agent_details(agent_list->child[i]);
	}
}
void search_agent_by_area(agent_tree *agent_list)
{
	char area[100];
	printf("Enter area: ");
	scanf("%s",area);
	agent_area_details(agent_list,area);
}
user_tree* makeuser()
{
	user_tree* nptr=(user_tree*)malloc(sizeof(user_tree));
	if(nptr!=NULL)
	{
		nptr->count=0;
		nptr->isLeaf=TRUE;
	}
	return nptr;
}
int insert_user_into(user_tree *nptr,user value)
{  int i,j,ans,temp,x=nptr->count;
   i=0;
   temp=0;
   while(i<x && temp==0 )
    {
       if(strcmp(value.phone_no,nptr->all_users[i].phone_no)>0)
        i++;
       else
         temp=1;
    }
   ans=i;
   (nptr->count)++;
   if(temp==0)
     nptr->all_users[i]=value;
   else
     { j=(nptr->count)-1;
       while(j>i)
        { nptr->all_users[j]=nptr->all_users[j-1];
          j--;
        }
       nptr->all_users[j]=value;    
	 }
     
   return ans;
}
user_tree *split_user(user_tree **parent,user_tree *childr,user value)
{ if(*parent==NULL)
   { user_tree *xptr=makeuser();
     user_tree *yptr=makeuser();
     user_tree *zptr=makeuser();
     int x=(childr->count)/2;
     int i,j,temp,k;
     yptr->count=0;
     for(i=0;i<x;i++)
      { yptr->all_users[i]=childr->all_users[i];     
        if(childr->isLeaf==FALSE) 
	   yptr->child[i]=childr->child[i];
       (yptr->count)++;
      }
     if(childr->isLeaf==FALSE) 
      yptr->child[i]=childr->child[i];
      zptr->count=0;
     k=i+1,i=0;
     for(j=x+1;j<childr->count;j++)
      { zptr->all_users[i]=childr->all_users[j];
        if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	 i++;
	 k++;
	 (zptr->count)++;
     
      }
     if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
     xptr->all_users[0]=childr->all_users[x];
     xptr->count=1;
     xptr->child[0]=yptr;
     xptr->child[1]=zptr;
     xptr->isLeaf=FALSE;
     free(childr);
     *parent=xptr;
     if(strcmp(value.phone_no,xptr->all_users[0].phone_no)>0)
      return zptr;
     else
      return yptr;
   }
  else
   { user_tree *yptr=makeuser();
     user_tree *zptr=makeuser();
     int x=(childr->count)/2;
     int i,j,temp,k,k1;
     yptr->count=0;
    for(i=0;i<x;i++)
     { yptr->all_users[i]=childr->all_users[i];
       if(childr->isLeaf==FALSE); 
       yptr->child[i]=childr->child[i];
       (yptr->count)++;
     }
    if(childr->isLeaf==FALSE) 
    yptr->child[i]=childr->child[i];
    zptr->count=0;
    k=i+1,i=0;
    for(j=x+1;j<childr->count;j++)
    { zptr->all_users[i]=childr->all_users[j];
       if(childr->isLeaf==FALSE) 
	zptr->child[i]=childr->child[k];
	i++;
	k++;
      (zptr->count)++;

    }
    if(childr->isLeaf==FALSE) 
     zptr->child[i]=childr->child[k];    
    k1= insert_user_into(*parent,childr->all_users[x]);      
    (*parent)->child[k1]=yptr;
    j=(*parent)->count;
    while(j>k1+1)
     {
      (*parent)->child[j]=(*parent)->child[j-1];
       j--;
     }
    (*parent)->child[j]=zptr;
    free(childr);
    if(strcmp(value.phone_no,(*parent)->all_users[k1].phone_no)>0)
     return zptr;
    else
     return yptr;
   }
}
void insert_user(user_tree **prev,user_tree **root,user value)
{
	if(*root==NULL)
	{
		user_tree* nptr=makeuser();
		int k;
		k=insert_user_into(nptr,value);
		//return nptr;
		*root=nptr;
	}
	else if((*root)->count==MAX)
	{
		if(*prev==NULL)
		{ //printf("SPLIT IS RUNNING\n");
		 *root=split_user(prev,*root,value);
		 *root=*prev;
		 *prev=NULL;
		 insert_user(prev,root,value);
	    }
	    else
	     { *root=split_user(prev,*root,value);
	     	insert_user(prev,root,value);
		 }
	    
	}
	else if((*root)->isLeaf==TRUE)
	{
		int k=insert_user_into(*root,value);
	}
	else
	{ //prev=root;
	  int i=0,temp=0,x=(*root)->count;
	  while(i<x && temp==0)
	  { if(strcmp(value.phone_no,(*root)->all_users[i].phone_no)>0)
	  	i++;
	  	else
	  	temp=1;
	  }
	  if(temp==1)
	  { 
	    insert_user(root,&((*root)->child[i]),value);	
	  }
	  else
	  { insert_user(root,&((*root)->child[(*root)->count]),value);
	  }
		
	}
	
}
order_tree* makeorder()
{
	order_tree* nptr=(order_tree*)malloc(sizeof(order_tree));
	if(nptr!=NULL)
	{
		nptr->count=0;
		nptr->isLeaf=TRUE;
	}
	return nptr;
}
int insert_order_into(order_tree *nptr,orders value)
{  int i,j,ans,temp,x=nptr->count;
   i=0;
   temp=0;
   while(i<x && temp==0 )
    {
       if(value.ord_id > nptr->all_orders[i].ord_id)
        i++;
       else
         temp=1;
    }
   ans=i;
   (nptr->count)++;
   if(temp==0)
     nptr->all_orders[i]=value;
   else
     { j=(nptr->count)-1;
       while(j>i)
        { nptr->all_orders[j]=nptr->all_orders[j-1];
          j--;
        }
       nptr->all_orders[j]=value;    
	 }
     
   return ans;
}
order_tree *split_order(order_tree **parent,order_tree *childr,orders value)
{ if(*parent==NULL)
   { order_tree *xptr=makeorder();
     order_tree *yptr=makeorder();
     order_tree *zptr=makeorder();
     int x=(childr->count)/2;
     int i,j,temp,k;
     yptr->count=0;
     for(i=0;i<x;i++)
      { yptr->all_orders[i]=childr->all_orders[i];     
        if(childr->isLeaf==FALSE) 
	   yptr->child[i]=childr->child[i];
       (yptr->count)++;
      }
     if(childr->isLeaf==FALSE) 
      yptr->child[i]=childr->child[i];
      zptr->count=0;
     k=i+1,i=0;
     for(j=x+1;j<childr->count;j++)
      { zptr->all_orders[i]=childr->all_orders[j];
        if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
	 i++;
	 k++;
	 (zptr->count)++;
     
      }
     if(childr->isLeaf==FALSE) 
	  zptr->child[i]=childr->child[k];
     xptr->all_orders[0]=childr->all_orders[x];
     xptr->count=1;
     xptr->child[0]=yptr;
     xptr->child[1]=zptr;
     xptr->isLeaf=FALSE;
     free(childr);
     *parent=xptr;
     if(value.ord_id >xptr->all_orders[0].ord_id)
      return zptr;
     else
      return yptr;
   }
  else
   { order_tree *yptr=makeorder();
     order_tree *zptr=makeorder();
     int x=(childr->count)/2;
     int i,j,temp,k,k1;
     yptr->count=0;
    for(i=0;i<x;i++)
     { yptr->all_orders[i]=childr->all_orders[i];
       if(childr->isLeaf==FALSE); 
       yptr->child[i]=childr->child[i];
       (yptr->count)++;
     }
    if(childr->isLeaf==FALSE) 
    yptr->child[i]=childr->child[i];
    zptr->count=0;
    k=i+1,i=0;
    for(j=x+1;j<childr->count;j++)
    { zptr->all_orders[i]=childr->all_orders[j];
       if(childr->isLeaf==FALSE) 
	zptr->child[i]=childr->child[k];
	i++;
	k++;
      (zptr->count)++;

    }
    if(childr->isLeaf==FALSE) 
     zptr->child[i]=childr->child[k];    
    k1= insert_order_into(*parent,childr->all_orders[x]);      
    (*parent)->child[k1]=yptr;
    j=(*parent)->count;
    while(j>k1+1)
     {
      (*parent)->child[j]=(*parent)->child[j-1];
       j--;
     }
    (*parent)->child[j]=zptr;
    free(childr);
    if(value.ord_id > (*parent)->all_orders[k1].ord_id)
     return zptr;
    else
     return yptr;
   }
}
void insert_order(order_tree **prev,order_tree **root,orders value)
{
	if(*root==NULL)
	{
		order_tree* nptr=makeorder();
		int k;
		k=insert_order_into(nptr,value);
		//return nptr;
		*root=nptr;
	}
	else if((*root)->count==MAX)
	{
		if(*prev==NULL)
		{ //printf("SPLIT IS RUNNING\n");
		 *root=split_order(prev,*root,value);
		 *root=*prev;
		 *prev=NULL;
		 insert_order(prev,root,value);
	    }
	    else
	     { *root=split_order(prev,*root,value);
	     	insert_order(prev,root,value);
		 }
	    
	}
	else if((*root)->isLeaf==TRUE)
	{
		int k=insert_order_into(*root,value);
	}
	else
	{ //prev=root;
	  int i=0,temp=0,x=(*root)->count;
	  while(i<x && temp==0)
	  { if(value.ord_id > (*root)->all_orders[i].ord_id)
	  	i++;
	  	else
	  	temp=1;
	  }
	  if(temp==1)
	  { 
	    insert_order(root,&((*root)->child[i]),value);	
	  }
	  else
	  { insert_order(root,&((*root)->child[(*root)->count]),value);
	  }
		
	}
	
}

int search_order(order_tree *nptr,int id)
{ int temp=0;
  int i=0,x=nptr->count;
  while(i<x && temp==0)
  { if( nptr->all_orders[i].ord_id==id)
  	 temp=1;
  	else
  	 i++;
  }
  if(temp==0)
   return -1;
  else
   return i;

}
order_tree* borrow_from_right(order_tree **root,int i)
{   //printf("right\n");
    int cont;
    //scanf("%d",&cont);
	(*root)->child[i]->all_orders[(*root)->child[i]->count]=(*root)->all_orders[i];
	((*root)->child[i]->count)++;
	(*root)->all_orders[i]=(*root)->child[i+1]->all_orders[0];
	if((*root)->child[i]->isLeaf==FALSE)
	(*root)->child[i]->child[(*root)->child[i]->count]=(*root)->child[i+1]->child[0];
	int j;
	for(j=1;j< (*root)->child[i+1]->count;j++)
	{
		(*root)->child[i+1]->all_orders[j-1]=(*root)->child[i+1]->all_orders[j];
		if((*root)->child[i+1]->isLeaf==FALSE)
		(*root)->child[i+1]->child[j-1]=(*root)->child[i+1]->child[j];
	}
	if((*root)->child[i+1]->isLeaf==FALSE)
	(*root)->child[i+1]->child[j-1]=(*root)->child[i+1]->child[j];
	((*root)->child[i+1]->count)--;
  
   return (*root)->child[i];
}
order_tree* borrow_from_left(order_tree **root,int i)
{  // printf("left=%d\n",(*root)->child[i-1]->count);
    int cont;
	//scanf("%d",&cont);
	int j=(*root)->child[i]->count;
	while(j>=1)
	{
		(*root)->child[i]->all_orders[j]=(*root)->child[i]->all_orders[j-1];
	    if((*root)->child[i]->isLeaf==FALSE)
		(*root)->child[i]->child[j+1]=(*root)->child[i]->child[j];
		j--;
	}
	if((*root)->child[i]->isLeaf==FALSE)
		(*root)->child[i]->child[j+1]=(*root)->child[i]->child[j];
	//printf("Running\n");
	//scanf("%d",&cont);
	((*root)->child[i]->count)++;
	(*root)->child[i]->all_orders[0]=(*root)->all_orders[i-1];
	//printf("Running\n");
	//scanf("%d",&cont);
	(*root)->all_orders[i-1]=(*root)->child[i-1]->all_orders[(*root)->child[i-1]->count -1];
	//printf("Running\n");
	//scanf("%d",&cont);
	if((*root)->child[i]->isLeaf==FALSE)
	(*root)->child[i]->child[0]=(*root)->child[i-1]->child[(*root)->child[i-1]->count];
	((*root)->child[i-1]->count)--;
	return (*root)->child[i];
}
order_tree* combine(order_tree **rooti,order_tree **root,int i)
{   /*printf("combine\n");
    int cont;
	scanf("%d",&cont);*/
	order_tree *new_loc=(order_tree*)malloc(sizeof(order_tree));
	int i1,x=(*root)->count;
	if(i!=x)
	i1=i;
	else
	i1=i;
	//printf("i=%d i1=%d=\n",i,i1);
	//scanf("%d",&cont);
	int j;
	for(j=0;j< (*root)->child[i1]->count;j++)
	{	
		new_loc->all_orders[j]=(*root)->child[i1]->all_orders[j];
		if((*root)->child[i1]->isLeaf==FALSE)
		new_loc->child[j]=(*root)->child[i1]->child[j];
		
	}
	//printf("Running\n");
	//scanf("%d",&cont);
	if((*root)->child[i1]->isLeaf==FALSE)
	new_loc->child[j]=(*root)->child[i1]->child[j];
	new_loc->all_orders[j]=(*root)->all_orders[i];
	j++;
	int k;
	//printf("j =%d\n",j);
	//scanf("%d",&cont);
	for(k=0;k< (*root)->child[i1+1]->count;k++)
	{ new_loc->all_orders[j]=(*root)->child[i1+1]->all_orders[k];
	  if((*root)->child[i1+1]->isLeaf==FALSE)
	  new_loc->child[j]=(*root)->child[i1+1]->child[k];
	  j++;
	}
	//printf("Running\n");
	//scanf("%d",&cont);
	if((*root)->child[i1+1]->isLeaf==FALSE)
	new_loc->child[j]=(*root)->child[i1+1]->child[k];
	new_loc->count=j;
	if((*root)->count==1)
	{   //printf("conditions encountered:\n");
	    //scanf("%d",&cont);
		*rooti=new_loc;
		if((*root)->child[i]->isLeaf==TRUE)
		new_loc->isLeaf=TRUE;
		free(*root);
		*root=NULL;
			
	}
    else
    {   if(i==0)
	     { for(j=1;j<x;j++)
	        {
	          (*root)->all_orders[j-1]=(*root)->all_orders[j];	
			}
			(*root)->child[0]=new_loc;
			for(j=2;j<=x;j++)
			 (*root)->child[j-1]=(*root)->child[j];
		  }
		else if(i==x)
		 {
		 	 (*root)->child[(*root)->count -1]=new_loc;
		 }
		else
		 { for(j=i+1;j< x;j++)
		    {
		    	(*root)->all_orders[j-1]=(*root)->all_orders[j-1];
			}
		   (*root)->child[i]=new_loc;
		   for(j=i+2;j<=x;j++)
		   {
		   	 (*root)->child[j-1]=(*root)->child[j-1];
		   }
	     }
        (*root)->count--;
	}
   return new_loc;	
}
order_tree *remove_order(order_tree **rooti,order_tree *root,int id)
{ int i=0,j;
  while(root->all_orders[i].ord_id!=id)
	i++;
  for(j=i+1;j< root->count;j++)
   root->all_orders[j-1]=root->all_orders[j];
   (root->count)--;
  if(root->count==0)
  {
  	free(root);
  	root=NULL;
  	*rooti=NULL;
  }
  return root;
}
void delete_order(order_tree **rooti,order_tree **root,order_tree **prev,int id)
{ 
 if(*root==NULL)
  {printf("root is NULL:\n");
  printf("restaurant not found\n"); 
  }
 else
 {
 int k=search_order(*root,id);
  if(k!=-1)
  { if(*prev==NULL && (*root)->isLeaf==TRUE) 
  	 { *root=remove_order(rooti,*root,id);
	 }
	else if((*root)->isLeaf==TRUE)
	 {
	 	remove_order(rooti,*root,id);
	 }
    else if((*root)->isLeaf==FALSE)
     { if((*root)->child[k]->count >=degree)
        {
        	(*root)->all_orders[k]=(*root)->child[k]->all_orders[(*root)->child[k]->count -1];
        	prev=root;
        	*root=(*root)->child[k];
        	delete_order(rooti,root,prev,(*root)->child[k]->all_orders[(*root)->child[k]->count -1].ord_id);
        	
		}
	   else
	    { int x=(*root)->count;
		  if((*root)->child[k+1]->count >=degree)
		   {
		   	(*root)->child[k]=borrow_from_right(root,k);
		   	(*root)->all_orders[k]=(*root)->child[k]->all_orders[(*root)->child[k]->count -1];
        	prev=root;
        	*root=(*root)->child[k];
            delete_order(rooti,root,prev,(*root)->all_orders[(*root)->count -1].ord_id);
		  }
		  else if(k!=0 && (*root)->child[k-1]->count >=degree)
		  {
		  	 (*root)->child[k]=borrow_from_left(root,k);
		     (*root)->all_orders[k]=(*root)->child[k]->all_orders[(*root)->child[k]->count -1];
            	prev=root;
        	*root=(*root)->child[k];
        	delete_order(rooti,root,prev,(*root)->all_orders[(*root)->count -1].ord_id);
          }
		  else 
		  {  order_tree *nptr;
		  	 nptr=combine(rooti,root,k);
		  	 if(*root!=NULL)
		  	 {(*root)->child[k]=nptr;
		  	  prev=root;
		  	  *root=(*root)->child[k];
		     }
		     else
		     {
		     	*prev=NULL;
		     	*root=nptr;
			 }
		  	 delete_order(rooti,root,prev,id);
		  }
		}
	         	
	 }
	 
   }
  else
   {  if((*root)->isLeaf==TRUE)    
       {
       	printf("Restauarant not found\n");
	   }
	  else
	   { int i=0,x=(*root)->count,temp=0;
	   	 while(i<x && temp==0)
	   	  {
	   	  	if( id > (*root)->all_orders[i].ord_id) 
	   	  	i++;
	   	  	else
	   	  	temp=1;
		  }
		  //printf("x=%d i=%d\n",x,i);
		  //int cont;
		 // scanf("%d",&cont);
		 if((*root)->child[i]->count <degree)
		  { if(i!=x && (*root)->child[i+1]->count >=degree)
		    (*root)->child[i]=borrow_from_right(root,i);
		  	else if(i!=0 && (*root)->child[i-1]->count >=degree)
		  	(*root)->child[i]=borrow_from_left(root,i);
		  	else
		  	 {  
			  order_tree *nptr;
		  	  nptr=combine(rooti,root,i);
		  	 /* int cont;
		  	  printf("combine returned successfully\n");
		  	  scanf("%d",&cont);
		  	  */
		      if(*root!=NULL)
		      (*root)->child[i]=nptr;
		      
			 }
		  }
		 if(*root!=NULL)
		 {
		 prev=root;
		 *root=(*root)->child[i];
         }
         else
         { *prev=NULL;
         	root=rooti;
		 }
		 delete_order(rooti,root,prev,id);
	   }
	
   }
 }
} 
location *search_particular_spot(loc_tree* root,char name[],char address[])
{
	location *ans=NULL;
	int x=root->count,i,temp=0;
	while(i<x && temp==0)
	{ if(strcmp(name,root->all_eatspots[i].name)>0 ||(strcmp(name,root->all_eatspots[i].name)==0 && strcmp(address,root->all_eatspots[i].address)>0) )
	   	i++;
	   else
	    temp=1;
	}
   if(temp==1)
   { if(strcmp(name,root->all_eatspots[i].name)==0 && strcmp(address,root->all_eatspots[i].address)==0)
   	  ans=&(root->all_eatspots[i]);
	 else if(root->isLeaf==FALSE)
   	  ans=search_particular_spot(root->child[i],name,address);
   	 
   }
  else
   { if(root->isLeaf==FALSE)
      ans=search_particular_spot(root->child[i],name,address); 
   }
   return ans;
}
void search_spot(loc_tree *root)
{
	char name[50],address[100];
	printf("Enter name:");
	scanf("%s",name);
	printf("Enter address:");
	scanf("%s",address);
	location *ans=NULL;
	ans=search_particular_spot(root,name,address);
	if(ans==NULL)
	printf("NOT FOUND:");
	else
	print_location(ans);
    
}

user *search_particular_user(user_tree* root,char phone_no[])
{   if(root==NULL)
     return  NULL;
	user *ans=NULL;
	int x=root->count,i,temp=0;
	while(i<x && temp==0)
	{ if(strcmp(phone_no,root->all_users[i].phone_no)>0 )
	   	i++;
	   else
	    temp=1;
	}
   if(temp==1)
   { if(strcmp(phone_no,root->all_users[i].phone_no)==0 )
   	  ans=&(root->all_users[i]);
	 else if(root->isLeaf==FALSE)
   	  ans=search_particular_user(root->child[i],phone_no);
   	 
   }
  else
   { if(root->isLeaf==FALSE)
      ans=search_particular_user(root->child[i],phone_no); 
   }
   return ans;
}
agent *search_free_agent(agent_tree *agent_list)
{ if(agent_list==NULL)
  return NULL; 
 int i,x=agent_list->count,temp=0;
  agent *ans=NULL;
  for(i=0;i<x && temp==0;i++)
  { if(agent_list->all_agents[i].is_available==TRUE)
     {
     	ans=&(agent_list->all_agents[i]);
     	temp=1;
	 }
	else if(agent_list->isLeaf==FALSE)
	 { ans=search_free_agent(agent_list->child[i]);
	   if(ans!=NULL)
	   {temp=1;
	   }
	   
	 }
  	
  }
 if(ans==NULL)
 { if(agent_list->isLeaf==FALSE)
 	ans=search_free_agent(agent_list->child[i]);
 }
 return ans;
	
}
void take_order(loc_tree *root,user_tree **users,order_tree **pending_orders,agent_tree **agent_list)
{ 
  int temp=1;
  int food_type;
  char name[100],address[100];
  //statuscode sc=SUCCESS;
 // printf("Enter cuisine you want:1)North Indian 2)South Indian 3)Continental ");
 //  scanf("%d",&food_type);
  int *x1;
  int x;
  x1=&x;
  search_by_Cuisine(root,x1);
  //printf("%d\n",*x1);
  if(*x1!=0)
  { location *ans=NULL;
    printf("\nEnter name of the restaurant:");
    scanf("%s",name);
    printf("Enter address of the restaurant: ");
    scanf("%s",address);
    orders *orde=NULL;
	ans=search_particular_spot(root,name,address); 
	if(ans==NULL)
	{
	   printf("Invalid name and address: ");
	}
   else
    { orde=(orders*)malloc(sizeof(orders));
      strcpy(orde->res_name,name);
      strcpy(orde->res_address,address);
      orde->username=NULL;
      printf("You can order now:\n ");
      printf("Below is the menu: \n");
	  visit_menu(ans->res_menu);
	  orde->ordered_restaurant=ans;
	  printf("Enter no of items you want to order: ");
	  int n;
	  scanf("%d",&n);
	  int a[n];
	  printf("Enter choice nos :");
	  int i=0,p=0; 
	  float price=0; 
	  for(i=0;i<n;i++)
	  {  printf("ENTER YOUR CHOICE No %d: \n",i+1);
	  	 scanf("%d",&a[i]);
	  	 int x;
	  	 printf("Enter quantity: ");
	  	 scanf("%d",&x);
	  	 if(a[i]<=ans->res_menu->no_of_items)
	  	  { 
			orde->item_index[p]=a[i]-1;
	  	    orde->quantity_index[p]=x;
	  	    price=price+(orde->ordered_restaurant->res_menu->item[a[i]-1].price)*x;
	  	  	p++;
		  }
		  
	  }
	 orde->no_of_items=p;
	 orde->total_price=price;
	 char us_phone[50];
	 printf("ENTER USER PHONE NO: ");
	 scanf("%s",us_phone);
	 user *uptr=NULL;
	 uptr=search_particular_user(*users,us_phone);
	 if(uptr==NULL)
	 { printf("Register new user:\n");
	   user* new_user=(user*)malloc(sizeof(user));
	   strcpy(new_user->phone_no,us_phone);
	   char us_name[50];
	   char us_address[50];
	   printf("Enter new user name: ");
	   scanf("%s",us_name);
	   printf("Enter new user address: ");
	   scanf("%s",us_address);
	   strcpy(new_user->name,us_name);   
	   strcpy(new_user->address,us_address);
	   new_user->ordered_items=NULL;
	   user_tree *us=*users,*pr=NULL;
	   insert_user(&pr,users,*new_user);
	   uptr=new_user;
	   printf("New user registered successfully!\n ");
	 }
	
   orde->username=uptr;
   agent *a_ptr=search_free_agent(*agent_list);
   if(a_ptr!=NULL)
    {  a_ptr->is_available=FALSE;
       orde->allocated_agent=a_ptr;
       orde->ord_id=order_id_allocator;
	   order_id_allocator++;
	   order_tree *ori=*pending_orders,*pori=NULL;
	   insert_order(&pori,pending_orders,*orde);
	   printf("\nYour order is registered successfully:\nAgent %s is assigned to you.\nContact no of agent:%s\n\n",a_ptr->name,a_ptr->phone_no);
	}
   else
    {
    	printf("\nSORRY !No free agents available!\n");
	}
     
  } 
  }
  
} 
void print_order(orders *nptr)
{
	
			printf("\n************************************\nOrder Id\t:%d\n",nptr->ord_id);
			printf("Restaurant Name\t:%s\n",nptr->res_name);
			printf("Restaurant Address\t:%s\n",nptr->res_address);
			printf("No. of items\t:%d\n",nptr->no_of_items);
		    int i;
			for(i=0;i<nptr->no_of_items;i++)
			{
				printf("%d. %s\n",i+1,nptr->ordered_restaurant->res_menu->item[nptr->item_index[i]].itemname);
				printf("  Quantity\t:%d\n",nptr->quantity_index[i]);
				printf("  Price\t:%f\n",nptr->ordered_restaurant->res_menu->item[nptr->item_index[i]].price);
			} 
			printf("Username\t:%s\n",nptr->username->name);
			printf("User Address\t:%s\n",nptr->username->address);
			printf("User Phone No.\t:%s\n",nptr->username->phone_no);
			printf("Total price to be paid\t:%f\n",nptr->total_price);
			if(nptr->allocated_agent!=NULL)
			{
			printf("Agent Name\t:%s\n",nptr->allocated_agent->name);
			printf("Agent Phone No.\t:%s\n",nptr->allocated_agent->phone_no);
		    }
}
void live_orders(order_tree *pending_orders)
{   
	if(pending_orders==NULL)
	printf("EMPTY LIST\n");
	else 
	{ int i,x=pending_orders->count;
	  for(i=0;i<x;i++)
	   { if(pending_orders->isLeaf==FALSE)
	     {
	     	live_orders(pending_orders->child[i]);
		 }
		 print_order(&(pending_orders->all_orders[i]));
		 
	   	
	   }
	  if(pending_orders->isLeaf==FALSE)
	   {
	     live_orders(pending_orders->child[i]);
	   }
		
	}
}
orders *search_specific_order(order_tree *pending_orders,int id)
{
	if(pending_orders==NULL)
	{
	   return NULL;
	}
    else
    { orders *ans=NULL;
	  int x=pending_orders->count,i,temp=0;
	  while(i<x && temp==0)
	  { if(id>pending_orders->all_orders[i].ord_id )
	   	i++;
	   else
	    temp=1;
	  }
    if(temp==1)
     { if(id==pending_orders->all_orders[i].ord_id )
   	   ans=&(pending_orders->all_orders[i]);
	 else if(pending_orders->isLeaf==FALSE)
   	  ans=search_specific_order(pending_orders->child[i],id);
   	 
     } 
   else
    { if(pending_orders->isLeaf==FALSE)
      ans=search_specific_order(pending_orders->child[i],id); 
    }
    	
     return ans;
	}
}
void delivery(order_tree **pending_orders,order_tree **archived_orders)
{
	int id;
	printf("ENTER ORDER ID: ");
	scanf("%d",&id);
	orders* ord;
    ord=search_specific_order(*pending_orders,id);
    if(ord==NULL)
    printf("ORDER NOT FOUND\n");
    else
    { 
     ord->allocated_agent->curr_accu_commi=ord->allocated_agent->curr_accu_commi+((ord->total_price)/10);
     ord->allocated_agent->is_available=TRUE;
	 order_tree *prev=NULL,*optr;
	 insert_order(&prev,archived_orders,*ord);
	 prev=NULL;
	 optr=*pending_orders;
	 delete_order(pending_orders,&optr,&prev,id);
	 	
	}
}
void cancel(order_tree **pending_orders)
{
	int id;
	printf("ENTER ORDER ID: ");
	scanf("%d",&id);
	orders* ord;
    ord=search_specific_order(*pending_orders,id);
	if(ord==NULL)
    printf("ORDER NOT FOUND\n");
    else
    { 
     //ord->allocated_agent->curr_accu_commi=ord->allocated_agent->curr_accu_commi+((ord->total_price)/10);
     ord->allocated_agent->is_available=TRUE;
	 order_tree *prev=NULL,*optr;
	 //insert_order(&prev,archived_orders,*ord);
	 //prev=NULL;
	 optr=*pending_orders;
	 delete_order(pending_orders,&optr,&prev,id);
	 	
	}
}
void order_history_user(order_tree *pending_orders,char phone_no[])
{
	
	if(pending_orders==NULL)
	printf("EMPTY LIST\n");
	else 
	{ int i,x=pending_orders->count;
	  for(i=0;i<x;i++)
	   { if(pending_orders->isLeaf==FALSE)
	     {
	     	order_history_user(pending_orders->child[i],phone_no);
		 }
		 if(strcmp(phone_no,pending_orders->all_orders[i].username->phone_no)==0)
		 print_order(&(pending_orders->all_orders[i]));
		 
	   	
	   }
	  if(pending_orders->isLeaf==FALSE)
	   {
	     order_history_user(pending_orders->child[i],phone_no);
	   }
		
	}
}
void order_history_particular_user(order_tree *archived_orders)
{
	char phone_no[11];
	printf("Enter user phine no:");
	scanf("%s",phone_no);
	order_history_user(archived_orders,phone_no);
}
int count_no_of_agents(agent_tree *agent_list)
{
	if(agent_list==NULL)
	return 0;
    int i=0,x=agent_list->count,count=0;
    for(i=0;i<x;i++)
    {   count++;
    	if(agent_list->isLeaf==FALSE)
    	{
    		count+=count_no_of_agents(agent_list->child[i]);
		}
	}
	if(agent_list->isLeaf==FALSE)
		{
	      count+=count_no_of_agents(agent_list->child[i]);
		}
  return count;
}
void write_info_file(agent value,FILE *ap)
{
	fprintf(ap,"%d\n%s\n%s\n%s\n%f\n%d\n",value.id,value.name,value.phone_no,value.area,value.curr_accu_commi,value.is_available);
}
void Write_agents_into_file(agent_tree *agent_list,FILE *ap)
{
   	int i,x=agent_list->count;
	for(i=0;i<x;i++)
	{ if(agent_list->isLeaf==FALSE)
		Write_agents_into_file(agent_list->child[i],ap);
	  write_info_file(agent_list->all_agents[i],ap);
	  
	}
   if(agent_list->isLeaf==FALSE)
	Write_agents_into_file(agent_list->child[i],ap);	 
}
int count_no_of_users(user_tree *users)
{
    if(users==NULL)
	return 0;
    int i=0,x=users->count,count=0;
    for(i=0;i<x;i++)
    {   count++;
    	if(users->isLeaf==FALSE)
    	{
    		count+=count_no_of_users(users->child[i]);
		}
	}
	if(users->isLeaf==FALSE)
		{
	      count+=count_no_of_users(users->child[i]);
		}
  return count;
	
}
void write_uinfo_file(user value,FILE *up)
{
	fprintf(up,"%s\n%s\n%s\n",value.phone_no,value.name,value.address);
}
void Write_user_into_file(user_tree *users,FILE *up)
{
	int i,x=users->count;
	for(i=0;i<x;i++)
	{ if(users->isLeaf==FALSE)
		Write_user_into_file(users->child[i],up);
	  write_uinfo_file(users->all_users[i],up);
	  
	}
   if(users->isLeaf==FALSE)
	Write_user_into_file(users->child[i],up);	
}

int count_no_of_orders(order_tree *pending_orders)
{
    if(pending_orders==NULL)
	return 0;
    int i=0,x=pending_orders->count,count=0;
    for(i=0;i<x;i++)
    {   count++;
    	if(pending_orders->isLeaf==FALSE)
    	{
    		count+=count_no_of_orders(pending_orders->child[i]);
		}
	}
	if(pending_orders->isLeaf==FALSE)
		{
	      count+=count_no_of_orders(pending_orders->child[i]);
		}
  return count;
	
}
void write_oinfo_file(orders nptr,FILE *op)
{
	fprintf(op,"%d\n%s\n%s\n%d\n",nptr.ord_id,nptr.res_name,nptr.res_address,nptr.no_of_items);
	int i=0;
	for(i=0;i< nptr.no_of_items;i++)
	{
		fprintf(op,"%d\n%d\n",nptr.item_index[i],nptr.quantity_index[i]);
	}
	fprintf(op,"%d\n",nptr.allocated_agent->id);
	fprintf(op,"%s\n%f\n",nptr.username->phone_no,nptr.total_price);
}
void Write_order_into_file(order_tree *pending_orders,FILE *op)
{
  orders value;
  int i,x=pending_orders->count;
  for(i=0;i<x;i++)
  {
  	 if(pending_orders->isLeaf==FALSE)
  	   Write_order_into_file(pending_orders->child[i],op);
  	 write_oinfo_file(pending_orders->all_orders[i],op);
  }
  if(pending_orders->isLeaf==FALSE)
  	Write_order_into_file(pending_orders->child[i],op);
  
	
}

agent *search_particular_agent(agent_tree* root,int id)
{   if(root==NULL)
     return  NULL;
	agent *ans=NULL;
	int x=root->count,i,temp=0;
	while(i<x && temp==0)
	{ //printf("Below are agents details:\n");
	 //printf("Age name=%s\nname=%s\n",root->all_agents[i].name,root->all_agents[i].phone_no);
	 //root->all_agents[i].phone_no;
	  if(id > root->all_agents[i].id)
	   	i++;
	   else
	    temp=1;
	}
   if(temp==1)
   { if(id == root->all_agents[i].id )
   	  ans=&(root->all_agents[i]);
	 else if(root->isLeaf==FALSE)
   	  ans=search_particular_agent(root->child[i],id);
   	 
   }
  else
   { if(root->isLeaf==FALSE)
      ans=search_particular_agent(root->child[i],id); 
   }
   return ans;
}
void load_database(loc_tree **root,agent_tree **agent_list,user_tree **users,order_tree **pending_orders,order_tree **archived_orders)
{
	location value;
	int x;
	FILE *fp;
	fp=fopen("restaurant.txt","r");
	fscanf(fp,"%d\n",&x);
	int i;
	for(i=0;i<x;i++)
	{
		fscanf(fp,"%s\n%s\n%s\n%d\n",value.name,value.address,value.zone,&(value.no_of_seats));
		menu *res_menu=(menu*)malloc(sizeof(menu));
		fscanf(fp,"%d\n",&(res_menu->no_of_items));
		int j;
		for(j=0;j< (res_menu->no_of_items);j++)
		{ 
		  fscanf(fp,"%s\n%f\n",res_menu->item[j].itemname,&(res_menu->item[j].price));	
		}
		value.res_menu=res_menu;
		fscanf(fp,"%d\n%d\n",&(value.category),&(value.cuis_category));
		loc_tree* prev=NULL;
		insert(&prev,root,value);
	}
	fclose(fp);
    int max=113;
    FILE *ap;
    ap=fopen("agents.txt","r");
    fscanf(ap,"%d\n",&x);
    agent valu;
    for(i=0;i<x;i++)
    {
       fscanf(ap,"%d\n%s\n%s\n%s\n%f\n%d\n",&(valu.id),valu.name,valu.phone_no,valu.area,&(valu.curr_accu_commi),&(valu.is_available));
	   if(valu.id>max)
	   	max=valu.id;
	   agent_tree *prev=NULL;
	   insert_agent(&prev,agent_list,valu);
	}
	fclose(ap);
	agent_id_allocator=max+1;
	FILE *up;
	up=fopen("user.txt","r");
	fscanf(up,"%d\n",&x);
	user val;
	for(i=0;i<x;i++)
	{
		 fscanf(up,"%s\n%s\n%s\n",val.phone_no,val.name,val.address);
		 user_tree *uptr=NULL;
		 insert_user(&uptr,users,val);
		 //fprintf(up,"%s\n%s\n%s\n",value.phone_no,value.name,value.address);
	}
	fclose(up);
	FILE *op;
	op=fopen("pending_orders.txt","r");
	fscanf(op,"%d\n",&x);
	orders optr;
	for(i=0;i<x;i++)
	{ 
	    fscanf(op,"%d\n%s\n%s\n%d\n",&(optr.ord_id),optr.res_name,optr.res_address,&(optr.no_of_items));
	    int j;
	    for(j=0;j< optr.no_of_items;j++)
	    {
	    	fscanf(op,"%d\n%d\n",&(optr.item_index[j]),&(optr.quantity_index[j]));
		}
	    location *lptr=search_particular_spot(*root,optr.res_name,optr.res_address);
	    optr.ordered_restaurant=lptr;
	    agent *aptr;
	    char phone_no[11];
	    fscanf(op,"%d\n",&j);
	    //printf("agent=%s\n",phone_no);
	    aptr=search_particular_agent(*agent_list,j);
	    optr.allocated_agent=aptr;
	    user *uiptr;
	    fscanf(op,"%s\n",phone_no);
	    //printf("user=%s\n",phone_no);
	    uiptr=search_particular_user(*users,phone_no);
	    optr.username=uiptr;
	    fscanf(op,"%f\n",&(optr.total_price));
	    order_tree *poptr=NULL;
	    insert_order(&poptr,pending_orders,optr);
	}
    fclose(op);
   FILE *alp;
   alp=fopen("allocator.txt","w");
   fscanf(alp,"%d\n",&order_id_allocator); 
   fclose(alp);   
   op=fopen("archived.txt","r");
   fscanf(op,"%d\n",&x);
   for(i=0;i<x;i++)
	{ 
	    fscanf(op,"%d\n%s\n%s\n%d\n",&(optr.ord_id),optr.res_name,optr.res_address,&(optr.no_of_items));
	    int j;
	    for(j=0;j< optr.no_of_items;j++)
	    {
	    	fscanf(op,"%d\n%d\n",&(optr.item_index[j]),&(optr.quantity_index[j]));
		}
	    location *lptr=search_particular_spot(*root,optr.res_name,optr.res_address);
	    optr.ordered_restaurant=lptr;
	    agent *aptr;
	    char phone_no[11];
	    fscanf(op,"%d\n",&j);
	    //printf("agent=%s\n",phone_no);
	    aptr=search_particular_agent(*agent_list,j);
	    optr.allocated_agent=aptr;
	    user *uiptr;
	    fscanf(op,"%s\n",phone_no);
	    //printf("user=%s\n",phone_no);
	    uiptr=search_particular_user(*users,phone_no);
	    optr.username=uiptr;
	    fscanf(op,"%f\n",&(optr.total_price));
	    order_tree *poptr=NULL;
	    insert_order(&poptr,archived_orders,optr);
	}
}
int main()
{int i; 
 statuscode sc;
 loc_tree *root=NULL;
 char name[50];
 char address[50];
 char zone[50];
 int no_of_seats,category,cuis_category;
 menu *res_menu;
 agent_tree *agent_list=NULL,*agent_busy_list=NULL;
 user_tree *users=NULL;
 order_tree *pending_orders=NULL,*archived_orders=NULL;
 load_database(&root,&agent_list,&users,&pending_orders,&archived_orders);
  int x,query=-1;
	printf("\t\t\t\t\t  *********************************************\n");
	printf("\t\t\t\t\t* WELCOME TO OUR LIVE FOOD ORDER TRACKING SYSTEM *\n");
	printf("\t\t\t\t\t  *********************************************\n");
    printf("\n\n\n");  
    int flag=0;
    int *x1;
    int x2;
    x1=&x2;
    while(query!=0 && flag==0)
    {
		printf("\nPress <0> to quit from the system\n");
    	printf("Press <1> to search a restaurant by category\t:\n");
    	printf("Press <2> to search a restaurant by cuisine\t:\n");
    	printf("Press <3> search a restaurant by area\t:\n");
    	printf("Press <4> to get all details of a eating spot\t:\n");
    	printf("Press <5> to take order from a eating pot\t:\n");
    	printf("Press <6> to get the list of all live orders\t:\n");
    	printf("Press <7> to get the details of available agents:\n");
    	printf("Press <8> to complete delivery of a order:\n");
    	printf("Press <9> to get the details of busy agents:\n");
    	printf("Press <10> to search areawise agents\t:\n");
    	printf("Press <11> to cancel a particular order:\n");
    	printf("Press <12> to get details of archived orders:\n");
    	printf("Press <13> to get order history of a particular user:\n");
    	printf("Press <14> to insert new restaurant:\n");
    	printf("Press <15> to insert new agent:\n");
    	printf("press <16> to delete a restaurant:\n");
    	printf("Press <17> to get all details of a particular eating spot\t:\n");
    	scanf("%d",&query);
    	switch(query)
		{
    		case 0:flag=1;break;
        	case 1:search_by_Category(root);break;
        	case 2:search_by_Cuisine(root,x1);break;
        	case 3:search_by_Area(root);break;
        	case 4:Traverse(root);break;
        	case 5:take_order(root,&users,&pending_orders,&agent_list);break;
        	case 6:live_orders(pending_orders);break;
			case 7:agent_details(agent_list);break;
			case 8:delivery(&pending_orders,&archived_orders);break;
			case 9:agent_busy_details(agent_list);break;
			case 10:search_agent_by_area(agent_list);break;
			case 11:cancel(&pending_orders);break;
            case 12:live_orders(archived_orders);break;
            case 13:order_history_particular_user(archived_orders);break;
			case 14:sc=insert_eating_location(&root,zone,name,address,no_of_seats,category,cuis_category,res_menu);break;
        	case 15:insert_new_agent(&agent_list);break;
        	case 16:delete_eating_spot(&root);break;
        	case 17:search_spot(root);
			default:printf("Please enter appropriate choice");
        			break;
    	}
	}
   FILE *fp;
   fp=fopen("restaurant.txt","w");
   x=count_no_of_restaurants(root); 
   fprintf(fp,"%d\n",x);
   if(root!=NULL)  
   Write_data_into_file(root,fp);
   fclose(fp);
   FILE *ap;
   ap=fopen("agents.txt","w");
   x=count_no_of_agents(agent_list);
   fprintf(ap,"%d\n",x);
   if(agent_list!=NULL)
   {
   	Write_agents_into_file(agent_list,ap);
   }
   fclose(ap);
   FILE *up;
   up=fopen("user.txt","w");
   x=count_no_of_users(users);
   fprintf(up,"%d\n",x);
   if(users!=NULL)
   {
   	 Write_user_into_file(users,up);
   }
   fclose(up);
   FILE *op;
   op=fopen("pending_orders.txt","w");
   x=count_no_of_orders(pending_orders);
   fprintf(op,"%d\n",x);
   if(pending_orders!=NULL)
   {
   	 Write_order_into_file(pending_orders,op);
   }
   fclose(op);
   FILE *alp;
   alp=fopen("allocator.txt","w");
   fprintf(alp,"%d\n",order_id_allocator);
   fclose(alp);
   op=fopen("archived.txt","w");
   x=count_no_of_orders(archived_orders);
   fprintf(op,"%d\n",x);
   if(archived_orders!=NULL)
   {
   	 Write_order_into_file(archived_orders,op);
   }
   fclose(op);
   return 0;
  
}
