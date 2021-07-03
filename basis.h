// 
// AYA version 5
//
// ��Ȑ�����s�Ȃ��N���X�@CBasis
// written by umeici. 2004
// 

#ifndef	BASISH
#define	BASISH

//----

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include <vector>

#include "aya5.h"
#include "globaldef.h"

class CValue;
class CAyaVM;

class   CDic1
{
public:
	aya::string_t path;
	char charset;

	CDic1(const aya::string_t &p,char c) : path(p), charset(c) {
	}
};

class   CBasisFuncPos
{
private:
	bool is_try_find;
	int pos_saved;

public:
	CBasisFuncPos() : is_try_find(false) , pos_saved(-1) { }
	~CBasisFuncPos() { }

	void Init() { is_try_find = false; pos_saved = -1; }

	bool IsNotFound() { return is_try_find && (pos_saved == -1); }

	int Find(CAyaVM &vm,const aya::char_t *name);
};

class	CBasis
{
public:
	CAyaVM &vm;

	aya::string_t	load_path;					// ��ʃ��W���[������load���ɓn�����p�X

protected:
	char	dic_charset;				// �����R�[�h�Z�b�g�iShift_JIS/UTF-8/OS�f�t�H���g�j
	char	output_charset;
	char	file_charset;
	char	save_charset;
	char    save_old_charset;
	char	extension_charset;
	bool	encode_savefile;
	bool	auto_save;

#if !defined(WIN32)
	typedef void* HWND;
#endif
	HWND	hlogrcvWnd;				// �`�F�b�N�c�[����hWnd

	aya::string_t	logpath;				// ���O�t�@�C���̃p�X
	aya::string_t	modulename;				// ���W���[���̎�t�@�C�����i�Ⴆ��a.dll�̏ꍇ��"a"�j
	aya::string_t  config_file_name_trailer;  // ��ݒ�t�@�C�����������̃g���C���[(�ʏ�͋�)
	aya::string_t	modename;				// ���݂̃��[�h (emergency or normal)
	char	suppress;				// ��������}�~�i�\����͂ŃG���[���N�����ۂɓ�����}�X�N���邽�߂Ɏg�p���܂��j
	char	checkparser;			// �\����͌��ʂ̃��O�ւ̋L�^���w������t���O
	char	iolog;					// ���o�͂̃��O�ւ̋L�^���w������t���O

	CBasisFuncPos loadindex;				// �֐� load �̈ʒu
	CBasisFuncPos unloadindex;				// �֐� unload �̈ʒu
	CBasisFuncPos requestindex;				// �֐� request �̈ʒu

	char	run;					// load������0��1��

private:
	CBasis(void);

public:
	CBasis(CAyaVM &vmr);
	~CBasis(void) {}

	char	IsRun(void) { return run; }

	void	SetModuleName(const aya::string_t &s,const aya::char_t *trailer,const aya::char_t *mode);
#if defined(WIN32) || defined(_WIN32_WCE)
	void	SetLogRcvWnd(long hwnd);
#endif
	void	SetPath(aya::global_t h, int len);

	void	Configure(void);
	void	Termination(void);
	
#if defined(WIN32)
	HWND	GetLogHWnd(void) { return hlogrcvWnd; }
#endif
	const aya::string_t& GetLogPath(void) const  { return logpath; }
	char	GetDicCharset(void)  { return dic_charset; }
	const aya::string_t& GetRootPath(void) const { return load_path;    }
	aya::string_t GetSavefilePath(void) const { return load_path + modulename + L"_variable.cfg"; }
	const aya::char_t* GetModeName(void) const { return modename.c_str(); }

	void	ExecuteLoad(void);
	aya::global_t	ExecuteRequest(aya::global_t h, long *len, bool is_debug);
	void	ExecuteUnload(void);

	void	SaveVariable(const aya::char_t* pName = NULL);
	void	RestoreVariable(const aya::char_t* pName = NULL);

	aya::string_t GetParameter(const aya::string_t &cmd);
	bool SetParameter(const aya::string_t &cmd, const aya::string_t &param, std::vector<CDic1> *dics = NULL);
	void SetLogger(void);

	void    SetOutputCharset(char cst) { output_charset = cst; }

	char	IsSuppress(void);

protected:
	void	SetSuppress(void);
	void	ResetSuppress(void);

	void	LoadBaseConfigureFile(std::vector<CDic1> &dics);

	void	RestoreArrayVariable(CValue &var, aya::string_t &value);
};

//----

#endif
