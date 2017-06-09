#ifndef __ARCHIVE_XML4CPP_CONTAINER_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
#define __ARCHIVE_XML4CPP_CONTAINER_H_AD81B9F0_043C_46de_861A_A5C30F60D657__

#include "./archive_xml4cpp_basic.h"
#include <set>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <hash_set>
#include <hash_map>

NS_SERIALIZATION_LITE_BEGIN


#define CONTAINER_TEMPLATE_NP1 P1
#define CONTAINER_TEMPLATE_NP2 CONTAINER_TEMPLATE_NP1, P2
#define CONTAINER_TEMPLATE_NP3 CONTAINER_TEMPLATE_NP2, P3
#define CONTAINER_TEMPLATE_NP4 CONTAINER_TEMPLATE_NP3, P4
#define CONTAINER_TEMPLATE_NP5 CONTAINER_TEMPLATE_NP4, P5
#define CONTAINER_TEMPLATE_CP1 class P1
#define CONTAINER_TEMPLATE_CP2 CONTAINER_TEMPLATE_CP1, class P2
#define CONTAINER_TEMPLATE_CP3 CONTAINER_TEMPLATE_CP2, class P3
#define CONTAINER_TEMPLATE_CP4 CONTAINER_TEMPLATE_CP3, class P4
#define CONTAINER_TEMPLATE_CP5 CONTAINER_TEMPLATE_CP4, class P5

//////////////////////////////////////////////////////////////////////////
// ��map�����������л�
#define CONTAINER_TRANSLATE_NP(container, method, N) \
	template <CONTAINER_TEMPLATE_CP##N>\
	inline void serialize_load(const iarchive_xml4cpp& ar, container<CONTAINER_TEMPLATE_NP##N>& t)\
	{\
		t.clear();\
		iarchive_xml4cpp i = ar;\
		while (i.exists())\
		{\
			P1 x;\
			i >> x;\
			t.method(x);\
			size_t nlen;\
			xml4cpp::XmlInResP inres;\
			wchar_t const* n = i.name(&nlen, inres);\
			i = i.next_sibling(n, nlen);\
		}\
	}\
	template <CONTAINER_TEMPLATE_CP##N>\
	inline void serialize_load(const iarchive_xml4cpp_attr& ar, container<CONTAINER_TEMPLATE_NP##N>& t)\
	{\
		t.clear();\
		P1 x;\
		ar >> x;\
		t.method(x);\
	}\
	template <CONTAINER_TEMPLATE_CP##N>\
	inline void serialize_save(oarchive_xml4cpp& ar, container<CONTAINER_TEMPLATE_NP##N> const& t)\
	{\
		for (typename container<CONTAINER_TEMPLATE_NP##N>::const_iterator ci = t.begin(), end = t.end(); ci != end; ci ++)\
		{\
			ar << *ci;\
			ar = ar.next_sibling(ar.name());\
		}\
	}\
	template <CONTAINER_TEMPLATE_CP##N>\
	inline void serialize_save(oarchive_xml4cpp_attr& ar, container<CONTAINER_TEMPLATE_NP##N> const& t)\
	{\
		for (typename container<CONTAINER_TEMPLATE_NP##N>::const_iterator ci = t.begin(), end = t.end(); ci != end; ci ++)\
		{\
			ar << *ci;\
			break;\
		}\
	}\

CONTAINER_TRANSLATE_NP(std::vector, push_back, 2)
CONTAINER_TRANSLATE_NP(std::list, push_back, 2)
CONTAINER_TRANSLATE_NP(std::queue, push, 2)
CONTAINER_TRANSLATE_NP(std::deque, push, 2)
CONTAINER_TRANSLATE_NP(std::set, insert, 3)
CONTAINER_TRANSLATE_NP(std::multiset, insert, 3)
CONTAINER_TRANSLATE_NP(stdext::hash_set, insert, 3)
CONTAINER_TRANSLATE_NP(stdext::hash_multiset, insert, 3)

//////////////////////////////////////////////////////////////////////////
// map�����������л�

#define MAP_TRANSLATE(container) \
	template <class Key, class Type, class Traits, class Allocator>\
	inline void serialize_load(const iarchive_xml4cpp& ar, container<Key, Type, Traits, Allocator>& t)\
	{\
		t.clear();\
		iarchive_xml4cpp i = ar;\
		while (i.exists())\
		{\
			std::pair<Key, Type> p;\
			if (addon::xml_to_map_key<Type>::is_attr())\
				i.attr(addon::xml_to_map_key<Type>::name(wchar_t()), xml4cpp::ZERO_TERMINATOR) >> p.first;\
			else\
				i.first_child(addon::xml_to_map_key<Type>::name(wchar_t()), xml4cpp::ZERO_TERMINATOR) >> p.first;\
			i >> p.second;\
			t.insert(p);\
			size_t nlen;\
			xml4cpp::XmlInResP inres;\
			wchar_t const* n = i.name(&nlen, inres);\
			i = i.next_sibling(n, nlen);\
		}\
	}\
	template <class Key, class Type, class Traits, class Allocator>\
	inline void serialize_load(const iarchive_xml4cpp_attr& ar, container<Key, Type, Traits, Allocator>& t)\
	{\
		/*
		t.clear();\
		for (iarchive_xml4cpp_attr i = ar; i.exists(); i = i.next_sibling())\
		{\
			std::pair<Key, Type> p;\
			size_t nlen;\
			xml4cpp::XmlInResP inres;\
			wchar_t const* n = i.name(&nlen, inres);\
			iarchive_pchar<wchar_t>(n, nlen) >> p.first;\
			i >> p.second;\
			t.insert(p);\
		}\
		*/\
	}\
	template <class Key, class Type, class Traits, class Allocator>\
	inline void serialize_save(oarchive_xml4cpp& ar, container<Key, Type, Traits, Allocator> const& t)\
	{\
		for (typename container<Key, Type, Traits, Allocator>::const_iterator ci = t.begin(), end = t.end(); ci != end; ci ++)\
		{\
			ar << ci->second;\
			wchar_t const* kn = addon::xml_to_map_key<Type>::name(wchar_t());\
			if (addon::xml_to_map_key<Type>::is_attr())\
			{\
				if (!ar.exists_attr(kn))\
					ar.attr(kn) << ci->first;\
			}\
			else\
			{\
				if (!ar.exists_child(kn, xml4cpp::node_element))\
					ar.first_child(kn, xml4cpp::node_text) << ci->first;\
			}\
			ar = ar.next_sibling(ar.name());\
		}\
	}\
	template <class Key, class Type, class Traits, class Allocator>\
	inline void serialize_save(oarchive_xml4cpp_attr& ar, container<Key, Type, Traits, Allocator> const& t)\
	{\
		for (typename container<Key, Type, Traits, Allocator>::const_iterator ci = t.begin(), end = t.end(); ci != end; ci ++)\
		{\
			ar << ci->second;\
		}\
	}\


MAP_TRANSLATE(std::map)
MAP_TRANSLATE(std::multimap)
MAP_TRANSLATE(stdext::hash_map)
MAP_TRANSLATE(stdext::hash_multimap)

#undef CONTAINER_TRANSLATE_NP
#undef MAP_TRANSLATE
#undef CONTAINER_TEMPLATE_NP1
#undef CONTAINER_TEMPLATE_NP2
#undef CONTAINER_TEMPLATE_NP3
#undef CONTAINER_TEMPLATE_NP4
#undef CONTAINER_TEMPLATE_NP5
#undef CONTAINER_TEMPLATE_CP1
#undef CONTAINER_TEMPLATE_CP2
#undef CONTAINER_TEMPLATE_CP3
#undef CONTAINER_TEMPLATE_CP4
#undef CONTAINER_TEMPLATE_CP5

NS_SERIALIZATION_LITE_END
#endif//__ARCHIVE_XML4CPP_CONTAINER_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
