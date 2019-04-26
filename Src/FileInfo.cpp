#include "stdafx.h" 
#include "FileInfo.h"
#include "CRC32.h"
#include "MD5.h"
#include <string>
#include <fstream>

int ClsFileInfo::FileInfoInit(CString &FilePath){
	m_cstrFilePath = FilePath ;

	if(m_cstrFilePath.IsEmpty()){ return -1; }

	SetFileCRCMD5Size();
	SetFileVerSion();
	return 0;
}

BOOL ClsFileInfo::GetFileMD5(CString &fileMD5){
	fileMD5 = m_cstrFileMD5.MakeUpper();
	return TRUE;
}

BOOL ClsFileInfo::GetFileCRC(CString &fileCRC){
	fileCRC = m_cstrFileCRC.MakeUpper();
	return TRUE;
}

BOOL ClsFileInfo::GetFilePath(CString &filePath){
	filePath = m_cstrFilePath;
	return TRUE;
}

BOOL ClsFileInfo::GetFileVersion(CString &fileVersion){
	fileVersion = m_cstrFileVersion;
	return TRUE;
}

BOOL ClsFileInfo::GetFileSize(CString &fileSize){
	fileSize = m_cstrFileSize;
	return TRUE;
}

//
// 文件版本信息
// 说明:要使用此函数必需在程序中加入   
// #pragma comment(lib,"Version.lib")   
//
BOOL ClsFileInfo::SetFileVerSion(){
	CString   strVersion = L"";   
	// 要获取信息的文件   
	TCHAR   szSrcfn[MAX_PATH];
	if(m_cstrFilePath.IsEmpty()) {
		::GetModuleFileName(NULL,   szSrcfn,   sizeof(szSrcfn));   
	} else {
		wsprintf(szSrcfn,L"%s",m_cstrFilePath); 
	}
		
	// 读文件信息   
	DWORD   dwVerHnd =   0;   
	DWORD   dwVerInfoSize   = ::GetFileVersionInfoSize(szSrcfn,   &dwVerHnd);   
	if(dwVerInfoSize){
		HANDLE     hMem;   
		LPVOID     lpvMem;   
		unsigned   int   uInfoSize   =   0;   
		VS_FIXEDFILEINFO   *   pFileInfo;   

		hMem=   ::GlobalAlloc(GMEM_MOVEABLE,   dwVerInfoSize);   
		lpvMem=   ::GlobalLock(hMem);   
		::GetFileVersionInfo(szSrcfn,   dwVerHnd,   dwVerInfoSize,   lpvMem);   
		::VerQueryValue(lpvMem,   (LPTSTR)_T("\\"),   (void**)&pFileInfo,   &uInfoSize);   

		WORD   nVer[4];   
		nVer[0]   =   HIWORD(pFileInfo->dwProductVersionMS);     
		nVer[1]   =   LOWORD(pFileInfo->dwProductVersionMS);   
		nVer[2]   =   HIWORD(pFileInfo->dwProductVersionLS);   
		nVer[3]   =   LOWORD(pFileInfo->dwProductVersionLS);     
		strVersion.Format(_T("%d.%d.%d.%d"),   nVer[0],   nVer[1],nVer[2],nVer[3]);     

		::GlobalUnlock(hMem);   
		::GlobalFree(hMem);   
	}

	m_cstrFileVersion = strVersion;   

	return TRUE;
}   

// 文件CRC
BOOL ClsFileInfo::SetFileCRCMD5Size(){
	MD5 md5 ;
	md5.reset();
	
	int uReadSize = 0;
	CString strTmpResult = L"";
	char szReadBuf[READ_BUF_SIZE] = "";
	CString filePath = m_cstrFilePath;
	unsigned int uRetCRCValue = 0;
	TCHAR* pszTest = filePath.GetBuffer(MAX_PATH);
	
	// 文件大小
	int nFileSize = 0;   

	FILE *pFile;
	pFile = _wfopen(pszTest, L"rb");

	if (pFile == NULL){ return FALSE; }

	while(TRUE){
		::ZeroMemory(szReadBuf, sizeof(szReadBuf) * sizeof(char));

		uReadSize = (int)fread(szReadBuf,  sizeof(char), READ_BUF_SIZE, pFile);

		if (uReadSize == 0){ break; }

		uRetCRCValue = CRC32_C(uRetCRCValue, szReadBuf, uReadSize);
		md5.update(szReadBuf, uReadSize);
		nFileSize += uReadSize;
	}
		
	m_cstrFileCRC.Format(L"%x", uRetCRCValue);
	m_cstrFileSize.Format(L"%d", nFileSize);

	CString strmd5(md5.toString().c_str());
	m_cstrFileMD5 = strmd5;

	fclose(pFile);	
	filePath.ReleaseBuffer();

	return TRUE;
}