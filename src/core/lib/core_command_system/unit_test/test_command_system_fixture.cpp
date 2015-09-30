#include "pch.hpp"
#include "core_unit_test/test_application.hpp"
#include "test_command_system_fixture.hpp"
#include "core_reflection/definition_manager.hpp"
#include "core_reflection/object_manager.hpp"
#include "core_reflection/reflected_types.hpp"
#include "core_command_system/command_manager.hpp"
#include "core_reflection_utils/reflected_types.hpp"
#include "core_reflection_utils/commands/set_reflectedproperty_command.hpp"
#include "core_reflection_utils/reflection_controller.hpp"
#include "core_command_system/command_system.hpp"

//==============================================================================
TestCommandSystemFixture::TestCommandSystemFixture()
	: application_( new TestApplication )
	, objectManager_( new ObjectManager() )
	, definitionManager_( new DefinitionManager( *objectManager_ ) )
	, commandManager_( new CommandManager( *definitionManager_ ) )
	, setReflectedPropertyCmd_( new SetReflectedPropertyCommand( *definitionManager_ ) )
	, reflectionController_( new ReflectionController() )
	, multiCommandStatus_( MultiCommandStatus_Begin )
{
	objectManager_->init( definitionManager_.get() );
	Reflection::initReflectedTypes( *definitionManager_ );
	Reflection_Utils::initReflectedTypes( *definitionManager_ );
	CommandSystem::initReflectedTypes( *definitionManager_ );
	auto metaTypeMgr = Variant::getMetaTypeManager();

	commandManager_->init( *application_ );
	commandManager_->registerCommand( setReflectedPropertyCmd_.get() );

	reflectionController_->init( *commandManager_ );
	commandManager_->registerCommandStatusListener( this );
}


//==============================================================================
TestCommandSystemFixture::~TestCommandSystemFixture()
{
	commandManager_->deregisterCommand( setReflectedPropertyCmd_->getId() );
	commandManager_->fini();

	setReflectedPropertyCmd_.reset();
	commandManager_.reset();
	objectManager_.reset(); 
	definitionManager_.reset();
}


//==============================================================================
IObjectManager & TestCommandSystemFixture::getObjectManager() const
{
	return *objectManager_;
}


//==============================================================================
IDefinitionManager & TestCommandSystemFixture::getDefinitionManager() const
{
	return *definitionManager_;
}


//==============================================================================
ICommandManager & TestCommandSystemFixture::getCommandSystemProvider() const
{
	return *commandManager_;
}


//==============================================================================
ReflectionController & TestCommandSystemFixture::getReflectionController() const
{
	return *reflectionController_;
}


//==============================================================================
const ICommandEventListener::MultiCommandStatus & TestCommandSystemFixture::getMultiCommandStatus() const
{
	return multiCommandStatus_;
}


//==============================================================================
void TestCommandSystemFixture::multiCommandStatusChanged( 
	ICommandEventListener::MultiCommandStatus multiCommandStatus ) const
{
	multiCommandStatus_ = multiCommandStatus;
}
