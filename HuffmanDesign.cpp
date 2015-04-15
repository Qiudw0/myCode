/*****���������͹���������Ĵ洢��ʾ*****/

typedef struct{
	char data;
	unsigned int weight;//���Ȩֵ�������д�ŵ�����ĸƵ��
	unsigned int parent,lchild,rchild;
}HTNode,*HuffmanTree; //��̬��������洢��������

typedef char * *HuffmanCode; //��̬��������洢�����������

/*****ջ�Ĵ洢��ʾ*****/
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
#define OK 1
#define ERROR 0
#define OVERFLOW -2

//���庯����������
typedef int Status;

//����ջ��Ԫ������
typedef struct{
	int blank;
	HTNode huTree;
}SElemType; 

//����ջ����
typedef struct{
    SElemType *base;
    SElemType *top;
    int stacksize;
}Stack;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<conio.h> 

#define PAUSE system("pause>nul") //������ʾ����
#define CLS system("cls")  //�궨������

//���幹���ջ����
Status InitStack(Stack &s){
    s.base=(SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if(!(s.base)) exit(OVERFLOW);  //�洢����ʧ��
    s.top=s.base;
    s.stacksize=STACK_INIT_SIZE;
    return OK;
}

//����ȡջԪ�غ���
Status GetTop(Stack s,SElemType &e){
    if(s.top==s.base) 
		return ERROR;
    e=*(s.top-1);
    return OK;
}

//�������ջԪ�غ���
Status Push(Stack &s,SElemType e){
    if(s.top-s.base>=s.stacksize){//ջ����׷�Ӵ洢�ռ�
        s.base=(SElemType *)realloc(s.base,(s.stacksize+STACKINCREMENT)*sizeof(SElemType));
        if(!s.base) exit(OVERFLOW);//�洢����ʧ��
    
        s.top=s.base+s.stacksize;
        s.stacksize=+STACKINCREMENT;
    }
    *s.top++=e;
    return OK;
    
}

//����ɾ��ջ��Ԫ�غ���
Status Pop(Stack &s,SElemType &e){
    if(s.top==s.base) 
		return ERROR;
    e=*--s.top;
    return OK;
}

//�����жϿ�ջ����
Status StackEmpty(Stack s){
    if(s.top==s.base) 
		return OK;
    else return ERROR;
}

/*****�����ַ����뺯�������Դ˴�����������*****/
void InputChars(){
	FILE *fp;
	int weight[100]; //�ֱ�洢�ַ���Ȩֵ
	int i,j,number,value;
	char ch,str[100],w[5]={'\0'};
	printf("Input the number of chars you want to creat HuffmanTree:");
	scanf("%d",&number);
	for(i=0;i<number;i++){
		printf("\n");
		printf("Input the char:");
		ch=getchar(); //���ܻ��з�
		str[i]=getchar();
		printf("\nInput its weight:");
		scanf("%d",&weight[i]);
	}
	
	if((fp=fopen("huffmanChars.txt","w"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	for(i=0;i<number;i++){ //д���ַ�
		fputc(str[i],fp);
		fputc(' ',fp);
		//ȨֵΪ��λ��
		if(weight[i]<10){
			fputc(weight[i]+48,fp); //������ת��Ϊ��Ӧ�ַ�
		}
		//Ȩֵ���ڸ�λ��
		else{
			value=weight[i];
			//��Ȩֵ���ô洢��w������
			j=0;
			while(value){
				w[j]=value%10+48;
				value/=10;
				j++;
			}
			for(j--;j>=0;j--)
				fputc(w[j],fp); //��w���飨�ַ����͵�Ԫ�أ��Ӻ�������ļ���
		}
		fputc(' ',fp);
		fputc(' ',fp);
	}
	fclose(fp);

	printf(" $ ---------������ַ��Ѿ��ɹ��洢��huffmanchars.txt�ļ���--------- $\n\n");
}

/*****���������Խ����Ѱ��Ȩֵ��С��û��˫�׵��������*****/
void Select(HuffmanTree HT,int n,int *s1,int *s2){
	int i,j,temp;
	*s1=0;
	//Ϊs1��s2����ֵ
	for(i=1;i<=n;i++){
		if(HT[i].parent==0){
			if(*s1==0){// s1δ����ֵ
				*s1=i;
			}
			else{//�����s2��ֵ
				*s2=i;
				break;
			}
		}
	}//for
	if(HT[*s1].weight>HT[*s2].weight){//ȨֵС�ĸ�ֵ��s1
		temp=*s1;
		*s1=*s2;
		*s2=temp;
	}

	for(j=i;j<=n;j++){//��s2ָ���λ�ÿ�ʼѰ��
		if(HT[j].parent==0){//�ý����δ��˫�ף�������Ѱ�ҷ�Χ
			if(HT[j].weight<HT[*s1].weight){//�ҵ���s1ָ���Ȩֵ��С��ֵ
				*s2=*s1;
				*s1=j;
			}
			else if(*s1!=j && HT[j].weight<HT[*s2].weight){//ǰһ��������ֹ��s1��s2����ʱ�ж�ʧ��
				*s2=j;
			}
		}
	}//for
}

/**********���崴�����������ĺ���*********/
void CreateHuffman(HuffmanTree &HT){
	HuffmanTree p;
	int num,m,i=0,j=0,k=0,value,wNum[100]={0};
	int s1,s2;
	char str[100]={'\0'},wStr[100]={'\0'},fstr[200]={'\0'},par[5]={'\0'};
	FILE *fp;
	if((fp=fopen("huffmanChars.txt","r"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	fgets(fstr,199,fp);
	fclose(fp);
	while(fstr[i]){
		if(fstr[i]!=' ') {
			if(fstr[i]>=48 && fstr[i]<=57){//�������ַ��洢��w������
				wStr[j]=fstr[i]; //�洢�����ַ�Ԫ��
				j++;
			}
			else{//��ĸ�ַ��洢��str������
				wStr[j]=' '; //�����������ĸ��Ȩֵ
				j++;
				str[k]=fstr[i];
				k++;
			}
			
		} 
		i++;
	}
	wStr[j]='\0';
	str[k]='\0';
	num=k; //ͳ���ļ��еķǿո���ַ�����
	for(i=0,j=1;wStr[j];j++){
		if(wStr[j]!=' '){//�����������ַ���ϳ�����Ȩֵ
			wNum[i]*=10;
			wNum[i]+=(wStr[j]-48);  //��ŵ���int���͵�Ԫ��
		}
		else 
			i++;
	}

	if(num<=1) return;
	m=2*num-1;
	HT=(HuffmanTree)malloc((m+1)*sizeof(HTNode)); //0�ŵ�Ԫδ��
	for(p=HT+1,i=1;i<=num;++i,++p){  //��ʼ�����������Ľ��
		p->data=str[i-1];
		p->weight=wNum[i-1];
		p->parent=0;
		p->lchild=0;
		p->rchild=0;
	}
	for(;i<=m;++i,++p){
		p->data='\0';
		p->weight=0;
		p->parent=0;
		p->lchild=0;
		p->rchild=0;
	}

	for(i=num+1;i<=m;++i){//������������
		//��HT[1..i-1]ѡ��parentΪ0��weight��С��������㣬����ŷֱ�Ϊs1��s2.
		Select(HT,i-1,&s1,&s2);
		HT[s1].parent=i;
		HT[s2].parent=i;
		HT[i].lchild=s1;
		HT[i].rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
		HT[i].data='#';  //˫�׽����'#'��ʾ
	}

	//�������Ĺ�����������hfmTree.txt��
	if((fp=fopen("hfmTree.txt","w"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	for(i=1;i<=num;i++){
		fputc(HT[i].data,fp);
		fputc(' ',fp);
		fputs("parent:",fp);
		if(HT[i].parent<10){
			fputc((HT[i].parent)+48,fp);
		}
		else{
			value=HT[i].parent;
			//��Ȩֵ���ô洢��par������
			j=0;
			while(value){
				par[j]=value%10+48;  //��ŵ����ַ�Ԫ��
				value/=10;
				j++;
			}
			for(j--;j>=0;j--)
				fputc(par[j],fp);   //��par�����е��ַ����뵽�ļ�
		}
		fputc(' ',fp);
		fputc(' ',fp);
	}
	fclose(fp);
	printf(" $ ----------- �����Ĺ��������Ѿ��ɹ��洢��hufTree.txt��----------- $\n\n");
}

/**********������뺯��***********/
//�㷨�����ļ�hufmanTree.txt�ж�ȡ������������Ҷ�ӵ���������ÿ���ַ��Ĺ���������
void Encoding(HuffmanTree &HT,HuffmanCode &HC){
	FILE *fp;
	int count=0,i=0,start,c,f;
	char fstr[200];
	if((fp=fopen("huffmanChars.txt","r"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	fgets(fstr,199,fp);
	fclose(fp);
	while(fstr[i]){
		if(fstr[i]!=' ') {
			if((fstr[i]>='a'&&fstr[i]<='z') || (fstr[i]>='A'&&fstr[i]<='Z')){
				count++;  //������ĸ�ַ�����
			}
			
		} 
		i++;
	}
	HC=(HuffmanCode)malloc((count+1)*sizeof(char *));//����n���ַ������ͷָ������
	char *cd;
	cd=(char *)malloc(count*sizeof(char));//���������Ĺ�������
	cd[count-1]='\0';//���������
	for(i=1;i<=count;i++){//����ַ������������
		start=count-1; //���������λ��
		for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent){//��Ҷ�ӵ������������
			if((int)HT[f].lchild==c) cd[--start]='0';
			else cd[--start]='1';
		}
		HC[i]=(char *)malloc((count-start)*sizeof(char));//Ϊ��i���ַ��������ռ�
		strcpy(HC[i],&cd[start]); //��cd������start��ַ��ʼ���ַ�����ֵ��HC

	}//for����ַ������������
	free(cd);
	if((fp=fopen("CodeFile.txt","w"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	for(i=1;i<=count;i++){//д���ļ�
		fputs(HC[i],fp);
	}
	fclose(fp);
	printf(" $ ----------���������Ѿ��ɹ����벢�洢��CodeFile.txt�ļ���------- $\n\n");
}

/*�������Ҷ�ӽ��ĺ���*/
int find(HuffmanTree &HT,char c,int *n){
	int temp=0;
	if(c=='0'){//���֧
		temp=HT[*n].lchild;
	}
	else if(c=='1'){//�ҷ�֧
		temp=HT[*n].rchild;
	}
	return temp;
}

/*�������뺯��*/
void Decoding(HuffmanTree &HT){
	char *str,*p,Cstr[200]={'\0'},fstr[100]={'\0'};
	FILE *fp,*fq;
	int i=0,k=0,num=49,m,n; //k������ʾ�����λ��
	//��ȡ���룬�洢��Cstr������
	if((fp=fopen("CodeFile.txt","r"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	fgets(Cstr,199,fp);
	fclose(fp);
	str=(char *)malloc((num+1)*sizeof(char));//����洢�����ַ��Ŀռ�
	for(k=1;HT[k].parent!=0;){
		k=HT[k].parent; //k��¼�����λ��
	}

	for(n=k,p=str,i=0;Cstr[i];i++){
		m=find(HT,Cstr[i],&n);
		if(HT[m].data!='#'){//���ý���Ƿ�ΪҶ�ӽ��
			*p=HT[m].data;
			p++;
			n=k;
		}
		else n=m;
	}//for
	*p='\0';
	if((fq=fopen("TextFile.txt","w"))==NULL){//  �򿪴����ļ�
		printf("Can't not open file!\n");
		exit(0);
	}
	fputs(str,fq);
	fclose(fq);
	printf(" $ ---------���������Ѿ��ɹ����벢�洢��TextFile.txt�ļ���--------- $\n\n");
}

/***�����ӡ����������ĺ���***/
void Print(){
	FILE *fp;
	char Cstr[200]={'\0'};
	int i;
	if((fp=fopen("CodeFile.txt","r"))==NULL){//�򿪴����ļ�ʧ��
		printf("Can't not open file!\n");
		exit(0);
	}
	fgets(Cstr,199,fp);
	fclose(fp);
	printf(" $ ------��ÿ�в�����50���ַ��ķ�ʽ����������������뵽��Ļ�ϣ�---- $\n");
	for(i=0;Cstr[i];i++){
		if(i%50==0) printf("\n");
		printf("%c",Cstr[i]);
	}
	printf("\n");
	printf("\n");
}

/**�����԰�͹��ʽ��ӡ���������ĺ���**/
void TreePrint(HuffmanTree &HT){
	FILE *fp;
	int i,k,count;
	SElemType e1,e2,e3;
	Stack s;
	InitStack(s);
	for(k=1;HT[k].parent!=0;){
		k=HT[k].parent; //k��¼�����λ��
	}
	//<-------�����������洢��TreePrint.txt�ļ�
	if((fp=fopen("TreePrint.txt","w"))==NULL){//�򿪴����ļ�ʧ��
		printf("Can't not open file!\n");
		exit(0);
	}
	//---------->
	for(i=0;i<10;i++){
		printf(" ");
		fputc(' ',fp);
	}
		
	i=k;
	count=0;
	while(i!=0){//���ڽ��//while(HT[i].data!='\0'){//���ڽ��
		printf("%c  ",HT[i].data);

		//<-------����TreePrint.txt�ļ�
		fputc(HT[i].data,fp);
		fputc(' ',fp);
		fputc(' ',fp);
		//------------->

		if(HT[i].rchild){ //�����Һ��ӵĽ����ջ
			e1.huTree=HT[i];
			e1.blank=10+3*count;
			Push(s,e1);
		}
		i=HT[i].lchild;//��iָ�����ӵ�λ��
		count++;
	}//���ڽ��
	printf("\n");

	//<-------����TreePrint.txt�ļ�
	fputs("\n",fp);
	//------------->

	GetTop(s,e1); //��ȡջ��Ԫ��
	while(!StackEmpty(s)){//ջ����
		Pop(s,e1); //ɾ��ջ��Ԫ��
		for(i=0;i<e1.blank+3;i++){
			printf(" ");

			//<-------����TreePrint.txt�ļ�
			fputc(' ',fp);
			//------------->

		}		
		printf("%c",HT[e1.huTree.rchild].data);

		//<-------����TreePrint.txt�ļ�
		fputc(HT[e1.huTree.rchild].data,fp);
		//------------->

		if(HT[e1.huTree.rchild].rchild){//�Һ��Ӳ��գ���ջ
			e2.huTree=HT[e1.huTree.rchild];
			e2.blank=e1.blank+3;
			Push(s,e2);
		}
		i=HT[e1.huTree.rchild].lchild;
		while(i!=0){
			printf("  %c",HT[i].data);

			//<-------����TreePrint.txt�ļ�
			fputc(' ',fp);
			fputc(' ',fp);
			fputc(HT[i].data,fp);
			//------------->

			if(HT[i].rchild){//��ջ
				GetTop(s,e2); //��ȡջ��Ԫ�أ���e2����
				e3.huTree=HT[i];
				e3.blank=e2.blank+3;
				Push(s,e3);	
			}
			i=HT[i].lchild;
		}
		GetTop(s,e1); //��ȡջ��Ԫ��
		printf("\n");

		//<-------����TreePrint.txt�ļ�
		fputs("\n",fp);
		//------------->

	}//ջ����
	fclose(fp);
	printf("\n");
	printf(" $ ------��͹��ʾ�Ĺ��������Ѿ��ɹ��洢��TreePrint.txt�ļ���------- $\n\n");
}

/**********�������ҳ����ʽ����**********/
//����ϵͳ����ʾ������ɫ��Χ�����ڴ�С 
void setWindow()
{
    system("mode con lines=40 cols=70");
    system("color AB");
}

void main(){
	int w;
	char choice,ch[5]={'\0'};
	HuffmanTree HT;
	HuffmanCode HC;
	setWindow();
    system("color 2F");
    system("color 2F");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf(" $ ---------------------------------------------------------------- $\n");
	printf("                     ��ӭʹ�ù�������/��������\n");
	printf(" $ ---------------------------------------------------------------- $\n");
	printf("\n");
	printf("\n");
	printf(" $ ---------------------------------------------------------------- $\n");
	printf("\n");
	printf(" *               ��������ΰ        ѧ�ţ�3112006207               * \n");
	printf("\n");
	printf(" *               �γ̣����ݽṹ      ֪����ʦ����޼                 * \n");
	printf("\n");
	printf(" $ ---------------------------------------------------------------- $\n");
	printf("\n");
	printf("\n");
	printf(" $ -------------------�밴Enter������˵�ҳ�棺-------------------- $\n");
	PAUSE;
	while(1){
		CLS;
        system("color 2D");
        system("color 2F");
		printf("\n");
		printf("\n");
		printf(" $ ---------------------------------------------------------------- $\n");
		printf("                       ��ӭʹ�ù�������/��������\n");
		printf(" $ ---------------------------------------------------------------- $\n");
		printf("\n");
		printf("\n");
		printf(" $ ---------------------------------------------------------------- $\n");
		printf("\n");
		printf(" *                A��������������          B�������������          *");
		printf("\n");
		printf("\n");
		printf(" *                C�������������          D�������������          *");
		printf("\n");
		printf("\n");
		printf(" *                Q���˳�                                           *");
		printf("\n");
		printf(" $ ---------------------------------------------------------------- $\n");
		printf("\n");
		printf("\n");
		printf(" $ -------------------��ѡ����Ҫִ�е�ָ�----------------------- $\n");
		choice=getchar();
		switch(choice){
			case 'A':
			case 'a':{	ch[0]='A';
						InputChars();
						CreateHuffman(HT);
						printf(" $ ----------------------�밴Enter�Լ���--------------------------- $\n");
						getch();
						break;
					}
			case 'B':
			case 'b':{	ch[1]='B';
						//����Ƿ��Ѿ������˹�������
						if(!ch[0]){
							InputChars();
							CreateHuffman(HT);
							ch[0]='A';
						}
						Encoding(HT,HC);
						Print();
						printf(" $ ----------------------�밴Enter�Լ���--------------------------- $\n");
						getch();
						break;
					}
			case 'C':
			case 'c':{	ch[2]='C';
						//����Ƿ��Ѿ������˹�������
						if(!ch[0]){
							InputChars();
							CreateHuffman(HT);
							ch[0]='A';
						}
						//�����������Ƿ��Ѿ�����
						if(!ch[1]){
							Encoding(HT,HC);
							ch[1]='B';
						}
						Decoding(HT);
						printf(" $ -----------------------�밴Enter�Լ���-------------------------- $\n");
						getch();
						break;
					}
			case 'D':
			case 'd':{	//����Ƿ��Ѿ������˹�������
						if(!ch[0]){
							InputChars();
							CreateHuffman(HT);
							ch[0]='A';
						}
						printf(" $ ------------------�԰�͹����ʽ�����������---------------------- $\n");
						TreePrint(HT);
						printf(" $ ----------------------�밴Enter�Լ���--------------------------- $\n");
						getch();
						break;
					}
			case 'Q':
			case 'q':{
						printf(" $ ----------------------��лʹ�ñ�ϵͳ��-------------------------- $\n");
						exit(1);
					}
		}//switch
		CLS;
	}//while
}

