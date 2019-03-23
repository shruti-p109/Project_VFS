#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h> //for windows

//user defined macros
#define MAXINODE 50

#define READ 1  //for mode and permission too 
#define WRITE 2

#define MAXFILESIZE 1024

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

//error code macros
#define ERR_WRONGPAR -1
#define ERR_FILENOTFOUND -2
#define ERR_NOINODES -3
#define ERR_FILEALREADYEXISTS -4
#define ERR_NOPERMISSION -5
#define ERR_MEMALLOCFAILED -6
#define ERR_FILENOTREGULAR -7
#define ERR_NOTENOUGHMEM -8

//dec of needed datastructures
typedef struct superblock
{
	int TotalInodes;
	int FreeInodes;
}SUPERBLOCK,*PSUPERBLOCK;

typedef struct inode
{
	char FileName[50];
	int InodeNo;
	int FileType;
	int permission;
	char* Buffer; //pointer to file where data is 
	int FileSize;
	int FileActualSize; //datasize in the file
	int LinkCount; //
	int RefCount;
	struct inode* next;
}INODE,*PINODE,**PPINODE;

typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count; //
	int mode;
	PINODE ptrinode; //ptr to inode(directly to the inode in DILB as in our proj DILB itself
					 //is on RAM not on HDD. so need for IIT in our proj
}FILETABLE,*PFILETABLE;

typedef struct ufdt
{
	PFILETABLE ptrftable;
}UFDT;



//func decs
void InitialiseSuperBlock();
void CreateDILB();
void ls_file();
void CloseAllFile();
void DisplyHelp();
int stat_file(char*);
int fstat_file(int);
int CloseFileByName(char*);
int rm_file(char*);
void man(char*);
int CreateFileX(char*, int);
int GetFDFromName(char*);
PINODE Get_Inode(char*);
int ReadFile(int, char*, int);
int WriteFile(int, char*, int);
int OpenFile(char*, int);
int LseekFile(int, int, int);
int truncate_file(char*);