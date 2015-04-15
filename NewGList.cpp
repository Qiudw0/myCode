#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<malloc.h>

#define AtomType char
#define MAXSIZE  1024                               
#define OK       1
#define ERROR    0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2

typedef int Status;
typedef enum {ATOM,LIST} ElemTag;
typedef struct GLNode                   
{                                      
	ElemTag tag;
	union
	{
		AtomType  atom;
		struct GLNode *hp;
	}ptr;
    struct GLNode *tp;
}GLNode,*GList;
                                     
/********************���帴���ַ����ĺ���**************************************/
//���ܣ���str2�ַ������Ƹ�str1
//���أ����Ƴɹ�����OK

Status StrCopy(char *str1,char *str2){
	int i;
	for(i=0;*(str2+i);i++)
		*(str1+i)=*(str2+i);
	*(str1+i)='\0';
	return OK;
}
                             
/*******************��������ַ�������*****************************************/
//���ܣ����str�ַ���
//���أ��ɹ�����OK

Status ClearString(char *str){  //��sever�����е���
	if(str){     //��Ϊ������
		*(str)='\0';
	}
	return OK;
}

/*******************��������ַ������ȵĺ���***********************************/
//���ܣ������ַ���str�ĳ���
//���أ��ɹ�����str�ĳ���len
int Strlen(char *str){
	int i,len=0;                              
	for(i=0;*(str+i);i++){
		len++;
	}	                  
	return len;
}

/*******************����ȡ�Ӵ�����*********************************************/
//���ܣ���str�ַ�����ȡ����posλ���𳤶�ΪLen���ַ���������sub���ظ��ַ���
//���أ��ɹ�����OK

Status SubString(char *sub,char *str,int pos,int len){//��sub�����ַ���str�е�posλ���𳤶�Ϊlen���ַ���   
	int i;
	if(pos<1||pos>Strlen(str)||len<0||len>Strlen(str)-pos+1) return ERROR;            
	else{
		if(!len){//���ؿ��Ӵ�
			*sub='\0';
		}                                    
		else{//���������Ӵ�                  
			for(i=0;i<len;i++){
				*(sub+i)=*(str+(pos-1+i));
			}                                         
			*(sub+i)='\0';
		}                                                   
		return OK;
	}
}                                            

/*******************������','Ϊ�ָ�㽫�ǿմ�str�ָ�������ֵĺ���*************/
//���ܣ���str�ַ�����ȡ����һ��','λ��֮ǰ���ַ���������sub���ظ��ַ���
//���أ��ɹ�����OK
           
Status sever(char* str,char *hstr){
	int i=0,k=0,n;
	char ch[5];
	n=Strlen(str);
	do{                                                    
		++i;
		SubString(ch,str,i,1);
		if(ch[0]=='(') ++k;   //k��¼��δ��Ե������ŵĸ���
		else if(ch[0]==')') --k;
	}while(i<n&&(ch[0]!=','||k!=0));                             
	if(i<n){        //i��ʾ��һ��','��λ��
		SubString(hstr,str,1,i-1);    //�����','֮ǰ���ַ���,Ҫ��Ҫ����','������
		SubString(str,str,i+1,n-i);   //��','֮����ַ�������str
	}
	else{                          
		StrCopy(hstr,str);  //�����һ���Ӵ�����hstr
		ClearString(str);  //�ͷŸ��ַ�����ռ�õĿռ�
	}
	return OK;
}

/*******************����visit���ʺ���*****************************************/
//���ܣ����ַ�c���
//���أ��ɹ�����0
int visit(char c){
	printf("%c ",c);
	return 0;              
}

/******************���幹���չ����ĺ���**************************************/
//���ܣ������չ����
//���أ����ع����Ŀչ����
GLNode * InitGList(){//�����յĹ����L
	GLNode *L;
	if(!(L=(GLNode *)malloc(sizeof(GLNode)))) exit(OVERFLOW);
		 L->tag=LIST;
		 L->ptr.hp=NULL;
		 L->tp=NULL;
	return L;                    
}

Status GListEmpty(GList L){//�жϹ����L�Ƿ�Ϊ��
	if(!L->ptr.hp && !L->tp) return TRUE;
	else return FALSE;
}

/******************���幹�������ĺ���****************************************/ 
//���ܣ������ַ���s�����������
//���أ��ɹ��򷵻ؽ����Ĺ����ı�ͷhead�����򣬷���NULL
GLNode * CreateGList(char *s)   
{
	GLNode *p,*q,*r,*head;
    char sub[MAXSIZE],hstr[MAXSIZE];
    int len;                           
	len = Strlen(s);
    if ( !strcmp(s,"()") || !len)  head = NULL;     //  �ձ����
	else if (len == 1)                              // ԭ�����
	{
		head=InitGList();   //������ջ     
		head->tag =ATOM;                              // ����ԭ�ӽ��
		head->ptr.atom = *s;
	}                                
    else                                            //  �ӱ����
	{
		head=InitGList();
		p = head;
		SubString(sub,s,2,Strlen(s)-2);  //��ȥ�������,��sub�����ַ���
		do 
		{
			sever(sub,hstr);  //����֮��hsub����sub�е�һ��','ǰ�Ĳ�������ͷ��sub��������沿������β
			r = CreateGList(hstr);
			p->ptr.hp = r;                           // β�巨����
			q=p;
			len = Strlen(sub);
			if (len > 0)
			{
				p = (GLNode*)malloc(sizeof(GLNode));
				if (!p) return NULL;
				p->tag =LIST;
				q->tp=p;                   
			}
		} while (len > 0);
		q->tp=NULL;
	}
    return head;
}

/******************�������ٹ����ĺ���****************************************/
//����: �����Ѿ����ڵĹ����L           
void DestroyGList(GList L)
{                                          
  if(L!=NULL) 
  L->ptr.hp=NULL; 
}

/******************�����ȡ������ͷԪ�صĺ���*********************************/ 
//���ܣ�ȡ�������L�ı�ͷ����
//���أ��ɹ��򷵻ع����L�ı�ͷ�����򣬷���NULL
GList GetHead(GList L)
{                                          
	GList P;
	P=InitGList();
	if (!L)  return (NULL);                    // �ձ��ޱ�ͷ
	if (L->tag == ATOM){                       // ԭ�ӽ�㲻�Ǳ�
		P->tag=ATOM;
		P->ptr.atom=L->ptr.atom;
		return P;		
	}             
	else return (L->ptr.hp);
}

/*****************�����ȡ������βԪ�صĺ���*********************************/
//���ܣ�ȡ�������L�ı�β����
//���أ��ɹ����ع����L�ı�β���֣����򣬷���NULL
GList GetTail(GList L)
{
	GList P;
	P=InitGList();
	if (!L) return (NULL);                        
	if (L->tag == ATOM){
		P->tag=ATOM;
		P->ptr.atom=L->ptr.atom;
		return P;
		
	}
	else return (L->tp);
}

/*****************�����������ȵĺ���*************************************/
//����:��������L�ĳ���
//����:�����ĳ���GLlen
int GListLength(GList L)
{
	int GLlen=0;
	GLNode *s;                                   
	if (!L) return 0;                    // �ձ�ĳ���Ϊ��
	if (L->tag ==ATOM) exit(0);            // ԭ�Ӳ��Ǳ�
	s=L;
	while(s)                             // ͳ�Ʊ�����ϲ�ĳ���
	{
		GLlen++;
		s=s->tp;
	}
	return GLlen;
}                                         

/****************����������L����ȵĺ���************************************/
//���ܣ���ù����L�����
//���أ������L�����dep
int GListDepth(GList L)
{
	int dep,max;
	GLNode *s;

	if (!L)  return (1);             // �ձ�����Ϊ 1
	if (L->tag==0)  return 0;        // ԭ�ӵ����Ϊ 0          
	s=L;
	for(max=0;s;s=s->tp){
		dep=GListDepth(s->ptr.hp);   //���ͷ���
		if(dep>max) max=dep;
	}
	return (max+1);                  // ������Ϊ�ӱ���ȼ�һ
}

/*************���彫�����res���Ƶ������dest�ĺ���******************************/
//���ܣ���ɹ����ĸ���,��res���Ƶ�dest��
//���أ��ɹ�����OK�����򣬷���ERROR
Status  CopyGList(GList *dest,GList res)
{
	*dest=InitGList();
	if (!res) {*dest = NULL;return 1;}         
	(*dest)->tag = res->tag;
	if (res->tag==ATOM)  (*dest)->ptr.atom = res->ptr.atom;
    else
    {
		CopyGList(&(*dest)->ptr.hp,res->ptr.hp);
		CopyGList(&(*dest)->tp,res->tp);
	}
	return OK;               
}

/**************�������Ԫ��e��Ϊ�����L�ĵ�һԪ�صĺ���***************************/
//���ܣ��������e���뵽�����L����Ϊ��ͷԪ��
//���أ��ɹ�����OK�����򣬷���ERROR
Status InsertFirst_GL(GList *L,GList e){      
	GList temp;
	CopyGList(&temp,*L);
	(*L)->ptr.hp=e;
	(*L)->tp=temp;
	return OK;
}

/******************����ɾ��������е�һԪ�صĺ���****************************/
//���ܣ��������L�еı�ͷԪ��ɾ��
//���أ�����L�ı�ͷP��ͬʱL�ı�
GList DeletFirst_GL(GList *L){//ɾ�������ĵ�һԪ�أ�����e����
	GList P;
	P=InitGList();
	if(*L){
		P=(*L)->ptr.hp;
		*L=(*L)->tp;   //Lָ���β
		//(*L)->ptr.hp=(*L)->tp;  //L�ı�β��ֵ��L�ı�ͷ���ﵽֻɾ����һԪ�ص�Ŀ��
		//(*L)->tp=NULL;
		P->tp=NULL;   //P�ı�β�ɲ�����Ϊ��
	}
	return P;                                     
} 

/******************����������ʹ����Ԫ�صĺ���*************************************/
//���ܣ����������L�е�Ԫ��ֵ�������ú���visit(char)������
void Traverse(GList L,int (*f)(char c))
{
	f=visit;
	if (L)
	{
		if (L->tag==ATOM){                   
			f(L->ptr.atom);
		} 
		else  Traverse(L->ptr.hp,f);            // ���±��������ƶ������е�������
		if (L->tp) Traverse(L->tp,f);           // ���ұ��������ƶ������е�������
	}
}                       

/******************������������Ԫ�صĺ���****************************************/
//���ܣ��Թ����headԪ�ذ���ԭʼ���ַ�����ʽ���
//���أ��ɹ�����֮�󷵻�OK

Status DisplayGList(GList head)
{
	GLNode *p,*q;    
	if (!head)  return OK;
	if (head->tag==0)
	{
		printf("%c",head->ptr.atom);
	}
	printf("(");
	if (head)
	{
		do 
		{
			p = head->ptr.hp;
			q = head->tp;
			//����ͬһ��ε�ԭ��,�������ӱ������£������Ϊ1
			while (q && p && p->tag == 0)                //  ͬһ���ԭ�ӽ��
			{
				printf("%c,",p->ptr.atom);
				p = q->ptr.hp;
				q = q->tp;
			}
			if (p && !p->tag)                           //����ͬһ��ԭ��֮��ʣ�µ����һ��ԭ�ӽ��
			{
				printf("%c",p->ptr.atom);
				break;
			}
			else                                        // �ӱ����
			{
				if (!p) printf("()");
				else DisplayGList(p);                  //�����ӱ�͵ݹ����
				if (q)  printf(",");
				head = q;
			}
		} while (head);
		printf(")");
	}
	return OK;
}

/******************������***********************************************************/ 
void main()                     
{
	char s[MAXSIZE],temp[MAXSIZE],choice='Y',input='Z';  //'Z'�ַ����κ�����
	GList head,GetH,GetT;
    GList L,Q,E,P;
    printf("\n\n\n");         
    printf("	---------------------  ADT �����ʵ��  -------------------------\n\n");
    printf("	*                                                              *\n\n");
    printf("	*              ����:  ��ΰ         ѧ��:3112006207           *\n\n");
    printf("	*              �γ̣����ݽṹ        ָ����ʦ����޼            *\n\n");
    printf("	*                                                              *\n\n");
    printf("	----------------------------------------------------------------\n");
	printf("\n\n\n");
	printf("	��������Ҫ�����Ĺ����:");
	gets(s);
	head = CreateGList(s);  //���������       
	if(head){
		 printf("	----------------------------------------------------------------\n");
		 printf("				��������ɹ���\n");
		 printf("	----------------------------------------------------------------\n");
		 printf("	�����Ĺ����Ϊ��");
		 DisplayGList(head);    //��������Ĺ����head
		 printf("\n");
	}
	else{
		 printf("	----------------------------------------------------------------\n");
		 printf("				�������ʧ�ܣ�\n");
		 printf("	----------------------------------------------------------------\n");
	}                   
    while (choice=='Y'){
		printf("	****************************************************************\n");     
        printf("	*                         ADT�����ʵ��                        *\n");
        printf("	****************************************************************\n");
        printf("	*      A.�½������      B.ȡ��ͷ         C.ȡ��β             *\n");
        printf("	*      D.�󳤶�          E.�����         F.���ձ�           *\n");
        printf("	*      G.���ƹ����      H.�����ͷԪ��   I.���������         *\n");
        printf("	*      J.ɾ����ͷԪ��    K.���ٱ�         Q.�˳�               *\n");
        printf("	*                                                              *\n");
        printf("	****************************************************************\n\n");
        printf("	��������Ҫѡ��Ĺ���:");
		if(input!='Z') choice=getchar();   //���ڽ��ܻ��з�,��������ֵ֮��Ļ��з�����һ��ȡֵ����Ӱ��
		scanf("%c",&input);                       
        switch (input)
        {
        case 'A':
        case 'a':             
            printf("\n	��������Ҫ�½��Ĺ����:");
        	scanf("%s",s);
        	head = CreateGList(s);
			printf("	�����Ĺ����Ϊ��");
			DisplayGList(head);
	        printf("\n");
            break;
        case 'B':
        case 'b':
            GetH=GetHead(head);   //��ȡ��ͷ
            if(GetH&&GetH->tag!=ATOM){
				printf("\n	������ͷ�ǣ�");
				DisplayGList(GetH);   //�����ͷ
			}
			else if(!head && !GetH){
				printf("\n	�ձ��ޱ�ͷ��");
			}
			else if(GetH->tag==ATOM){
				printf("\n	������ͷ�ǣ�%c",GetH->ptr.atom);
			}
			else{
				printf("\n	������ͷ�ǣ�()");
			}
			printf("\n");          
            break;
        case 'C':
        case 'c':
			GetT=GetTail(head);   //��ȡ��β
            if(GetT){
				printf("\n	������β��: ");
				DisplayGList(GetT);   //�����β
			}
			else if(!head && !GetH){
				printf("\n	�ձ��ޱ�ͷ��");
			}
			else if(GetH->tag==ATOM){
				printf("\n	������ͷ�ǣ�%c",GetH->ptr.atom);
			}
			else{
				printf("\n	������ͷ�ǣ�()");
			}
			printf("\n");        
            break;
        case 'D':
        case 'd':
       	    printf("\n	��������ǣ� %d\n",GListLength(head));
            break;
        case 'E':
        case 'e':
            printf("\n	����������: %d\n",GListDepth(head));
            break;
		case 'F':
        case 'f':
            if(GListEmpty(head))
				printf("\n	����һ���յĹ����\n");
            else printf("\n	 ����һ���ǿչ����\n");
            break;
		case 'G': 
        case 'g':           
            CopyGList(&L,head);
			printf("\n	���ƹ����:");
        	DisplayGList(L);   //������ƵĹ����
	        printf("\n");           
            break;
		case 'H':  
        case 'h':
			choice=getchar();
            printf("\n	������Ҫ�����Ԫ�أ�");     
			gets(temp);
			E=CreateGList(temp);       //ͨ���ַ���temp���������E
			InsertFirst_GL(&head,E);   // �������E��Ϊ��ͷ����head�������
			printf("\n	����Ԫ��֮��Ĺ�������£�\n");
			printf("		");            //��������б���
			DisplayGList(head);        //����±�        
            break;
		case 'I':
        case 'i':           
            printf("\n	�������ʹ����:\n");
			printf("	");//���һ���ַ����
	        Traverse(head,visit);
        	printf("\n");          
            break;
		case 'J': 
        case 'j':        
            P=DeletFirst_GL(&head);    //ɾ����ͷԪ��
			printf("\n	ɾ����ͷԪ��֮��Ĺ�������£�\n");
			printf("	");            //���һ���ַ����
			DisplayGList(head);        //����±�
			printf("\n");
			break;
		case 'K':
        case 'k':
            DestroyGList(L);
            if(L->ptr.hp==NULL) 
            printf("\n	���ٳɹ�!\n");
            else 
            printf("\n	����ʧ�ܣ�\n");     
            break; 
        case 'Q':
        case 'q':                              
			exit(1) ;
		default:
			printf("\n	----------------------------------------------------------------\n");
			printf("			������������\n");
			printf("	----------------------------------------------------------------\n");

        }//switch
		if(input!='h'&& input!='H') choice=getchar();      //�������ܻ��з�
		printf("\n	�����Ҫ����ִ������ָ��밴��Y��������'N'��");
		choice=getchar();      //��������ָ��
    }//while		
}//main

