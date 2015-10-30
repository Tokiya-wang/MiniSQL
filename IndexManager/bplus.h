/*Index Manager
Index Manager����B+��������ʵ�֣�ʵ��B+���Ĵ�����ɾ�����������Ķ�����ɾ�����𣩡���ֵ���ҡ������ֵ��ɾ����ֵ�Ȳ������������ṩ��Ӧ�Ľӿڡ�
B+���нڵ��СӦ�뻺�����Ŀ��С��ͬ��B+���Ĳ����ɽڵ��С����������С����õ���*/
#ifndef _BPLUS_H_
#define _BPLUS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "BufferManager.h" 
using namespace std;

class node{
protected:
	int type;
	int N;
	string* value;
public:
	node(int q);
	virtual int gettype();
	virtual int Haveson();
	virtual string getvalue(int i);
	virtual void setvalue(string k,int i);
	virtual int getadree(int i); //�޸� 
	virtual void setadree(int place,int i);
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void clear();
	virtual void setNext(int place);
	virtual int getNext();
	virtual int whoareyou();
};
class normalnode :public node{
private:
	int* P; //�ӽڵ�λ�� 
public:
	normalnode(int q);
	virtual int Haveson();
	virtual int getadree(int i); //�޸� 
	virtual void setadree(int place,int i);//�޸� 
	void clear();
	virtual int whoareyou();
};
class leafnode :public node{
protected:
	int* Offset; //ƫ����
private:
	int Next;
public:
	leafnode(int q);
	virtual int Haveson();
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void clear();
	virtual void setNext(int place);
	virtual int getNext();
	virtual int whoareyou();
};
class tempnode :public leafnode{
private:
	string* value2;
public:
	tempnode(int q);
	tempnode(int q,node *p);//�޸� 
	virtual int Haveson();
	virtual string getvalue(int i);
	virtual void setvalue(string k,int i);
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
};
class neicun{
public:
	int type;
	int N;
	string* value;
	int* p;
	neicun(int q);
	neicun(int q,node *p);
};

node* getfromfile(char* re);//0 int, 1 float, 2 char*
char* getfromnode(node* p);
node* getnode(int off,string filename);

int insertleaf(string k,int off,int place,string filename);

int insert(string k,int off,int place,string filename);

int find1(int root,string s,string filename);

int findfirst(int root,string filename);

vector<int> findsmallthan(int root,string value,string filename,int equal);

vector<int> findbigthan(int root,string value,string filename,int equal);

vector<int> findnoeq(int root,string value,string filename);

int deletenode(string k,int place,string filename);

int deletereall(string k,int place,int root,string filename);


#endif

