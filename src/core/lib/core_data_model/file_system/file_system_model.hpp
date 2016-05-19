#ifndef FILE_SYSTEM_MODEL_HPP
#define FILE_SYSTEM_MODEL_HPP

#include "core_data_model/abstract_item_model.hpp"

#include <memory>

namespace wgt
{
class IFileSystem;

class FileSystemModel : public AbstractTreeModel
{
public:
	FileSystemModel( IFileSystem & fileSystem, const char * rootDirectory );
	~FileSystemModel();

	AbstractItem * item( const ItemIndex & index ) const override;
	ItemIndex index( const AbstractItem * item ) const override;

	int rowCount( const AbstractItem * item ) const override;
	int columnCount() const override;

private:
	struct Impl;
	std::unique_ptr< Impl > impl_;
};

} // end namespace wgt
#endif//FILE_SYSTEM_MODEL_HPP
