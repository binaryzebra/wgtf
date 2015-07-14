#ifndef FOLDER_CONTENT_OBJECT_MODEL_HPP
#define FOLDER_CONTENT_OBJECT_MODEL_HPP

#include "generic_plugin/interfaces/i_context_manager.hpp"
#include "data_model/i_tree_model.hpp"
#include "data_model/i_item.hpp"
#include "reflection/reflected_object.hpp"
#include "reflection/object_handle.hpp"
#include "serialization/interfaces/i_file_system.hpp"

#include <memory>

//------------------------------------------------------------------------------
// FolderContentObjectModel
//
// Defines a single object contained by an asset folder
//------------------------------------------------------------------------------

class FolderContentObjectModel
{
public:

	FolderContentObjectModel();
	FolderContentObjectModel( const FolderContentObjectModel& rhs );
	FolderContentObjectModel( const FileInfo& fileInfo );

	virtual ~FolderContentObjectModel();

	void init( const FileInfo& fileInfo );

	const FileInfo& getFileInfo() const;
	const char* getFileName() const;
	const char* getFullPath() const;
	const char* getThumbnail() const;
	uint64_t getSize() const;
	uint64_t getCreatedTime() const;
	uint64_t getModifiedTime() const;
	uint64_t getAccessedTime() const;
	bool isDirectory() const;
	bool isReadOnly() const;

private:

	struct Implementation;
	std::unique_ptr<Implementation> impl_;
};

#endif // FOLDER_CONTENT_OBJECT_MODEL_HPP