/*********************************************************
 *
 * This source code is part of the Carnegie Mellon Robot
 * Navigation Toolkit (CARMEN)
 *
 * CARMEN Copyright (c) 2002 Michael Montemerlo, Nicholas
 * Roy, and Sebastian Thrun
 *
 * CARMEN is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; 
 * either version 2 of the License, or (at your option)
 * any later version.
 *
 * CARMEN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General 
 * Public License along with CARMEN; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307 USA
 *
 ********************************************************/

#ifndef BASE_INTERFACE_H
#define BASE_INTERFACE_H

#include <carmen/base_messages.h>

#ifdef __cplusplus
extern "C" {
#endif

void carmen_base_subscribe_odometry_message(carmen_base_odometry_message 
					    *odometry,
					    carmen_handler_t handler,
					    carmen_subscribe_t subscribe_how);

void carmen_base_subscribe_sonar_message(carmen_base_sonar_message *sonar,
					 carmen_handler_t handler,
					 carmen_subscribe_t subscribe_how);

void carmen_base_subscribe_reset_message(carmen_base_reset_message *sonar,
					 carmen_handler_t handler,
					 carmen_subscribe_t subscribe_how);
void carmen_base_reset(void);
#ifdef __cplusplus
}
#endif

#endif
