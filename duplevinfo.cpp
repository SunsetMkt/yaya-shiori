// 
// AYA version 5
//
// �d����𐧌���s�Ȃ��N���X�@CDuplEvInfo
// - �又����
// written by umeici. 2004
// 

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include <vector>
#include <functional>

#include "selecter.h"

#include "log.h"
#include "globaldef.h"
#include "sysfunc.h"
#include "ayavm.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

/* -----------------------------------------------------------------------
 *  �֐���  �F  CDuplEvInfo::Choice
 *  �@�\�T�v�F  ��₩��I�����ďo�͂��܂�
 * -----------------------------------------------------------------------
 */
CValue	CDuplEvInfo::Choice(CAyaVM &vm,int areanum, const std::vector<CVecValue> &values, int mode)
{
	// �̈斈�̌�␔�Ƒ������X�V�@�ω����������ꍇ�͏��񏇏�������������
	if ( UpdateNums(areanum, values) ) {
		lastroundorder = -1;
		InitRoundOrder(vm,mode);
	}

	// �l�̎擾�Ə��񐧌�
	CValue	result = GetValue(vm, areanum, values);

	lastroundorder = roundorder[index];

	// ����ʒu��i�߂�@���񂪊��������珄�񏇏�������������
	index++;
	if ( index >= static_cast<int>(roundorder.size()) ) {
		InitRoundOrder(vm,mode);
	}

	return result;
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CDuplEvInfo::InitRoundOrder
 *  �@�\�T�v�F  ���񏇏������������܂�
 * -----------------------------------------------------------------------
 */
void	CDuplEvInfo::InitRoundOrder(CAyaVM &vm,int mode_param)
{
	// ������
	index = 0;
	roundorder.clear();
	roundorder.reserve(total);

	int mode = mode_param & CHOICETYPE_SELECT_FILTER;

    if ( mode == CHOICETYPE_NONOVERLAP_FLAG ) {
		for(size_t i = 0; i < total; ++i) {
			if ( i != lastroundorder ) {
				roundorder.emplace_back(i);
			}
		}

		//�ً}���G���[���p
		if ( ! roundorder.size() ) {
			roundorder.emplace_back(0);
		}

		//�V���b�t������
		size_t n = roundorder.size();
		if ( n >= 2 ) {
			for (size_t i = 0 ; i < n ; ++i ) {
				size_t s = vm.genrand_int(n);
				if ( i != s ) {
					int tmp = roundorder[i];
					roundorder[i] = roundorder[s];
					roundorder[s] = tmp;
				}
			}
		}

		//lastroundorder�� i = 1 �ȍ~ (2�ڈȍ~) �̃����_���Ȉʒu�ɍ�������
		if ( lastroundorder >= 0 ) {
			if ( n >= 2 ) {
				size_t lrand = vm.genrand_int(n) + 1;
				if ( lrand == n ) {
					roundorder.emplace_back(lastroundorder);
				}
				else {
					roundorder.insert(roundorder.begin() + lrand,lastroundorder);
				}
			}
			else {
				roundorder.emplace_back(lastroundorder);
			}
		}
	}
	else {
		for(size_t i = 0; i < total; ++i) {
			roundorder.emplace_back(i);
		}
	}
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CDuplEvInfo::UpdateNums
 *  �@�\�T�v�F  �̈斈�̌�␔�Ƒ������X�V���܂�
 *  �Ԓl�@�@�@  0/1=�ω��Ȃ�/����
 * -----------------------------------------------------------------------
 */
bool	CDuplEvInfo::UpdateNums(int areanum, const std::vector<CVecValue> &values)
{
	// ���̌�␔��ۑ����Ă���
	int	bef_numlenm1 = num.size() - 1;

	// �̈斈�̌�␔�Ƒg�ݍ��킹�������X�V
	// ��␔�ɕω����������ꍇ�̓t���O�ɋL�^����
	bool changed = areanum != bef_numlenm1;
	if ( changed ) {
		num.resize(areanum+1);
	}
	total = 1;

	for(int i = 0; i <= areanum; i++) {
		int	t_num = values[i].array.size();

		if (num[i] != t_num) {
			changed = true;
		}

		total *= t_num;
		num[i] = t_num;
	}

	return changed;
}

bool	CDuplEvInfo::UpdateNums(const CValue& value)
{
	bool changed = false;

	if(num.size()!=1) {
		num.resize(1);
		changed = true;
	}

	if (num[0] != value.array().size()) {
		changed = true;
		total = num[0] = value.array().size();
	}

	return changed;
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CDuplEvInfo::GetValue
 *  �@�\�T�v�F  ���݂̏���ʒu����l���擾���܂�
 *
 *  �i�[�̈悪������Ȃ��ꍇ�͂�������̂܂܏o���̂Œl�̌^���ی삳��܂��B
 *  �̈悪��������ꍇ�͂����͕�����Ƃ��Č�������܂��̂ŁA������^�ł̏o�͂ƂȂ�܂��B
 * -----------------------------------------------------------------------
 */
CValue	CDuplEvInfo::GetValue(CAyaVM &vm,int areanum, const std::vector<CVecValue> &values)
{
	int	t_index = roundorder[index];

	vm.sysfunction().SetLso(t_index);

	if (areanum) {
		yaya::string_t	result;
		for ( int i = 0; i <= areanum; i++ ) {
			if ( num[i] ) {
				int	next = t_index/num[i];
				result += values[i].array[t_index - next*(num[i])].GetValueString();
				t_index = next;
			}
		}
		return CValue(result);
	}
	else
		return values[0].array[t_index];
}
