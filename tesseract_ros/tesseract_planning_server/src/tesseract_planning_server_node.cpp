/**
 * @file tesseract_planning_server_node.cpp
 * @brief The Tesseract planning server node
 *
 * @author Levi Armstrong
 * @date August 18, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <ros/ros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_environment/core/environment.h>
#include <tesseract_planning_server/tesseract_planning_server.h>

using namespace tesseract_environment;

const std::string ROBOT_DESCRIPTION_PARAM = "robot_description"; /**< Default ROS parameter for robot description */

int main(int argc, char** argv)
{
  ros::init(argc, argv, "tesseract_planning_server");
  ros::NodeHandle pnh("~");
  std::string robot_description;
  std::string discrete_plugin;
  std::string continuous_plugin;
  std::string monitor_namespace;
  std::string monitored_namespace;
  bool publish_environment{ false };

  if (!pnh.getParam("monitor_namespace", monitor_namespace))
  {
    ROS_ERROR("Missing required parameter monitor_namespace!");
    return 1;
  }

  pnh.param<std::string>("monitored_namespace", monitored_namespace, "");
  pnh.param<std::string>("robot_description", robot_description, ROBOT_DESCRIPTION_PARAM);
  pnh.param<std::string>("discrete_plugin", discrete_plugin, "");
  pnh.param<std::string>("continuous_plugin", continuous_plugin, "");
  pnh.param<bool>("publish_environment", publish_environment, publish_environment);

  tesseract_planning_server::TesseractPlanningServer planning_server(
      robot_description, monitor_namespace, discrete_plugin, continuous_plugin);

  if (publish_environment)
    planning_server.getEnvironmentMonitor().startPublishingEnvironment(
        tesseract_monitoring::EnvironmentMonitor::UPDATE_ENVIRONMENT);

  if (!monitored_namespace.empty())
    planning_server.getEnvironmentMonitor().startMonitoringEnvironment(monitored_namespace);

  ros::AsyncSpinner spinner(4);
  spinner.start();

  ros::waitForShutdown();

  return 0;
}