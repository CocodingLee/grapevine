#ifndef SRC_UTILITY_XPARAM_XML_XUTILS_H_
#define SRC_UTILITY_XPARAM_XML_XUTILS_H_

#include "utility/xparam/xml/xstr.h"
#include "utility/xparam/xml/xchar.h"
#include <assert.h>

using namespace xstr;
using namespace xchar;

inline static size_t base64_encode_len(size_t slen)
{
    assert(slen < INT_MAX);
    return (slen + 2) / 3 * 4;
}

inline static size_t base64_decode_len(size_t slen)
{
    assert(slen < INT_MAX);
    return (slen + 3) / 4 * 3;
}

template <typename CharType>
inline static bool Base64Encode(void const* s
                                , size_t slen
                                , CharType* d
                                , size_t& dlen)
{
    // �ַ�����ΪҪ���ݿ�խ�����ַ���������ַ���ascii��������д�����顣
    static const CharType s_table[64] = {
        65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
        81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102,
        103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
        119, 120, 121, 122, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 43, 47};
    
    // ������
    size_t reqlen = base64_encode_len(slen);
    if (!s || !slen || !d || dlen < reqlen)
    {
        assert(false);
        return false;
    }
    dlen = reqlen;
    
    // ѭ����������
    unsigned char const* p = reinterpret_cast<unsigned char const*>(s);
    unsigned char const* end = p + slen;
    for (; p + 2 < end; p += 3, d += 4)
    {
        d[0] = s_table[p[0] >> 2];
        d[1] = s_table[(p[0] << 6 >> 2 | p[1] >> 4) & 0x3F];
        d[2] = s_table[(p[1] << 4 >> 2 | p[2] >> 6) & 0x3F];
        d[3] = s_table[p[2] & 0x3F];
    }
    
    // ����ĩβ����3���ֽڵ����
    if (p < end)
    {
        size_t rest = end - p;
        d[0] = s_table[p[0] >> 2];
        if (rest == 1)
        {
            d[1] = s_table[p[0] << 6 >> 2];
            d[2] = 61; // '='
        }
        else
        {
            d[1] = s_table[p[0] << 6 >> 2 | p[1] >> 4];
            d[2] = s_table[p[1] << 4 >> 2];
        }
        d[3] = 61; // '='
    }
    return true;
}

template <typename CharType>
inline static unsigned char DecodeBase64Char(CharType ch)
{
    if (ch >= 65 && ch <= 90) // 'A' to 'Z'
        return ch - 65;
    if (ch >= 97 && ch <= 122) // 'a' to 'z'
        return ch - 97 + 26;
    if (ch >= 48 && ch <= 57) // '0' to '9'
        return ch - 48 + 52;
    if (ch == 43)
        return 62;
    if (ch == 47)
        return 63;
    if (ch == 9 || ch == 32 || ch == 10 || ch ==13)
        return 64; // ignored
    return 65; // invalid;
}

/*
 state:
 st_start, st_group0, st_group1, st_group2, st_group3, st_failed, st_succeed, st_finish
 event:
 ev_valid_char, ev_ignored_char, ev_invalid_char, ev_nomore_char
 transition:
 tr_group0_group1, tr_group1_group2, tr_group2_group3, tr_group3_group0, tr_failed, tr_succeed, tr_tail1, tr_tail2
 state_machine:
 (st_start, , st_group0, )
 
 (st_group0, ev_vliad_char, st_group1, tr_group0_group1)
 (st_group0, ev_ignored_char, st_group0, )
 (st_group0, ev_invalid_char, st_failed, tr_failed)
 (st_group0, ev_nomore_char, st_succeed, tr_succeed)
 
 (st_group1, ev_vliad_char, st_group2, tr_group1_group2)
 (st_group1, ev_ignored_char, st_group1, )
 (st_group1, ev_invalid_char, st_failed, tr_failed)
 (st_group1, ev_nomore_char, st_failed, tr_failed)
 
 (st_group2, ev_vliad_char, st_group3, tr_group2_group3)
 (st_group2, ev_ignored_char, st_group2, )
 (st_group2, ev_invalid_char, st_failed, tr_failed)
 (st_group2, ev_nomore_char, st_succeed, tr_tail1)
 
 (st_group3, ev_vliad_char, st_group0, tr_group3_group0)
 (st_group3, ev_ignored_char, st_group3, )
 (st_group3, ev_invalid_char, st_failed, tr_failed)
 (st_group3, ev_nomore_char, st_succeed, tr_tail2)
 
 (st_succeed, , st_finish, )
 (st_failed, , st_finish, )
 */
template <typename CharType>
inline static bool base64_decode(CharType const* s, size_t slen, void* d, size_t& dlen)
{
    // ������
    xstring<wchar_t>::normalize_size(s, slen);
    size_t reqlen = base64_decode_len(slen);
    if (!s || !slen || !d || dlen < reqlen)
        return false;
    
    // ȥ��ĩβ��pad�ַ�(=)
    if (slen > 0 && s[slen - 1] == 61)
        --slen;
    if (slen > 1 && s[slen - 2] == 61)
        --slen;
    
    CharType const* end = s + slen;
    unsigned char* current_p = reinterpret_cast<unsigned char*>(d);
    
    // ����״̬
    enum state{st_group0, st_group1, st_group2, st_group3};
    state current_state = st_group0;
    
    // ״̬�����п�ʼ
    while (s < end)
    {
        unsigned char input = DecodeBase64Char(*s++);
        
        // ��Ч�ַ��¼�
        if (input == 65)
            return false;
        
        // �����ַ�(�հ��ַ�)�¼�
        if (input == 64){continue;}
        
        // ��Ч�ַ��¼�
        switch (current_state)
        {
            case st_group0:
                current_state = st_group1;
                *current_p = input << 2;
                break;
            case st_group1:
                current_state = st_group2;
                *current_p += input >> 4;
                *(++current_p) = input << 4;
                break;
            case st_group2:
                current_state = st_group3;
                *current_p += input >> 2;
                *(++current_p) = input << 6;
                break;
            case st_group3:
                current_state = st_group0;
                *current_p += input;
                ++current_p;
                break;
            default:
                break;
        }
    }
    // û�и����ַ��¼�
    switch (current_state)
    {
        case st_group0:
            break;
        case st_group1:
            return false;
        case st_group2:
            --current_p;
            break;
        case st_group3:
            --current_p;
            break;
        default:
            break;
    }
    // ״̬������
    
    dlen = current_p - reinterpret_cast<unsigned char*>(d);
    return true;
}

//////////////////////////////////////////////////////////////////////////
class XmlSrcReaderImpl
{
public:
    XmlSrcReaderImpl()
    {
        reset();
    }
    inline void reset()
    {
        m_begin = NULL;
        m_p = NULL;
        m_end = NULL;
        m_row = 1;
        m_col = 0;
        m_offset = 0;
        m_last_char = 0;
    }
    inline void feed(const wchar_t* s, size_t len)
    {
        m_offset += m_p - m_begin;
        xstring<wchar_t>::normalize_size(s, len);
        m_begin = s;
        m_p = s;
        m_end = s + len;
    }
    inline wchar_t read()
    {
        wchar_t c = *(m_p++);
        // ����س�����
        if ( m_last_char == 10 || (m_last_char == 13 && c != 10) )
        {
            m_col = 1;
            ++m_row;
        }
        else
            ++m_col;
        m_last_char = c;
        return c;
    }
    inline bool can_read() const
    {
        return m_p < m_end;
    }
    inline size_t row() const
    {
        return m_row;
    }
    inline size_t col() const
    {
        return m_col;
    }
    inline size_t offset() const
    {
        return m_p - m_begin + m_offset;
    }
private:
    size_t m_row;
    size_t m_col;
    size_t m_offset;
    const wchar_t* m_begin;
    const wchar_t* m_p;
    const wchar_t* m_end;
    wchar_t m_last_char;
};

//////////////////////////////////////////////////////////////////////////
namespace detail
{
    static const int entity_count = 5;
    static const wchar_t EntitiesStr[entity_count][8] = {L"&amp;", L"&lt;", L"&gt;", L"&quot;", L"&apos;"};
    static const size_t EntitiesLen[entity_count] = {5, 4, 4, 6, 6};
    static const wchar_t EntitiesChr[entity_count] = {L'&', L'<', L'>', L'"', L'\''};
    static const wchar_t HexTable[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102};
}

class XmlEscaperImpl
{
public:
    XmlEscaperImpl()
    : m_ignore_crlf(true)
    {
    }
    static void escape(wchar_t c, xstring<wchar_t>& str)
    {
        using namespace detail;
        if (c > 62)
            str.append(c);
        else if (c == L'&')
            str.append(L"&amp;", 5);
        else if (c == L'<')
            str.append(L"&lt;", 4);
        else if (c == L'>')
            str.append(L"&gt;", 4);
        else if (c == L'"')
            str.append(L"&quot;", 6);
        else if (c == L'\'')
            str.append(L"&apos;", 6);
        else if (c < 32 && (!chartype_space(c)))
        {
            wchar_t num[6] = {38, 35, 120, HexTable[c >> 4], HexTable[c & 0xF], 59}; /*'&#xdd;'*/
            str.append(num, 6);
        }
        else
            str.append(c);
    }
    static void escapeStr(const xstring<wchar_t>&src, xstring<wchar_t>& str)
    {
        size_t len = src.length();
        for (size_t i = 0; i < len; ++i)
            escape(src[i], str);
    }
    static void escapeStr(wchar_t const* s, size_t slen, xstring<wchar_t>& str)
    {
        xstring<wchar_t>::normalize_size(s, slen);
        for (wchar_t const* end = s + slen; s < end; ++s)
            escape(*s, str);
    }
    bool unescape(wchar_t c, xstring<wchar_t>& str)
    {
        using namespace detail;
        bool result = true;
        if (m_data.empty())
        {
            if (c != 38/*'&'*/)
            {
                if (!chartype_crlf(c) || !m_ignore_crlf)
                    str.append(c);
            }
            else
                m_data.append(c);
        }
        else
        {
            m_data.append(c);
            if (c == 59/*';'*/) //finished
            {
                if (m_data.length() > 2 && m_data[1] == 35/*'#'*/)	//number
                {
                    wchar_t wchr;
                    if (str2wchr(m_data, wchr))
                        str.append(wchr);
                    else
                        result = false;
                    
                }
                else
                {
                    int i = 0;
                    while (i < entity_count && (!m_data.equal(EntitiesStr[i], EntitiesLen[i])))
                        i++;
                    if (i < entity_count)
                        str.append(EntitiesChr[i]);
                    else
                        result = false;
                }
                m_data.clear();
            }
        }
        return result;
    }
    inline void reset()
    {
        m_data.clear();
    }
    inline void reset(bool ingore_crlf)
    {
        m_data.clear();
        m_ignore_crlf = ingore_crlf;
    }
    inline bool empty() const
    {
        return m_data.empty();
    }
private:
    xstring<wchar_t> m_data;
    bool str2wchr(const xstring<wchar_t>& str, wchar_t& chr)
    {
        size_t len = str.length() - 1;
        chr = 0;
        if (str[2] == 120/*'x'*/)
        {
            for (size_t i = 3; i < len; ++i)
            {
                chr <<= 4;
                wchar_t c = str[i];
                if (c >= 48/*'0'*/ && c <= 57/*'9')*/)
                    chr += (c - 48/*'0'*/);
                else if (c >= 65/*'A'*/ && c <= 70/*'F'*/)
                    chr += (c - 65/*'A'*/ + 10);
                else if (c >= 97/*'a'*/ && c <= 102/*'f'*/)
                    chr += (c - 97/*'a'*/ + 10);
                else
                    return false;
            }
        }
        else
        {
            for (size_t i = 2; i < len; ++i)
            {
                chr *= 10;
                wchar_t c = str[i];
                if (c >= 48/*'0'*/ && c <= 57/*'9'*/)
                    chr += (c - 48/*'0'*/);
                else
                    return false;
            }
        }
        return true;
    }
    bool m_ignore_crlf;
};

//////////////////////////////////////////////////////////////////////////
template <size_t size> struct typer;
template <> struct typer<4> {public :typedef unsigned long type;};
template <> struct typer<8> {public :typedef unsigned long long type;};
typedef typer<sizeof(void*)>::type type;

struct ptr_int
{
    static type& cast(void*& p) {return *(type*)(&p);}
    static type& cast(void const*& p) {return *(type*)(&p);}
    static type& cast(void* const& p) {return *(type*)(&p);}
    static void*& cast(type& i){return *(void**)(&i);};
    static void*& cast(type const& i){return *(void**)(&i);};
};

#endif  // SRC_UTILITY_XPARAM_XML_XUTILS_H_
