/*
 * DES.cpp
 *
 *  Created on: 2012-7-26
 *      Author: 李敏
 */
#include <stdlib.h>
#include "DES.h"

/**********  DES 加密算法  ****************/

static unsigned char bitmask[]={ 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
static unsigned char KEY[64];
static unsigned char CDD[56];                     /* 56 bit C0 & D0       */
static unsigned char LR0[64],LR1[64];
static unsigned char SUBKEY[16][48];             /* 48 bit sub_key *16   */
static unsigned char bit48[48];
static unsigned char bit32[32];
unsigned char deskey[9];

int getbit(unsigned char *buf,int i)
{
    return ((*(unsigned char*)((unsigned char*)buf+(i>>3))) & bitmask[i&0x07])!=0;
}

void setbit(unsigned char *buf, int i)
{
    *((unsigned char *)buf+(i>>3)) |= bitmask[i&0x07];
}

void getsubkey()
{
    int i,j,s1,s2;
    for (i=0; i<56; i++)
        CDD[i]=KEY[PC_1[i]];
    for (i=0; i<16; i++)
    {
        s1=CDD[0]; s2=CDD[1];
        for (j=0; j<28-Shift[i]; j++)
            CDD[j]=CDD[j+Shift[i]];
        if (Shift[i]==2)
        {
            CDD[26]=s1;
            CDD[27]=s2;
        }
        else
            CDD[27]=s1;
        s1=CDD[28]; s2=CDD[29];
        for (j=28; j<56-Shift[i]; j++)
            CDD[j]=CDD[j+Shift[i]];
        if (Shift[i]==2)
        {
            CDD[54]=s1;
            CDD[55]=s2;
        }
        else
            CDD[55]=s1;
        for (j=0; j<48; j++)
            SUBKEY[i][j]=CDD[PC_2[j]];
    }
}

void DES_setkey(unsigned char *key)
{
    int i;
    for (i=0; i<64; i++)
        KEY[i]=getbit(key,i);
    getsubkey();
}

/******************************************************
DES加/解密函数
入口参数:int Mode =0 加密 =1 解密
         unsigned char *DataIn:输入数据 8字节
         unsigned char *Key:   密钥 8字节
出口参数:unsigned char *DataOut:加/解密结果 8字节
*******************************************************/
void DES(int Mode,unsigned char *DataIn,unsigned char *Key,unsigned char *DataOut)
{
    int i,j,k,r,op; /*op -1 解密, 1 加密  */
    unsigned char v4,v6;

    for(i=0; i<8; i++)   deskey[i]=Key[i];
    DES_setkey(deskey);
    if(Mode==DECRYPT)
    {
        r=15;
        op=-1;
    }
    else
    {
         r=0;
         op=1;
    }
    for (i=0; i<64; i++)
        LR0[i]=getbit(DataIn,IIP[i]);

    for (i=0; i<16; i++)
    {
        memcpy(LR1,LR0+32,32);
        memset(LR1+32,0,32);

        for (j=0; j<48; j++)
            bit48[j]=(SUBKEY[r][j]!=LR0[E[j]]);

        for (j=0; j<8; j++)
        {                          /* S box */
            v6=0; v4=(j<<2)+(j<<1);
            for (k=5; k>=0; k--)
                v6=(v6<<1)+bit48[v4+k];
            v4=S[j][v6];
            v6=(j<<2)+3;
            for (k=0; k<4; k++)
                bit32[v6-k]=v4&1,v4>>=1;
        }
        for (j=0; j<32; j++)
            LR1[j+32]=(LR0[j]!=bit32[P[j]]);
        memcpy(LR0,LR1,64);
        r+=op;
    }
    memcpy(LR1,LR0+32,32);
    memcpy(LR1+32,LR0,32);
    memset(DataOut,0,8);
    for (i=0; i<64; i++)
        if (LR1[IIP_1[i]]) setbit(DataOut,i);
}
/*******************************************************
3DES加/解密函数
入口参数:int Mode =0 加密 =1 解密
         unsigned char *DataIn:输入数据 8字节
         unsigned char *Key:   密钥 16字节
出口参数:unsigned char *DataOut:加/解密结果 8字节
********************************************************/
void TriDes(int Mode,unsigned char*DataIn,unsigned char*Key,unsigned char*DataOut)
{
        unsigned char keyl[9]="\0",keyr[9]="\0";
        unsigned char buf1[9]="\0",buf2[9]="\0";
        int i;

        for (i=0;i<8;i++) keyl[i]=Key[i];
        for (i=0;i<8;i++) keyr[i]=Key[i+8];
        for (i=0;i<8;i++) buf1[i]=DataIn[i];
        if (Mode==ENCRYPT)
        {
            DES(ENCRYPT,buf1,keyl,buf2);
            DES(DECRYPT,buf2,keyr,buf1);
            DES(ENCRYPT,buf1,keyl,buf2);
        }
        if (Mode==DECRYPT)
        {
            DES(DECRYPT,buf1,keyl,buf2);
            DES(ENCRYPT,buf2,keyr,buf1);
            DES(DECRYPT,buf1,keyl,buf2);
        }
        for (i=0;i<8;i++) DataOut[i]=buf2[i];
}

