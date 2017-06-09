#ifndef SRC_XPARAM_MODEL_MARCODEF_H_
#define SRC_XPARAM_MODEL_MARCODEF_H_

#ifdef WIN32

// win32
#ifdef XPARAM_EXPORTS
#define XPARAM_API __declspec(dllexport)
#else
#define XPARAM_API __declspec(dllimport)
#endif

#ifndef __IUnknown_INTERFACE_DEFINED__
struct IUnknown;
#endif
#ifndef __IDispatch_INTERFACE_DEFINED__
struct IDispatch;
#endif


#else

// macOS iOS
#define XPARAM_API


#endif

namespace xparam
{
    enum {ZERO_TERMINATOR = -1};
    
    enum X_VALUE_TYPE
    {
        xvt_empty = 0,
        xvt_beginof_basic = 0x10,
        xvt_char,
        xvt_uchar,
        xvt_short,
        xvt_ushort,
        xvt_long,
        xvt_ulong,
        xvt_longlong,
        xvt_ulonglong,
        xvt_int,
        xvt_uint,
        xvt_float,
        xvt_double,
        xvt_longdouble,
        xvt_bool,
        xvt_endof_basic = 0x2f,
        
        xvt_beginof_pointer = 0x30,
        xvt_pvoid,
        xvt_sp,
        xvt_iunknown,
        xvt_idispatch,
        xvt_endof_pointer = 0x4f,
        
        xvt_string,
        xvt_blob,
    };
    
    enum X_VALUE_NODE
    {
        xvn_text,
        xvn_cdata,
        xvn_attr
    };
    
    enum X_PARSE_ERRORS
    {
        xpe_no_error                = 0,    // ��ȷ��û�д���
        xpe_unknown                 = 1,    // δ֪����
        xpe_callback_cancel,                // �������̱��ص�����ȡ��
        xpe_null_root,                      // û�и��ڵ㣬ͨ������Ϊû�е���BeginParse�������ڽ��������е�����DetachRoot
        xpe_need_initialize,                // ����Parsingǰû�е���BeginParse
        xec_unescape_failed,                // ��ת��ʧ�ܣ�ͨ���Ƕ����˴����ת�����
        xec_unexpected_char,                // �����Ƿ��ַ�
        xec_unmatched_tag,                  // ��ǩ��ƥ��
        xec_too_much_doc_element,           // �ĵ�Ԫ��̫�࣬xml�淶�涨ÿ��xml�ĵ�ֻ����һ���ĵ�Ԫ��
        xec_decode_failed,                  // ����ʧ��
        xec_cannot_switch_coding,           // �޷��л�����
        xec_mssing_tail,                    // �Ҳ����ĵ�ĩβ�Ľ�����ǩ
        xec_read_file_failed,               // ��ȡ�ļ�����
        xec_write_file_failed,              // д�ļ�����
        xec_not_supported                   // ��֧�ֵĲ���
    };
    
} // namespace xparam


#endif // SRC_XPARAM_MODEL_MARCODEF_H_
