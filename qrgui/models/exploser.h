#pragma once

#include <QtCore/QList>
#include <QtWidgets/QTreeWidgetItem>

#include <qrkernel/ids.h>

#include "qrgui/models/modelsDeclSpec.h"

namespace qReal {

namespace gui {
class PaletteTreeWidget;
}

namespace commands {
class AbstractCommand;
}

namespace models {
class LogicalModelAssistApi;
class GraphicalModelAssistApi;

/// This class contains methods for processing all system explosion-based operations
/// from the model aspect
class QRGUI_MODELS_EXPORT Exploser : public QObject
{
	Q_OBJECT

public:
	explicit Exploser(models::LogicalModelAssistApi &api);

	/// Returns information about all elements used by all explosion instances on the
	/// diagrams of the given type. Keys are source types, values are target instances.
	QMultiMap<Id, Id> explosions(const Id &diagram) const;

	/// Returns all elements that have specified id as explosion target with hard nessesarity
	IdList elementsWithHardDependencyFrom(const Id &id) const;

	/// Appends to given command child commands that clear outgoing explosion and
	/// all non-hard incoming explosions
	void handleRemoveCommand(const Id &logicalId, commands::AbstractCommand * const command);

	/// This method checks if explosion must be created immediately for given type
	/// and if yes returns type with explosion target. Otherwise returns Id().
	Id immediateExplosionTarget(const Id &id);

	/// Produces and returns command that binds either logical or graphical elements
	/// instances with explosion link and adds explosion instance into user blocks
	/// palette if nessesary.
	commands::AbstractCommand *addExplosionCommand(const Id &source, const Id &target
			, models::GraphicalModelAssistApi * const graphicalApi = nullptr);

	/// Produces and returns command that unbinds elements with explosion link and
	/// removes explosion instance from user blocks palette if nessesary.
	commands::AbstractCommand *removeExplosionCommand(const Id &source, const Id &target);

	/// Produces and returns command that creates element of target type and
	/// binds given element with new element with explosion link
	commands::AbstractCommand *createElementWithIncomingExplosionCommand(const Id &source
			, const Id &targetType, models::GraphicalModelAssistApi &graphicalApi);

	/// Produces and returns a command that performs the renaming of the whole
	/// explosions  hierarchy and binded palette items
	/// @param oneOfIds The id of one of the elements belonging to the hierarchy
	/// @param newNames The name which will bew assigned to all the elements in heirarchy
	commands::AbstractCommand *renameCommands(const Id &oneOfIds, const QString &newNames) const;

signals:
	/// Emitted each time when automatically created explosion target was removed
	void explosionTargetRemoved();

	/// @todo: Emit it exaclty when explosions set changes.
	/// Emitted when explosions set potentially changes (when elements are added or removed
	/// or renamed or explosion link was added or edited directly).
	void explosionsSetCouldChange();

private:
	/// This method takes one of the elements of the diagram and tries
	/// to move through its explosions hierarchy to the root.
	/// In case of cyclic explosions the behaviour is undefined
	/// (most probably you will get infinite cycle)
	Id explosionsRoot(const Id &id) const;

	/// Returns the hierarchy tree of binded with explosion link elements;
	/// if elements are binded with cyclic explosions the behaviour is undefined
	/// @param oneOfIds belonging to this hierarchy.
	IdList explosionsHierarchy(const Id &oneOfIds) const;

	/// Recursively travels through the explosions tree and puts all elements
	/// ids into @param targetIds
	void explosionsHierarchyPrivate(const Id &currentId, IdList &targetIds) const;

	void refreshPalette(gui::PaletteTreeWidget * const tree, const Id &diagram);

	void connectCommand(commands::AbstractCommand const *command) const;

	models::LogicalModelAssistApi &mApi;
};

}
}
