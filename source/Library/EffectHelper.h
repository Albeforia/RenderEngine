#pragma once

#include "Common.h"
#include <boost\preprocessor\stringize.hpp>
#include <boost\preprocessor\control\if.hpp>
#include <boost\preprocessor\variadic\size.hpp>
#include <boost\preprocessor\variadic\elem.hpp>
#include <boost\preprocessor\variadic\to_list.hpp>
#include <boost\preprocessor\list\at.hpp>
#include <boost\preprocessor\list\for_each.hpp>
#include <boost\preprocessor\repetition\repeat.hpp>

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509647(v=vs.85).aspx
#define VS_SEMANTIC_INDEX_POSITION 0
#define VS_SEMANTIC_INDEX_COLOR 1

#define VS_SEMANTICS_TYPES XMFLOAT4, XMFLOAT4

#define VS_SEMANTIC_TYPE(SEMANTIC) BOOST_PP_VARIADIC_ELEM(VS_SEMANTIC_INDEX_ ## SEMANTIC, VS_SEMANTICS_TYPES)

#define VS_SEMANTIC_DECLARATION(R, DATA, SEMANTIC) VS_SEMANTIC_TYPE(SEMANTIC) SEMANTIC;
#define VS_SEMANTICS_DECLARATION(...) BOOST_PP_LIST_FOR_EACH(VS_SEMANTIC_DECLARATION, _, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

#define VERTEX_STRUCT_DECLARATION(MATERIAL, ...)		\
		struct Vertex ## MATERIAL {						\
			VS_SEMANTICS_DECLARATION(__VA_ARGS__)		\
		};

#define MATERIAL_VARIABLE_DECLARATION(R, DATA, NAME)	\
		public:											\
            Variable& NAME() const;						\
		private:										\
            Variable* m_ ## NAME;
#define MATERIAL_VARIABLES_DECLARATION(...) BOOST_PP_LIST_FOR_EACH(MATERIAL_VARIABLE_DECLARATION, _, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

#define MATERIAL_VARIABLE_DEFINITION(R, MATERIAL, NAME)	\
        Variable& MATERIAL::NAME() const {				\
            return *m_ ## NAME;							\
        }
#define MATERIAL_VARIABLES_DEFINITION(MATERIAL, ...) BOOST_PP_LIST_FOR_EACH(MATERIAL_VARIABLE_DEFINITION, MATERIAL, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

#define MATERIAL_VARIABLE_INITIALIZATION(R, DATA, NAME) m_ ## NAME {},
#define MATERIAL_VARIABLES_INITIALIZATION(...) BOOST_PP_LIST_FOR_EACH(MATERIAL_VARIABLE_INITIALIZATION, _, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

#define MATERIAL_VARIABLE_RETRIEVE(R, DATA, NAME) m_ ## NAME = m_effect->variables_by_name().at(#NAME);
#define MATERIAL_VARIABLES_RETRIEVE(...) BOOST_PP_LIST_FOR_EACH(MATERIAL_VARIABLE_RETRIEVE, _, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

/*
typedef struct D3D11_INPUT_ELEMENT_DESC {
	LPCSTR SemanticName;
	UINT SemanticIndex;
	DXGI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
} 	D3D11_INPUT_ELEMENT_DESC;
*/
#define GENERATE_INPUT_ELEMENT_DESC_ENTRY(Z, N, ELEMENTS)\
	{BOOST_PP_STRINGIZE(BOOST_PP_LIST_AT(ELEMENTS, N)), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
/*
// unknown bug
BOOST_PP_IF(N,\
		{BOOST_PP_STRINGIZE(BOOST_PP_LIST_AT(ELEMENTS, N)), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},\
		{BOOST_PP_STRINGIZE(BOOST_PP_LIST_AT(ELEMENTS, N)), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}),
*/
#define GENERATE_INPUT_ELEMENT_DESC(...) BOOST_PP_REPEAT(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), GENERATE_INPUT_ELEMENT_DESC_ENTRY, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

namespace Library {

	class EffectHelper {

	public:
		EffectHelper() = delete;
		EffectHelper(const EffectHelper&) = delete;
		EffectHelper& operator=(const EffectHelper&) = delete;

	};

}