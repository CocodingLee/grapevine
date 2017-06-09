/********************************************************************
 purpose:	�ṩ���л���������Ĺ��ܣ�Ҳ���������л���ĸ���ԭ��
 ���л���ԭ�������������ÿ����Ҫ���л������ͣ�����Ҫ
 �ṩ���л������������������ֿ��ĺ������ֱ����������
 �����Ҳ������һ������ͬʱ��������������Ƽ�����
 ���л�����������������͵ĳ�Ա����serialize��Ҳ������
 ȫ�ֺ���serialize_load��serialize_save��������ͨ���ػ�
 ��serializer<>��ʵ�֡�
 ����ÿ�����л�Ŀ�����ݸ�ʽ��˵����Ҫʵ���������������
 �࣬һ����iarchive_xxx��һ����oarchive_xxx���ֱ��
 iarchive_base<>��oarichive_base�̳ж���������Ҫʵ������
 �������ͶԻ������͵����л������Ͷ�member_nvio���͵����л�
 ������
 ���л������̴�ĳ��archive�������������&��������ʼ��
 ��Ѱ�ұ����л�������û��ʵ��serialize_load/serialize_save
 �ĺ������أ�����У������֮�����û�У����ɷ�����
 serialize_load����serialize_saveģ�溯��������ת����
 serializerģ���࣬���������л�������û��ʵ��serializer
 ���ػ�������У������֮�����û�����ɷ���serializer��
 ����ת���������л������serialize��Ա����������������
 ���ڣ������֮���������л�ʧ�ܣ�����������
 *********************************************************************/
#ifndef __SRLZ_BASE_H_FD9DE0CE_AE8A_43B7_A93B_E6F7E3C2A6CC__
#define __SRLZ_BASE_H_FD9DE0CE_AE8A_43B7_A93B_E6F7E3C2A6CC__

#define NS_SERIALIZATION_LITE_BEGIN	namespace srlz{
#define NS_SERIALIZATION_LITE_END	}


NS_SERIALIZATION_LITE_BEGIN

//struct SerializeOption
//{
//
//	enum OptContFlag
//	{
//		ocf_replace = 0,
//		ocf_append = 1,
//		ocf_appendnew = 2,
//		ocf_overwrite = 3,
//	};
//	enum OptPtrFlag
//	{
//		opf_matchtype = 0,
//		opf_notmatchtype = 1,
//	};
//	OptContFlag contoption : 2;
//	OptPtrFlag ptroption : 1;
//
//	SerializeOption()
//	{
//		*reinterpret_cast<long*>(this) = 0;
//	}
//	SerializeOption(SerializeOption const& that)
//	{
//		*reinterpret_cast<long*>(this) = *reinterpret_cast<long*>(&that);
//	}
//	SerializeOption& operator=(SerializeOption const& rhs)
//	{
//		*reinterpret_cast<long*>(this) = *reinterpret_cast<long*>(&rhs);
//		return this;
//	}
//};

//////////////////////////////////////////////////////////////////////////
// ���������л������࣬��Ҫ�����л������Ϳ���ѡ��ͨ���ػ�serializer��ʵ��
// �Լ������л�����������T������Ҫ�����л������ͣ��ڲ���Archiveģ�������
// ���л��ĵ������͡�
// �������������λ������ط�,��ʾserialize����ĳ����ĳ�Ա,˵�������
// û��ʵ�������л�����
template <typename T>
struct serializer
{
    enum {specialized = 0};
    template <class Archive>
    inline static void serialize(Archive& ar, T& t)
    {
        t.serialize(ar);
    }
};

//////////////////////////////////////////////////////////////////////////
// ������serialize_load��serialize_save��������Ҫ�����л������Ϳ���ѡ��ͨ��
// ����������������ʵ�����л������в���ar�����л��ĵ�����t�Ǳ����л��Ķ���
// serialize_load����ʵ�ֽ�����t�ӵ���ar�ж�ȡ������serialize_save����ʵ��
// ������t���浽����ar�С�
template <class Archive, class T>
inline void serialize_load(Archive const& ar, T& t)
{
    serializer<T>::serialize(const_cast<Archive&>(ar), t);
}

template <class Archive, class T>
inline void serialize_save(Archive& ar, T const& t)
{
    serializer<T>::serialize(ar, const_cast<T&>(t));
}


//////////////////////////////////////////////////////////////////////////
// ���뵵���Ļ��࣬�������뵵�����������������Archiveģ��������Ǿ����
// �����ࡣ
template <class Archive>
class iarchive_base
{
public:
    
    // ��������������������жϵ��������뻹�������
    struct direction
    {
        enum {value = 1};
    };
    
    // ������������أ����õ���serialize_load
    template <class T>
    inline const Archive& operator>> (T& t) const
    {
        serialize_load(*This(), t);
        return *This();
    }
    
    // ʹ��&��Ϊͨ�����л����������������뵵��������>>��һ���Ĺ���
    template <class T>
    inline const Archive& operator& (T& t) const
    {
        serialize_load(*This(), t);
        return *This();
    }
    
protected:
    
    // �����������ڲ�ʹ��
    inline const Archive* This() const
    {
        return static_cast<Archive const*>(this);
    }
};

//////////////////////////////////////////////////////////////////////////
// ��������Ļ��࣬��������������������������Archiveģ��������Ǿ����
// �����ࡣ
template <class Archive>
class oarchive_base
{
public:
    
    // ��������������������жϵ��������뻹�������
    struct direction
    {
        enum {value = 0};
    };
    
    // ������������أ����õ���serialize_save
    template<class T>
    inline Archive& operator<< (const T& t)
    {
        serialize_save(*This(), t);
        return *This();
    }
    
    // ʹ��&��Ϊͨ�����л��������������������������<<��һ���Ĺ���
    template<class T>
    inline Archive& operator& (const T& t)
    {
        serialize_save(*This(), t);
        return *This();
    }
    
protected:
    
    // �����������ڲ�ʹ��
    inline Archive* This()
    {
        return static_cast<Archive*>(this);
    }
};


#define __LL(x) L ## x
#define _LL(x) __LL(x)
#define _ANULL ((const char*)0)
#define _WNULL ((const wchar_t*)0)

#if defined(UNICODE) || defined(_UNICODE)
#define _TNULL _WNULL
#else
#define _TNULL _ANULL
#endif

NS_SERIALIZATION_LITE_END

#endif//__SRLZ_BASE_H_FD9DE0CE_AE8A_43B7_A93B_E6F7E3C2A6CC__
