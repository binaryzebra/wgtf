#include "core_generic_plugin/generic_plugin.hpp"

class Python27TestPlugin
	: public PluginMain
{
public:
	Python27TestPlugin( IComponentContext & contextManager )
	{
	}


	bool PostLoad( IComponentContext & contextManager ) override
	{
		return true;
	}


	void Initialise( IComponentContext & contextManager ) override
	{
	}


	bool Finalise( IComponentContext & contextManager ) override
	{
		return true;
	}


	void Unload( IComponentContext & contextManager )
	{
	}
};

PLG_CALLBACK_FUNC( Python27TestPlugin )

