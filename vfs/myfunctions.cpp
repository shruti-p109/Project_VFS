#include"myheader.h"
//creating req global variables of above structures
UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;
void InitialiseSuperBlock()
{
	int i = 0; // as ufdtarr is an array
	while(i<MAXINODE) // as ufdtarr is an array,loop-from 0 to 49
	{
		UFDTArr[i].ptrftable = NULL;
		i++;
	}
	SUPERBLOCKobj.TotalInodes = MAXINODE;
	SUPERBLOCKobj.FreeInodes = MAXINODE;
}
void CreateDILB() //creating and initialising inode LL
{
	int i = 1; //DILB is LL not array and we started indexing from 1->inodeno- from 1 to 50
	PINODE temp = head; //insertlast fun, we need to traverse and modify(here create nodes)
	PINODE newN = NULL;

	while (i <= MAXINODE)//from 1 to 50
	{
		newN = (PINODE)malloc(sizeof(INODE));

		newN->LinkCount = newN->RefCount = 0;
		newN->FileType = newN->FileSize = 0;
		newN->Buffer = NULL;
		newN->next = NULL;
		newN->InodeNo = i;

		if (temp == NULL)
		{
			head = newN;
			temp = head;
		}
		else
		{
			temp->next = newN;
			temp = temp->next;
		}
		i++;
		//printf("\nCreated DILB Successfully\n");
	}
}
void ls_file() 
{
	int i = 0;
	PINODE temp = head;
	if (SUPERBLOCKobj.FreeInodes == MAXINODE)
	{
		printf("There are no files\n");
		return;
	}
	printf("\nFile Name\tInode NUmber\tFile Size\tLink Count\n");
	printf("--------------------------------------------------");
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n", temp->FileName, temp->InodeNo, temp->FileSize, temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("--------------------------------------------------");
}
void CloseAllFile()
{
	int i = 0;
	while (i < 50)
	{
		if (UFDTArr[i].ptrftable != NULL)
		{
			UFDTArr[i].ptrftable->readoffset = 0;
			UFDTArr[i].ptrftable->writeoffset = 0;
			(UFDTArr[i].ptrftable->ptrinode->RefCount)--;
		}
		i++;
	}
}
void DisplyHelp()
{
	printf("ls: To list out all files\n");
	printf("clear: To clear the screen\n");
	printf("create: To create a new file\n");
	printf("open: To open an existing file\n");
	printf("close: To close a file\n");
	printf("closeall: To close all open files\n");
	printf("read: To read contents from file\n");
	printf("write: To write contents into file\n");
	printf("exit: To terminate file system\n");
	printf("stat: To display information of file using name\n");
	printf("fstat: To display information of file using file descriptor\n");
	printf("truncate: To remove all the data from file\n");
	printf("rm: To delete a file\n");
	printf("All Commands are case insensitive\n");
}
int stat_file(char* name)
{
	PINODE temp = NULL;
	if (name == NULL)
	{
		return ERR_WRONGPAR;
	}
	temp = Get_Inode(name);
	if (temp == NULL)
	{
		return ERR_FILENOTFOUND;
	}
	printf("\n-------Statictical Information about file-------\n");
	printf("File Name: %s\n", temp->FileName);
	printf("Inode Number: %d\n", temp->InodeNo);
	printf("File Size: %d\n", temp->FileSize);
	printf("File Actual Size: %d\n", temp->FileActualSize);
	printf("Link Count: %d\n", temp->LinkCount);
	printf("Reference Count: %d\n", temp->RefCount);
	if (temp->permission == 1)
	{
		printf("Permission: Read Only\n");
	}
	else if (temp->permission == 2)
	{
		printf("Permission: Write\n");
	}
	else if (temp->permission == 3)
	{
		printf("Permission: Read & Write\n");
	}
	printf("--------------------------------------\n\n");

	return 0;
}
int fstat_file(int fd)
{
	PINODE temp = NULL;
	if (fd < 0)
	{
		return ERR_WRONGPAR;
	}
	if (UFDTArr[fd].ptrftable == NULL)
	{
		return ERR_FILENOTFOUND;
	}
	temp = UFDTArr[fd].ptrftable->ptrinode;

	printf("\n-------Statictical Information about file-------\n");
	printf("File Name: %s\n", temp->FileName);
	printf("Inode Number: %d\n", temp->InodeNo);
	printf("File Size: %d\n", temp->FileSize);
	printf("File Actual Size: %d\n", temp->FileActualSize);
	printf("Link Count: %d\n", temp->LinkCount);
	printf("Reference Count: %d\n", temp->RefCount);
	if (temp->permission == 1)
	{
		printf("Permission: Read Only\n");
	}
	else if (temp->permission == 2)
	{
		printf("Permission: Write\n");
	}
	else if (temp->permission == 3)
	{
		printf("Permission: Read & Write\n");
	}
	printf("--------------------------------------\n\n");

	return 0;
}
int CloseFileByName(char* name)
{
	int i = 0;
	i = GetFDFromName(name);
	if (i == -1)
	{
		return ERR_FILENOTFOUND;
	}
	UFDTArr[i].ptrftable->readoffset = 0;
	UFDTArr[i].ptrftable->writeoffset = 0;
	(UFDTArr[i].ptrftable->ptrinode->RefCount)--;

	return 0;
}
int rm_file(char* name)
{
	int fd = 0;
	fd = GetFDFromName(name);//would return -1 if file isnt there
	if (fd == -1)
	{
		return ERR_FILENOTFOUND;
	}
	(UFDTArr[fd].ptrftable->ptrinode->LinkCount)--; //
	if (UFDTArr[fd].ptrftable->ptrinode->LinkCount == 0) //
	{
		UFDTArr[fd].ptrftable->ptrinode->FileType = 0; //
		free(UFDTArr[fd].ptrftable);
	}
	(SUPERBLOCKobj.FreeInodes)++;

	return 0;
}
void man(char* name)
{
	if (name == NULL)
	{
		return;
	}
	if(_stricmp(name,"create")==0)
	{
		printf("NAME : create\n");
		printf("SYNOPSIS : create file_name file_permission\n");
		printf("DESCRIPTION : Used to create new regular file\n");
	}
	else if (_stricmp(name, "open") == 0)
	{
		printf("NAME : open\n");
		printf("SYNOPSIS : open file_name file_mode\n");
		printf("DESCRIPTION : Used to open an existing file\n");
	}
	else if (_stricmp(name, "close") == 0)
	{
		printf("NAME : close\n");
		printf("SYNOPSIS : close file_name\n");
		printf("DESCRIPTION : Used to close opened file\n");
	}
	else if (_stricmp(name, "write") == 0)
	{
		printf("NAME : write\n");
		printf("SYNOPSIS : write file_name\nAfter this enter the data to write\n");
		printf("DESCRIPTION : Used to write into regular file\n");
	}
	else if (_stricmp(name, "read") == 0)
	{
		printf("NAME : read\n");
		printf("SYNOPSIS : read file_name no_of_bytes_to_read\n");
		printf("DESCRIPTION : Used to read from regular file\n");
	}
	else if (_stricmp(name, "rm") == 0)
	{
		printf("NAME : rm\n");
		printf("SYNOPSIS : rm file_name\n");
		printf("DESCRIPTION : Used to delete the file\n");
	}
	else if (_stricmp(name, "fstat") == 0)
	{
		printf("NAME : fstat\n");
		printf("SYNOPSIS : fstat file_descriptor\n");
		printf("DESCRIPTION : Used to display information about file using file descriptor\n");
	}
	else if (_stricmp(name, "stat") == 0)
	{
		printf("NAME : stat\n");
		printf("SYNOPSIS : Used to display information about file using file name\n");
		printf("DESCRIPTION : stat file_name\n");
	}
	else if (_stricmp(name, "exit") == 0)
	{
		printf("NAME : exit\n");
		printf("SYNOPSIS : exit\n");
		printf("DESCRIPTION : Used to terminate the file system\n");
	}
	else if (_stricmp(name, "help") == 0)
	{
		printf("NAME : help\n");
		printf("SYNOPSIS : help\n");
		printf("DESCRIPTION : Displays all commands with description\n");
	}
	else if (_stricmp(name, "closeall") == 0)
	{
		printf("NAME : closeall\n");
		printf("SYNOPSIS : closeall\n");
		printf("DESCRIPTION : Used to close all opened files\n");
	}
	else if (_stricmp(name, "clear") == 0)
	{
		printf("NAME : clear\n");
		printf("SYNOPSIS : clear\n");
		printf("DESCRIPTION : Used to clear the screen\n");
	}
	else if (_stricmp(name, "ls") == 0)
	{
		printf("NAME : ls\n");
		printf("SYNOPSIS : ls\n");
		printf("DESCRIPTION : List information about the FILEs (the current directory by default).\n");
	}
	else if (_stricmp(name, "truncate") == 0)
	{
		printf("NAME : truncate\n");
		printf("SYNOPSIS : truncate file_name\n");
		printf("DESCRIPTION : Used to remove the data from file\n");
	}
	else if (_stricmp(name, "lseek") == 0)
	{
		printf("NAME : truncate\n");
		printf("SYNOPSIS : lseek file_name change_in_offset starting_point\n");
		printf("DESCRIPTION : Used to change file offset\n");
	}
	else
	{
		printf("No Manual entry available\n");
	}
}
PINODE Get_Inode(char* name)
{
	PINODE temp = head;
	if (name == NULL)
	{
		return NULL;
	}
	while (temp != NULL)
	{
		if (strcmp(name, temp->FileName)== 0)
		{
			break;
		}
		temp = temp->next;
	}
	return temp;
}
int CreateFileX(char* name, int permission) //returns file descriptor
{
	int i = 0;
	PINODE temp = head;

	if ((name == NULL) || (permission < 1) || (permission > 3))
	{
		return ERR_WRONGPAR;
	}
	if (SUPERBLOCKobj.FreeInodes == 0) //inode not av
	{
		return ERR_NOINODES;
	}
	if(Get_Inode(name)!=NULL)
	{
		return ERR_FILEALREADYEXISTS;
	}
	(SUPERBLOCKobj.FreeInodes)--;
	while (temp != NULL) // finding and getting to free inode
	{
		if (temp->FileType == 0)
		{
			break;
		}
		temp = temp->next;
	}
	while (i < 50) //finding and getting to empty ufdtarr index
	{
		if (UFDTArr[i].ptrftable == NULL)
		{
			break;
		}
		i++;
	}
	UFDTArr[i].ptrftable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrftable == NULL)
	{
		return ERR_MEMALLOCFAILED;
	}
	//initialising file table contents
	UFDTArr[i].ptrftable->count = 1;
	UFDTArr[i].ptrftable->mode = permission;
	UFDTArr[i].ptrftable->readoffset = 0;
	UFDTArr[i].ptrftable->writeoffset = 0;

	//initialising inode contents
	UFDTArr[i].ptrftable->ptrinode = temp; //free inode addr found from above set in file table
	UFDTArr[i].ptrftable->ptrinode->FileType = REGULAR;
	strcpy(UFDTArr[i].ptrftable->ptrinode->FileName,name);
	UFDTArr[i].ptrftable->ptrinode->permission = permission;
	UFDTArr[i].ptrftable->ptrinode->RefCount = 1;
	UFDTArr[i].ptrftable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrftable->ptrinode->FileSize = MAXFILESIZE;
	UFDTArr[i].ptrftable->ptrinode->FileActualSize = 0;
	UFDTArr[i].ptrftable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);
	memset(UFDTArr[i].ptrftable->ptrinode->Buffer, 0, 1024);//to clear that 1024 memory block
	//memset is used  to fill a block of memory with a particular value
	// ptr ==> Starting address of memory to be filled
	// x   ==> Value to be filled
	// n   ==> Number of bytes to be filled starting 
	//         from ptr to be filled
	//void* memset(void* ptr, int x, size_t n);

	return i;
}
int GetFDFromName(char* name)
{
	int i = 0;
	while(i < 50)
	{
		if (UFDTArr[i].ptrftable != NULL)
		{
			if (_stricmp((UFDTArr[i].ptrftable->ptrinode->FileName), name) == 0)
			{
				break;
			}
		}
		i++;
	}
	if (i == 50)
	{
		return ERR_FILENOTFOUND;
	}
	else
	{
		return i;
	}
}
int ReadFile(int fd, char* arr, int isize)//isize=no of bytes to be read
{
	int read_size = 0;

	if (UFDTArr[fd].ptrftable == NULL)
	{
		return ERR_FILENOTFOUND;
	}
	if ((UFDTArr[fd].ptrftable->mode != READ) && (UFDTArr[fd].ptrftable->mode != READ + WRITE))
	{
		return ERR_NOPERMISSION;
	}
	if ((UFDTArr[fd].ptrftable->ptrinode->permission != READ) && (UFDTArr[fd].ptrftable->ptrinode->permission != READ + WRITE))
	{
		return ERR_NOPERMISSION;
	}
	if (UFDTArr[fd].ptrftable->readoffset == UFDTArr[fd].ptrftable->ptrinode->FileActualSize) //reached end of file
	{
		return ERR_NOTENOUGHMEM;
	}
	if (UFDTArr[fd].ptrftable->ptrinode->FileType != REGULAR)
	{
		return ERR_FILENOTREGULAR;
	}
	read_size = UFDTArr[fd].ptrftable->ptrinode->FileActualSize - UFDTArr[fd].ptrftable->readoffset;
	if (read_size < isize)
	{
		strncpy(arr, (UFDTArr[fd].ptrftable->ptrinode->Buffer) + (UFDTArr[fd].ptrftable->readoffset), read_size);
		UFDTArr[fd].ptrftable->readoffset +=read_size;
	}
	else
	{
		strncpy(arr, (UFDTArr[fd].ptrftable->ptrinode->Buffer) + (UFDTArr[fd].ptrftable->readoffset),isize);
		UFDTArr[fd].ptrftable->readoffset +=isize;
	}
	return isize;
}
int WriteFile(int fd, char* arr, int isize)
{
	if (((UFDTArr[fd].ptrftable->mode) != WRITE) && ((UFDTArr[fd].ptrftable->mode) != (READ + WRITE)))
	{
		return ERR_NOPERMISSION;
	}
	if (((UFDTArr[fd].ptrftable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrftable->ptrinode->permission) != (READ + WRITE)))
	{
		return ERR_NOPERMISSION;
	}
	if ((UFDTArr[fd].ptrftable->writeoffset) == MAXFILESIZE)
	{
		return ERR_NOTENOUGHMEM;
	}
	if ((UFDTArr[fd].ptrftable->ptrinode->FileType) != REGULAR)
	{
		return ERR_FILENOTREGULAR;
	}
	strncpy((UFDTArr[fd].ptrftable->ptrinode->Buffer) + (UFDTArr[fd].ptrftable->writeoffset), arr, isize);
	UFDTArr[fd].ptrftable->writeoffset += isize;
	UFDTArr[fd].ptrftable->ptrinode->FileActualSize += isize;

	return isize;
}
int OpenFile(char* name, int mode)
{
	int i = 0;
	PINODE temp = NULL;
	if ((name == NULL) || (mode <= 0) || (mode > 3))
	{
		return ERR_WRONGPAR;
	}
	temp = Get_Inode(name);
	if (temp == NULL)
	{
		return ERR_FILENOTFOUND;
	}
	if (temp->permission < mode)
	{
		return ERR_NOPERMISSION;
	}
	//finding and getting empty ufdt index(fd)
	/*while (i < 50)
	{
		if (UFDTArr[i].ptrftable == NULL)
		{
			break;
		}
		i++;
	}*/
	i = GetFDFromName(name);
	/*UFDTArr[i].ptrftable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrftable == NULL)
	{
		return ERR_MEMALLOCFAILED;
	}*/
	UFDTArr[i].ptrftable->count = 1;
	UFDTArr[i].ptrftable->mode = mode;
	if (mode == READ + WRITE)
	{
		UFDTArr[i].ptrftable->readoffset = 0;
		UFDTArr[i].ptrftable->writeoffset = 0;
	}
	else if (mode == READ)
	{
		UFDTArr[i].ptrftable->readoffset = 0;
	}
	else if (mode == WRITE)
	{
		UFDTArr[i].ptrftable->writeoffset = 0;
	}
	UFDTArr[i].ptrftable->ptrinode = temp;
	(UFDTArr[i].ptrftable->ptrinode->RefCount)++;

	return i;
}
int LseekFile(int fd, int size, int from)
{
	if ((fd < 0) || (from > 2) || (from < 0))
	{
		return ERR_WRONGPAR;
	}
	if (UFDTArr[fd].ptrftable == NULL)
	{
		return ERR_WRONGPAR;
	}
	if ((UFDTArr[fd].ptrftable->mode == READ) || (UFDTArr[fd].ptrftable->mode == READ + WRITE))//you cant read after data ends
	{ //even though offset crossed fileactualsize and went ahead file size wont change as mode has read but it will 
		//chage(increase) in case of write only
		if (from == CURRENT)
		{
			//filters
			if ((UFDTArr[fd].ptrftable->readoffset) + size > (UFDTArr[fd].ptrftable->ptrinode->FileActualSize))
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrftable->readoffset) + size) < 0)
			{
				return -1;
			}
			//filters
			(UFDTArr[fd].ptrftable->readoffset) = (UFDTArr[fd].ptrftable->readoffset) + size;
		}
		else if (from == START)
		{
			//filters
			if (size > (UFDTArr[fd].ptrftable->ptrinode->FileActualSize))
			{
				return -1;
			}
			if (size < 0)
			{
				return -1;
			}
			//filters
			(UFDTArr[fd].ptrftable->readoffset) = size;
		}
		else if (from == END)
		{
			//filters
			if ((UFDTArr[fd].ptrftable->ptrinode->FileActualSize) + size > MAXFILESIZE)
			{
				return -1;
			}
			if ((UFDTArr[fd].ptrftable->readoffset) + size < 0)
			{
				return -1;
			}
			//filters
			(UFDTArr[fd].ptrftable->readoffset) = (UFDTArr[fd].ptrftable->ptrinode->FileActualSize) + size;
		}
	}
	else if (UFDTArr[fd].ptrftable->mode == WRITE)
	{
		if (from == CURRENT)
		{
			//filters
			if ((UFDTArr[fd].ptrftable->writeoffset) + size > MAXFILESIZE)
			{
				return -1;
			}
			if ((UFDTArr[fd].ptrftable->writeoffset) + size < 0)
			{
				return -1;
			}
			//filters
			if ((UFDTArr[fd].ptrftable->writeoffset) + size > (UFDTArr[fd].ptrftable->ptrinode->FileActualSize))
			{
				(UFDTArr[fd].ptrftable->ptrinode->FileActualSize) = ((UFDTArr[fd].ptrftable->writeoffset) + size);
			}
			(UFDTArr[fd].ptrftable->writeoffset) += size;
		}
		else if (from == START)
		{
			//filters
			if (size > MAXFILESIZE)
			{
				return -1;
			}
			if (size < 0)
			{
				return -1;
			}
			//filters
			if (size > (UFDTArr[fd].ptrftable->ptrinode->FileActualSize))
			{
				(UFDTArr[fd].ptrftable->ptrinode->FileActualSize) = size;
			}
			(UFDTArr[fd].ptrftable->writeoffset) = size;
		}
		else if (from == END)
		{
			//filters
			if ((UFDTArr[fd].ptrftable->ptrinode->FileActualSize) + size > MAXFILESIZE)
			{
				return -1;
			}
			if ((UFDTArr[fd].ptrftable->writeoffset) + size < 0)
			{
				return -1;
			}
			//filters
			(UFDTArr[fd].ptrftable->writeoffset) = (UFDTArr[fd].ptrftable->ptrinode->FileActualSize) + size;
			//(UFDTArr[fd].ptrftable->ptrinode->FileActualSize) += size;
		}
	}
}
int truncate_file(char* name)
{
	int fd = GetFDFromName(name);
	if (fd == -1)
	{
		return ERR_FILENOTFOUND;
	}
	memset(UFDTArr[fd].ptrftable->ptrinode->Buffer, 0, 1024);
	UFDTArr[fd].ptrftable->readoffset = 0;
	UFDTArr[fd].ptrftable->writeoffset = 0;
	UFDTArr[fd].ptrftable->ptrinode->FileActualSize = 0;

	return 0;
}