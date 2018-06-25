/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月24日 星期日 15时50分05秒
 ************************************************************************/
#include<crypt.h>
#include<stdio.h>
int main(void)
{
    char *buf, *pass;
	buf = crypt("1234567890","1122");
	
	
	pass = decrypt(buf,"1122");
	
	printf("%s\n",pass);
    return 0;
}
