#ifndef ClsFileInfo_H
#define ClsFileInfo_H
#define READ_BUF_SIZE	20480
#pragma   comment(lib,   "Version.lib") 
class ClsFileInfo
{
protected:
	ClsFileInfo():
	m_cstrFilePath(L""),
	m_cstrFileMD5(L""),
	m_cstrFileCRC(L""),
	m_cstrFileSize(L""),
	m_cstrFileVersion(L"")
	{}
public:
	static ClsFileInfo* getInstance(){
		static ClsFileInfo obj;
		return &obj;
	}

	//��������
	~ClsFileInfo(){

	}

	BOOL FileInfoInit(CString &filePath);
	BOOL GetFileMD5(CString &fileMD5);
	BOOL GetFileCRC(CString &fileCRC);
	BOOL GetFilePath(CString &filePath);
	BOOL GetFileVersion(CString &fileVersion);
	BOOL GetFileSize(CString &fileSize);
	
private:
	BOOL SetFileCRCMD5Size();
	BOOL SetFileVerSion();
	
	CString m_cstrFilePath;
	CString m_cstrFileMD5;
	CString m_cstrFileCRC;
	CString m_cstrFileSize;
	CString m_cstrFileVersion;
};
#endif