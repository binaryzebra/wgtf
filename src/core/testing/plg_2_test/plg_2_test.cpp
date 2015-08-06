#include "core_generic_plugin/generic_plugin.hpp"

#include "../interfaces_test/test_interface.hpp"
#include "core_common/ngt_windows.hpp"

class TestClassB
	: public Implements< TestInterface > //Always implement latest version
{
public:
	TestClassB( float value )
		: value_( value )
	{
	}

	void test( int value )
	{
		char buffer[ 256 ];
		sprintf( buffer, "%s - %f - %d\n", typeid( *this ).name(), value_, value );
		::OutputDebugStringA( buffer );
	}

private:
	float value_;
};

//==============================================================================
class TestPlugin2
	: public PluginMain
{
public:
	//==========================================================================
	TestPlugin2( IComponentContext & contextManager ){}

	//==========================================================================
	bool PostLoad( IComponentContext & contextManager )
	{
		contextManager.registerInterface( new TestClassB( 0.5f ) );
		contextManager.registerInterface( new TestClassB( 2.5f ) );
		return true;
	}

	//==========================================================================
	void Initialise( IComponentContext & contextManager )
	{

	}
};

PLG_CALLBACK_FUNC( TestPlugin2 )
