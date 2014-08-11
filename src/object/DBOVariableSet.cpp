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
 * DBOVariableSet.cpp
 *
 *  Created on: May 22, 2012
 *      Author: sk
 */

#include <algorithm>
#include "DBOVariable.h"
#include "DBOVariableSet.h"

DBOVariableSet::DBOVariableSet()
{
  changed_=false;
}

DBOVariableSet::~DBOVariableSet()
{
}

bool DBOVariableSet::add (DBOVariable *var)
{
  if (find (set_.begin(), set_.end(), var) == set_.end())
  {
    set_.push_back (var);
    changed_=true;
    return true;
  }
  return false;
}

bool DBOVariableSet::add (const DBOVariable *var)
{
  if (find (set_.begin(), set_.end(), var) == set_.end())
  {
    set_.push_back ((DBOVariable *)var);
    changed_=true;
    return true;
  }
  return false;
}

bool DBOVariableSet::add (DBOVariableSet &set)
{
  std::vector <DBOVariable*> &setset = set.getSet();
  std::vector <DBOVariable*>::iterator it;

  bool added=false;

  for (it=setset.begin(); it != setset.end(); it++)
  {
    if (find (set_.begin(), set_.end(), *it) == set_.end())
    {
      set_.push_back (*it);
      changed_=true;
      added=true;
    }
  }
  return added;
}

bool DBOVariableSet::addOnly (DBOVariableSet &set, DB_OBJECT_TYPE dbo_type)
{
  logdbg  << "DBOVariableSet: addOnly: type " << dbo_type;
  std::vector <DBOVariable*> &setset = set.getSet();

  std::vector <DBOVariable*>::iterator it;
  bool added=false;

  for (it=setset.begin(); it != setset.end(); it++)
  {
    if (find (set_.begin(), set_.end(), *it) == set_.end())
    {
      //loginf  << "DBOVariableSet: addOnly: new var";
      if ((*it)->existsIn(dbo_type))
      {
        logdbg  << "DBOVariableSet: addOnly: pushback";
        set_.push_back ((*it)->getFor(dbo_type));
        changed_=true;
        added=true;
      }
    }
  }

  return added;
}

void DBOVariableSet::removeVariableAt (unsigned int index)
{
  assert (index < set_.size());

  set_.erase ( set_.begin()+index );

  changed_=true;
}

DBOVariableSet *DBOVariableSet::getFor (DB_OBJECT_TYPE dbo_type)
{
  logdbg  << "DBOVariableSet: getFor: type " << dbo_type;

  DBOVariableSet *type_set = new DBOVariableSet ();
  std::vector <DBOVariable*>::iterator it;

  for (it=set_.begin(); it != set_.end(); it++)
  {
    if ((*it)->existsIn(dbo_type))
    {
      logdbg  << "DBOVariableSet: getFor: add";
      assert (!(*it)->getFor(dbo_type)->isMetaVariable());
      type_set->add ((*it)->getFor(dbo_type));
    }
  }

  return type_set;
}


DBOVariableSet& DBOVariableSet::operator= (const DBOVariableSet &source)
{
    loginf  << "DBOVariableSet: copy constructor";

    if (this == &source) // self assignment
        return *this;

    std::vector <DBOVariable*>::const_iterator it;

    loginf  << "DBOVariableSet: copy constructor: copying " << source.set_.size() << " elements";

    // do the copy
    for (it=source.set_.begin(); it != source.set_.end(); it++)
    {
        add ((*it));
    }

    return *this;
}

DBOVariable *DBOVariableSet::getVariable (unsigned int index)
{
  assert (index < set_.size());
  return set_.at(index);
}

bool DBOVariableSet::intersect (DBOVariableSet &set)
{

  std::vector <DBOVariable*> org_set=set_;
  set_.clear();
  bool added=false;

  std::vector <DBOVariable*> &setset = set.getSet();
  std::vector <DBOVariable*>::iterator it;
  std::vector <DBOVariable*>::iterator it2;

  for (it=org_set.begin(); it != org_set.end(); it++) // traverse original list
  {
    it2 = find (setset.begin(), setset.end(), *it);
    if (it2 == setset.end()) // other set hasn't var
    {
      changed_=true;
      added=true;
    }
    else
      set_.push_back(*it);
  }

  logdbg  << "DBOVariableSet: intersect: size " << set_.size() << " other size " << set.getSet().size() <<  " done";
  return added;
}

void DBOVariableSet::print ()
{
  logdbg  << "DBOVariableSet: print: size" << set_.size() << " changed " << changed_;
  std::vector <DBOVariable*>::iterator it;

  for (it=set_.begin(); it != set_.end(); it++)
  {
    (*it)->print();
  }
}

void DBOVariableSet::clear ()
{
  set_.clear();
  changed_=true;
}

bool DBOVariableSet::hasVariable (DBOVariable *variable)
{
  return find (set_.begin(), set_.end(), variable) != set_.end();
}

PropertyList DBOVariableSet::getPropertyList (DB_OBJECT_TYPE dbo_type)
{
  std::vector <DBOVariable*>::iterator it;
  PropertyList list;

  for (it=set_.begin(); it != set_.end(); it++)
  {
    if ((*it)->existsIn(dbo_type))
    {
      logdbg  << "DBOVariableSet: getPropertyList: getfor";
      DBOVariable *var = (*it)->getFor(dbo_type);
      list.addProperty (var->id_, (PROPERTY_DATA_TYPE) var->data_type_int_);
    }
  }

  return list;
}