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
#define CHOICETYPE_MELT_FLAG         0x0400U
#define CHOICETYPE_ALL_FLAG          0x1000U
#define CHOICETYPE_LAST_FLAG         0x2000U
#define CHOICETYPE_VOID_FLAG         0x4000U

#define CHOICETYPE_SELECT_FILTER     0x00FFU
#define CHOICETYPE_OUTPUT_FILTER     0xFF00U
#define CHOICETYPE_SPECOUT_FILTER    0xF000U

typedef enum choicetype_t {
	CHOICETYPE_VOID             = CHOICETYPE_VOID_FLAG,									/* �o�͂Ȃ� */
	CHOICETYPE_ALL				= CHOICETYPE_ALL_FLAG,									/* Sum all outputs as strings */
	CHOICETYPE_LAST				= CHOICETYPE_LAST_FLAG,									/* return last var only */
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

	std::vector<size_t>	num;			// --�ŋ�؂�ꂽ�̈斈�̌�␔
	std::vector<size_t>	roundorder;		// ���񏇏�

	ptrdiff_t	lastroundorder; // ���O�̏��񏇏��l
	size_t	total;			// �o�͌��l�̑���
	size_t	index;			// ���݂̏���ʒu

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

	CValue	Choice(CAyaVM &vm, size_t areanum, const std::vector<CVecValue> &values, int mode);

protected:
	void	InitRoundOrder(CAyaVM &vm,int mode);
	bool	UpdateNums(size_t areanum, const std::vector<CVecValue> &values);
	bool	UpdateNums(const CValue& value);
	CValue	GetValue(CAyaVM &vm, size_t areanum, const std::vector<CVecValue> &values);
};

//----

class CSelecter
{
protected:
	CAyaVM &vm;
	std::vector<CVecValue>	values;			// �o�͌��l
	size_t					areanum;		// �o�͌���~�ς���̈�̐�
	CDuplEvInfo				*duplctl;		// �Ή�����d��������ւ̃|�C���^
	size_t					aindex;			// switch�\���Ŏg�p

	friend class CFunction;//for pool
private:
	CSelecter(void);
public:
	CSelecter(CAyaVM& vmr, CDuplEvInfo* dc, size_t aid);

	void	AddArea(void);
	void	Append(const CValue &value);
	CValue	Output(void);

	static choicetype_t			GetDefaultBlockChoicetype(choicetype_t nowtype);
	static choicetype_t			StringToChoiceType(const yaya::string_t& ctypestr, CAyaVM &vm, const yaya::string_t& dicfilename, size_t linecount);
	static const yaya::char_t*	ChoiceTypeToString(choicetype_t ctype);

protected:
	CValue	StructArray1(size_t index);
	CValue	StructArray(void);
	CValue	StructString1(size_t index);
	CValue	StructString(void);
	CValue	ChoiceRandom(void);
	CValue	ChoiceRandom1(size_t index);
	CValue	ChoiceByIndex(void);
	CValue	ChoiceByIndex1(size_t index);
};

//----

#endif
