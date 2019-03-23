#include"myheader.h"

int main()
{
	char* ptr = NULL;
	int j = 0;
	int ret = 0, fd = 0, count = 0;
	char command[4][80], str[80], arr[1024];

	InitialiseSuperBlock();
	CreateDILB();

	while (1)
	{
		//fflush(stdin);
		if (j == -1) { getchar(); }
		j = 0;
		strcpy(str, "");
		printf("\nShruti VFS : >");
		//system("pause");
		fgets(str, 80, stdin);
		count = sscanf(str, "%s%s%s%s", command[0], command[1], command[2], command[3]);

		if (count == 1) 
		{
			if (_stricmp(command[0], "ls") == 0)
			{
				ls_file();
				//continue;
			}
			else if (_stricmp(command[0], "closeall") == 0)
			{
				CloseAllFile();
				printf("All files closed successfuly\n");
				continue;
			}
			else if (_stricmp(command[0], "clear") == 0)
			{
				system("cls");
				continue;
			}
			else if (_stricmp(command[0], "help") == 0)
			{
				DisplyHelp();
				continue;
			}
			else if (_stricmp(command[0], "exit") == 0)
			{
				printf("\nTerminating the ShrutiP Virtual File System\n");
				break;
			}
			else
			{
				printf("\nERROR: Command not found\n");
				continue;
			}
		}
		else if (count == 2)
		{
			if (_stricmp(command[0], "stat") == 0)
			{
				ret = stat_file(command[1]);
				if (ret == ERR_WRONGPAR)
				{
					printf("ERROR: \n");
				}
				if (ret == ERR_FILENOTFOUND)
				{
					printf("ERROR: There is no file named \"%s\"\n", command[1]);
				}
				continue;
			}
			else if (_stricmp(command[0], "fstat") == 0) // file description using fd
			{
				ret = fstat_file(atoi(command[1]));//command[1] contains file descriptor
				if (ret == ERR_WRONGPAR)
				{
					printf("ERROR: \n");
				}
				if (ret == ERR_FILENOTFOUND)
				{
					printf("ERROR: There is no file with file descriptor \"%d\"\n", command[1]);
				}
				continue;
			}
			else if (_stricmp(command[0], "close") == 0)
			{
				ret = CloseFileByName(command[1]);
				if (ret == ERR_FILENOTFOUND)
				{
					printf("ERROR: There is no file named \"%s\"\n", command[1]);
				}
				continue;
			}
			else if (_stricmp(command[0], "rm") == 0)
			{
				ret = rm_file(command[1]);
				if (ret == ERR_FILENOTFOUND)
				{
					printf("ERROR: There is no file named \"%s\"\n", command[1]);
				}
				continue;
			}
			else if (_stricmp(command[0], "man") == 0)
			{
				man(command[1]);
				//continue;
			}
			else if (_stricmp(command[0], "write") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("Error: File Not Found\n");
					continue; 
				}
				printf("Enter the data: \n");
				scanf("%[^'\n']s", arr);
				ret = strlen(arr);
				if (ret == 0)
				{
					printf("Invalid Command\n");
					continue;
				}
				ret = WriteFile(fd, arr, ret);
				if (ret == ERR_NOPERMISSION)
				{
					printf("Error: Permission denied\n");
					//continue;
				}
				if (ret == ERR_NOTENOUGHMEM)
				{
					printf("Error: Not sufficient memory in file\n");
					//continue;
				}
				if (ret == ERR_FILENOTREGULAR)
				{
					printf("It is not a regular file\n");
					//continue;
				}
				if (ret > 0)
				{
					printf("Data successfully written in file\n");
					j=-1;
					continue;
				}
				//continue;
				j = -1;
			}
			else if (_stricmp(command[0], "truncate") == 0)
			{
				ret = truncate_file(command[1]);
				if (ret == -1)
				{
					printf("File Not Found\n");
				}
				//printf("\nTerminating the ShrutiP Virtual File System\n");
				//continue;
			}
			else
			{
				printf("\nERROR: Command not found\n");
				continue;
			}
		}
		else if (count == 3)
		{
			if ((_stricmp(command[0], "create") == 0))
			{
				ret = CreateFileX(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File Successfully created, file descrptor=%d\n",ret);
					//continue;
				}
				if (ret == ERR_WRONGPAR)
				{
					printf("Error: Invalid Command\n");
					//continue;
				}
				if (ret == ERR_NOINODES)
				{
					printf("Error: No inodes left\n");
					//continue;
				}
				if (ret == ERR_FILEALREADYEXISTS)
				{
					printf("Error: File named %s already exists\n", command[1]);
					//continue;
				}
				if (ret == ERR_MEMALLOCFAILED)
				{
					printf("Error: Memory allocation failed\n");
					//continue;
				}
				continue;
			}
			else if ((_stricmp(command[0], "open") == 0))
			{
				ret = OpenFile(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File Successfully opened\n", ret);
					//continue;
				}
				if (ret == ERR_WRONGPAR)
				{
					printf("Error: Invalid Command\n");
					//continue;
				}
				if (ret == ERR_FILENOTFOUND)
				{
					printf("Error: File not found\n");
					//continue;
				}
				if (ret == ERR_NOPERMISSION)
				{
					printf("Error: Permission denied\n", command[1]);
					//continue;
				}
				if (ret == ERR_MEMALLOCFAILED)
				{
					printf("Error: Memory allocation failed\n");
					//continue;
				}
				continue;
			}
			else if ((_stricmp(command[0], "read") == 0))
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("Error: File not found\n");
					continue;
				}
				ptr = (char*)malloc(sizeof(atoi(command[2]) + 1)); // +1 for'\0'
				if (ptr == NULL)
				{
					printf("Error: Memory allocation failed\n");
					continue;
				}
				ret = ReadFile(fd, ptr, atoi(command[2]));
				if (ret == ERR_FILENOTFOUND)
				{
					printf("Error: Invalid Command\n");
					//continue;
				}
				if (ret == ERR_NOPERMISSION)
				{
					printf("Error: File not found\n");
					//continue;
				}
				if (ret == ERR_NOTENOUGHMEM)
				{
					printf("Error: Permission denied\n", command[1]);
					//continue;
				}
				if (ret == ERR_FILENOTREGULAR)
				{
					printf("Error: Memory allocation failed\n");
					//continue;
				}
				if (ret > 0)
				{
					_write(2, ptr, ret);//it should be 1 though?
					//continue;
				}
				continue;
			}
			else
			{
				printf("\nERROR: Command not found\n");
				continue;
			}
		}
		else if (count == 4)
		{
			if ((_stricmp(command[0], "lseek") == 0))
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("Error: File not found\n");
					continue;
				}
				ret = LseekFile(fd, atoi(command[2]), atoi(command[3]));
				if (ret == -1)
				{
					printf("Unable to perform lseek\n");
				}
			}
			else
			{
				printf("\nERROR: Command not found\n");
				continue;
			}
		}
		else
		{
			printf("\nERROR: Command not found\n");
			continue;
		}
	}
	j++;
	return 0;
}