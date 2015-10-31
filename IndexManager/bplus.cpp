#include "bplus.h" 
extern MBuffer m1;

node::node(int q)
{
	type=q;
	if(type==2) N=15;
	else N=501;
	value=new string[N-1];
	for(int i=0;i<N-1;i++)
	{
		value[i]="";
	}
}
node::~node()
{
	delete[] value;
}
int node::gettype(){
	return type;
}
int node::Haveson()
{
	return 0;
}
string node::getvalue(int i){
	return value[i];
}
void node::setvalue(string k,int i){
	value[i]=k;
}
int node::getOffset(int i){ 
	return -1;
}
void node::setOffset(int Off,int i){
}
int node::getadree(int i){
	return -1;
}
void node::setadree(int place,int i){
}
void node::clear(){
}
void node::setNext(int place){
}
int node::getNext(){
	return -1;
}
int node::whoareyou(){
	return 0;
}

void normalnode::clear(){
	for(int i=0;i<N;i++)
	{
		if(i!=N-1)
			value[i]="";
		P[i]=0;
	}
}
int normalnode::Haveson(){
	if(P[0]==0) return 0;
	else return 1;
}
normalnode::normalnode(int q):node(q){
	P=new int[N]; 
	for(int i=0;i<N;i++)
	{
		P[i]=0;
	}
}
normalnode::~normalnode(){
	delete[] P;
}
int normalnode::getadree(int i){
	return P[i];
}
void normalnode::setadree(int place,int i){
	P[i]=place;
}
int normalnode::whoareyou(){
	return 1;
}

int leafnode::Haveson(){
	return 0;
}
leafnode::leafnode(int q):node(q){
	Next=0;
	Offset=new int[N]; //ƫ����
	for(int i=0;i<N;i++)
	{
		Offset[i]=0;
	}
}
leafnode::~leafnode(){
	delete[] Offset;
}
int leafnode::getOffset(int i){
	return Offset[i];
}
void leafnode::setOffset(int Off,int i){
	Offset[i]=Off;
}
void leafnode::clear(){
	for(int i=0;i<N;i++)
	{
		if(i!=N-1)
			this->setvalue("",i);
		Offset[i]=0;
	}
}
void leafnode::setNext(int place){
	Next=place;
}
int leafnode::getNext(){
	return Next;
}
int leafnode::whoareyou(){
	return 2;
}

tempnode::tempnode(int q):leafnode(q){
	value2=new string[N];
	for(int i=0;i<N;i++)
	{
		value2[i]="";
		Offset[i]=0;
	}
}
tempnode::tempnode(int q,node *p):leafnode(q){   //�޸� ///////////////////////////////////// 
	value2=new string[N];
	for(int i=0;i<N-1;i++)
	{
		value2[i]=p->getvalue(i);
		Offset[i]=p->getOffset(i);
	}
}
tempnode::~tempnode(){
	delete[] value2;
}
int tempnode::Haveson(){
	return 0;
}
int tempnode::getOffset(int i){
	return Offset[i];
}
void tempnode::setOffset(int Off,int i){
	Offset[i]=Off;
}
string tempnode::getvalue(int i){
	return value2[i];
}
void tempnode::setvalue(string k,int i){
	value2[i]=k;
}

neicun::neicun(int q){
	type=q;
	if(q==2) N=15;
	else N=501;
	value=new string[N];
	p=new int[N+1];
	for(int i=0;i<N;i++)
	{
		value[i]="";
		p[i]=0;
	}
	p[N]=0;
}
neicun::neicun(int q,node *n){   //�޸�     ////////////////// 
	type=q;
	if(q==2) N=15;
	else N=501;
	value=new string[N];
	p=new int[N+1];
	for(int i=0;i<N;i++)
	{
		if(i!=N-1)
			value[i]=n->getvalue(i);
		p[i]=n->getadree(i);
	}
}
neicun::~neicun(){
	delete[] value;
	delete[] p;
}
char* getfromnode(node* p)
{
	int type=p->gettype();
	int k=0;
	int who=p->whoareyou();
	char* re = new char[4096];
	memcpy(re+k,&type,4); k+=4;
	memcpy(re+k,&who,4); k+=4;
	if(type==2)
	{
		int i=0;
		char c[256];
		int a;
		for(i=0;i<14;i++)
		{
			if(who==1)
				a=p->getadree(i);
			else
				a=p->getOffset(i);
			memcpy(re+k,&a,4); k+=4;
			strcpy(c,(p->getvalue(i)).c_str());
			memcpy(re+k,c,256); k+=256;
		}
		if(who==1)
			a=p->getadree(i);
		else
			a=p->getNext();
		memcpy(re+k,&a,4); k+=4;
	}
	else
	{
		int i=0;
		int a;
		for(i=0;i<500;i++)
		{
			if(who==1)
				a=p->getadree(i);
			else
				a=p->getOffset(i);
			memcpy(re+k,&a,4); k+=4;
			if(type==0)
			{
				int num=atoi((p->getvalue(i)).c_str());
				memcpy(re+k,&num,4); k+=4;
			}
			else
			{
				float num=atof((p->getvalue(i)).c_str());
				memcpy(re+k,&num,4); k+=4;
			}
			
		}
		if(who==1)
			a=p->getadree(i);
		else
			a=p->getNext();
		memcpy(re+k,&a,4); k+=4;
	}
	return re;
}
node* getfromfile(char* re)//0 int, 1 float, 2 char*
{
	int type;
	int who;
	int k=0;
	node* p;
	memcpy(&type,re+k,4); k+=4;
	memcpy(&who,re+k,4); k+=4;
	if(who==1) p=new normalnode(type);
	else p=new leafnode(type);
	if(type==2)
	{
		int i;
		int a;
		char c[256];
		for(i=0;i<14;i++)
		{
			memcpy(&a,re+k,4); k+=4;
			if(who==1)
				p->setadree(a,i);
			else
				p->setOffset(a,i);
			memcpy(c,re+k,256); k+=256;
			string s(c);
			p->setvalue(s,i);			
		}
		memcpy(&a,re+k,4); k+=4;
		if(who==1)
			p->setadree(a,i);
		else
			p->setNext(a);
	}
	else
	{
		int i,a;
		char c[4];
		for(i=0;i<500;i++)
		{
			memcpy(&a,re+k,4); k+=4;
			if(who==1)
				p->setadree(a,i);
			else
				p->setOffset(a,i);
			memcpy(c,re+k,4); k+=4;
			string s(c);
			p->setvalue(s,i);
		}
		memcpy(&a,re+k,4); k+=4;
		if(who==1)
			p->setadree(a,i);
		else
			p->setNext(a);
	}
	return p;
}
node* getnode(int off,string filename){
	if(off==0) return NULL;
	Block* b=m1.GetBlock(filename,off,0);
	node* tree=getfromfile(b->record);
	return tree;
}
void rewrite(int off,node *p,string filename){
	Block* b=m1.GetBlock(filename,off,0);	
	char* c=getfromnode(p);
	memcpy(b->record,c,4096);
	delete p;
	delete[] c;
	return;
}
bool ltnode(string s1,string s2,node*p){
	if(p->gettype()==2)
		return (s1<s2);
	else if(p->gettype()==1)
		return (atof(s1.c_str())<atof(s2.c_str()));
	else
		return (atoi(s1.c_str())<atoi(s2.c_str()));
}
bool eqnode(string s1,string s2,node*p){
	if(p->gettype()==2)
		return (s1==s2);
	else if(p->gettype()==1)
		return (atof(s1.c_str())==atof(s2.c_str()));
	else
		return (atoi(s1.c_str())==atoi(s2.c_str()));
}
bool lenode(string s1,string s2,node*p){
	return (ltnode(s1,s2,p) || eqnode(s1,s2,p) ) ;
}
int find1(int root,string s,string filename){
	node *tree=getnode(root,filename);
	int i=-1;
	int N=(tree->gettype()==2)?15:501;
	while(tree->Haveson())
	{
		for(i=0;i<N-1;i++)//�����ýڵ�
		{
			if(tree->getvalue(i).empty())//����
				break;
			if(lenode(s,tree->getvalue(i),tree))
				break;
		}
		if(i==N-1) //�ýڵ㲻����
		{
			node *temp=tree;
			tree=getnode(tree->getadree(i),filename);
			delete temp;
		}
		else//�ڸýڵ���
		{
			if(eqnode(s,tree->getvalue(i),tree))
			{
				node *temp=tree;
				tree=getnode(tree->getadree(i+1),filename);
				delete temp;
			}
			else
			{
				node *temp=tree;
				tree=getnode(tree->getadree(i),filename);
				delete temp;
			}
		}
	}
	for(i=0;i<N-1;i++)
	{
		if(eqnode(s,tree->getvalue(i),tree))
			break;
	}
	if(i==N-1)
	{
		delete tree;
		tree=NULL;
		return -1;
	} 
	int q=tree->getOffset(i);
	delete tree;
	tree=NULL;
	return q;
}


int findfirst(int root,string filename){
	node *p=getnode(root,filename);
	if(p->whoareyou()==2)
	{
		delete p;
		return root;
	}
	else
	{
		do
		{
			root=p->getadree(0);
			delete p;
			p=NULL;
			p=getnode(root,filename);
		}while(p->whoareyou()==1);
		delete p;
		return root;
	}	
}

vector<int> findsmallthan(int root,string value,string filename,int equal){
	int first=findfirst(root,filename);
	vector<int> intvec;
	node *p=getnode(first,filename);
	int N=(p->gettype()==2)?15:501;
	int flag=0;
	while(1)
	{
		for(int i=0;i<N-1;i++)
		{
			if(equal)
			{
				if( !p->getvalue(i).empty() )
				{
					if(lenode(p->getvalue(i),value,p))
						intvec.push_back(p->getOffset(i));
					else{
						flag=1;
						break;
					}
				}
				else break;
			}
			else{
				if( !p->getvalue(i).empty() )
				{
					if(ltnode(p->getvalue(i),value,p))
						intvec.push_back(p->getOffset(i));
					else{
						flag=1;
						break;
					}
				}
				else break;
			}
		}
		if(flag) break;
		if(p->getNext()==0) break;
		int pla=p->getNext();
		delete p;
		p=getnode(pla,filename);
	}
	delete p;
	return intvec;
}
vector<int> findbigthan(int root,string value,string filename,int equal){
	int first=findfirst(root,filename);
	vector<int> intvec;
	node *p=getnode(first,filename);
	int N=(p->gettype()==2)?15:501;
	while(1)
	{
		for(int i=0;i<N-1;i++)
		{
			if(equal)
			{
				if( !p->getvalue(i).empty() )
				{
					if(lenode(value,p->getvalue(i),p))
						intvec.push_back(p->getOffset(i));
				}
				else break;
			}
			else{
				if( !p->getvalue(i).empty() )
				{
					if(ltnode(value,p->getvalue(i),p))
						intvec.push_back(p->getOffset(i));
				}
				else break;
			}
		}
		if(p->getNext()==0) break;
		int pla=p->getNext();
		delete p;
		p=getnode(pla,filename);
	}
	delete p;
	return intvec;
}
vector<int> findnoeq(int root,string value,string filename){
	int first=findfirst(root,filename);
	vector<int> intvec;
	node *p=getnode(first,filename);
	int N=(p->gettype()==2)?15:501;
	while(1)
	{
		for(int i=0;i<N-1;i++)
		{

			if( !p->getvalue(i).empty() )
			{
				if(!eqnode(value,p->getvalue(i),p))
					intvec.push_back(p->getOffset(i));
			}
			else break;
		}
		if(p->getNext()==0) break;
		int pla=p->getNext();
		delete p;
		p=getnode(pla,filename);
	}
	delete p;
	return intvec;
}
int parent(int p,int root,string filename) //��p�ĸ��ڵ� 
{
	if(root==p) return 0;
	int j,i;
	node *r=getnode(root,filename);
	int N=(r->gettype()==2)?15:501;
	node *t=getnode(p,filename);
	string k=t->getvalue(0);
	delete t;
	while(root!=p)
	{
		delete r;
		r=getnode(root,filename);
		for(j=0;j<N;j++)
		{
			if(p == r->getadree(j))//����value��Ҫȥ�ĵط�
				break;
		}
		if(j<N) break;//�ҵ��˸ýڵ�ĸ��ڵ�

		for(i=0;i<N-1;i++)//�����ýڵ�
		{
			if(r->getvalue(i).empty())//����
				break;
			//if(k<= root->getvalue(i))//����value��Ҫȥ�ĵط�
			if(lenode(k,r->getvalue(i),r))
				break;
		}
		if(i==N-1) //�ýڵ㲻����
			root=r->getadree(i);
		else//�ڸýڵ���
		{
			if(eqnode(k,r->getvalue(i),r))
				root=r->getadree(i+1);
			else
				root=r->getadree(i);
		}
	}
	delete r;
	return root;
}

int insertleaf(string k,int off,int place,string filename) //place ji p
{
	int i=0;
	node *p=getnode(place,filename);
	//while(k>p->getvalue(i) && !p->getvalue(i).empty()) 
	while( ltnode(p->getvalue(i),k,p) && !p->getvalue(i).empty() )
		i++;
	if(eqnode(k,p->getvalue(i),p)) //�ظ�
	{
		delete p;
		return place;
	}
	
	if(p->getvalue(i).empty())//�����һ��
	{
		p->setvalue(k,i);
		p->setOffset(off,i);
	//	char* getfromnode(p);
		rewrite(place,p,filename);
		return place;
	}
	else//���м�
	{
		int c=i;
		while(!p->getvalue(i).empty())
			i++;
		for(;i>c;i--)
		{
			p->setvalue(p->getvalue(i-1),i);
			p->setOffset(p->getOffset(i-1),i);
		}
		p->setvalue(k,i);
		p->setOffset(off,i);	
		rewrite(place,p,filename);
		return place;
	}
}

int insertpar(string k,int newp,int old,int root,string filename)
{
	int root2=root;
	node *r=getnode(root,filename);
	int N=(r->gettype()==2)?15:501;
	int type=r->gettype();
	if(old==root)//�����
	{
		//node *newroot=new normalnode;
		int who=1;
		int newr=Block_num(filename);
		Block* b=m1.GetBlock(filename,newr,1);
		memcpy(b->record,&type,4);
		memcpy(b->record+4,&who,4);	
		node *newroot=getnode(newr,filename);
		newroot->setadree(old,0);
		newroot->setvalue(k,0); 
		newroot->setadree(newp,1);
		rewrite(newr,newroot,filename);
		delete r;
		return newr;
	}
	else
	{
		int i=0,j=0;
		root=parent(old,root,filename);
		delete r;
		r=getnode(root,filename);
		for(j=0;j<N;j++)
		{
			if(old == r->getadree(j))//����value��Ҫȥ�ĵط�
				break;
		}
		if(r->getvalue(N-2).empty())//���ڵ㻹�п�λ
		{
			for(i=N-1;i>j+1;i--)
			{
				r->setadree(r->getadree(i-1),i);
				r->setvalue(r->getvalue(i-2),i-1);
			}
			r->setadree(newp,j+1);
			r->setvalue(k,j);
			rewrite(root,r,filename);
			return root2;
		}
		else//û��λ�ˣ����Ѹ��ڵ�
		{
			neicun *nei=new neicun(type,r);
			for(i=N;i>j+1;i--)
			{
				nei->p[i]=nei->p[i-1];
				nei->value[i-1]=nei->value[i-2];
			}
			nei->p[j+1]=newp;
			nei->value[j]=k;
			r->clear();
			//node* second=new normalnode;
			int who=1;
			int sec=Block_num(filename);
			Block* b=m1.GetBlock(filename,sec,1);
			memcpy(b->record,&type,4);
			memcpy(b->record+4,&who,4);	
			node *second=getnode(sec,filename);
			
			int m=0,q=0;
			for(q=0;q<N/2;q++)
			{
				r->setvalue(nei->value[q],q);
				r->setadree(nei->p[q],q);
			}
			r->setadree(nei->p[q],q);
			q++;
			for(;q<N;q++)
			{
				second->setvalue(nei->value[q],m);
				second->setadree(nei->p[q],m);
				m++;
			} 
			second->setadree(nei->p[q],m);
			delete nei; nei=NULL;
			
			rewrite(sec,second,filename);
			rewrite(root,r,filename);
			r=NULL;
			
			second=getnode(sec,filename);
			while(second->Haveson())
			{
				node *t=second;
				second=getnode(second->getadree(0),filename);
				delete t;
			}
			int t2=insertpar(second->getvalue(0),sec,root,root2,filename);	
			delete second;	
			return t2;
		}
	}
}

node* insertleafnode(string k,int off,node* p)
{
	int i=0;
	while(ltnode(p->getvalue(i),k,p) && !p->getvalue(i).empty()) 
		i++;
	if(eqnode(k,p->getvalue(i),p))
		return p;
	if(p->getvalue(i).empty())//�����һ��
	{
		p->setvalue(k,i);
		p->setOffset(off,i);
		return p;
	}
	else//���м�
	{
		int c=i;
		while(!p->getvalue(i).empty())
			i++;
		for(;i>c;i--)
		{
			p->setvalue(p->getvalue(i-1),i);
			p->setOffset(p->getOffset(i-1),i);
		}
		p->setvalue(k,i);
		p->setOffset(off,i);
		return p;
	}
}
int insert(string k,int off,int place,string filename){
	int t=place;
	node *p=getnode(place,filename);
	int type=p->gettype();
	int N=(p->gettype()==2)?15:501;
	if(!p->Haveson() && p->getvalue(N-2).empty())//p��û����Ҷ�ڵ� �� ����
	{
		delete p;
		return insertleaf(k,off,place,filename);
	}
	else //1.Ҷ�ڵ㣬�������� 2.��Ҷ�ڵ�
	{
		if(p->Haveson())//2.��Ҷ�ڵ�
		{
			int i;
			while(p->Haveson())//�ҵ�K����Ҷ�ڵ�
			{
				for(i=0;i<N-1;i++)
				{
					if(p->getvalue(i).empty())//����
						break;
					if(lenode(k,p->getvalue(i),p))
						break;
				}
				if(i==N-1)
					place=p->getadree(i);
				else{
					if(eqnode(k,p->getvalue(i),p))
						place=p->getadree(i+1);
					else
						place=p->getadree(i);
				}
				delete p;
				p=getnode(place,filename);
			}
		}
		//��Ҷ�ڵ�,�����ҵ��Ŀ���û����Ҷ�ڵ�
		if(p->getvalue(N-2).empty()) //û��  t or p
		{
			place=insertleaf(k,off,place,filename);
			delete p;
			return t;
		}
		else //���� ��1.һ��ʼ����Ҷ�ڵ� 2.�ҵ����ڵ�Ҷ�ڵ����ˣ�
		{
			for(int q=0;q<N-2;q++)//����ظ���
			{
				if(eqnode(k,p->getvalue(q),p))
				{
					delete p;
					return t;
				}
			}
			//node *newtree=new leafnode;
			int who=2;
			int newp=Block_num(filename);
			Block* b=m1.GetBlock(filename,newp,1);
			memcpy(b->record,&type,4);
			memcpy(b->record+4,&who,4);	
			node *newtree=getnode(newp,filename);
			node* temp=new tempnode(type,p);
			insertleafnode(k,off,temp);
			p->clear(); //ɾ��
			newtree->setNext(p->getNext());//nextָ�뽻��
			p->setNext(newp);
			int j,m=0;
			for(j=0;j<N/2;j++)
			{
				p->setvalue(temp->getvalue(j),j);
				p->setOffset(temp->getOffset(j),j);
			}
			for(;j<N;j++)
			{
				newtree->setvalue(temp->getvalue(j),m);
				newtree->setOffset(temp->getOffset(j),m);
				m++;
			}
			delete temp;
			rewrite(place,p,filename);
			rewrite(newp,newtree,filename);
			
			newtree=getnode(newp,filename);
			string k=newtree->getvalue(0);
			int ot=insertpar(k,newp,place,t,filename);	
			delete newtree;
			return ot;
		}
	}
}

void changepar(int place,string k,int r,string k2,string filename){
	
	node* p=getnode(place,filename);
	int N=(p->gettype()==2)?15:501;
	if(place==r) {
		int i=0;
		while(i<N-1 && !(eqnode(k,p->getvalue(i),p)))
			i++;
		if(i<N-1)
		{
			p->setvalue(k2,i);
			rewrite(place,p,filename);
		}
		return;
	}
//node *p_parent=parent(p,root);//���p���Ǹ��ڵ�,����p_parent=NULL��
	int p_p=parent(place,r,filename);
	node *p_parent=getnode(p_p,filename);//���p���Ǹ��ڵ�,����p_parent=NULL��
	int i=0;
//	while(!(p_parent->getvalue(i)==k) && i<N-1)
	while(i<N-1 && !(eqnode(k,p_parent->getvalue(i),p_parent)) )
		i++;
	if(i<N-1)
	{
		p_parent->setvalue(k2,i);
		rewrite(p_p,p_parent,filename);
		delete p;
		return;
	}
	else {
		delete p;
		delete p_parent;
		changepar(p_p,k,r,k2,filename);
	}		
}

int deletereall(string k,int place,int r,string filename){
//node* deletereall(string k,node* p,node* root){
	int i=0; //��p�����ɾ��off��value ע�⣺Ҫ�ж���leafnode����normalnode����������
	int where=0;//��¼���ﱻɾ���� 
	string sss1="";
	node *p=getnode(place,filename);
	int N=(p->gettype()==2)?15:501;
	if(p->whoareyou()==2)//leafnode
	{
		for(i=0;i<N-1;i++)
		{
			//if(k==p->getvalue(i))
			if(eqnode(k,p->getvalue(i),p))
				break;
		}
		where=i; 
		for(;i<N-2;i++)
		{
			if(p->getvalue(i+1).empty()) break;
			p->setOffset(p->getOffset(i+1),i);
			p->setvalue(p->getvalue(i+1),i);
		}
		p->setOffset(0,i);
		p->setvalue("",i);
	}
	else//normalnode
	{
		for(i=0;i<N-1;i++)
		{
			if(eqnode(k,p->getvalue(i),p))
				break;
		}
		where=i;
		//if(i==N-1) exit(-1);
		for(;i<N-2;i++)
		{
			if(p->getvalue(i+1).empty()) break;
			p->setadree(p->getadree(i+2),i+1);//ע�⣺Ԥ����normalnode�ĵ�һ��ָ���޷��޸�
			p->setvalue(p->getvalue(i+1),i);
		}
		p->setadree(0,i+1);
		p->setvalue("",i);
	}
	//p->setvalue("",i);  i����
	if(place==r)//p�Ǹ��ڵ� 1.�ս�����ʼ��Ҷ�ڵ� 2.normalnode�Ľ��
	{
		if(p->whoareyou()==2) //1.�ս�����ʼ��Ҷ�ڵ�
		{
			rewrite(place,p,filename);
			return r;
		}
		else //2.normalnode
		{
			if(i==0)//���ڵ�ֻ��һ���ӽڵ���
			{
				int qqq=p->getadree(0);
				delete p;
				return qqq;
			}
			else 
			{
				rewrite(place,p,filename);
				return r;
			}
		}
	}
	else//p���Ǹ��ڵ�  1.�и���Ҷ�ڵ� 2.normalnode�Ľ��(���м�) 
	{
		if((p->whoareyou()==1 && i+1>=N/2) || (p->whoareyou()==2 && i>=N/2)) //���ݲ���  (�޸�)/////////////
		{
			if(where==0 && p->whoareyou()==2) //Ҫ�޸ĸ��ڵ� 
			{
				string kk=p->getvalue(0);
				changepar(place,k,r,kk,filename);
			}
			rewrite(place,p,filename);
			return r; 
		}
		else//�������̫�٣�1.�ϲ� 2.���·ֲ�   �Ƿ���i=0�� 
		{
			int j=-1,num=0;
			string k1;
			//node *p_parent=parent(p,root);//���p���Ǹ��ڵ�,����p_parent=NULL; 		
			int p_p=parent(place,r,filename);
			node* p_parent=getnode(p_p,filename);
			int p_b;	
			node *p_bro=NULL;
			
			
			if(p_parent!=NULL)
			{
				for(j=0;j<N;j++)
				{
					if(place == p_parent->getadree(j))//����value��Ҫȥ�ĵط�
						break;
				}
			}
			else //���� 
			{
				cout<<"error(no parent.)"<<endl;
				//exit(-1);
			}
			//p_parent->getadree(j)=p;
			if(j!=0) 
			{
				p_b=p_parent->getadree(j-1);//����ֵ� 
				p_bro=getnode(p_b,filename);
				k1=p_parent->getvalue(j-1);
			}
			else 
			{
				p_b=p_parent->getadree(j+1); //�ұ��ֵ� 
				p_bro=getnode(p_b,filename);
				k1=p_parent->getvalue(j);
			}
			while(!p_bro->getvalue(num).empty())//�����ֵܽ���key��  num��value 
			{
				num++;
				if(num==N-1) break;
			}
			if((p->whoareyou()==2 && i+num<=N-1) || (p->whoareyou()==1 && i+num<=N-2))
			//���Ժϲ�  leafnode��normal���Ժϲ���Ҫ��һ�� 
			{
				if(p->whoareyou()==2)//Ҷ�ڵ� 
				{
					if(j!=0)//������ֵܺϲ� 
					{
						int m=0,q=num;
						while(!p->getvalue(m).empty())
						{
							p_bro->setvalue(p->getvalue(m),q);
							p_bro->setOffset(p->getOffset(m),q);
							q++;m++;
						}
						p_bro->setNext(p->getNext());
						rewrite(p_b,p_bro,filename);
						delete p;
						delete p_parent;
						//rewrite(place,p,filename);
						return deletereall(k1,p_p,r,filename); //deletp���ģ�	
					} 
					else//��youbian�ֵܺϲ� 
					{
						int m=0,q=i;
						for(;m<num;q++,m++)
						{
							p->setvalue(p_bro->getvalue(m),q);
							p->setOffset(p_bro->getOffset(m),q);
						}
						p->setNext(p_bro->getNext());
						rewrite(place,p,filename);
						delete p_bro;
						delete p_parent;
						return deletereall(k1,p_p,r,filename); //deletp���ģ�	
					}
				} 
				else //normalnode
				{
					if(j!=0)//������ֵܺϲ� 
					{
						int m=0,q=num;
						p_bro->setvalue(k1,q);
						p_bro->setadree(p->getadree(0),q+1);
						q++;
						while(!p->getvalue(m).empty())
						{
							p_bro->setvalue(p->getvalue(m),q);
							p_bro->setadree(p->getadree(m+1),q+1);
							q++;
							m++;
						}
						//delete(p);	
						rewrite(p_b,p_bro,filename);
						delete p;
						delete p_parent;
						return deletereall(k1,p_p,r,filename); //deletp���ģ�	
					}
					else//��youbian�ֵܺϲ� 
					{
						int m=0,q=i;
						p->setvalue(k1,q);
						p->setadree(p_bro->getadree(0),q+1);
						q++;
						for(;m<num;q++,m++)
						{
							p->setvalue(p_bro->getvalue(m),q);
							p->setadree(p_bro->getadree(m+1),q+1);
						}
						rewrite(place,p,filename);
						delete p_bro;
						delete p_parent;
						return deletereall(k1,p_p,r,filename); //deletp���ģ�	
					}
				} 
			}
			else//���·���  �ǵ�i�п��ܵ���0 
			{
				if(j!=0)//������ֵܺϲ� 
				{
					if(p->whoareyou()==2)//Ҷ�ڵ� 
					{
						int op=p_bro->getOffset(num-1);
						string s=p_bro->getvalue(num-1);
						p_bro->setOffset(0,num-1);
						p_bro->setvalue("",num-1);
						p=insertleafnode(s,op,p);
						changepar(place,k1,r,s,filename);				
						rewrite(place,p,filename);
						rewrite(p_b,p_bro,filename);
						delete p_parent;
						return r;
					}
					else//normalnode
					{
						//node* temp=p_bro->getadree(num);
						int t=p_bro->getadree(num);
						string s=p_bro->getvalue(num-1);
						p_bro->setadree(0,num);
						p_bro->setvalue("",num-1);
						for(int m=N-1;m>=2;m--)
						{
							p->setadree(p->getadree(m-1),m);
							p->setvalue(p->getvalue(m-2),m-1);
						}
						p->setadree(p->getadree(0),1);
						p->setadree(t,0);
						p->setvalue(k1,0);			
						rewrite(place,p,filename);
						rewrite(p_b,p_bro,filename);
						changepar(place,k1,r,s,filename);
						delete p_parent;
						return r;	
					}
				}
				
				else//��you���ֵܺϲ� 
				{
					if(p->whoareyou()==2)//Ҷ�ڵ� 
					{					
						int m=0;
						int op=p_bro->getOffset(0);
						string s=p_bro->getvalue(1);
						string s1=p->getvalue(0);
						p->setOffset(p_bro->getOffset(0),i);
						p->setvalue(p_bro->getvalue(0),i);
						for(m=0;m<N-2;m++)
						{
							p_bro->setOffset(p_bro->getOffset(m+1),m);
							p_bro->setvalue(p_bro->getvalue(m+1),m);
						}
						p_bro->setOffset(0,m);
						p_bro->setvalue("",m);
						changepar(place,k1,r,s,filename);
						if(where==0) changepar(place,k,r,s1,filename);
						
						rewrite(place,p,filename);
						rewrite(p_b,p_bro,filename);
						delete p_parent;
						return r;
					}
					else//normalnode
					{
						int m=0;
						int temp=p_bro->getadree(0);
						string s=p_bro->getvalue(0);
						p->setadree(temp,i+1);
						p->setvalue(k1,i);
						for(m=0;m<N-2;m++)
						{
							p_bro->setadree(p_bro->getadree(m+1),m);
							p_bro->setvalue(p_bro->getvalue(m+1),m);
						}
						p_bro->setadree(p_bro->getadree(m+1),m);
						p_bro->setadree(0,N-1);
						p_bro->setvalue("",N-2);
						changepar(place,k1,r,s,filename);
						rewrite(place,p,filename);
						rewrite(p_b,p_bro,filename);
						delete p_parent;
						return r;	
					}
				}
			}
		}
	}
}

int deletenode(string k,int place,string filename){
//node* deletenode(string k,node* p){
	node *p=getnode(place,filename);
	int N=(p->gettype()==2)?15:501;
	int root=place;
	//node* root=p;
	int i=-1; //�ҵ�����K�Ľ��
	while(p->Haveson())
	{
		for(i=0;i<N-1;i++)//�����ýڵ�
		{
			if(p->getvalue(i).empty())//����
				break;
			if(lenode(k,p->getvalue(i),p))
				break;
		}
		if(i==N-1) //�ýڵ㲻����
			place=p->getadree(i);//p=p->getadree(i);
		else//�ڸýڵ���
		{
			//if(k== p->getvalue(i))
			if(eqnode(k,p->getvalue(i),p))
				place=p->getadree(i+1);
			else
				place=p->getadree(i);
		}
		delete p;
		p=getnode(place,filename);
	}
	for(i=0;i<N-1;i++)
	{
		if(eqnode(k,p->getvalue(i),p))
			break;
	}
	delete p;
	if(i==N-1) return root; //û�и�ֵ
	return deletereall(k,place,root,filename);
}
























