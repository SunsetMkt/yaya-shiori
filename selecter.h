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

#define CHOICETYPE_RANDOM_FLAG       0x0001U
#define CHOICETYPE_SEQUENTIAL_FLAG   0x0002U
#define CHOICETYPE_NONOVERLAP_FLAG   0x0004U
#define CHOICETYPE_ARRAY_FLAG        0x0008U
#define CHOICETYPE_PICKONE_FLAG      0x0100U
#define CHOICETYPE_POOL_FLAG         0x0200U
#define CHOICETYPE_VOID_FLAG         0x0400U

#define CHOICETYPE_SELECT_FILTER     0x00FFU
#define CHOICETYPE_OUTPUT_FILTER     0xFF00U

typedef enum choicetype_t {
	CHOICETYPE_VOID             = CHOICETYPE_VOID_FLAG,									/* �o�͂Ȃ� */
	CHOICETYPE_RANDOM           = CHOICETYPE_PICKONE_FLAG | CHOICETYPE_RANDOM_FLAG,		/* ��ɖ���ׂɃ����_���i�f�t�H���g�j*/
	CHOICETYPE_NONOVERLAP       = CHOICETYPE_PICKONE_FLAG | CHOICETYPE_NONOVERLAP_FLAG,	/* �����_�������ꏄ����܂ŏd���I�����Ȃ� */
	CHOICETYPE_SEQUENTIAL       = CHOICETYPE_PICKONE_FLAG | CHOICETYPE_SEQUENTIAL_FLAG,	/* ���ԂɑI������ */
	CHOICETYPE_ARRAY            = CHOICETYPE_PICKONE_FLAG | CHOICETYPE_ARRAY_FLAG,		/* �ȈՔz��Ґ� */
	CHOICETYPE_POOL             = CHOICETYPE_POOL_FLAG    | CHOICETYPE_RANDOM_FLAG,		/* random�̃X�R�[�v������ */
	CHOICETYPE_POOL_ARRAY       = CHOICETYPE_POOL_FLAG    | CHOICETYPE_ARRAY_FLAG,		/* array�̃X�R�[�v������ : �S�I������z��Ƃ��ĕԂ� */
	CHOICETYPE_NONOVERLAP_POOL  = CHOICETYPE_POOL_FLAG    | CHOICETYPE_NONOVERLAP_FLAG,	/* nonoverlap�̃X�R�[�v������ */
	CHOICETYPE_SEQUENTIAL_POOL  = CHOICETYPE_POOL_FLAG    | CHOICETYPE_SEQUENTIAL_FLAG,	/* sequential�̃X�R�[�v������ */
} choicetype_t;

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

	static choicetype_t			GetDefaultBlockChoicetype(choicetype_t nowtype);
	static choicetype_t			StringToChoiceType(const yaya::string_t& ctypestr);
	static const yaya::char_t*	ChoiceTypeToString(choicetype_t ctype);

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
