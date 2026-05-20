#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void printRadar(int rad[10][40]) {

printf("\n\n\n\n\n\n");
    printf("\n================ RADAR VIEW ================\n");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 40; j++) {
            if (rad[i][j]>0)
                printf("🟥");
            else
                printf("🟩");
        }
     printf("\n");
    }
    
    printf("\n===========================================\n");
}

int main()
{
   srand(clock());
   int  rad[10][40]={0};
   
   int t, s,u, reflect;
   
   int m, n;
   printf("enter attack:");
   scanf("%d",&t);
   int x,p, q;
   for(int a=0;a<t;a++){
   x=rand()%15+1;
   int r[x], c[x];
   for(int k=0;k<x;k++) {
    r[k]=rand()%5;
    c[k]=rand()%35+5;
   rad[r[k]][c[k]]=1;}
   
   printf("\a");
    printf("initial phase targets located\n"); 
    sleep(1/2);
    printRadar(rad);
    system("clear");
     for(int i=9;i>=0;i--) {
     for(int j=39;j>=0;j--)
     { reflect=rand()%19+1;
     if(rad[i][j]==1&&(j==0||rad[i][j-1]!=1)) 
      { 
      if(reflect<3) {
      printf("\nhypersonic target detected\n\a");
      printf("position:%d\t%d\n",i,j);
      printf("intercepter missile targeted\n");
      
     sleep(1/2);
     m=rand()%2,n=rand()%2+1; 
     rad[i][j]=0;
     int pro=rand()%2;
     if(pro==0&&i+n<10&&j+m<40)
     rad[i+n][j+m]=1;
     else
     if(pro==1&&i+n<10&&j-m>=0)
     rad[i+n][j-m]=1;
     
     
     for(int p=i;p<=i+2&&p<10;p++){
     for(int q=(j-1>=0?j-1:0);q<=j+1&&q<40;q++) {
     if(rad[p][q]==1) 
     {printf("target location:%d\t%d\n",p,q);
     s=rand()%2,u=rand()%2;
     if((s==1||u==1)&&p<8) {
     printf("\ntarget destroyed\n\a");
      rad[p][q]=0;
      }
     else
    { printf("target missed ‼️\n\a");
      m=rand()%2,n=rand()%2+1; 
     rad[i][j]=0;
     int pro=rand()%2;
     if(pro==0&&i+n<10&&j+m<40)
     rad[i+n][j+m]=2;
     else
     if(pro==1&&i+n<10&&j-m>=0) 
     rad[i+n][j-m]=2;
      
      }}
     }}
     
     printRadar(rad);
     sleep(1);
    system("clear");
    
    } 
     if(reflect>=3&&reflect<=10){
     printf("\ncruise missile detected\n\a");
      printf("position:%d\t%d\n",i,j);
      printf("intercepter missile targeted\n");
    sleep(1/2);
     
      m=rand()%2+1,n=rand()%2+1; 
     rad[i][j]=0;
      int pro=rand()%2;
     if(pro==0&&i+n<10&&j+m<40)
     rad[i+n][j+m]=1;
     else
     if(pro==1&&i+n<10&&j-m>=0)
     rad[i+n][j-m]=1;
     

     for(int p=i;p<=i+2&&p<10;p++){
     for(int q=(j-2>=0?j-2:0);q<=j+2&&q<40;q++) {
     if(rad[p][q]==1) 
     {printf("target location:%d\t%d\n",p,q);
      s=rand()%2,u=rand()%2;
     if((s==1||u==1)&&p<8)
      {printf("\ntarget destroyed\n\a");
      rad[p][q]=0;
      
      }
     else
    { printf("target missed‼️\n\a");
    
      m=rand()%2+1,n=rand()%2+1; 
     rad[i][j]=0;
      int pro=rand()%2;
     if(pro==0&&i+n<10&&j+m<40)
     rad[i+n][j+m]=3;
     else
     if(pro==1&&i+n<10&&j-m>=0)
     rad[i+n][j-m]=3;}
     }
     }
     }
    
     printRadar(rad);
     sleep(1);
    system("clear");
    
     }}
    if(reflect<3&&j>0&&rad[i][j]==1&&rad[i][j-1]==1) {
     printf("\ndrone squadron detected\n\a");
      printf("position:%d\t%d\n",i,j);
      printf("radio jamming targeted\n");
      
      sleep(1/2);
      
      s=rand()%2,u=rand()%2;
     if(s==1)
      {printf("\ntarget destroyed\n\a");
      rad[i][j]=0;
      rad[i][j-1]=0;
      
      }
     else{
     printf("drone intercepter targeted\n");
     if(u==1)
     {printf("\ntarget destroyed\n\a");
     rad[i][j]=0;
     rad[i][j-1]=0;

     }
     else
     printf("target missed⚠️\n\a");
     rad[i][j]=4;
     rad[i][j-1]=4;  
     }
     printRadar(rad);
     sleep(1);
    system("clear");
     
     }
     
     if(rad[i][j]==1&&reflect>10) 
      {printf("\nbalistic  target detected\n\a");
      printf(" targeted position:%d\t%d\n",i,j);
      
      printf("anti-balistic intercepter missile targeted\n");
      
    sleep(1/2);
    
    int m=rand()%2;
    int n=rand()%3+1;
    
     int pro=rand()%2;
     if(pro==0&&i+n<10&&j+m<40)
     rad[i+n][j+m]=1;
     else
     if(pro==1&&i+n<10&&j-m>=0)
     rad[i+n][j-m]=1;
     
      for(int p=i;p<=i+3&&p<10;p++){
     for(int q=(j-1>=0?j-1:0);q<=j+1&&q<40;q++) {
     if(rad[p][q]==1) 
     {printf("target location:%d\t%d\n",p,q);
      s=rand()%2,u=rand()%2;
     if((s==1||u==1)&&p<7)
      {printf("\ntarget destroyed\n\a");
      rad[p][q]=0;
      
      }
     else
    { printf("target penetrated ‼️\n\a");
        
    int m=rand()%2;
    int n=rand()%3+1;
    
     int pro=rand()%2;
     if(pro==0&&i+n<10&&j+m<40)
     rad[i+n][j+m]=1;
     else
     if(pro==1&&i+n<10&&j-m>=0)
     rad[i+n][j-m]=1;
    
    }
     }
     }
     }
     
     
     
     printRadar(rad);
     sleep(1);
    system("clear");
     
      }
      
      
      }}
      
      
      // --- THIS BLOCK CHANGED TO REVERSE AS REQUESTED ---
      for(int i=9;i>=0;i--)
      {
          for(int j=39;j>=0;j--)
          {
            if(rad[i][j]==2) 
            {rad[i][j]=0;
             if(i+2<10) // added bounds protection
             rad[i+2][j]=2;
            }
            else
            if(rad[i][j]==3) 
            {
             rad[i][j]=0;
             if(i+2<10) // added bounds protection
             rad[i+2][j]=2;
            }
            else
            if(rad[i][j]==4) 
            {
             rad[i][j]=0;
             if(i+1<10) // added bounds protection
             rad[i+1][j]=2;
            }
            else
            if(rad[i][j]==1) 
            {
             rad[i][j]=0;
             if(i+3<10) // added bounds protection
             rad[i+3][j]=1;
            
            }
          
          }
              
      }
      
      printRadar(rad);
      sleep(1);
     system("clear");
      
    printf("\n--- searching for Re-engaging all missed targets ---\n");
for(int i=9; i>=0; i--) {
    for(int j=39; j>=0; j--) {
        if(rad[i][j]==1) {
            printf("Retrying balistics targets at position:%d\t%d\n\a", i, j);
           sleep(1/2);
            
            rad[i][j]=0;
            if(i+2<10&&j+2<40) // added bounds protection
            rad[i][j+2]=1;
            
            
            printf("\ntageting position:%d\t%d\n",i,j+2);
            s = rand() % 2;
            if(s == 1) {
                printf("Target finally hit!\n\a");
                if(i+2<10&&j+2<40) // added bounds protection
                rad[i][j+2] = 0;
                
                }
            else 
              {printf("‼️target penetrated‼️\n\a");
              if(i+2<10&&j+2<40) // added bounds protection
              rad[i][j+2]=0;
              }
            
              printRadar(rad) ;    
              sleep(1);
            system("clear");
            }
            
            
           } }
        
     
      for(int e=9; e>=0; e--) {
    for(int f=39; f>=0; f--) {
        if(rad[e][f]==2) {
            printf("Retrying tactical targets at position:%d\t%d\n\a", e, f);
            sleep(1/2);
            s = rand() % 2;
            if(s == 1) {
                printf("Target finally hit!\n\a");
                rad[e][f] = 0;
      
     
            } else 
              {printf("‼️target penetrated‼️\n\a");
              rad[e][f] = 0;
              
              }
              printRadar(rad);
              sleep(1);
             system("clear");
    
            }
            
           } }
     
      
      
     
     
     printf("\nwave neutralized\n");
    
     }
    
    

    
    return 0;
}
    