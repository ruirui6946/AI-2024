#include<stdio.h>
#include<string.h>
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error input!!\n");
        return 0;
    }
    FILE *fp,*fp2;
    fp=fopen(argv[1],"r");
    char aline[50];
    int num;
    num=atoi(argv[2]);

    int flag=1;
    int wnum=1;
    char tmp0[50]=".txt";
    char tmp1[50]="wenjian";
    char tmp2[50];
    char tmp3[50];
    while(1)
    {
        fgets(aline,50,fp);

        if(flag==1)
        {
            itoa(wnum,tmp3,10);
            strcpy(tmp2,tmp1);
            strcat(tmp2,tmp3);
            strcat(tmp2,tmp0);
            fp2=fopen(tmp2,"w");
        }
        flag++;

        fprintf(fp2,"%s",aline);

        if(flag>num)
        {
            fclose(fp2);
            flag=1;
            wnum++;
        }

        if(feof(fp))
        {
            close(fp);
            break;
        }
    }
    return 0;
}
