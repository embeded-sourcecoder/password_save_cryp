/*************************************************************************
    > File Name: main.c
    > Author: 
    > Mail: 
    > Created Time: 2018年06月23日 星期六 22时11分20秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<locale.h>
#include<wchar.h>
#include "/usr/include/elf.h"

//#define DBGON
#ifdef DBGON

#define DBG(fmt, args...)  printf("[DeBug...][%s %d] "fmt"\n", __FILE__, __LINE__, ##args);   

#else

#define DBG 
#endif

typedef struct passinfo{
	char type[20];
	char name[30];
	char passwd[30];
}PASSWDINFO;

int buf[10240] = { 0xffff };
void show(FILE *fp,Elf64_Shdr Section_header);
void m_add(FILE *fp,Elf64_Shdr Section_header);
void m_delete(FILE *fp,Elf64_Shdr Section_header);
void find_data_session(FILE *fp, Elf64_Shdr *Section_header);
void cryp(PASSWDINFO *m_info);
void decryp(PASSWDINFO *m_info);

void m_init(FILE *fp,Elf64_Shdr Section_header);

void show_list(FILE *fp,Elf64_Shdr Section_header);

void user_init(FILE *fp,Elf64_Shdr Section_header);

int user_verify(FILE *fp,Elf64_Shdr Section_header);



#define STRUCT

int main(int argc,char *argv[])
{
		char *a[3];
        FILE * fp;
        int result = 0;
		Elf64_Shdr Section_header; 
		system("cp a.out .ab.out");
        fp= fopen("./.ab.out","r+");
		find_data_session(fp, &Section_header);
        
		printf("read section data offset %lx size %lx sh_entsize %lx sh_addralign %lx\n",Section_header.sh_offset,Section_header.sh_size,Section_header.sh_entsize,Section_header.sh_addralign);
		char key[20];
		char select;


		m_init(fp,Section_header);
		result = user_verify(fp,Section_header);
		if(result !=0)
		{
			printf("login error...exit\n");
			return -1;
		}

		while(select != 'q')
		{
			printf("input your select a for add d for delete s for show q for quit\n");
			scanf("%c",&select);
			getchar();
			switch(select)
			{
				case 'a':
					m_add(fp,Section_header);
					break;
				case 'd':
					
					printf("input key\n");
					scanf("%s",key);
					m_delete(fp,Section_header);
					break;
				case 's':
					show(fp,Section_header);
					break;
				case 'q':
					break;
				
			}
		}
		
		// for(i=0;i<Section_header.sh_size;i++)
			// DBG("%c",p[i]);
			// DBG("\n");
		fclose(fp);
		system("rm a.out");
		system("cp .ab.out a.out");
		system("rm .ab.out");
		return 0;
} 

void find_data_session(FILE *fp, Elf64_Shdr *Section_header)
{

     	int i;
        Elf64_Ehdr elfheader;
        
        fread(&elfheader,sizeof(Elf64_Ehdr),1,fp);
        for(i=0;i<16; i++)
            DBG("%x",elfheader.e_ident[i]);
        DBG("\n");
        DBG("e_shoff %lx\n", elfheader.e_shoff);
		
		fseek(fp,elfheader.e_shoff,SEEK_SET);
		DBG("read section header table\n");
		int size = 0;
		while(elfheader.e_shnum--)
		{
			fread(Section_header,sizeof(Elf64_Shdr),1,fp);
			DBG("read sh_type %lu offset %lx size %lx sh_entsize %lx sh_addralign %lx Section_header.sh_name %lx\n",Section_header->sh_type,Section_header->sh_offset,Section_header->sh_size,Section_header->sh_entsize,Section_header->sh_addralign,Section_header->sh_name);

			// if(Section_header.sh_type == 1 && Section_header.sh_name == 0xf8)
			if(Section_header->sh_type == 1 && Section_header->sh_name == 0xea)
			//	size += Section_header.sh_size;
				break;
		}



}


void m_init(FILE *fp,Elf64_Shdr Section_header)
{
	int i = 0, randnum;
	int count = 0, offset = 0;
	offset = Section_header.sh_offset + Section_header.sh_addralign;
	srand((unsigned int)time(NULL));
//	offset = ftell(fp);
	fseek(fp,offset,SEEK_SET);
	DBG("offset %x",offset);
	fread(&count,sizeof(count),1,fp);
	if(count == 0xffff)
	{
		count = 1;
		fseek(fp,offset,SEEK_SET);
		fwrite(&count,sizeof(count),1,fp);
	


	for(i = 0;i < Section_header.sh_size - Section_header.sh_addralign;i +=4)
	{
		count ++;
		randnum = rand();
	//	DBG("randnum %d\n",randnum);
		fwrite(&randnum, sizeof(randnum), 1,fp);

	}
	count = fflush(fp);

	user_init(fp,Section_header);
	//fwrite()

	}
	//fclose(fp);
	DBG("init sizeof fp %d\n",count);

}

void m_add(FILE *fp,Elf64_Shdr Section_header)
{
	int i = 0;
	int count = 0, offset = 0;
	offset = Section_header.sh_offset + Section_header.sh_addralign;
	PASSWDINFO m_info;
	memset(&m_info,0,sizeof(m_info));
	// memcpy(&m_info.type,argv[1],strlen(argv[1]));
	// memcpy(&m_info.name,argv[2],strlen(argv[2]));
	// memcpy(&m_info.passwd,argv[3],strlen(argv[3]));
	//fflush(stdin);

	fseek(fp,offset,SEEK_SET);
	fread(&count,sizeof(count),1,fp);
	if(count == 0xffff)
	{
		count = 0;
		fseek(fp,offset,SEEK_SET);
		fwrite(&count,sizeof(count),1,fp);
	//	m_init_data(fp,Section_header);
	}
	DBG("count %d sizeof info %ld \n",count,sizeof(m_info));

	printf("type: ");
	fgets(m_info.type,sizeof(m_info.type),stdin);
	printf("\n");
	printf("user: ");
	fgets(m_info.name,sizeof(m_info.name),stdin);
	printf("\n");
	printf("passwd: ");
	fgets(m_info.passwd,sizeof(m_info.passwd),stdin);
	printf("\n");

	cryp(&m_info);


	fseek(fp,offset + 4 + count * sizeof(m_info),SEEK_SET);
	
	fwrite(&m_info,sizeof(m_info),1,fp);
//	DBG("1 %ld  2 %ld  3 %ld\n",strlen(argv[1]),strlen(argv[2]),strlen(argv[3]));
	count ++;
	fseek(fp,offset,SEEK_SET);
	fwrite(&count,sizeof(count),1,fp);
	fseek(fp,offset,SEEK_SET);
	fread(&count,sizeof(count),1,fp);
	printf("count %d \n",count);

//for(i = 0;i<13;i++)
//	DBG("%c",argv[1][i]);
//	fwrite(argv[1],strlen(argv[1]),1,fp);

}

void m_delete(FILE *fp,Elf64_Shdr Section_header)
{
	int count = 0, offset = 0;

	offset = Section_header.sh_offset + Section_header.sh_addralign;


}

void show_list(FILE *fp,Elf64_Shdr Section_header)
{
	int count = 0, offset = 0, i;
	PASSWDINFO m_info;
	offset = Section_header.sh_offset + Section_header.sh_addralign;
	fseek(fp,offset,SEEK_SET);
	fread(&count,sizeof(count),1,fp);
	for(i = 0;i < count;i ++)
	{
		fread(&m_info,sizeof(m_info),1,fp);

		cryp(&m_info);
		printf("num %d m_info.type %s\n",i,m_info.type);
	}

}

void show(FILE *fp,Elf64_Shdr Section_header)
{
	int count = 0, offset = 0, i;
	char key[10];
	offset = Section_header.sh_offset + Section_header.sh_addralign;

	show_list(fp, Section_header);


	printf("input key to search\n");
	fgets(key, sizeof(key),stdin);
	
	PASSWDINFO m_info;
	fseek(fp,offset,SEEK_SET);
	fread(&count,sizeof(count),1,fp);
	for(i = 0;i < count;i ++)
	{
		fread(&m_info,sizeof(m_info),1,fp);

		cryp(&m_info);
		if(strncmp(key,m_info.type,strlen(key))==0)
		printf(" num %d m_info.type %s  \n m_info.name %s  \n m_info.passwd %s \n",i,m_info.type,m_info.name,m_info.passwd);
	}
}



void cryp(PASSWDINFO *m_info)
{
	int i = 0;
	unsigned int key[20] = { 88483429, 294728934, 123943821, 329281842, 193943828,121227, 197539, 1364525, 568295, 48800401, 399323300, 3929391047, 35268314, 12345222, 3856104, 19926385,1362847321, 2134535, 242423442, 42413234 };
	int *p = (int *)m_info;
	for(i = 0;i < sizeof(*m_info)/4;i ++)
	{
		p[i] ^= key[i];
	}

}


void user_init(FILE *fp,Elf64_Shdr Section_header)
{
	int count = 0, offset = 0, i;
	char key[10];
	offset = Section_header.sh_offset + Section_header.sh_addralign;
	fseek(fp,offset + 4,SEEK_SET);
	printf("input your user name and passwd for register\n");
	PASSWDINFO m_info;
	printf("user name:");
	fgets(m_info.name,sizeof(m_info.name),stdin);
	printf("user passwd:");
	fgets(m_info.passwd,sizeof(m_info.passwd),stdin);
	cryp(&m_info);
	fwrite(&m_info,sizeof(m_info),1,fp);
	fflush(fp);

	// fseek(fp,offset,SEEK_SET);
	// fread(&count,sizeof(count),1,fp);
	// count +=1;
	// fseek(fp,offset,SEEK_SET);
	// fwrite(&count,sizeof(count),1,fp);
	// fflush(fp);	

}


int user_verify(FILE *fp,Elf64_Shdr Section_header)
{
	int count = 0, offset = 0, i;
	
	offset = Section_header.sh_offset + Section_header.sh_addralign;
//	fseek(fp,offset + 4,SEEK_SET);
	PASSWDINFO m_info,input_user;
	printf("login:\n");
	printf("input user name:");
	fgets(input_user.name,sizeof(input_user.name),stdin);
	printf("input passwd:");
	fgets(input_user.passwd,sizeof(input_user.passwd),stdin);
	fseek(fp,offset + 4,SEEK_SET);
	fread(&m_info,sizeof(m_info),1,fp);
	cryp(&m_info);
	if(strcmp(m_info.name,input_user.name) == 0 && strcmp(m_info.passwd,input_user.passwd) == 0)
	{
		printf("login ok\n");
		return 0;
	}
	else
	return -1;
}