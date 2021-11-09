// 
// AYA version 5
//
// �o�͂̑I�����s�Ȃ��N���X�@CSelecter/CDuplEvInfo
// written by umeici. 2004
// 
// CSelecter�͏o�͂̑I�����s�Ȃ��܂��B
// CDuplEvInfo�͏d��������s�Ȃ��܂��B
//

#ifndef	SELECTERH
#define	SELECTERH

//----

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include <vector>

#include "cell.h"
#include "value.h"
#include "variable.h"

typedef enum choicetype_t {
	CHOICETYPE_RANDOM = 0,		/* ��ɖ���ׂɃ����_���i�f�t�H���g�j*/
	CHOICETYPE_NONOVERLAP,		/* �����_�������ꏄ����܂ŏd���I�����Ȃ� */
	CHOICETYPE_SEQUENTIAL,		/* ���ԂɑI������ */
	CHOICETYPE_VOID,			/* �o�͂Ȃ� */
	CHOICETYPE_ARRAY,			/* �ȈՔz��Ґ� */
	CHOICETYPE_POOL,			/* random�̃X�R�[�v������ */
	CHOICETYPE_POOL_ARRAY,		/* array�̃X�R�[�v������ : �S�I������z��Ƃ��ĕԂ� */
	CHOICETYPE_NONOVERLAP_POOL,	/* nonoverlap�̃X�R�[�v������ */
	CHOICETYPE_SEQUENTIAL_POOL,	/* sequential�̃X�R�[�v������ */
} choicetype_t;

const struct { yaya::char_t *name; choicetype_t type; } choicetype[] = {
	{ L"random", CHOICETYPE_RANDOM } ,
	{ L"nonoverlap", CHOICETYPE_NONOVERLAP } ,
	{ L"sequential", CHOICETYPE_SEQUENTIAL } ,
	{ L"void", CHOICETYPE_VOID } ,
	{ L"array", CHOICETYPE_ARRAY } ,
	{ L"pool", CHOICETYPE_POOL } ,
	{ L"pool_array", CHOICETYPE_POOL_ARRAY } , //human-error safety!
	{ L"array_pool", CHOICETYPE_POOL_ARRAY } ,
	{ L"pool_nonoverlap", CHOICETYPE_NONOVERLAP_POOL } , //human-error safety!
	{ L"nonoverlap_pool", CHOICETYPE_NONOVERLAP_POOL } ,
	{ L"pool_sequential",CHOICETYPE_SEQUENTIAL_POOL } , //human-error safety!
	{ L"sequential_pool",CHOICETYPE_SEQUENTIAL_POOL } ,
};


choicetype_t GetDefaultBlockChoicetype(choicetype_t nowtype);

class CAyaVM;

//----

class CVecValue
{
public:
	std::vector<CValue> array;
};

//----

class	CDuplEvInfo
{
protected:
	choicetype_t	type;			// �I�����

	std::vector<int>	num;			// --�ŋ�؂�ꂽ�̈斈�̌�␔
	std::vector<int>	roundorder;		// ���񏇏�

	int				lastroundorder; // ���O�̏��񏇏��l
	int				total;			// �o�͌��l�̑���
	int				index;			// ���݂̏���ʒu

private:
	CDuplEvInfo(void);

public:
	CDuplEvInfo(choicetype_t tp)
	{
		type = tp;
		total = 0;
		index = 0;
		lastroundorder = -1;
	}

	choicetype_t	GetType(void) { return type; }

	CValue	Choice(CAyaVM &vm,int areanum, const std::vector<CVecValue> &values, int mode);

protected:
	void	InitRoundOrder(CAyaVM &vm,int mode);
	bool	UpdateNums(int areanum, const std::vector<CVecValue> &values);
	bool	UpdateNums(const CValue& value);
	CValue	GetValue(CAyaVM &vm,int areanum, const std::vector<CVecValue> &values);
};

//----

class CSelecter
{
protected:
	CAyaVM &vm;
	std::vector<CVecValue>	values;			// �o�͌��l
	int					areanum;		// �o�͌���~�ς���̈�̐�
	CDuplEvInfo			*duplctl;		// �Ή�����d��������ւ̃|�C���^
	int					aindex;			// switch�\���Ŏg�p

	friend class CFunction;//for pool
private:
	CSelecter(void);
public:
	CSelecter(CAyaVM &vmr, CDuplEvInfo *dc, int aid);

	void	AddArea(void);
	void	Append(const CValue &value);
	CValue	Output(void);

protected:
	CValue	StructArray1(int index);
	CValue	StructArray(void);
	CValue	ChoiceRandom(void);
	CValue	ChoiceRandom1(int index);
	CValue	ChoiceByIndex(void);
	CValue	ChoiceByIndex1(int index);
};

//----

#endif
