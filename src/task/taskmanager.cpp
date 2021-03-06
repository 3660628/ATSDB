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

#include "atsdb.h"
#include "taskmanager.h"
#include "jsonimportertask.h"
#include "jsonimportertaskwidget.h"
#include "radarplotpositioncalculatortask.h"
#include "radarplotpositioncalculatortaskwidget.h"

#include <cassert>

TaskManager::TaskManager(const std::string &class_id, const std::string &instance_id, ATSDB *atsdb)
: Configurable (class_id, instance_id, atsdb, "task.xml")
{
    createSubConfigurables();
}

TaskManager::~TaskManager()
{
    assert (!json_importer_task_);
    assert (!radar_plot_position_calculator_task_);
}

JSONImporterTask* TaskManager::getJSONImporterTask()
{
    assert (json_importer_task_);
    return json_importer_task_;
}

RadarPlotPositionCalculatorTask* TaskManager::getRadarPlotPositionCalculatorTask()
{
    assert (radar_plot_position_calculator_task_);
    return radar_plot_position_calculator_task_;
}

void TaskManager::generateSubConfigurable (const std::string &class_id, const std::string &instance_id)
{
    if (class_id.compare ("JSONImporterTask") == 0)
    {
        assert (!json_importer_task_);
        json_importer_task_ = new JSONImporterTask (class_id, instance_id, this);
        assert (json_importer_task_);
    }
    else if (class_id.compare ("RadarPlotPositionCalculatorTask") == 0)
    {
        assert (!radar_plot_position_calculator_task_);
        radar_plot_position_calculator_task_ = new RadarPlotPositionCalculatorTask (class_id, instance_id, this);
        assert (radar_plot_position_calculator_task_);
    }
    else
        throw std::runtime_error ("TaskManager: generateSubConfigurable: unknown class_id "+class_id );
}

void TaskManager::checkSubConfigurables ()
{
    if (!json_importer_task_)
    {
        json_importer_task_ = new JSONImporterTask ("JSONImporterTask", "JSONImporterTask", this);
        assert (json_importer_task_);
    }
    else if (!radar_plot_position_calculator_task_)
    {
        radar_plot_position_calculator_task_ = new RadarPlotPositionCalculatorTask (
                    "RadarPlotPositionCalculatorTask", "RadarPlotPositionCalculatorTask0", this);
        assert (radar_plot_position_calculator_task_);
    }
}

void TaskManager::disable ()
{
    if (json_importer_task_)
    {
        json_importer_task_->widget()->close();
    }

    if (radar_plot_position_calculator_task_)
    {
        radar_plot_position_calculator_task_->widget()->close();
    }
}

void TaskManager::shutdown ()
{
    loginf << "TaskManager: shutdown";
    // TODO waiting for tasks?

    if (json_importer_task_)
    {
        delete json_importer_task_;
        json_importer_task_ = nullptr;
    }

    if (radar_plot_position_calculator_task_)
    {
        delete radar_plot_position_calculator_task_;
        radar_plot_position_calculator_task_ = nullptr;
    }
}
