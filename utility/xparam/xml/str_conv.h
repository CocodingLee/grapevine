/**
 *	@file	str_conv.h
 *	@date	2006-7-15	0:01:53
 *	@author	shaoyoushi
 *	@brief	�ṩһЩ�ַ������������͵�ת��������������C��׼��Ľӿڻ���Ч��
 */
#ifndef __STR_CONV_H_C98AC0D7_6DBF_4a59_81CB_345963291831__
#define __STR_CONV_H_C98AC0D7_6DBF_4a59_81CB_345963291831__
#include <math.h>
#include <wchar.h>
#include <stdlib.h>

namespace tcfl{


/**
 *	@fn			easy_atoi
 *	@brief		���ַ���ת�����ͣ�֧�ָ����ַ����ͺ�����
 *	@param		s	Դ�ַ�����ʼλ�ã����end < s�������0������ 
 *	@param		i	Ŀ�����ͣ��ַ���s��ת���ɹ��󣬴浽i�У����ת��ʧ�ܣ����޸�i
 *	@param		end	Դ�ַ�������λ�ã����end < s�������0������
 *	@return		�Ƿ�ɹ�������ַ�������һ���Ϸ����������򷵻�false���Ҳ��޸Ĳ���i��ֵ�����򷵻�true������ת����Ľ������i
 *
 *				�������֧�ָ����ַ����ͺ����ͣ��Ϸ���������ʽ��ָ��c���Ե����͸�ʽ��֧��10���ƺ�16���ƣ�����ǰ�����
 *				���������ţ�16������x����0x���߸����0����һ��x��ʼ��x���Ա�$���(pascal)���ַ����г��ֵ��κοո񶼻ᱻ�򵥺��ԡ�
 *				���׼����ȣ�������ʹ��ģ��֧�ֶ������ͣ���֧�ַ��سɹ�����ʧ�ܣ����Ҿ������ԣ������ܴ�Լ�Ǳ�׼���atoi
 *				�㷨��3����
 *  @exception	sss
 */
template <typename CharType, typename IntType>
bool easy_atoi(const CharType* s, IntType& i, const CharType* end = NULL)
{
	IntType result = 0;

	if (s == NULL || s == end || *s == 0)
		return false;

	bool positive = true;
	IntType base = 10;

	const CharType* p = s;
	CharType c;
	bool succeed = false;
	do
	{
		c = *p;
		if (c >= 0x30 && c <= 0x39) // numbers
		{
			result = result * base + (c - 0x30);
		}
		else if(c >= 0x61 && c <= 0x66) // a - f
		{
			if (base != 16)
				break;
			result = result * base + (c - (0x61 - 0xa));
		}
		else if (c >= 0x41 && c <= 0x46) // A - F
		{
			if (base != 16)
				break;
			result = result * base + (c - (0x41 - 0xA));
		}
		else if (c == 0x58 || c == 0x78 || c == 0x24) // X or x or $
		{
			if (result || base != 10)
				break;
			base = 16;
		}
		else if (c == 0x20 || c == 0x9) // space or tab
			continue;
		else if (c == 0)
		{
			succeed = true;
			break;
		}
		else if (c == 0x2d) // minus
		{
			if (result)
				break;
			positive = false;
		}
		else if (c == 0x2b) // plus
		{
			if (result || !positive)
				break;
		}
		else
			break;
	}
	while (++p != end);

	if (succeed || p == end)
	{
		i = positive ? result : 0 - result;
		return true;
	}
	return false;
}


/**
*	@fn			easy_atof
*	@brief		���ַ���ת�ɸ����ͣ�֧�ָ����ַ����ͺ͸�����
*	@param		s	Դ�ַ�����ʼλ�ã����end < s�������0������
*	@param		f	Ŀ�긡���ͣ��ַ���s��ת���ɹ��󣬴浽f�У����ת��ʧ�ܣ����޸�f
*	@param		end	Դ�ַ�������λ�ã����end < s�������0������
*	@return		�Ƿ�ɹ�������ַ�������һ���Ϸ��ĸ��������򷵻�false���Ҳ��޸Ĳ���f��ֵ�����򷵻�true������ת����Ľ������f
*
*				�������֧�ָ����ַ����ͺ͸����ͣ��Ϸ��ĸ����ʽ��ָ��c���Եĸ����͸�ʽ������ǰ�����
*				���������ţ��ַ����г��ֵ��κοո񶼻ᱻ�򵥺��ԡ�
*				���׼����ȣ�������ʹ��ģ��֧�ֶ������ͣ���֧�ַ��سɹ�����ʧ�ܣ�����֧�ֿ�ѧ��������ʽ��
*				ֻ��ʶ��ͨ����ʽ�ĸ������֣��������ԣ������ܴ�Լ�Ǳ�׼���atof�㷨��6����
*  @exception	sss
*/

template <typename CharType, typename FloatType>
bool easy_atof(const CharType* s, FloatType& f, const CharType* end = NULL)
{
	FloatType result = 0;

	if (s == NULL || s == end || *s == 0)
		return false;

	bool positive = true;
	bool before_dot = true;
	bool succeed = false;
	FloatType base = 10.0;

	const CharType* p = s;
	CharType c;
	do
	{
		c = *p;
		if (c >= 0x30 && c <= 0x39) // numbers
		{
			if (before_dot)
				result = result * base + (c - 0x30);
			else
				result += (base /= 10.0) * (c - 0x30);
		}
		else if (c == 0x20 || c == 0x9) // space or tab
			continue;
		else if (c == 0x2e) // dot
		{
			if (!before_dot)
				break;
			before_dot = false;
			base = 1.0;
		}
		else if (c == 0)
		{
			succeed = true;
			break;
		}
		else if (c == 0x2d) // minus
		{
			if (result > 0 || result < 0)
				break;
			positive = false;
		}
		else if (c == 0x2b) // plus
		{
			if (result > 0 || result < 0 || !positive)
				break;
		}
		else
			break;
	} while (++p != end);

	if (succeed || p == end)
	{
		f = positive ? result : -result;
		return true;
	}
	return false;
}


/**
*	@fn			easy_itoa
*	@brief		������ת���ַ�����֧�ָ����ַ����ͺ�����
*	@param		i	Դ����
*	@param		s	Ŀ���ַ�����i��ת���ɹ��󣬴浽�ַ���s�У����ת��ʧ�ܣ����޸�s�������������s���0������
*	@param		len �ַ����ڴ�Ĵ�С����λ���ַ�����
*	@param		base	������ʹ��10���ƻ���16���ƽ���ת����Ŀǰֻ֧��10��16����ֵ
*	@return		����iת����s������Ҫ���ڴ��С��������0���������������ֵ���ڲ���len��˵����һЩ����û�д���s�У�
*				����������£��洢���µĸ�λ���ֻᱻ�������������Ϊ0�����ʾ�д�������ͨ���ǲ�������ȷ��
*
*				�������֧�ָ����ַ����ͺ����ͣ���֧��ת�ɴ������ŵ�16���Ƹ�ʽ��
*				���׼����ȣ�������ʹ��ģ��֧�ֶ������ͣ���֧�ַ�������Ҫ�ռ䣬�����ܺͱ�׼���itoa�㷨������ͬ��
*/
template <typename CharType, typename IntType>
size_t easy_itoa(IntType i, CharType* s, size_t len, unsigned int base)
{
	if ((base != 10 && base != 16) || (i < 0 && base == 16))
		return 0;

	if (s == NULL)
		len = 0;

	size_t result = 0;
	IntType val;

	// if negative
	if (i < 0)
	{
		val = 0 - i;
		if (len > 1)
		{
			*s++ = 0x2d; // minus
			--len;
		}
		++result;
	}
	else
		val = i;
	IntType digival;
	CharType* p = s;
	CharType* end = s + len;

	do
	{
		++ result;
		digival = val % base;
		val /= base;
		if (p != end)
		{
			CharType digiChar = static_cast<CharType>
				(digival > 9 ? digival + (0x61 - 0xa) : digival + 0x30);
			*p++ = digiChar;
		}
	} while (val);

	// reverse string
	size_t real_len = p - s;  // exact length
	CharType* mid = s + real_len / 2;
	CharType* left = s;
	CharType* right = p - 1;
	while (left < mid)
	{
		CharType t = *left;
		*left = *right;
		*right = t;
		++left;
		--right;
	}
	return result;
}

/**
*	@fn			easy_ftoa
*	@brief		��������ת���ַ�����֧�ָ����ַ����ͺ͸�����
*	@param		f	Դ���������������ֺ�С�����־����ܳ���9λ��Ч���֣�С�����ֳ������ֽ������������������������������
*	@param		s	Ŀ���ַ�����i��ת���ɹ��󣬴浽�ַ���s�У����ת��ʧ�ܣ����޸�s�������������s���0������
*	@param		len �ַ����ڴ�Ĵ�С����λ���ַ�����
*	@return		����fת����s������Ҫ���ڴ��С��������0���������������ֵ���ڲ���len��˵����һЩ����û�д���s�С�
*
*				���׼����ȣ�������ʹ��ģ��֧�ֶ������ͣ���֧�ַ�������Ҫ�ռ䣬�������Ǳ�׼���ftoa�㷨��7����
*/
template <typename CharType, typename FloatType>
size_t easy_ftoa(FloatType f, CharType* s, size_t len)
{
	if (s == NULL)
		len = 0;

	FloatType fractional;
	FloatType integer;
    
	fractional = modfl((long double)f, (long double*)&integer);
    if (fractional < 0)
    {
		fractional = 0 - fractional;
    }
	
    size_t result = easy_itoa((long)integer, s, len, 10);
	s += result;

	const static long digi = 1000000000;
	long lfra = long(fractional * digi);
	if (lfra != 0)
	{
		if (len > result)
        {
			*s++ = 0x2e; // dot
        }
        
		++result;

		for (long _div = digi / 10; _div && lfra / _div == 0; _div /= 10)
		{
			if (len > result)
            {
				*s++ = 0x30; //'0'
            }
            
			++result;
		}
        
		while (lfra % 10 == 0)
        {
			lfra /= 10;
        }
        
		result += easy_itoa(lfra, s, len > result ? len - result : 0, 10);
	}
    
	return result;
}


//��ʵ�����Ǽ���ģ��������������...
inline int safe_wtoi(const wchar_t *str)
{
	if(str == NULL)
	{
		return 0;
	}
    
    wchar_t* end;
    size_t len = wcslen(str);
	return (int)wcstol(str , &end , (int)len);
}

inline long safe_wtol(const wchar_t *str)
{
	if(str == NULL)
	{
		return 0;
	}
    
    wchar_t* end;
    size_t len = wcslen(str);
    return wcstol(str , &end , (int)len);
}

inline int safe_atoi(const char *str)
{
	if(str == NULL)
	{
		return 0;
	}
	return atoi(str);
}

inline long safe_atol(const char *str)
{
	if(str == NULL)
	{
		return 0;
	}
	return atol(str);
}

}
#endif//__STR_CONV_H_C98AC0D7_6DBF_4a59_81CB_345963291831__
