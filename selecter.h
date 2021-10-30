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

#define	CHOICETYPE_RANDOM				0	/* ��ɖ���ׂɃ����_���i�f�t�H���g�j*/
#define	CHOICETYPE_NONOVERLAP			1	/* �����_�������ꏄ����܂ŏd���I�����Ȃ� */
#define	CHOICETYPE_SEQUENTIAL			2	/* ���ԂɑI������ */
#define	CHOICETYPE_VOID					3	/* �o�͂Ȃ� */
#define	CHOICETYPE_ARRAY				4	/* �ȈՔz��Ґ� */
#define	CHOICETYPE_POSSIBILITY_LIST		5	/* �S�I������z��Ƃ��ĕԂ� */
#define	CHOICETYPE_POOL					6	/* random�̃X�R�[�v������ */
#define	CHOICETYPE_POOL_ARRAY			7	/* array�̃X�R�[�v������ */
#define	CHOICETYPE_NONOVERLAP_POOL		8	/* nonoverlap�̃X�R�[�v������ */
#define	CHOICETYPE_SEQUENTIAL_POOL		9	/* sequential�̃X�R�[�v������ */

#define	CHOICETYPE_NUM			10

const wchar_t* const choicetype[CHOICETYPE_NUM] = {
	L"random",
	L"nonoverlap",
	L"sequential",
	L"void",
	L"array",
	L"possibility_list",
	L"pool",
	L"pool_array",
	L"nonoverlap_pool",
	L"sequential_pool",
};

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
	int				type;			// �I�����

	std::vector<int>	num;			// --�ŋ�؂�ꂽ�̈斈�̌�␔
	std::vector<int>	roundorder;		// ���񏇏�

	int				lastroundorder; // ���O�̏��񏇏��l
	int				total;			// �o�͌��l�̑���
	int				index;			// ���݂̏���ʒu

private:
	CDuplEvInfo(void);

public:
	CDuplEvInfo(int tp)
	{
		type = tp;
		total = 0;
		index = 0;
		lastroundorder = -1;
	}

	int		GetType(void) { return type; }

	CValue	Choice(CAyaVM &vm,int areanum, const std::vector<CVecValue> &values, int mode);
	CValue	ChoiceValue(CAyaVM& vm, CValue& value, int mode);

protected:
	void	InitRoundOrder(CAyaVM &vm,int mode);
	bool	UpdateNums(int areanum, const std::vector<CVecValue> &values);
	bool	UpdateNums(const CValue& value);
	CValue	GetValue(CAyaVM &vm,int areanum, const std::vector<CVecValue> &values);
	CValue	GetValue(CAyaVM& vm, const CValue& value);
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

private:
	CSelecter(void);
public:
	CSelecter(CAyaVM &vmr, CDuplEvInfo *dc, int aid);

	void	AddArea(void);
	void	Append(const CValue &value);
	CValue	Output(void);

protected:
	CValue	StructArray1(int index);
	CValue	StructPool(void);
	CValue	StructPossibilityList(void);
	CValue	StructArray(void);
	CValue	ChoiceRandom(void);
	CValue	ChoiceRandom1(int index);
	CValue	ChoiceByIndex(void);
	CValue	ChoiceByIndex1(int index);
};

//----

#endif
