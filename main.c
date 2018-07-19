/*************************************************************************
    > File Name: main.c
    > Author: 
    > Mail: 
    > Created Time: 2018年06月23日 星期六 22时11分20秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "/usr/include/elf.h"

char buf[10240] = {"00"};
void show(FILE *fp,Elf64_Shdr Section_header);
void m_add(FILE *fp,int offset,char *argv[]);
void m_delete(FILE *fp,int offset,char *key,Elf64_Shdr Section_header);

typedef struct passinfo{
	char type[20];
	char name[20];
	char passwd[30];
}PASSWDINFO;


#define STRUCT

int main(int argc,char *argv[])
{
	char *a[3];

	printf("1 %ld  2 %ld  3 %ld\n",strlen(a[1]),sizeof(a[2]),sizeof(a[3]));
         FILE * fp;
         int i;
         Elf64_Ehdr elfheader;
		 Elf64_Shdr Section_header; 
		 system("cp a.out .ab.out");
         fp= fopen("./.ab.out","r+");
         fread(&elfheader,sizeof(Elf64_Ehdr),1,fp);
         for(i=0;i<16; i++)
                  printf("%x",elfheader.e_ident[i]);
         printf("\n");
         printf("e_shoff %lx\n", elfheader.e_shoff);
		
		fseek(fp,elfheader.e_shoff,SEEK_SET);
		printf("read section header table\n");
		int size = 0;
		while(elfheader.e_shnum--)
		{
			fread(&Section_header,sizeof(Elf64_Shdr),1,fp);
			printf("read sh_type %lu offset %lx size %lx sh_entsize %lx sh_addralign %lx Section_header.sh_name %lx\n",Section_header.sh_type,Section_header.sh_offset,Section_header.sh_size,Section_header.sh_entsize,Section_header.sh_addralign,Section_header.sh_name);

			// if(Section_header.sh_type == 1 && Section_header.sh_name == 0xf8)
			if(Section_header.sh_type == 1 && Section_header.sh_name == 0xea)
			//	size += Section_header.sh_size;
				break;
		}
		printf("size buf %d %d\n",sizeof(buf),strlen(buf));

		printf("read section data offset %lx size %lx sh_entsize %lx sh_addralign %lx\n",Section_header.sh_offset,Section_header.sh_size,Section_header.sh_entsize,Section_header.sh_addralign);
		fseek(fp,Section_header.sh_offset,SEEK_SET);
		printf("read section data\n");
		char *p = (char *)malloc(Section_header.sh_size);
		int count = fread(p,sizeof(char),Section_header.sh_size,fp);
		int offset =0;
		//for(offset=32;p[offset]!=0;offset++);
		
		
		

		
		printf("size buf %d %d  i = %d offset = %d count %d\n",sizeof(buf),strlen(buf),i,offset,count);
		char select;
		char key[100];
		printf("input your select a d s\n");
		scanf("%c",&select);
		switch(select)
		{
			case 'a':
				m_add(fp,Section_header.sh_offset + Section_header.sh_addralign,argv);
				break;
			case 'd':
				
				printf("input key\n");
				scanf("%s",&key);
				m_delete(fp,Section_header.sh_offset,key,Section_header);
				break;
			case 's':
				show(fp,Section_header);
				break;
			
		}
		
		// for(i=0;i<Section_header.sh_size;i++)
			// printf("%c",p[i]);
			// printf("\n");
		fclose(fp);
		system("rm a.out");
		system("cp .ab.out a.out");
		system("rm .ab.out");
		return 0;
} 

#ifndef STRUCT
void m_add(FILE *fp,int offset,char *argv[])
{
		fseek(fp,offset,SEEK_SET);
		printf("%s   %s   %s\n",argv[1],argv[2],argv[3]);
		//| qq ! user & passwd |
		fwrite("|",1,1,fp);
		
		fwrite(argv[1],strlen(argv[1]),1,fp);
		fwrite("!",1,1,fp);
		
		fwrite(argv[2],strlen(argv[2]),1,fp);
		fwrite("&",1,1,fp);
		
		fwrite(argv[3],strlen(argv[3]),1,fp);
		fwrite("|",1,1,fp);
	
	
	
}

void m_delete(FILE *fp,int offset,char *key,Elf64_Shdr Section_header)
{
		int i = 0,j = 0;
		int result = -1;
		char *p = (char *)malloc(Section_header.sh_size);
		fseek(fp,offset + 34,SEEK_SET);
		fread(p,sizeof(char),Section_header.sh_size - 34,fp);
		for(i=0;i<Section_header.sh_size - 34;i++)
		{
			if(p[i] == '|' && p[i + strlen(key)+1] == '!')
			{
				printf("i = %d %c",i,p[i]);
				result = strncmp(p+i+1,key,strlen(key));
			}
			if(result == 0)
			{
				printf("find ok deleting\n");
				
				for(j=i+1;p[j]!='|';j++);
				printf("j = %d %c",j,p[j]);
				for(i,j;j<Section_header.sh_size - 34 -j -1 ;i++,j++)
				{
					p[i] = p[j+1];
					printf("i = %d %c",i,p[i]);
					printf("j = %d %c",j,p[j]);
					
				}
				
				break;
			}
			
		}
		fseek(fp,offset + 34,SEEK_SET);
	fwrite(p,Section_header.sh_size,1,fp);
	
}

void show(FILE *fp,Elf64_Shdr Section_header)
{
		int i = 0;
		char *p = (char *)malloc(Section_header.sh_size);
		fseek(fp,Section_header.sh_offset + 34,SEEK_SET);
		fread(p,sizeof(char),Section_header.sh_size - 34,fp);
		
		for(i=0;i<Section_header.sh_size - 34 && p[i]!=0;i++)
		{
			switch(p[i])
			{
				case '|':
					printf("\n");
					if(p[i+1]!='|' && p[i+1]!=0)
						printf("Type: ");
					break;
				case '!':
				printf("\033[1m\033[45;33m user: \033[0m");
					break;
				case '&':
				printf("\033[1m\033[43;30m passwd: \033[0m");
					break;
				default:
					printf("\033[1m\033[47;30m%c\033[0m",p[i]);

			}
		}

}
#else

void m_add(FILE *fp,int offset,char *argv[])
{
	int i = 0;
	PASSWDINFO m_info;
	memset(&m_info,0,sizeof(m_info));
	memcpy(&m_info.type,argv[1],strlen(argv[1]));
	memcpy(&m_info.name,argv[2],strlen(argv[2]));
	memcpy(&m_info.passwd,argv[3],strlen(argv[3]));

	fseek(fp,offset,SEEK_SET);
	
	fwrite(&m_info,sizeof(m_info),1,fp);
	printf("1 %ld  2 %ld  3 %ld\n",strlen(argv[1]),strlen(argv[2]),strlen(argv[3]));
for(i = 0;i<13;i++)
	printf("%c",argv[1][i]);
//	fwrite(argv[1],strlen(argv[1]),1,fp);

}

void m_delete(FILE *fp,int offset,char *key,Elf64_Shdr Section_header)
{



}


void show(FILE *fp,Elf64_Shdr Section_header)
{
	PASSWDINFO m_info;
	fseek(fp,Section_header.sh_offset + Section_header.sh_addralign,SEEK_SET);
	fread(&m_info,sizeof(m_info),1,fp);

	printf("m_info.type %s  m_info.name %s   m_info.passwd %s \n",m_info.type,m_info.name,m_info.passwd);

}

#endif