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
 * PropertyList.h
 *
 *  Created on: Dec 7, 2011
 *      Author: sk
 */

#ifndef PROPERTYLIST_H_
#define PROPERTYLIST_H_

#include <vector>
#include "Property.h"
#include "Global.h"
#include "Logger.h"

/**
 * @brief List of Property instances
 *
 * Uses copy constructor for assignment.
 *
 */
class PropertyList
{
protected:
    /// Container with all properties
    std::vector <Property*> properties_;

public:
    /// @brief Constructor
    PropertyList()
    {
        logdbg << "PropertyList: constructor";
    };
    /// @brief Desctructor. Calls clear.
    virtual ~PropertyList()
    {
        logdbg << "PropertyList: desctructor: start";
        clear();
        logdbg << "PropertyList: desctructor: end";
    };

    /// @brief Copy constructor
    PropertyList(const PropertyList& org)
    {
        for (unsigned int cnt=0; cnt < org.getNumProperties(); cnt++)
        {
            addProperty (org.getProperty(cnt)->id_,(PROPERTY_DATA_TYPE) org.getProperty(cnt)->data_type_int_);
        }
    }

    /// @brief Copy operator
    void operator= (const PropertyList &org)
    {
        for (unsigned int cnt=0; cnt < org.getNumProperties(); cnt++)
        {
            addProperty (org.getProperty(cnt)->id_,(PROPERTY_DATA_TYPE) org.getProperty(cnt)->data_type_int_);
        }
    }

    void addPropertyList (const PropertyList &org)
    {
        for (unsigned int cnt=0; cnt < org.getNumProperties(); cnt++)
        {
            addProperty (org.getProperty(cnt)->id_,(PROPERTY_DATA_TYPE) org.getProperty(cnt)->data_type_int_);
        }
    }

    /// @brief Adds a property
    void addProperty (std::string id, PROPERTY_DATA_TYPE type)
    {
        logdbg << "PropertyList: addProperty: start";
        logdbg << "PropertyList: addProperty:  id '" << id << "' type " << type;
        assert (!id.empty());
        assert (type < P_TYPE_SENTINEL);

        if(hasProperty(id))
        {
            logwrn << "PropertyList: addProperty: property " << id << " already added";
            return;
        }

        Property *prop = new Property (id, type);
        assert (prop);

        properties_.push_back (prop);
        logdbg << "PropertyList: addProperty: end";
    };

    /// @brief Adds a property
    void addProperty (Property &property)
    {
        logdbg << "PropertyList: addProperty: start";

        Property *prop = new Property (property.id_, (PROPERTY_DATA_TYPE) property.data_type_int_);
        assert (prop);

        properties_.push_back (prop);
        logdbg << "PropertyList: addProperty: end";
    };

    /// @brief Return container with all properties
    std::vector <Property*> *getProperties ()
	        {
        return &properties_;
	        };

    /**
     * @brief Removes a property
     *
     * \exception std::runtime_error if identifier not found
     */
    void removeProperty (std::string id)
    {
        logdbg << "PropertyList: removeProperty: start";
        if (!hasProperty(id))
            throw std::runtime_error ("PropteryList: removeProperty: property "+id+" does not exists");

        std::vector <Property *>::iterator it;

        for (it=properties_.begin(); it != properties_.end(); it++)
        {
            if ((*it)->id_.compare (id) == 0)
            {
                properties_.erase (it);
                logdbg << "PropertyList: removeProperty: end";
                return;
            }
        }
        throw std::runtime_error("PropteryList: removeProperty: property "+id+" could not be removed");
    };

    /**
     * @brief Returns index of a property
     *
     * \exception std::runtime_error if identifier not found
     */
    unsigned int getPropertyIndex (std::string id)
    {
        logdbg << "PropertyList: getPropertyIndex: start";
        if (!hasProperty(id))
            throw std::runtime_error ("PropteryList: getPropertyIndex: property "+id+" does not exists");

        std::vector <Property *>::iterator it;

        unsigned int cnt=0;
        for (it=properties_.begin(); it != properties_.end(); it++)
        {
            if ((*it)->id_.compare (id) == 0)
            {
                return cnt;
            }
            cnt++;
        }
        throw std::runtime_error("PropteryList: getPropertyIndex: property "+id+" not found");
    };

    /// @brief Returns flag indicating if property is in list
    bool hasProperty (std::string id) const
    {
        logdbg << "PropertyList: hasProperty: start";
        for (unsigned int cnt=0; cnt < properties_.size(); cnt++)
        {
            Property *prop = properties_.at(cnt);
            assert (prop);
            if (prop->id_.compare(id) == 0)
            {
                logdbg << "PropertyList: hasProperty: end";
                return true;
            }
        }
        return false;
    };

    /// @brief Removes all properties
    void clear ()
    {
        logdbg << "PropertyList: clear: start";
        for (unsigned int cnt=0; cnt < properties_.size(); cnt++)
        {
            Property *prop = properties_.at(cnt);
            assert (prop);
            delete prop;
        }
        properties_.clear();
        logdbg << "PropertyList: clear: end";
    }

    /// @brief Return number of properties in list
    unsigned int getNumProperties () const
    {
        return properties_.size();
    }

    /// @brief Returns property at index i
    Property *getProperty (unsigned int i) const
    {
        assert (i < properties_.size());
        return properties_.at(i);
    }
};


#endif /* PROPERTYLIST_H_ */