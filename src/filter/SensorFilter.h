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
 * SensorFilter.h
 *
 *  Created on: Nov 4, 2012
 *      Author: sk
 */

#ifndef SENSORFILTER_H_
#define SENSORFILTER_H_

#include "DBFilter.h"
#include "ActiveSourcesObserver.h"

/**
 * @brief Definition for a data source in a SensorFilter
 */
class SensorFilterDataSource
{
public:
    /// @brief Constructor
    SensorFilterDataSource () {}
    /// @brief Destructor
    virtual ~SensorFilterDataSource () {}

protected:
    /// Number id
    unsigned int number_;
    /// Name id
    std::string name_;
    /// Flag indicating if active in data
    bool active_in_data_;
    /// Flag indicating if active in filtler
    bool active_in_filter_;

public:
    bool isActiveInData() const
    {
        return active_in_data_;
    }

    void setActiveInData(bool active_in_data)
    {
        active_in_data_ = active_in_data;
    }

    bool isActiveInFilter() const
    {
        return active_in_filter_;
    }

    void setActiveInFilter(bool active_in_filter)
    {
        active_in_filter_ = active_in_filter;
    }

    bool *getActiveInFilterPointer () { return &active_in_filter_; }

    std::string getName() const
    {
        return name_;
    }

    void setName(std::string name)
    {
        name_ = name;
    }

    unsigned int getNumber() const
    {
        return number_;
    }

    void setNumber(unsigned int number)
    {
        number_ = number;
    }

};

/**
 * @brief DBFilter specialization for non-generic sensor filters
 *
 * Each DBObject can have data sources, and if such data is contained in the database, a filter for the seperate data sources
 * should always exist. Should not have sub-filters or conditions.
 *
 */
class SensorFilter : public DBFilter, public ActiveSourcesObserver
{
public:
  /// @brief Constructor
  SensorFilter(std::string class_id, std::string instance_id, Configurable *parent);
  /// @brief Desctructor
  virtual ~SensorFilter();

  virtual std::string getConditionString (DB_OBJECT_TYPE dbo_type, bool &first, std::vector<std::string> &variable_names);

  /// @brief Should not have sub-configurables. Do not call.
  virtual void generateSubConfigurable (std::string class_id, std::string instance_id);

  virtual void reset ();

  DB_OBJECT_TYPE getDBOType () { return (DB_OBJECT_TYPE) dbo_type_int_; }

  virtual void notifyActiveSources ();

  std::map<int, SensorFilterDataSource> &getDataSources () { return data_sources_; }

protected:
  /// DBO type
  int dbo_type_int_;
  /// Sensor id column name in database table
  std::string sensor_column_name_;
  /// Container with all possible data sources and active flag pointers
  std::map<int, SensorFilterDataSource> data_sources_;

  /// @brief Load data sources and updates data_sources_ container
  void updateDataSources ();
  /// @brief Updates the data sources active in data information
  void updateDataSourcesActive ();

  /// @brief Does nothing.
  virtual void checkSubConfigurables ();
};

#endif /* SENSORFILTER_H_ */