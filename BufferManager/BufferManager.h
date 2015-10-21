#ifndef _BufferManager_H_
#define _BufferManager_H_
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

struct Block
{
	string tablename;
	char* record;
	bool written;
	bool accessed;
	int offset; 
};

class MBuffer{
	private:
		Block* Buffer[64];
	public:
		MBuffer(){
			int i;
			for(i=0;i<64;i++)
				Buffer[64]=NULL;
		};
		bool Init();
		Block* GBlock();
		Block* GetBlock(string tablename, int offset);
	 	bool Sche(string tablename, int offset);
	 	bool Drop(string tablename);
	 	Block* Exchange(string tablename, int offset, Block* Repalced);
		void test(); 
	 	~MBuffer();
};

#endif 
