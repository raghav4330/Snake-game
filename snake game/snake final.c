#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
#define maxrow 25
#define maxcol 90    //i,e   41 by 41
#define num 3

struct snake
{
    int x,y;
    char value;
    struct snake *next;
};
struct food
{
    int x,y;
    char value;
};
struct poison
{
    int x,y;
    char value;
};


void print(struct snake *head);
struct snake *snake_shifter(struct snake *head,int x,int y,struct food *f,struct poison *p);
void swap(int *a,int *b);
int valid_move(struct snake *head,char m);
void game_over(struct snake *head,int eat[maxrow][maxcol]);
int gameover_level2(struct snake *head,int y,char m);
int gameover_level3(struct snake *head,int x,int y,char m);
struct snake *increase_snake(struct snake *head,int x,int y);
struct snake *decrease_snake(struct snake *head);
int if_space_find(struct snake *head,int x,int y,struct food *f,struct poison *p);
int is_food_eaten(struct snake *head,struct food *f);

struct snake *savegame(struct snake *head,struct food *f,struct poison *p,char m,int *flag);
void save_new_highscore(int score);
struct snake* assign_values(struct snake *temp,int x,int y,char val);


static int over=0;
static int pause=0;
int saved=0;
 int score,highscore;




 //////

 enum colors{ BLACK,BLUE,GREEN,AQUA,RED,PURPLE,YELLOW,WHITE,GRAY,
LIGHT_BLUE,LIGHT_GREEN,LIGHT_AQUA,LIGHT_RED,
LIGHT_PURPLE,LIGHT_YELLOW,BRIGHT_WHITE
};


void gotoxy(int x, int y)
{

	//x=x+A;
	//y=y+B;

    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}



/*typedef struct CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
void font()
{
	CONSOLE_FONT_INFOEX cfi;

cfi.cbSize = sizeof(cfi);
cfi.nFont = 0;
cfi.dwFontSize.X = 20;                  // Width of each character in the font
cfi.dwFontSize.Y = 24;                  // Height
cfi.FontFamily = FF_DONTCARE;
cfi.FontWeight = FW_NORMAL;
wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
//SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}*/

void setcolor(int color)
{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
SetConsoleTextAttribute(hStdOut,color);

}

 /////





void setboard(char mat[maxrow][maxcol],struct snake *head,struct food f[],struct poison p[])
{
    int i,j;

    for(i=0;i<maxrow;i++)
    {

        for(j=0;j<maxcol;j++)
        {
            if(j==0 || j==maxcol-1)
                mat[i][j]='|';
            else
                mat[i][j]=' ';
        }
    }


   struct snake *t=head;
        while(t!=NULL)
        {
            //printf("%c",t->value);
            mat[t->x][t->y]=t->value;
            t=t->next;
        }

        for(i=0;i<num;i++)
        {
            mat[f[i].x][f[i].y]=f[i].value;
            mat[p[i].x][p[i].y]=p[i].value;
        }

}


print_board(char mat[maxrow][maxcol],int time)
{
    //char mat[maxrow][maxcol];
    gotoxy(0,0);
int i,j;
printf("%d\n",time);
setcolor(LIGHT_YELLOW);
printf("\nSCORE %d \t\t\t\t\t\t\t\t\t\t\t\t  \t\t HIGHSCORE %d\n\n\n",score,highscore);

gotoxy(20,6);
setcolor(BRIGHT_WHITE);

       if(time>=250)
          setcolor(LIGHT_RED);


    for(i=0;i<=maxrow+maxrow+39;i++)
        printf("!");

//gotoxy(20,5);
       if(time>=150)
          setcolor(LIGHT_RED);



      for(i=0;i<maxrow;i++)
    {
        gotoxy(20,7+i);
        for(j=0;j<maxcol;j++)
        {
            if(mat[i][j]=='Q' )  /// || mat[i][j]=='*'
                setcolor(LIGHT_BLUE);
            else if(mat[i][j]=='P')
                setcolor(LIGHT_RED);
            else if(mat[i][j]=='F')
                setcolor(LIGHT_GREEN);
            else if(mat[i][j]=='*' || mat[i][j]=='0')
                 setcolor(BRIGHT_WHITE);

            printf("%c",mat[i][j]);

            setcolor(BRIGHT_WHITE);
            if(time>=150)
               setcolor(LIGHT_RED);

        }

    }
setcolor(BRIGHT_WHITE);
///if(time>=150)
   ///setcolor(LIGHT_RED);

    gotoxy(20,7+i);
    for(i=0;i<=maxrow+maxrow+39;i++)
        printf("!");


}



struct snake *create_snake()
{
    struct snake *temp;
    temp=(struct snake *)malloc(sizeof(struct snake));
    return temp;
}









int if_space_find(struct snake *head,int x,int y,struct food *f,struct poison *p)
{
    //printf("h\n");
    //printf("%d %d\n",x,y);
    struct snake *t=head;
    while(t!=NULL)
    {
        if(t->x==x && t->y==y)
            return 0;
        t=t->next;
    }

    if(y==0)               ///for food on boundary
        return 0;

int i=0;

    for(i=0;i<num;i++)
    {
        if(f[i].x==x && f[i].y==y)
        return 0;
    }

    for(i=0;i<num;i++)
    {
            if(p[i].x==x && p[i].y==y)
            return 0;
    }


    return 1;
}



void getfood_poison(struct snake *head,struct food *f,struct poison *p,int index,int flag)
{
    //printf("h\n");

    int x,y;
    //int flag=1;

    srand(time(0));

    do
    {
     x=(rand()%(maxrow-1));
     y=(rand()%(maxcol-2));
    }
    while(!if_space_find(head,x,y,f,p));

//printf("%d %d\n",index,flag);
    if(flag==1)
    {
       f[index].x=x;
       f[index].y=y;
       f[index].value='F';
    }
    else
    {
       p[index].x=x;
       p[index].y=y;
       p[index].value='P';
    }


}



struct snake *start(struct snake *head,struct food *f,struct poison *p)
{
    struct snake *temp,*t;
    int k=0,i,m=7;

    if(head==NULL)
    {

        for(i=0;i<4;i++)
        {
            temp=create_snake();
            if(i==0)
            {
                temp->value='Q';
                temp->x=10;
                temp->y=8;
                temp->next=NULL;

                head=temp;
                t=head;
            }
            else if(i==1||i==2)
            {
                temp->value='0';
                temp->x=10;
                temp->y=m--;
                temp->next=NULL;
                t->next=temp;
                t=temp;
            }
            else
            {
                temp->value='*';
                temp->x=10;
                temp->y=5;
                temp->next=NULL;
                t->next=temp;
            }
            //printf("%c",head->value);
        }

        ///setting food
        for(i=0;i<num;i++)
        getfood_poison(head,f,p,i,1);
        ///setting poison
        for(i=0;i<num;i++)
        getfood_poison(head,f,p,i,0);

    }
    return head;
}






char getmove(struct snake *head,char m)
{
    char c;
    if(kbhit())
        {
            c=getch();
            if(c==' ')
                pause=1;
            else if(valid_move(head,c))
                m=c;

        }

    return m;
}


char get_direction(struct snake *head)
{
    char c='z';   ///random character except w,a,s,d
    if(head->next!=NULL)
    {

    int k=head->x;
    int l=head->next->x;

    int m=head->y;
    int n=head->next->y;

    if(k==l)
    {
        if(m==n+1 ||(m==1 && n==maxcol-2))
            c=100;
        else
            c=97;
    }
    else if(m==n)
    {
        if(k==l+1 || (k==maxrow-1 && l==0))
           c=115;
        else
            c=119;

    }

    }
    return c;
}


int valid_move(struct snake *head,char m)
{
    pause=0;

    char c=get_direction(head);

    if(m==100 && c!=97)
        return 1;
    else if(m==97 && c!=100)
        return 1;
    else if(m==119 && c!=115)
        return 1;
    else if(m==115 && c!=119)
        return 1;
    else
        return 0;

}


struct snake *move(struct snake *head,char m,struct food *f,struct poison *p,int time)
{

        int x=head->x;
        int y=head->y;

        if(m==119)
        {
            if(head->x==0)
                x=maxrow-1;
            else
                x--;
        }
        else if(m==115)
        {
            if(head->x==maxrow-1)
                x=0;
            else
                x++;
        }

        else if(m==97)
        {
            if(head->y==1)
                y=maxcol-2;
            else
                y--;
        }
        else if(m==100)
        {
            if(y==maxcol-2)
                y=1;
            else
                y++;
        }
 /*if(time>=10)
        if(gameover_level2(head,y,m))
           return head;

         if(time>=20)
        if(gameover_level3(head,x,y,m))
           return head;*/


        head=snake_shifter(head,x,y,f,p);  ///in this function gameover function is also called



    return head;
}



struct snake *snake_shifter(struct snake *head,int x,int y,struct food *f,struct poison *p)
{
    struct snake *t=head;
    int eat[maxrow][maxcol]={0};

    while(t!=NULL)
    {
        eat[x][y]++;
        swap(&x,&t->x);
        swap(&y,&t->y);

        t=t->next;
    }



    int index=is_food_eaten(head,f);
     if(index!=-1)
        {
            eat[x][y]++;
            //printf("yes");
            head=increase_snake(head,x,y);
            getfood_poison(head,f,p,index,1);
        }


        index=is_poison_eaten(head,p);
         if(index!=-1)
        {
            //printf("yes");
            head=decrease_snake(head);
            getfood_poison(head,f,p,index,0);
        }

game_over(head,eat);

    return head;
}




struct snake *increase_snake(struct snake *head,int x,int y)
{
    struct snake *temp=create_snake();
    temp->x=x;
    temp->y=y;
    temp->value='*';
    temp->next=NULL;

    if(head->next==NULL)
        head->next=temp;
    else
    {
        struct snake *t=head;
        while(t->next!=NULL)
        {
            t=t->next;
        }

        t->value='0';
        t->next=temp;
    }

    return head;
}




struct snake *decrease_snake(struct snake *head)
{
    //printf("hihbjhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
    struct snake *t=head,*p=head;

    if(head->next!=NULL)
    while(t->next!=NULL)
    {
        p=t;
        t=t->next;
    }

    if(p!=head)  ///i,e atleast snake have 3 nodes
    {
        p->value='*';
        p->next=NULL;
    }
    else if(p==head && t==head->next)
       p->next=NULL;
    else
    {
        over=1;
        head=NULL;
    }

    return head;
}


int is_food_eaten(struct snake *head,struct food *f)
{
    int i;
    for(i=0;i<num;i++)
    {
        if(head->x==f[i].x && head->y==f[i].y)
        {
            score++;
            {
                if(score>highscore)
                    save_new_highscore(score);
            }
            return i;
        }
    }

        return -1;
}


void save_new_highscore(int score)
{
    highscore=score;
    FILE *ff=fopen("savehighscore.txt","w");
    fprintf(ff,"%d%c",highscore,'\0');
    fclose(ff);
}

int is_poison_eaten(struct snake *head,struct food *p)
{
    int i;
     for(i=0;i<num;i++)
    {
        if(head->x==p[i].x && head->y==p[i].y)
        return i;
    }

        return -1;
}



void game_over(struct snake *head,int eat[maxrow][maxcol])
{
    int x,y,flag=0;

    if(head!=NULL)
    {

        x=head->x;
        y=head->y;

        if(eat[x][y]==2)
            flag=1;
    }
    if(head==NULL || flag==1)
    {
        over=1;
        //printf("\n\ngame over....better luck next time\n\n\n");
        //exit(1);
    }
}

/*int gameover_level2(struct snake *head,int y,char m)
{

    if((m==100 && y==1)||(m==97 && y==maxcol-2))
    {
        over=1;
        return 1;
    }
    return 0;
}

int gameover_level3(struct snake *head,int x,int y,char m)
{

    if((m==100 && y==1)||(m==97 && y==maxcol-2))
    {
        over=1;
        return 1;
    }
    if((m==119 && x==maxrow-1)||(m==115 && x==0))
    {
        over=1;
        return 1;
    }
    return 0;
}*/

void swap(int *a,int *b)
{
    char z=*a;
    *a=*b;
    *b=z;
}



struct snake* pause_menu(struct snake *head,struct food *f,struct poison *p,char m)
{
	setcolor(LIGHT_YELLOW);
    printf("\nscore:\n",score);
    char pp;
    int flag=0;
    printf("\n1.save game\n2.exit\n\npress spacebar to resume");
    do{
    	setcolor(BRIGHT_WHITE);
            printf("\n->");
    pp=getch();
    if(pp==48+2)
        exit(1);

    else if(pp==48+1)
        head=savegame(head,f,p,m,&flag);
    }while(pp!=' ');

    pause=0;
system("cls");
    return head;
}


int start_menu()
{
	setcolor(BRIGHT_WHITE);
    int n,i=1;
	FILE *fp=fopen("snakegame.txt","r");
   do
    {
        system("cls");
    printf("SNAKE GAME\n\n");
    setcolor(LIGHT_YELLOW);
    printf("%d.New Game\n",i++);
    if(fp!=NULL)
    printf("%d.Load Game\n",i++);
    printf("%d.Exit\n",i++);

setcolor(BRIGHT_WHITE);
 fclose(fp);
    scanf("%d",&n);
    if(n==i-1)
    exit(1);
    else if(n==1 || (n==2 && fp!=NULL))
        return n;
    else
      printf("\nselect right option:\n");
      i=1;
    getch();
    }while(1);
}

/*void print(struct snake *head)
{
    struct snake *t=head;
    while(t!=NULL)
    {
        printf("%c",t->value);
        t=t->next;
    }
}*/
////////




struct snake *savegame(struct snake *head,struct food *f,struct poison *p,char m,int *flag)
{
	if(*flag==1)
	return head;


	FILE *fp;
	fp=fopen("snakegame.txt","w");
	struct snake *t=head;
	while(t!=NULL)
    {
		fprintf(fp,"#%d,%d,%c",t->x,t->y,t->value);
		t=t->next;
	}

	int i;
	for(i=0;i<num;i++)
	{
		fprintf(fp,"+%d,%d,%c",f[i].x,f[i].y,f[i].value);
	}


	for(i=0;i<num;i++)
	{
		fprintf(fp,"-%d,%d,%c",p[i].x,p[i].y,p[i].value);
	}

    fprintf(fp,"$%c",m);
    //printf("score=%d",score);
	fprintf(fp,"@%d",score);
	fprintf(fp,'\0');

	fclose(fp);

    saved=1;
	printf("\nGame have been saved");
	*flag=1;
	return head;
}





struct snake *loadgame(struct snake *head,struct food *f,struct poison *p,char *m)
{

	struct snake *t=head;
	FILE *fp=fopen("snakegame.txt","r");
	char s[300];
	fgets(s,300,fp);
fclose(fp);

	int i=0,x,y,k=0,b=0;
	struct snake *temp;

	while(s[i]!='\0')
	{

		if(s[i]=='#')
		{
			temp=create_snake();
			i++;
		    i=fun(s,i,&x);
			i++;
			i=fun(s,i,&y);
			i++;
		    temp=assign_values(temp,x,y,s[i]);
			if(head==NULL)
			{
				head=temp;
				t=head;
			}
			else
			{
			    t->next=temp;
			    t=temp;
			}
			//printf("%d %d %c",temp->x,temp->y,temp->value);
		}

		else if(s[i]=='+')
		{
			i++;
			i=fun(s,i,&x);
			i++;
			fun(s,i,&y);
			i++;
			f[k].x=x;
			f[k].y=y;
			f[k].value='F';
			k++;
		}
		else if(s[i]=='-')
		{
			i++;
			i=fun(s,i,&x);
			i++;
			fun(s,i,&y);
			i++;
			p[b].x=x;
			p[b].y=y;
			p[b].value='P';
			b++;
		}
		else if(s[i]=='$')
        {
            i++;
            *m=s[i];
        }
		else if(s[i]=='@')
		{
			i++;
			int n;
			i=fun(s,i,&n);
			score=n;
			//printf("%d",n);
			//getch();
		}

		i++;
	}

	return head;
}

int fun(char *s,int i,int *n)
{
	int sum=0;
	char c;
		while(s[i]!=',' && s[i]!='\0')  //space if for score which is place d at last
			{
				c=s[i];
				sum=(sum*10)+(c-48);
			    ++i;
			}
			*n=sum;
			return i;
}

struct snake* assign_values(struct snake *temp,int x,int y,char val)
{
		    temp->x=x;
			temp->y=y;
			temp->value=val;
			temp->next=NULL;
			return temp;
}


/*void print(struct snake *head)
{
    struct snake *t=head;
    while(t!=NULL)
    {
        printf("%c",t->value);
        t=t->next;
    }
}*/

void thankyou()
{
	setcolor(LIGHT_YELLOW);
    printf("\n\nyour score=%d",score);
    printf("\n\nhighscore=%d",highscore);
    printf("\n\ngame over....Better luck next time\n\n\n");
    exit(1);
}

int get_time(int starting)
{
	int end=clock();
    int t=(end-starting)/CLOCKS_PER_SEC;
    return t;
}

void fullscreen()
{
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}
////////



int main()
{
    fullscreen();
    //void sound(1);
    //sound(200);
time_t starting,end;


    FILE *ff=fopen("savehighscore.txt","r");
    fscanf(ff,"%d",&highscore);
    fclose(ff);
    system("color 4A");
    struct snake *head=NULL;
    struct food f[num];
    struct poison p[num];
   // font();

    int y=start_menu(),start_flag=0,t=0;
    system("cls");
char m=100;

    if(y==1)
    head=start(head,f,p);
    else if(y==2)
        head=loadgame(head,f,p,&m);


    char board[maxrow][maxcol];
    setboard(board,head,f,p);
    print_board(board,t);

setcolor(LIGHT_YELLOW);
    printf("\npress any button to start\n");
    char c=getch();
    system("cls");
    //head=move(head,m,f,p);

    //char m=100;
    while(1)
    {

if(start_flag==0)
{
	start_flag=1;
	starting=clock();
}
int t= get_time(starting);
//printf("%d\n",t);


       m=getmove(head,m);
       if(pause==1)
       {
           print_board(board,t);    ///t is for time elapsed
           head=pause_menu(head,f,p,m);
       }
       head=move(head,m,f,p,t);

       if(over==1)
       {
           print_board(board,t);
           printf("\n");
           system("pause");
       	if(y==2)                       ///if game was loaded
		remove("snakegame.txt");
         thankyou();
       }

setboard(board,head,f,p);
print_board(board,t);           ///t is for time elapsed



//Sleep(40);
//       system("cls");
    }
    //print(head)*/;
}


