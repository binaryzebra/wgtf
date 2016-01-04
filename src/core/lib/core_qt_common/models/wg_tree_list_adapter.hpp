#ifndef WG_TREE_LIST_ADAPTER_HPP
#define WG_TREE_LIST_ADAPTER_HPP

#include <QAbstractItemModel>
#include <QQmlListProperty>

#include "core_qt_common/qt_new_handler.hpp"
#include "core_qt_common/models/adapters/i_list_adapter.hpp"
#include "qt_model_macros.hpp"
#include <memory>

class IModelExtension;

/**
 * This is the Qt data model which accepts a QModelIndex and adapts it into a list-friendly format
 * so that it may use its own extensions and allow the use of a single ITreeModel to represent
 * directory structures.
 */
class WGTreeListAdapter : public IListAdapter
{
	Q_OBJECT

	Q_PROPERTY( QVariant	parentIndex
				READ		getParentIndex
				WRITE		setParentIndex
				NOTIFY		parentIndexChanged )

	Q_PROPERTY( QQmlListProperty< IModelExtension > extensions 
				READ getExtensions )

	Q_CLASSINFO( "DefaultProperty", "extensions" )

	DECLARE_QT_MEMORY_HANDLER

public:
	WGTreeListAdapter();
	virtual ~WGTreeListAdapter();
	
	QAbstractItemModel * parentModel() const;
	QAbstractItemModel * model() const Q_DECL_OVERRIDE;
	QHash< int, QByteArray > roleNames() const;
			
	Q_INVOKABLE virtual QModelIndex index(
		int row, int column = 0, const QModelIndex &parent = QModelIndex() ) const override;
	QModelIndex adaptedIndex(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	Q_INVOKABLE int indexRow( const QModelIndex& index ) const;

	template< typename T >
	void registerExtension()
	{
		auto extension = new T();
		extension->setParent( this );
		registerExtension( extension );
	}

	QVariant getParentIndex() const;
	void setParentIndex( const QVariant & sourceIndex );
	
protected:
	virtual void connect() override;
	virtual void disconnect() override;

private:
	void registerExtension( IModelExtension * extension );

	// QAbstractItemModel
	int rowCount( const QModelIndex &parent ) const Q_DECL_OVERRIDE;
	int columnCount( const QModelIndex &index ) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

	// IListAdapter
	void onParentDataChanged(const QModelIndex &topLeft, 
		const QModelIndex &bottomRight, const QVector<int> &roles);
	void onParentLayoutAboutToBeChanged(const QList<QPersistentModelIndex> & parents, 
		QAbstractItemModel::LayoutChangeHint hint);
	void onParentLayoutChanged(const QList<QPersistentModelIndex> & parents, 
		QAbstractItemModel::LayoutChangeHint hint);
	void onParentRowsAboutToBeInserted(const QModelIndex & parent, int first, int last);
	void onParentRowsInserted(const QModelIndex & parent, int first, int last);
	void onParentRowsAboutToBeRemoved(const QModelIndex & parent, int first, int last);
	void onParentRowsRemoved(const QModelIndex & parent, int first, int last);


	// Extensions Handling
	QQmlListProperty< IModelExtension > getExtensions() const;
	static void appendExtension( QQmlListProperty< IModelExtension > * property, IModelExtension * value );
	static IModelExtension * extensionAt( QQmlListProperty< IModelExtension > * property, int index );
	static void clearExtensions( QQmlListProperty< IModelExtension > * property );
	static int countExtensions( QQmlListProperty< IModelExtension > * property );

signals:
	void parentIndexChanged();

private:
	class Impl;
	std::unique_ptr< Impl > impl_;
};

#endif // WG_TREE_LIST_ADAPTER_HPP