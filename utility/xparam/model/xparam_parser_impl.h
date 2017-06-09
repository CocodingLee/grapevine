#ifndef SRC_XPARAM_MODEL_XPARAM_PARSER_IMPL_H_
#define SRC_XPARAM_MODEL_XPARAM_PARSER_IMPL_H_
#include "utility/xparam/model/macrodef.h"
#include "utility/xparam/model/xparam_parser_interface.h"
#include "utility/xparam/xml/xml.h"

namespace xparam
{
    class XParamParserImpl
    : public XParamParser
    {
    public:
        XParamParserImpl();
        ~XParamParserImpl() {}
        
        static XParamParserImplPtr create();
        
        void parse_begin();
        bool parsing(wchar_t const* s, size_t len);
        bool parsing(char const* s
                     , size_t len
                     , unsigned int codepage);
        bool parse_end();
        
        virtual bool parse_string(wchar_t const* s
                                  , size_t len);
        virtual bool parse_string(char const* s
                                  , size_t len
                                  , unsigned int codepage);
        virtual bool parse_data(
                                const void* data,
                                size_t size);
        
        virtual bool parse_file(const wchar_t* filename);
        virtual bool parse_file(const char* filename);
        
        virtual size_t row() const;
        virtual size_t col() const;
        virtual size_t offset() const;
        virtual unsigned int error_code() const;
        virtual XParamPtr root() const;
        virtual XParamPtr detach_root();
        
        //////////////////////////////////////////////////////////////////////////
        
        template <typename CH>
        static bool file_read(const CH* filename
                              , void*& data
                              , size_t& size)
        {
            FILE* file = NULL;
            data = NULL;
            long fsize;
            static const CH mode[] = {114, 98, 0}; // "rb"
            if ( (file = xfopen(filename, mode)) == NULL )
            {
                goto error_exit;
            }
            
            if ( fseek(file, 0, SEEK_END) )
            {
                goto error_exit;
            }
            
            if ( (fsize = ftell(file)) < 0 )
            {
                goto error_exit;
            }
            
            if ( fseek(file, 0, SEEK_SET) )
            {
                goto error_exit;
            }
            
            if ( (data = malloc(fsize)) == NULL )
            {
                goto error_exit;
            }
            
            size = static_cast<size_t>(fsize);
            if ( fread(data, 1, size, file) != size )
            {
                goto error_exit;
            }
            
            fclose(file);
            return true;
            
        error_exit:
            if (file)
            {
                fclose(file);
            }
            
            delete (char*)data;
            data = NULL;
            size = 0;
            return false;
        }
        
        template <typename CH>
        bool parse_file_impl(const CH* filename)
        {
            void* data = NULL;
            size_t size = 0;
            if (!file_read(filename, data, size))
            {
                parse_begin();
                m_error_code = xec_read_file_failed;
                return false;
            }
            bool bret = parse_data(data, size);
            free(data);
            data = NULL;
            return bret;
        }
        
        //////////////////////////////////////////////////////////////////////////
        
    private:
        // ��ʼ����һ���ַ�������<
        bool process_begin();
        // ��'<'(&lt;)��������<���ַ�unescape������Text�ڵ�
        bool find_lt();
        // ��">", >ǰ�Ŀհ��ַ��������ԣ������ַ�������Ϊ����;
        bool find_gt();
        // ��">"�����������κ��ַ�
        bool skip_to_gt();
        // �ֱ�"<"��ڵ�����
        bool resolve_node();
        // ��"<!"��Ľڵ���;
        bool process_special();
        // ��"<!["��Ľڵ���;
        bool process_lbracket();
        //�����Ƿ���comment
        bool test_comment();
        //����comment
        bool process_comment();
        //��"]]>"
        bool find_2rbracket_gt();
        //��"</"���Ԫ����
        bool process_close();
        //����PI��target����
        bool process_pi();
        //��Ԫ�����Ľ�β
        bool process_element();
        //���������Ŀ�ͷ
        bool find_attr();
        //���������Ľ�β
        bool find_attr_name();
        //���������е�"="
        bool find_equal();
        //��������ֵ��ͷ
        bool find_attr_value_begin();
        //��������ֵ��β
        bool find_attr_value();
        //�ҵ�PI�Ľ�β������PI������
        bool skip_over_pi();
        //��һ���ַ�������">"�������ʧ��
        bool process_must_gt();
        //�����ַ���PI��Instruction����
        bool build_instruction(wchar_t c);
        //�رյ�ǰ��node
        void leave();
        //�رյ�ǰ��node
        void leave2();
        
        //////////////////////////////////////////////////////////////////////////
        
    private:
        typedef bool (XParamParserImpl::*process_routine)();
        XmlSrcReaderImpl m_reader;
        XmlEscaperImpl m_escaper;
        xstring<wchar_t> m_text;
        xstring<wchar_t> m_nname;
        XParamPtr m_root;
        XParamPtr m_current_node;
        process_routine m_processor;
        unsigned int m_error_code;
        unsigned int m_mark;
        wchar_t m_current_quote;
    };
    //////////////////////////////////////////////////////////////////////////
    
} // namespace xparam

#endif // SRC_XPARAM_MODEL_XPARAM_PARSER_IMPL_H_