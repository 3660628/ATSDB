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

/*
 * DBObject.h
 *
 *  Created on: Apr 6, 2012
 *      Author: sk
 */

#ifndef DBOBJECT_H_
#define DBOBJECT_H_

#include <string>
#include "Global.h"
#include "DBOVariableSet.h"
#include "Configurable.h"

class DBOVariable;
class PropertyList;
class MetaDBTable;
class ActiveSourcesObserver;

/**
 * @brief Definition of a meta table in a schema in a DBObject
 *
 * Simple storage class for a schema and a meta table, as strings. Used in a DBObject to save the definitions into the configuration,
 * and generate the pointers to the defined structures at from them.
 */
class DBOSchemaMetaTableDefinition : public Configurable
{
public:
  /// @brief Constructor, registers parameters
  DBOSchemaMetaTableDefinition(std::string class_id, std::string instance_id, Configurable *parent)
   : Configurable (class_id, instance_id, parent)
  {
    registerParameter ("schema", &schema_, (std::string)"");
    registerParameter ("meta_table", &meta_table_,  (std::string)"");
  }
  /// @brief Destructor
  virtual ~DBOSchemaMetaTableDefinition() {}

  std::string getSchema () { return schema_; }
  std::string getMetaTable () { return meta_table_; }

protected:
  /// DBSchema identifier
  std::string schema_;
  /// MetaDBTable identifier
  std::string meta_table_;
};

/**
 * @brief Definition of a data source for a DBObject
 */
class DBODataSourceDefinition : public Configurable
{
public:
  /// @brief Constructor, registers parameters
  DBODataSourceDefinition(std::string class_id, std::string instance_id, Configurable *parent)
   : Configurable (class_id, instance_id, parent)
  {
    registerParameter ("schema", &schema_, (std::string)"");
    registerParameter ("local_key", &local_key_, (std::string)"");
    registerParameter ("meta_table", &meta_table_,  (std::string)"");
    registerParameter ("foreign_key", &foreign_key_, (std::string)"");
    registerParameter ("name_column", &name_column_, (std::string)"");
  }
  virtual ~DBODataSourceDefinition() {}

  std::string getSchema () { return schema_; }
  std::string getLocalKey () { return local_key_; }
  std::string getMetaTableName () { return meta_table_; }
  std::string getForeignKey () { return foreign_key_; }
  std::string getNameColumn () { return name_column_; }

protected:
  /// DBSchema identifier
  std::string schema_;
  /// Identifier for key in main table
  std::string local_key_;
  /// Identifier for meta table with data sources
  std::string meta_table_;
  /// Identifier for key in meta table with data sources
  std::string foreign_key_;
  /// Identifier for sensor name column in meta table with data sources
  std::string name_column_;
};

/**
 * @brief Abstract data description of an object stored in a database
 *
 * A database object serves as definition of a data container stored in a database. It is identified by name and a type,
 * and mainly consists of one or a number of meta tables. One meta table is considered the main meta table, all others
 * are sub meta tables. Columns from all such meta tables are collected and abstracted as DBOVariables of an DBObject.
 *
 * The meta table information is depended on the DBSchema, since for different schemas different table structures might exist.
 * With such a construct it is possible to abstract from different database schemas, by creating one set of DBObjects, which
 * are based on different meta tables (depended on the used schema). All DBOVariable instances have a different table variable,
 * also based on the schema.
 *
 * From an outside view, a DBObject is a collection of DBOVariables. However, a specialization exists, which is called the
 * meta DBObject (of type DBO_UNKNOWN), which serves only as an collection of meta DBOVariables and does not have a meta table.
 *
 * The distinction between meta DBOjects and normal ones can be made using the is_meta_ flag or is_loadable_ flag.
 *
 * For loadable DBObjects, a read list is held. This list contains the information, which variables are to be loaded on
 * read statements from the database. This read list is only held, and other classes can add variables to the retrieved
 * reference of the list.
 *
 * Also holds functionality about its data sources. If such information is present (marked in the database schema),
 * such information can be generated and is executed asynchronously. Interested instances have to be registered as observer
 * and receive a callback once the information is present.
 *
 * \todo Check if DBOVariables can exist only in some schemas, finish checkVariables
 */
class DBObject : public Configurable
{
public:
  /// @brief Constructor
  DBObject(std::string class_id, std::string instance_id, Configurable *parent);
  /// @brief Desctructor
  virtual ~DBObject();

  /// @brief Returns flag indication if a DBOVariable identified by id exists
  bool hasVariable (std::string id);
  /// @brief Returns variable identified by id
  DBOVariable *getVariable (std::string id);
  /// @brief Deletes a variable identified by id
  void deleteVariable (std::string id);

  /// @brief Returns container with all variables
  std::map<std::string, DBOVariable*> &getVariables ();
  /// @brief Returns number of existing variables
  unsigned int getNumVariables () { return variables_.size(); }

  /// @brief Returns container with all variables in a DBTable identified by table
  std::vector <DBOVariable*> getVariablesForTable (std::string table);
  /// @brief Returns name of the object
  std::string getName () { return name_; }
  /// @brief Sets name of the object
  void setName (std::string name) { assert (name.size()>0); name_=name; }

  /// @brief Returns description of the object
  std::string getInfo () { return info_; }
  /// @brief Sets description of the object
  void setInfo(std::string info) { info_=info; }

  /// @brief Sets if an object can be loaded
  void setLoadable(bool loadable) { is_loadable_=loadable; }
  /// @brief Returns if an object can be loaded
  bool isLoadable () { return is_loadable_; }

  /// @brief Returns the type of the object
  DB_OBJECT_TYPE getType () { return (DB_OBJECT_TYPE) type_int_; }
  /// @brief Sets the type of the object
  void setType (DB_OBJECT_TYPE type) { type_int_=type; }

  /// @brief Returns container with all meta tables
  std::map <std::string, std::string> &getMetaTables () { return meta_tables_; }
  /// @brief Returns identifier of main meta table under DBSchema defined by schema
  std::string getMetaTable (std::string schema);
  /// @brief Returns main meta table for current schema
  MetaDBTable *getCurrentMetaTable ();
  /// @brief Returns if current schema has main meta table
  bool hasCurrentMetaTable ();


  /// @brief Returns if object is meta
  bool isMeta () { return is_meta_; }

  /// @brief Returns if a data source is defined in the current schema
  bool hasCurrentDataSource ();
  /// @brief Returns current data source definition
  DBODataSourceDefinition *getCurrentDataSource ();
  /// @brief Returns container with all data source definitions
  std::map <std::string, DBODataSourceDefinition *> &getDataSourceDefinitions () { return data_source_definitions_; }

  virtual void generateSubConfigurable (std::string class_id, std::string instance_id);

  /// @brief Registers an observer to the active data sources information
  void addActiveSourcesObserver (ActiveSourcesObserver *observer);
  /// @brief Removes an observer to the active data sources information
  void removeActiveSourcesObserver (ActiveSourcesObserver *observer);

  /// @brief Return if active data sources info is available
  bool hasActiveDataSourcesInfo ();
  /// @brief Triggers build process of the active data sources ino
  void buildActiveDataSourcesInfo ();
  /// @brief Sets the container with the active data sources information
  void setActiveDataSources (std::set<int> active_data_sources);

  /// @brief Returns container with the active data sources information
  std::set<int> getActiveDataSources () { return active_data_sources_; }

  /// @brief In meta object, registers meta variables as parent variables
  void registerParentVariables ();
  /// @brief In meta object, unregisters meta variables as parent variables
  void unregisterParentVariables ();

protected:
  /// DBO type, as in enum DB_OBJECT_TYPE
  unsigned int type_int_;
  /// DBO name
  std::string name_;
  /// DBO description
  std::string info_;
  /// DBO is loadable flag
  bool is_loadable_; // loadable on its own
  /// DBO is meta flag
  bool is_meta_;

  /// Container with all DBOSchemaMetaTableDefinitions
  std::vector <DBOSchemaMetaTableDefinition *> meta_table_definitions_;
  /// Container with the main meta tables for schemas (schema identifier -> meta_table identifier)
  std::map <std::string, std::string> meta_tables_;

  /// Container with data source definitions (schema identifier -> data source definition pointer)
  std::map <std::string, DBODataSourceDefinition *> data_source_definitions_;
  /// Container with all variables (variable identifier -> variable pointer)
  std::map<std::string, DBOVariable*> variables_;

  /// Container with all observers of the active data sources information
  std::vector <ActiveSourcesObserver *> active_sources_observers_;
  /// Container with all active data sources
  std::set<int> active_data_sources_;

  /// Current (in the current schema) main meta table
  MetaDBTable *current_meta_table_;

  /// Flag indicating if varaibles where checked. Not really used yet.
  bool variables_checked_;
  /// Flag indicating if the active data sources information is present
  bool has_active_data_sources_info_;

  virtual void checkSubConfigurables ();
  /// @brief Checks if variables really exist. Not used yet.
  void checkVariables ();

  /// Notifies all observers of the active data sources information
  void notifyActiveDataSourcesObservers ();
};

#endif /* DBOBJECT_H_ */