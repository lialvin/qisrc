
#include  "stdio.h"
#include  <time.h>
#include  "BizDeal.h"
#include  "netpdu.h"
#include  "stdlib.h"
#include  <string.h> 
#include  <string> 
#include  <vector> 

CBizDeal::CBizDeal()
{	
	m_pSendBuf= NULL;
	m_pFileName=NULL;
}

void  CBizDeal::Init()
{
	//m_pTaskScan = pTaskScan;
	m_pSendBuf = new BYTE[64 * 1024];
	SET_HEAD_FLAG(m_pSendBuf);
	m_pFileName = new char[1024];
	memset(m_pFileName, 0, 1024);
	//g_pTcpServer->ReadCfg();	
}

CBizDeal::~CBizDeal()
{	
	delete [] m_pSendBuf;
	delete [] m_pFileName;
	
	m_pSendBuf= NULL;
	m_pFileName=NULL;
}

/*
	
*/

void CBizDeal::Run(BYTE* pBuf,int nLen)
{
		
	NET_PKG_HEAD * p = (NET_PKG_HEAD*)pBuf;
	if( !(CHECK_HEAD_FLAG(pBuf)))
	{
		return;
	}
	
	FILE_SCAN_STRU * pScan = (FILE_SCAN_STRU *)p->u.szData;
	switch(pScan->cmdtype)
	{
		case  CMD_DIR_REQ_SCAN:
			ScanDir(pScan->context);
			break;
		case  CMD_DIR_RES_SCAN:
			RecvDirFile(NULL, pScan->flag, (BYTE*)pScan->context, pScan->dataLen);
			break;
		case  CMD_FILE_REQ_TRANS:
			TransFile(pScan->context);
			break;
		case  CMD_FILE_RES_TRANS:
			RecvBinFile(NULL, pScan->flag,(BYTE*) pScan->context, pScan->dataLen);
			break;		 			
		case  CMD_REQ_CMD_EXEC:
			ExecCmdReq(pScan->context, pScan->dataLen);
			break;
		case  CMD_RES_CMD_EXEC:
			RecvCmdExecRes(pScan->flag, (BYTE*)pScan->context, pScan->dataLen);
			break;
		case  CMD_SCREEN_REQ_COPY: 
			RecvCmdScreenReq(pScan->flag, (BYTE*)pScan->context, pScan->dataLen);
			break;
		case  CMD_SCREEN_RES_COPY:
			RecvCmdScreenRes(pScan->flag, (BYTE*)pScan->context, pScan->dataLen);
			break;
		case  CMD_REQ_FAILED:
			RecvCmdExecFail(pScan->flag, (BYTE*)pScan->context, pScan->dataLen);
			break;
		case  CMD_REQ_VERSION:
			break;			
	};
 
}

void   CBizDeal::RecvCmdExecFail(WORD flag, BYTE * pBuf, int nLen)
{
	//SEND_LOGLX(LOG_INFO, "RecvCmdExecFail");

}

void   CBizDeal::RecvCmdExecRes(WORD flag, BYTE * pBuf, int nLen)
{
	//SEND_LOGLX(LOG_INFO, "RecvCmdExecRes success ");

}


int  CBizDeal::ExecCmdReq(char * context, int dataLen)
{	
	system(context);
	if(0) SendFailedPkg();
	SendCmdPkg(CMD_RES_CMD_EXEC, 0x8, 0, 0, 0);
	return 0;
}

void CBizDeal::ScanDir(char * context)
{
	int nLen = 0;
	std::string strBuff;
	std::vector<std::string> namevect;
/*
	FindFilesInFloder(context, NULL, namevect, 3, FALSE); 
	std::vector<std::string>::iterator it;
	
	for (it = namevect.begin(); it != namevect.end(); it++)
	{		
		strBuff += *it;
		strBuff += " ;\n";
	}
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	pSendHead->nServerType = 1;

	FILE_SCAN_STRU   * pScanResp = (FILE_SCAN_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = CMD_DIR_RES_SCAN;
	pScanResp->dataLen =(WORD) strBuff.length();
	strncpy(pScanResp->context, strBuff.c_str(),strBuff.length());	
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD)+ sizeof(FILE_SCAN_STRU)-8 + strBuff.length();
       */
	//m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}

void CBizDeal::TransFile(char * context)
{
	/*if (IsFileExist(context)==FALSE) 
	{
		SendFailedPkg();
		return ;
	}

	if (SendBinFile(context) == false)
	{
		SendFailedPkg();
	}*/
	return;
 

}
// 
bool CBizDeal::SendBinFile(  const char * szOrigFile)
{
	bool bRet = true;
/*
	if (szOrigFile == NULL)
	{
		return false;
	}
	BYTE  flag = 0x1;
	int  nReadLen;
	char szBuf[16 * 1024] = { 0 };
	std::ifstream fin(szOrigFile, std::ios::binary);
	
	if (fin.bad())
	{
		bRet = false;
	}
	else
	{
		while (!fin.eof())
		{			
			fin.read(szBuf, sizeof(char) * 15*1024);
			nReadLen =(int) fin.gcount();
			SendCmdPkg(CMD_FILE_RES_TRANS,flag, (BYTE*)szBuf, nReadLen, szOrigFile);
			if (flag == 0x1)
				flag = 0x2;
		}
		SendCmdPkg(CMD_FILE_RES_TRANS, 0x8, (BYTE*)szBuf, 0, szOrigFile);
	}

	fin.close();
*/	
	return bRet;
}
//
void CBizDeal::SendCmdPkg(int nCmdType, WORD flag, BYTE * pBuf, int nLen,const char * fileName)
{
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	pSendHead->nServerType = 1;
	SCAN_FILE_RESP_STRU   * pScanResp = (SCAN_FILE_RESP_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = nCmdType;
	pScanResp->flag = flag;
	pScanResp->dataLen = nLen;
	if(nLen>0)
		memcpy(pScanResp->context, pBuf, nLen);
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD) + sizeof(FILE_SCAN_STRU) - 4 + nLen;

	//m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}

bool CBizDeal::RecvDirFile(char * szDestFile, WORD flag, BYTE * pBuf, int nLen)
{ 
	strcpy(m_szTranFile, "tmpdir.tmp");
 	
	bool bRet = true;
	
	/*std::ofstream fout(m_szTranFile, std::ios::binary | std::ios::ate | std::ios::out);// 
	fout.write((char*)pBuf, nLen);	
	fout.close();
*/
	return bRet;
}
//
bool CBizDeal::RecvBinFile(char * szDestFile, WORD flag, BYTE * pBuf, int nLen)
{
	bool bRet = true;
	
/*	static std::ofstream fout;
	
	if (flag == 0x1)
	{
		if (strlen(m_szTranFile) == 0)
		{
			strcpy(m_szTranFile, "tmpfile.tmp");
			return false;
		}		
		//outfile.open("c:\\abc.txt", ios::ate | ios::out);
		fout.open(m_szTranFile, std::ios::binary | std::ios::ate | std::ios::out);
	}
	

	//std::ofstream fout(m_szTranFile, std::ios::binary | std::ios::app);
	 
	if (!((flag&0x8)==0x8))
	{
		if (fout.bad())
		{
			bRet = false;
			//SEND_LOGLX(LOG_INFO, "file error");
		}
		//   
		fout.write((char*)pBuf, nLen);
		
	}
	else if ((flag & 0x8) == 0x8)
	{
		//SEND_LOGLX(LOG_INFO, "Recv File Finished");
		if(nLen >0)
			fout.write((char*)pBuf, nLen);
		fout.close();
	}	 	 
*/	
	return bRet;
}

int  CBizDeal::SendFailedPkg()
{
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	pSendHead->nServerType = 1;
	SCAN_FILE_RESP_STRU   * pScanResp = (SCAN_FILE_RESP_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = CMD_REQ_FAILED;
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD) + sizeof(SCAN_FILE_RESP_STRU) - 4;
	return 0; // m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}
//
int  CBizDeal::BeginReqFile(char * szFile)
{
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	strcpy(m_szTranFile, "D:\\");
	pSendHead->nServerType = 1;
	FILE_SCAN_STRU   * pScanResp = (FILE_SCAN_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = CMD_FILE_REQ_TRANS;
	pScanResp->dataLen = (WORD)strlen(szFile);
	strcpy(pScanResp->context, szFile);
	char * pos = strrchr(szFile, '\\');
	strcat(m_szTranFile,pos+1 );
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD) + sizeof(FILE_SCAN_STRU) - 4 + strlen(szFile);
	return  0;// m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}
//
int  CBizDeal::BeginReqDir(char * szFile)
{
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	pSendHead->nServerType = 1;
	FILE_SCAN_STRU   * pScanResp = (FILE_SCAN_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = CMD_DIR_REQ_SCAN;
	pScanResp->dataLen = (WORD)strlen(szFile);
	strcpy(pScanResp->context, szFile);
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD) + sizeof(FILE_SCAN_STRU) - 4 + strlen(szFile);
	return 0;//m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}

int  CBizDeal::BeginReqExecCmd(char * szFile)
{
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	pSendHead->nServerType = 1;
	FILE_SCAN_STRU   * pScanResp = (FILE_SCAN_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = CMD_REQ_CMD_EXEC;
	pScanResp->dataLen = (WORD)strlen(szFile);
	strcpy(pScanResp->context, szFile);
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD) + sizeof(FILE_SCAN_STRU) - 4 + strlen(szFile);
	return  0;//m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}
int  CBizDeal::BeginReqScreen(char * szFile)
{
	NET_PKG_HEAD  * pSendHead = (NET_PKG_HEAD *)m_pSendBuf;
	pSendHead->nServerType = 1;
	FILE_SCAN_STRU   * pScanResp = (FILE_SCAN_STRU   *)pSendHead->u.szData;
	pScanResp->cmdtype = CMD_SCREEN_REQ_COPY;
	pScanResp->dataLen = (WORD)strlen(szFile);
	strcpy(pScanResp->context, szFile);
	pSendHead->nPackageLength = sizeof(NET_PKG_HEAD) + sizeof(FILE_SCAN_STRU) - 4 + strlen(szFile);
	return  0;//m_pTaskScan->SendCxt(m_pSendBuf, pSendHead->nPackageLength);
}

void   CBizDeal::RecvCmdScreenReq(WORD flag, BYTE * pBuf, int nLen)
{
	ScreenShot((char*)pBuf);
	SendCmdPkg(CMD_SCREEN_RES_COPY, 0, (BYTE*)pBuf, 0, 0);
}
void   CBizDeal::RecvCmdScreenRes(WORD flag, BYTE * pBuf, int nLen)
{
	//SEND_LOGLX(LOG_INFO, "screen copy finished");
}

void   CBizDeal::ScreenShot (char * filename)
{
	/*HWND         hWnd;
	HDC         hdc;
	PAINTSTRUCT ps;
	HBITMAP        hBitmap;
	HDC            hdcMem;
	int screenx=0, screeny=0;

	hWnd = GetTopWindow(NULL);
	hdc = GetDC(NULL);

	hBitmap = CreateCompatibleBitmap(hdc, screenx, screeny);
	hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, hBitmap);
	BitBlt(hdcMem, 0, 0, screenx, screeny, hdc, 0, 0, SRCCOPY);
	//SaveBitmapToFile(filename,  hBitmap);

	ReleaseDC(hWnd, hdcMem);
	ReleaseDC(hWnd, hdc);
*/
}
