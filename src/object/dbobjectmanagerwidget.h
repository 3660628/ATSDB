/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DBOBJECTMANAGERWIDGET_H_
#define DBOBJECTMANAGERWIDGET_H_

#include <QFrame>
#include <map>

class DBObject;
class DBObjectManager;
class DBSchemaManager;
//class DBObjectWidget;
//class MetaDBObjectEditWidget;
class QGridLayout;
class QScrollArea;
class QPushButton;
class QLineEdit;
class QComboBox;

/**
 * @brief Shows all DBObjects, allows editing and adding new ones
 */
class DBObjectManagerWidget : public QFrame
{
    Q_OBJECT

public slots:
    /// @brief Adds a DBObject
    void addDBO ();
    /// @brief Is called when a DBObject was changed
    void changedDBO ();
    /// @brief Starts editing if a DBObject
    void editDBO ();
    /// @brief Updates the DBObject list
    void updateDBOs ();
    /// @brief Updates the meta tables selection widget
    void updateMetaTables ();

public:
    /// @brief Constructor
    DBObjectManagerWidget(DBObjectManager &object_manager, DBSchemaManager &schema_manager);
    /// @brief Destructor
    virtual ~DBObjectManagerWidget();

    /// @brief Unlocks editing functionality
    void unlock ();

private:
    DBObjectManager &object_manager_;
    DBSchemaManager &schema_manager_;
    /// Grid with all DBObjects
    QGridLayout *grid_;
    /// Editing functionality unlocked flag
    bool unlocked_;

    /// New DBO name edit field
    QLineEdit *new_edit_;
    /// New meta DBO meta table selection
    QComboBox *new_meta_table_;
    /// New DBO add button
    QPushButton *new_button_;

    /// New meta DBO name edit field
    //QLineEdit *new_meta_edit_;
    /// New meta DBO add button
    //QPushButton *new_meta_button_;

    /// Container with DBO edit buttons
    std::map <QPushButton *, DBObject *> edit_dbo_buttons_;
    /// Container with already existing edit DBO widgets
    //std::map <DBObject *, DBObjectEditWidget*> edit_dbo_widgets_;
    /// Container with already existing edit meta DBO widgets
    //std::map <DBObject *, MetaDBObjectEditWidget*> edit_metadbo_widgets_;
};

#endif /* DBOBJECTMANAGERWIDGET_H_ */