/*************************************************************************
 > File Name: fingerprint_interface.h
 > Author:
 > Mail:
 > Created Time: Fri 15 Jan 2016 09:31:10 AM CST
 ************************************************************************/

#ifndef _FINGERPRINT_INTERFACE_H
#define _FINGERPRINT_INTERFACE_H
#ifdef __cplusplus
extern "C" {
#endif

typedef int (*fp_open)(void);

typedef int (*fp_close)(void);

typedef int (*fp_get_fea)(unsigned char *pFeaBuffer, int nFeaLength,
		int *pRealFeaLength, int n_TimeOut_S);

typedef int (*fp_getLastImage)(unsigned char *pImgBuffer, int nImgLength,
		int *pRealImaLength, int *pImgWidth, int *pImgHeight);

typedef int (*fp_match)(unsigned char *pFeaBuffer1, int nFea1Length,
		unsigned char *pFealBuffer2, int nFea2Length);

typedef int (*fp_cancel)(void);

#ifdef __cplusplus
}
#endif
#endif
