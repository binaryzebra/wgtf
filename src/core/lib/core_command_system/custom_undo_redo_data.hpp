#ifndef CUSTOM_UNDO_REDO_DATA_HPP
#define CUSTOM_UNDO_REDO_DATA_HPP

#include "undo_redo_data.hpp"

class CommandInstance;

class CustomUndoRedoData : public UndoRedoData
{
public:
	CustomUndoRedoData( CommandInstance & commandInstance );

	void undo() override;
	void redo() override;

private:
	CommandInstance & commandInstance_;
};

#endif // UNDO_REDO_DATA_HPP