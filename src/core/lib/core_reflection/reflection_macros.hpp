#ifndef REFLECTION_MACROS_HPP
#define REFLECTION_MACROS_HPP

#include "type_class_definition.hpp"

#define BEGIN_EXPOSE_2( baseSpace, meta ) \
	template<>\
	TypeClassDefinition< baseSpace >::TypeClassDefinition() \
		: parentName_( nullptr )\
		, metaData_( &( meta ) )\
	{ \
	}\
	\
	void TypeClassDefinition< baseSpace >::init(\
		IClassDefinitionModifier & collection )\
	{\

#define BEGIN_EXPOSE_3( baseSpace, base, meta ) \
	template<>\
	TypeClassDefinition< baseSpace >::TypeClassDefinition() \
		: parentName_( getClassIdentifier< base >() )\
		, metaData_( &( meta ) ) \
	{ \
		parentName_ =\
			strcmp(\
				getClassIdentifier< baseSpace >(), parentName_ ) == 0\
				? nullptr : parentName_; \
	}\
	\
	void TypeClassDefinition< baseSpace >::init(\
		IClassDefinitionModifier & collection )\
	{\

#define EXPOSE_2( name, _1 )\
	collection.addProperty( \
		FunctionPropertyHelper< SelfType >::getBaseProperty( \
		name, &SelfType::_1), \
		NULL );

#define EXPOSE_3( name, _1, meta )\
	collection.addProperty( \
		FunctionPropertyHelper< SelfType >::getBaseProperty( \
		name, &SelfType::_1), \
		&(meta ) );

#define EXPOSE_4( name, _1, _2, meta )\
	collection.addProperty(\
		FunctionPropertyHelper< SelfType >::getBaseProperty(\
		name, &SelfType::_1, &SelfType::_2 ),\
		&(meta ) );

#define EXPOSE_5( name, _1, _2, _3, meta )\
	collection.addProperty(\
	FunctionPropertyHelper< SelfType >::getBaseProperty(\
	name, &SelfType::_1, &SelfType::_2, &SelfType::_3 ),\
	&(meta ) );

#define EXPOSE_6( name, _1, _2, _3, _4, meta )\
	collection.addProperty(\
	FunctionPropertyHelper< SelfType >::getBaseProperty(\
	name, &SelfType::_1, &SelfType::_2, &SelfType::_3, &SelfType::_4 ),\
	&(meta ) );

//Example of expansion
//
//4 arguments
//EXPOSE( a, b, c, d )
//MACRO_CHOOSER_( NUM_ARGS_( a, b, c, d ) ) ( a, b, c, d )
//EXPOSE_( NUM_ARGS__( ( a, b, c, d ), REVERSE_SEQ_NUM_() ) ) ) ( a, b, c, d )
//EXPOSE_##( STRIP_USELESS_ARGS_( a, b, c, d, 4, 3, 2, 1, 0 ) ) ) ( a, b, c, d )
//EXPOSE_##4( a, b, c, d )
//EXPOSE_4( a, b, c, d )
//
//
//3 arguments
//EXPOSE( a, b, c )
//MACRO_CHOOSER( NUM_ARGS_( a, b, c ) )( a, b, c )
//EXPOSE_( NUM_ARGS__( ( a, b, c ), REVERSE_SEQ_NUM() ) ) )( a, b, c )
//EXPOSE_##( STRIP_USELESS_ARGS_( a, b, c, 4, 3, 2, 1, 0 ) ) )( a, b, c )
//EXPOSE_##( 3 )( a, b, c )
//EXPOSE_3( a, b, c )

//We need this because the Msft compiler tries to expand one step too early
//and causes the __VA_ARGS__ to be calculated wrongly, so we add one level of indirection
#define EXPAND_( X ) X

#define REVERSE_SEQ_NUM_() 6, 5, 4, 3, 2, 1, 0
#define STRIP_USELESS_ARGS_(_1, _2, _3, _4, _5, _6, N, ...) N

#define NUM_ARGS__(...) EXPAND_( STRIP_USELESS_ARGS_(__VA_ARGS__) )
#define NUM_ARGS_(...) EXPAND_( NUM_ARGS__(__VA_ARGS__, REVERSE_SEQ_NUM_()) )

#define EXPOSE_( N ) EXPOSE_##N
#define MACRO_CHOOSER_( N ) EXPOSE_( N )
#define EXPOSE(...)\
	EXPAND_( MACRO_CHOOSER_( EXPAND_( NUM_ARGS_(__VA_ARGS__ ) ) )\
		( __VA_ARGS__) ) //Real macro arguments

#define BEGIN_EXPOSE_( N ) BEGIN_EXPOSE_##N
#define MACRO_CHOOSER_2( N ) BEGIN_EXPOSE_( N )
#define BEGIN_EXPOSE(...)\
	EXPAND_( MACRO_CHOOSER_2( EXPAND_( NUM_ARGS_(__VA_ARGS__ ) ) )\
	( __VA_ARGS__) ) //Real macro arguments

#define END_EXPOSE() \
	} /* End definition */


#define REGISTER_DEFINITION( type )\
	definitionManager.registerDefinition( new TypeClassDefinition< type > );

#endif // REFLECTION_MACROS_HPP