#ifndef IDENTITY_CARD_INTERFACE_H_
#define IDENTITY_CARD_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct tagIDCardData
{
	unsigned char strName[30];
	unsigned char strSex[2];
	unsigned char strNation[4];
	unsigned char strBorn[16];
	unsigned char strAddress[70];
	unsigned char strIDCardNo[36];
	unsigned char strGrantDept[30];
	unsigned char strUserLifeBegin[16];
	unsigned char strUserLifeEnd[16];
	unsigned char strReserved[36];
	unsigned char strPicture[1024];
}IDCARD_PROPERTY;

typedef int (*idcard_open)();

typedef int (*idcard_close)(int nHandle);

typedef int (*idcard_get_fixed_information)(int nHandle, struct tagIDCardData *pIDCardData);

typedef int (*idcard_search_target)(int nHandle);

#ifdef __cplusplus
}
#endif

#endif /* IDENTITY_CARD_INTERFACE_H_ */
