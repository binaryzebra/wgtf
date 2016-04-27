#ifndef ABSTRACT_ITEM_MODEL_HPP
#define ABSTRACT_ITEM_MODEL_HPP

#include "abstract_item.hpp"


/**
 *	Base class for all types of data models.
 */
class AbstractItemModel : public AbstractItem
{
public:
	struct ItemIndex
	{
		ItemIndex( int row = -1, int column = -1, const AbstractItem * parent = nullptr )
			: row_( row )
			, column_( column )
			, parent_( parent )
		{}

		bool isValid() const
		{
			return row_ >= 0 && column_ >= 0;
		}

		bool operator==( const ItemIndex & other ) const
		{
			if (!isValid() && !other.isValid())
			{
				return true;
			}

			return row_ == other.row_ && column_ == other.column_ && parent_ == other.parent_;
		}

		bool operator!=( const ItemIndex & other ) const
		{
			return !this->operator==( other );
		}

		int row_;
		int column_;
		const AbstractItem * parent_;
	};

	typedef void DataSignature( const ItemIndex & index, size_t role, const Variant & value );
	typedef void RangeSignature( const ItemIndex & index, int pos, int count );
	typedef std::function< DataSignature > DataCallback;
	typedef std::function< RangeSignature > RangeCallback;

	virtual ~AbstractItemModel() {}

	virtual AbstractItem * item( const ItemIndex & index ) const = 0;
	virtual void index( const AbstractItem * item, ItemIndex & o_Index ) const = 0;

	virtual int rowCount( const AbstractItem * item ) const = 0;
	virtual int columnCount( const AbstractItem * item ) const = 0;

	virtual Connection connectPreItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPostItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreColumnsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostColumnsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreColumnsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostColumnRemoved( RangeCallback callback ) { return Connection(); }
};


/**
 *	Provide models with items arranged one after the other; a list.
 *	Items in rows are all connected, cannot have different items in different columns.
 */
class AbstractListModel : public AbstractItemModel
{
public:
	typedef void DataSignature( int row, int column, size_t role, const Variant & value );
	typedef void RangeSignature( int pos, int count );
	typedef std::function< DataSignature > DataCallback;
	typedef std::function< RangeSignature > RangeCallback;

	virtual ~AbstractListModel() {}

	virtual AbstractItem * item( int row ) const = 0;
	virtual int index( const AbstractItem * item ) const = 0;

	virtual int rowCount() const = 0;
	virtual int columnCount() const = 0;

	virtual Connection connectPreItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPostItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsRemoved( RangeCallback callback ) { return Connection(); }

private:
	AbstractItem * item( const AbstractItemModel::ItemIndex & index ) const override
	{
		if (index.parent_ != nullptr || index.column_ != 0)
		{
			return nullptr;
		}

		return item( index.row_ );
	}

	void index( const AbstractItem * item, AbstractItemModel::ItemIndex & o_Index ) const
	{
		int row = index( item );

		o_Index.row_ = row;
		o_Index.column_ = 0;
		o_Index.parent_ = nullptr;
	}

	int rowCount( const AbstractItem * item ) const override
	{
		if (item != nullptr)
		{
			return 0;
		}
		
		return rowCount();
	}

	int columnCount( const AbstractItem * item ) const override
	{
		return columnCount();
	}

	Connection connectPreItemDataChanged( AbstractItemModel::DataCallback callback ) 
	{ 
		return connectPreItemDataChanged( ( DataCallback )[=]( int row, int column, size_t role, const Variant & value )
		{
			callback( AbstractItemModel::ItemIndex( row, column, nullptr ), role, value );
		}); 
	}

	Connection connectPostItemDataChanged( AbstractItemModel::DataCallback callback ) 
	{ 
		return connectPostItemDataChanged( ( DataCallback )[=]( int row, int column, size_t role, const Variant & value )
		{
			callback( AbstractItemModel::ItemIndex( row, column, nullptr ), role, value );
		}); 
	}

	Connection connectPreRowsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreRowsInserted( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPostRowsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostRowsInserted( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPreRowsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreRowsRemoved( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPostRowsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostRowsRemoved( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}
};


/**
 *	Provide models with items arranged in a hierarchy; a tree.
 *	Items in rows are all connected, cannot have different items in different columns.
 */
class AbstractTreeModel : public AbstractItemModel
{
public:
	struct ItemIndex
	{
		ItemIndex( int row = -1, const AbstractItem * parent = nullptr )
			: row_( row )
			, parent_( parent )
		{}

		bool isValid() const
		{
			return row_ >= 0;
		}

		bool operator==( const ItemIndex & other ) const
		{
			if (!isValid() && !other.isValid())
			{
				return true;
			}

			return row_ == other.row_ && parent_ == other.parent_;
		}

		bool operator!=( const ItemIndex & other ) const
		{
			return !this->operator==( other );
		}

		int row_;
		const AbstractItem * parent_;
	};

	typedef void DataSignature( const ItemIndex & index, int column, size_t role, const Variant & value );
	typedef void RangeSignature( const ItemIndex & index, int pos, int count );
	typedef std::function< DataSignature > DataCallback;
	typedef std::function< RangeSignature > RangeCallback;

	virtual ~AbstractTreeModel() {}

	virtual AbstractItem * item( const ItemIndex & index ) const = 0;
	virtual ItemIndex index( const AbstractItem * item ) const = 0;

	virtual int rowCount( const AbstractItem * item ) const = 0;
	virtual int columnCount() const = 0;

	virtual Connection connectPreItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPostItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsRemoved( RangeCallback callback ) { return Connection(); }

// Private fns never/cannot get called?
private:
	AbstractItem * item( const AbstractItemModel::ItemIndex & index ) const override
	{
		if (index.column_ != 0)
		{
			return nullptr;
		}

		return item( ItemIndex( index.row_, index.parent_ ) );
	}

	void index( const AbstractItem * item, AbstractItemModel::ItemIndex & o_Index ) const
	{
		ItemIndex index = this->index( item );

		o_Index.row_ = index.row_;
		o_Index.column_ = 0;
		o_Index.parent_ = index.parent_;
	}

	int columnCount( const AbstractItem * item ) const override
	{
		return columnCount();
	}

	Connection connectPreItemDataChanged( AbstractItemModel::DataCallback callback ) 
	{ 
		return connectPreItemDataChanged( ( DataCallback )[=]( const ItemIndex & index, int column, size_t role, const Variant & value )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, column, index.parent_ ), role, value );
		}); 
	}
	
	Connection connectPostItemDataChanged( AbstractItemModel::DataCallback callback ) 
	{ 
		return connectPostItemDataChanged( ( DataCallback )[=]( const ItemIndex & index, int column, size_t role, const Variant & value )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, column, index.parent_ ), role, value );
		}); 
	}
	
	Connection connectPreRowsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreRowsInserted( ( RangeCallback )[=]( const ItemIndex & index, int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, 0, index.parent_ ), pos, count );
		}); 
	}
	
	Connection connectPostRowsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostRowsInserted( ( RangeCallback )[=]( const ItemIndex & index, int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, 0, index.parent_ ), pos, count );
		}); 
	}
	
	Connection connectPreRowsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreRowsRemoved( ( RangeCallback )[=]( const ItemIndex & index, int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, 0, index.parent_ ), pos, count );
		}); 
	}
	
	Connection connectPostRowsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostRowsRemoved( ( RangeCallback )[=]( const ItemIndex & index, int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, 0, index.parent_ ), pos, count );
		}); 
	}
};


/**
 *	Provide models with items arranged in a grid; a table.
 *	Items in each (row, column) can be unrelated.
 */
class AbstractTableModel : public AbstractItemModel
{
public:
	struct ItemIndex
	{
		ItemIndex( int row = -1, int column = -1 )
			: row_( row )
			, column_( column )
		{}

		bool isValid() const
		{
			return row_ >= 0 && column_ >= 0;
		}

		bool operator==( const ItemIndex & other ) const
		{
			if (!isValid() && !other.isValid())
			{
				return true;
			}

			return row_ == other.row_ && column_ == other.column_;
		}

		bool operator!=( const ItemIndex & other ) const
		{
			return !this->operator==( other );
		}

		int row_;
		int column_;
	};

	typedef void DataSignature( const ItemIndex & index, size_t role, const Variant & value );
	typedef void RangeSignature( int pos, int count );
	typedef std::function< DataSignature > DataCallback;
	typedef std::function< RangeSignature > RangeCallback;

	virtual ~AbstractTableModel() {}

	virtual AbstractItem * item( const ItemIndex & index ) const = 0;
	virtual ItemIndex index( const AbstractItem * item ) const = 0;

	virtual int rowCount() const = 0;
	virtual int columnCount() const = 0;

	virtual Connection connectPreItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPostItemDataChanged( DataCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreRowsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostRowsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreColumnsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostColumnsInserted( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPreColumnsRemoved( RangeCallback callback ) { return Connection(); }
	virtual Connection connectPostColumnsRemoved( RangeCallback callback ) { return Connection(); }

private:
	AbstractItem * item( const AbstractItemModel::ItemIndex & index ) const override
	{
		if (index.parent_ != nullptr)
		{
			return nullptr;
		}

		return item( ItemIndex( index.row_, index.column_ ) );
	}

	void index( const AbstractItem * item, AbstractItemModel::ItemIndex & o_Index ) const
	{
		ItemIndex index = this->index( item );

		o_Index.row_ = index.row_;
		o_Index.column_ = index.column_;
		o_Index.parent_ = nullptr;
	}

	int rowCount( const AbstractItem * item ) const override
	{
		if (item != nullptr)
		{
			return 0;
		}

		return columnCount();
	}

	int columnCount( const AbstractItem * item ) const override
	{
		if (item != nullptr)
		{
			return 0;
		}

		return columnCount();
	}

	Connection connectPreItemDataChanged( AbstractItemModel::DataCallback callback ) 
	{ 
		return connectPreItemDataChanged( ( DataCallback )[=]( const ItemIndex & index, size_t role, const Variant & value )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, index.column_, nullptr ), role, value );
		}); 
	}

	Connection connectPostItemDataChanged( AbstractItemModel::DataCallback callback ) 
	{ 
		return connectPostItemDataChanged( ( DataCallback )[=]( const ItemIndex & index, size_t role, const Variant & value )
		{
			callback( AbstractItemModel::ItemIndex( index.row_, index.column_, nullptr ), role, value );
		}); 
	}

	Connection connectPreRowsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreRowsInserted( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPostRowsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostRowsInserted( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPreRowsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreRowsRemoved( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPostRowsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostRowsRemoved( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPreColumnsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreColumnsInserted( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPostColumnsInserted( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostColumnsInserted( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPreColumnsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPreColumnsRemoved( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}

	Connection connectPostColumnsRemoved( AbstractItemModel::RangeCallback callback ) 
	{ 
		return connectPostColumnsRemoved( ( RangeCallback )[=]( int pos, int count )
		{
			callback( AbstractItemModel::ItemIndex(), pos, count );
		}); 
	}
};

#endif//ABSTRACT_ITEM_MODEL_HPP
