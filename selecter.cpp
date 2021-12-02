// 
// AYA version 5
//
// �o�͂̑I�����s�Ȃ��N���X�@CSelecter
// - �又����
// written by umeici. 2004
// 

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include "selecter.h"

#include "globaldef.h"
#include "sysfunc.h"
#include "ayavm.h"
#include "wsex.h"
#include "messages.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

/* -----------------------------------------------------------------------
 * CSelecter�R���X�g���N�^
 * -----------------------------------------------------------------------
 */
CSelecter::CSelecter(CAyaVM &vmr, CDuplEvInfo *dc, size_t aid) : vm(vmr), duplctl(dc), aindex(aid)
{
	areanum = 0;

	CVecValue	addvv;
	values.emplace_back(addvv);
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::AddArea
 *  �@�\�T�v�F  �V�����o�͊i�[�p�̗̈��p�ӂ��܂�
 * -----------------------------------------------------------------------
 */
void	CSelecter::AddArea(void)
{
	// �ǉ��O�̗̈悪�󂾂����ꍇ�̓_�~�[�̋󕶎����ǉ�
	if (!values[areanum].array.size())
		Append(CValue());

	// �̈��ǉ�
	CVecValue	addvv;
	values.emplace_back(addvv);
	areanum++;
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::Append
 *  �@�\�T�v�F  ���l��ǉ����܂�
 * -----------------------------------------------------------------------
 */
void	CSelecter::Append(const CValue &value)
{
	//�󕶎���͂���ς�ǉ����Ȃ��Ƃ܂���
	//if (value.GetType() == F_TAG_STRING && !value.s_value.size())
	//	return;
	if (value.GetType() == F_TAG_VOID)
		return;

	values[areanum].array.emplace_back(value);
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::Output
 *  �@�\�T�v�F  �e�̈悩��l�𒊏o���ďo�͂��쐬���Ԃ��܂�
 *  �����@�@�F  duplctl �d��������ւ̃|�C���^
 *  �@�@�@�@�@          NULL�ɏꍇ�̓����_���I���œ��삵�܂�
 *
 *  �����_���I���͂��̃N���X�ŏ������܂��B
 *  nonoverlao/sequential�I����CDuplEvInfo�N���X�ɔC���܂��B
 * -----------------------------------------------------------------------
 */
CValue	CSelecter::Output()
{
	// switch�I��
	if (aindex >= BRACE_SWITCH_OUT_OF_RANGE) {
		vm.sysfunction().SetLso(aindex);
		return ChoiceByIndex();
	}

	// �̈悪1�����Ȃ��A����₪���݂��Ȃ��ꍇ�͏o�͂Ȃ�
	if (!areanum && !values[0].array.size()) {
		vm.sysfunction().SetLso(-1);
		return CValue(F_TAG_NOP, 0/*dmy*/);
	}

	// �Ō�̗̈悪�󂾂����ꍇ�̓_�~�[�̋󕶎����ǉ�
	if (!values[areanum].array.size())
		Append(CValue());

	// �����_���I��
	if (duplctl == NULL)
		return ChoiceRandom();

	// �d����𐧌�t���I��
	if (duplctl->GetType() == CHOICETYPE_VOID)
		return CValue(F_TAG_NOP, 0/*dmy*/);

	switch ( duplctl->GetType() & CHOICETYPE_SELECT_FILTER ) {
	case CHOICETYPE_NONOVERLAP_FLAG:
		return duplctl->Choice(vm, areanum, values, CHOICETYPE_NONOVERLAP_FLAG);
	case CHOICETYPE_SEQUENTIAL_FLAG:
		return duplctl->Choice(vm, areanum, values, CHOICETYPE_SEQUENTIAL_FLAG);
	case CHOICETYPE_ARRAY_FLAG:
		return StructArray();
	case CHOICETYPE_RANDOM_FLAG:
	default:
		return ChoiceRandom();
	};
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::ChoiceRandom
 *  �@�\�T�v�F  �e�̈悩�烉���_���ɒl�𒊏o���ďo�͂��쐬���Ԃ��܂�
 *
 *  �i�[�̈悪������Ȃ��ꍇ�͂�������̂܂܏o���̂Œl�̌^���ی삳��܂��B
 *  �̈悪��������ꍇ�͂����͕�����Ƃ��Č�������܂��̂ŁA������^�ł̏o�͂ƂȂ�܂��B
 * -----------------------------------------------------------------------
 */
CValue	CSelecter::ChoiceRandom(void)
{
	if (areanum) {
		yaya::string_t	result;
		for(size_t i = 0; i <= areanum; i++)
			result += ChoiceRandom1(i).GetValueString();
		return CValue(result);
	}
	else
		return ChoiceRandom1(0);
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::ChoiceRandom1
 *  �@�\�T�v�F  �w�肳�ꂽ�̈悩�烉���_���ɒl�𒊏o���܂�
 * -----------------------------------------------------------------------
 */
CValue	CSelecter::ChoiceRandom1(size_t index)
{
	if ( ! values[index].array.size() ) {
		return CValue();
	}

	size_t choice = vm.genrand_uint(values[index].array.size());

    vm.sysfunction().SetLso(choice);

    return values[index].array[choice];
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::ChoiceByIndex
 *  �@�\�T�v�F  �e�̈悩��w��ʒu�̒l�𒊏o���ďo�͂��쐬���Ԃ��܂�
 *
 *  �i�[�̈悪������Ȃ��ꍇ�͂�������̂܂܏o���̂Œl�̌^���ی삳��܂��B
 *  �̈悪��������ꍇ�͂����͕�����Ƃ��Č�������܂��̂ŁA������^�ł̏o�͂ƂȂ�܂��B
 *
 *  �w�����␔�����Ȃ��ꍇ�͋󕶎��񂪏o�͂���܂��B
 * -----------------------------------------------------------------------
 */
CValue	CSelecter::ChoiceByIndex()
{
	// �Ō�̗̈悪�󂾂����ꍇ�̓_�~�[�̋󕶎����ǉ�
	if (!values[areanum].array.size())
		Append(CValue());

	// �又��
	if (areanum) {
		yaya::string_t	result;
		for(size_t i = 0; i <= areanum; i++)
			result += ChoiceByIndex1(i).GetValueString();
		return CValue(result);
	}
	else
		return ChoiceByIndex1(0);
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::ChoiceByIndex
 *  �@�\�T�v�F  �w�肳�ꂽ�̈悩��w��ʒu�̒l�𒊏o���܂�
 * -----------------------------------------------------------------------
 */
CValue	CSelecter::ChoiceByIndex1(size_t index)
{
	size_t	num = values[index].array.size();

	if (!num)
		return CValue();

	return (aindex >= 0 && aindex < num) ? values[index].array[aindex] : CValue();
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::StructArray
 *  �@�\�T�v�F  �e�̈�̒l�����������ėp�z����쐬���Ԃ��܂�
 * -----------------------------------------------------------------------
 */
CValue CSelecter::StructArray()
{
	if (areanum) {
		CValue	result(F_TAG_ARRAY, 0/*dmy*/);
		for(size_t i = 0; i <= areanum; i++) {
			result = result + StructArray1(i);
//			result = result + StructArray1(i).GetValueString();
		}
		return result;
	}
	else {
		return StructArray1(0);
	}
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::StructArray1
 *  �@�\�T�v�F  �w�肳�ꂽ�̈�̒l�����������ėp�z����쐬���܂�
 * -----------------------------------------------------------------------
 */
CValue CSelecter::StructArray1(size_t index)
{
	CValue	result(F_TAG_ARRAY, 0/*dmy*/);

    for(size_t i = 0; i < values[index].array.size(); ++i) {
		const CValue &target = values[index].array[i];
		int	valtype = target.GetType();
		
		if (valtype == F_TAG_ARRAY) {
			result.array().insert(result.array().end(), target.array().begin(), target.array().end());
		}
		else {
			result.array().emplace_back(CValueSub(target));
		}
	}

    return result;
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::GetDefaultBlockChoicetype
 *  �@�\�T�v�F  { } �̏o�̓^�C�v���w��̎��̕W���^�C�v
 * -----------------------------------------------------------------------
 */
choicetype_t CSelecter::GetDefaultBlockChoicetype(choicetype_t nowtype)
{
	switch (nowtype) {
	case CHOICETYPE_RANDOM:
	case CHOICETYPE_NONOVERLAP:
	case CHOICETYPE_SEQUENTIAL:
		return CHOICETYPE_RANDOM;
	case CHOICETYPE_VOID:
		return CHOICETYPE_VOID;
	case CHOICETYPE_POOL:
	case CHOICETYPE_POOL_ARRAY:
	case CHOICETYPE_NONOVERLAP_POOL:
	case CHOICETYPE_SEQUENTIAL_POOL:
		return CHOICETYPE_POOL;
	case CHOICETYPE_ARRAY:
	default:
		return CHOICETYPE_ARRAY;
	}
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::StringToChoiceType
 *  �@�\�T�v�F  ������->choicetype_t
 * -----------------------------------------------------------------------
 */
choicetype_t CSelecter::StringToChoiceType(const yaya::string_t& ctypestr, CAyaVM &vm, const yaya::string_t& dicfilename, size_t linecount)
{
	unsigned int outtype = CHOICETYPE_PICKONE_FLAG;

	yaya::string_t checkstr = ctypestr;

	if ( checkstr.find(L"pool") != yaya::string_t::npos ) {
		outtype = CHOICETYPE_POOL_FLAG;
		yaya::ws_replace(checkstr,L"pool",L"");
	}
	else if ( checkstr.find(L"void") != yaya::string_t::npos ) {
		outtype = CHOICETYPE_VOID_FLAG;
		yaya::ws_replace(checkstr,L"void",L"");
	}

	unsigned int choicetype = 0;

	if ( outtype != CHOICETYPE_VOID_FLAG ) {
		choicetype = CHOICETYPE_RANDOM_FLAG;

		if ( checkstr.find(L"sequential") != yaya::string_t::npos ) {
			choicetype = CHOICETYPE_SEQUENTIAL_FLAG;
			yaya::ws_replace(checkstr,L"sequential",L"");
		}
		else if ( checkstr.find(L"array") != yaya::string_t::npos ) {
			choicetype = CHOICETYPE_ARRAY_FLAG;
			yaya::ws_replace(checkstr,L"array",L"");
		}
		else if ( checkstr.find(L"nonoverlap") != yaya::string_t::npos ) {
			choicetype = CHOICETYPE_NONOVERLAP_FLAG;
			yaya::ws_replace(checkstr,L"nonoverlap",L"");
		}
		else {
			yaya::ws_replace(checkstr,L"random",L"");
		}
	}

	yaya::ws_replace(checkstr,L"_",L"");

	if ( checkstr.size() > 0 ) {
		//�Ȃɂ��]���Ȃ��̂���������
		vm.logger().Error(E_E, 30, ctypestr, dicfilename, linecount);
	}

	return static_cast<choicetype_t>(outtype | choicetype);
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CSelecter::ChoiceTypeToString
 *  �@�\�T�v�F  choicetype_t->������
 * -----------------------------------------------------------------------
 */
const yaya::char_t* CSelecter::ChoiceTypeToString(choicetype_t ctype)
{
	switch ( ctype ) {
	case CHOICETYPE_RANDOM:
		return L"random";
	case CHOICETYPE_NONOVERLAP:
		return L"nonoverlap";
	case CHOICETYPE_SEQUENTIAL:
		return L"sequential";
	case CHOICETYPE_VOID:
		return L"void";
	case CHOICETYPE_ARRAY:
		return L"array";
	case CHOICETYPE_POOL:
		return L"pool";
	case CHOICETYPE_POOL_ARRAY:
		return L"array_pool";
	case CHOICETYPE_NONOVERLAP_POOL:
		return L"nonoverlap_pool";
	case CHOICETYPE_SEQUENTIAL_POOL:
		return L"sequential_pool";
	}

	return L"unknown";
}

