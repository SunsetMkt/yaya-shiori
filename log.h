// 
// AYA version 5
//
// ���M���O�p�N���X�@CLog
// written by umeici. 2004
// 

#ifndef	LOGGERH
#define	LOGGERH

//----

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include <vector>
#include <deque>

#include "globaldef.h"

#define	CLASSNAME_CHECKTOOL	"TamaWndClass"	/* �`�F�b�N�c�[���̃E�B���h�E�N���X�� */

//----

class	CLog
{
protected:
	aya::string_t		path;		// ���O�t�@�C���̃p�X
	int			charset;	// �����R�[�h�Z�b�g
#if defined(WIN32)
	HWND		hWnd;		// �`�F�b�N�c�[����HWND
#endif

	char		enable;		// ���M���O�L���t���O
	char		open;		// ���M���O�J�n�t���O
	char		fileen;		// �t�@�C���ւ̃��M���O�L���t���O
	char		iolog;		// ���o�̓��M���O�L���t���O

	//���͂ł��̕����񂪂������烍�O�o�͂��Ȃ����X�g
	std::vector<aya::string_t> ignore_iolog_strings;
	char		ignore_iolog_noresult;//���̓��͌�ɏo�͂��}�����邽�߂̃t���O

	std::deque<aya::string_t> error_log_history;

public:
	CLog(void)
	{
		enable = 1;
		open = 0;
		fileen = 1;
		iolog  = 1;
		ignore_iolog_noresult=0;
	}

#if defined(POSIX)
	typedef void* HWND;
#endif
	void	Start(const aya::string_t &p, int cs, HWND hw, char il);
	void	Termination(void);

	void	Write(const aya::string_t &str, int mode = 0);
	void	Write(const aya::char_t *str, int mode = 0);

	void	Message(int id, int mode = 0);
	void	Filename(const aya::string_t &filename);

	void	Error(int mode, int id, const aya::char_t *ref, const aya::string_t &dicfilename, int linecount);
	void	Error(int mode, int id, const aya::string_t &ref, const aya::string_t &dicfilename, int linecount);
	void	Error(int mode, int id, const aya::char_t *ref);
	void	Error(int mode, int id, const aya::string_t &ref);
	void	Error(int mode, int id, const aya::string_t &dicfilename, int linecount);
	void	Error(int mode, int id);

	void	Io(char io, const aya::char_t *str);
	void	Io(char io, const aya::string_t &str);

	void	IoLib(char io, const aya::string_t &str, const aya::string_t &name);

	void	SendLogToWnd(const aya::char_t *str, int mode);
	void	SendLogToWnd(const aya::string_t &str, int mode);

	void	AddIgnoreIologString(const aya::string_t &ignorestr);
	void	ClearIgnoreIologString();

	std::deque<aya::string_t> & GetErrorLogHistory(void);
	void SetErrorLogHistory(std::deque<aya::string_t> &log);

protected:
#if defined(WIN32)
	HWND	GetCheckerWnd(void);
#endif

	void    AddErrorLogHistory(const aya::string_t &err);

};

//----

#endif
