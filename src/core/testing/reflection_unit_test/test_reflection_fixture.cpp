#include "pch.hpp"
#include "test_reflection_fixture.hpp"
#include "reflection/definition_manager.hpp"
#include "reflection/object_manager.hpp"
#include "reflection/reflected_types.hpp"
#include "reflection/variant_handler.hpp"

//==============================================================================
TestReflectionFixture::TestReflectionFixture()
	: objectManager_( new ObjectManager() )
	, definitionManager_( new DefinitionManager( *objectManager_ ) )
{
	objectManager_->init( definitionManager_.get() );
	Reflection::initReflectedTypes( *definitionManager_ );

	testObjects_.initDefs( *definitionManager_ );

	auto metaTypeMgr = Variant::getMetaTypeManager();

	auto metaType = metaTypeMgr->findType< ObjectHandle >();
	variantStorageLookupHandler_.reset( new ReflectionStorageLookupHandler(
		definitionManager_.get(),
		metaType ) );

	metaTypeMgr->registerDynamicStorageHandler(
		*variantStorageLookupHandler_ );
}


//==============================================================================
TestReflectionFixture::~TestReflectionFixture()
{
	auto metaTypeMgr = Variant::getMetaTypeManager();
	metaTypeMgr->deregisterDynamicStorageHandler(
		*variantStorageLookupHandler_ );

	objectManager_.reset(); 
	definitionManager_.reset();
}


//==============================================================================
IObjectManager & TestReflectionFixture::getObjectManager() const
{
	return *objectManager_;
}


//==============================================================================
IDefinitionManager & TestReflectionFixture::getDefinitionManager() const
{
	return *definitionManager_;
}


//==============================================================================
TestStructure & TestReflectionFixture::getTestStructure()
{
	return testObjects_.getTestStructure();
}