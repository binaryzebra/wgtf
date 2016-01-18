#ifndef COLLECTION_MODEL_HPP
#define COLLECTION_MODEL_HPP

#include "core_data_model/i_list_model.hpp"
#include "core_variant/collection.hpp"
#include <memory>

/**
 *	Data model for displaying a Collection as a list.
 */
class CollectionModel
	: public IListModel
{
public:
	CollectionModel();
	virtual ~CollectionModel();

	void setSource( Collection & collection );
	Collection & getSource();

	// IListModel
	virtual IItem * item( size_t index ) const override;
	virtual size_t index( const IItem* item ) const override;

	virtual bool empty() const override;
	virtual size_t size() const override;
	virtual int columnCount() const override;

protected:
	Collection collection_;
	mutable std::vector< std::unique_ptr< IItem > > items_;
};

#endif // COLLECTION_LIST_MODEL_HPP
