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

#ifndef CARMEN_MAP_GRAPHICX_H
#define CARMEN_MAP_GRAPHICX_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_LIBART
#include <carmen/artwrap.h>  
#endif

#include <carmen/global_graphics.h>

typedef struct {
  carmen_map_t * internal_map;
  int draw_flags;
  carmen_world_point_t centre;
  double zoom;
  carmen_graphics_screen_config_t screen_defn;
  int port_size_x, port_size_y;
  GtkWidget *image_widget;
  GtkWidget *map_box;
  GtkWidget *window;

#ifdef USE_LIBART
  unsigned char *image_data;
  art_buffer_p art_buffer;
  art_context_p art_context;
#else
  GdkPixmap *current_pixmap;
  GdkGC *drawing_gc;
#endif
  GdkPixmap *drawing_pixmap;
  GtkObject *zoom_adjustment;
  GtkAdjustment *x_scroll_adj;
  GtkAdjustment *y_scroll_adj;
  int button_two_down;
  void (*user_draw_routine)();
  GtkSignalFunc motion_handler;
  GtkSignalFunc button_release_handler;
  GtkSignalFunc button_press_handler;
} GtkMapViewer;

typedef void (* drawing_func)(GtkMapViewer *map_view);


GtkMapViewer *carmen_map_graphics_new_viewer(int x_size, int y_size,
					     double initial_zoom);

void carmen_map_graphics_add_drawing_func(GtkMapViewer *map_view, 
					  drawing_func new_func);
void carmen_map_graphics_add_map(GtkMapViewer *map_view, carmen_map_p new_map, 
				 int new_flags);
void carmen_map_graphics_modify_map(GtkMapViewer *map_view, float *data, 
				    int new_flags);

void carmen_map_graphics_adjust_scrollbars(GtkMapViewer *map_view, 
					   carmen_world_point_p new_centre);

void carmen_map_graphics_redraw(GtkMapViewer *map_view);
void carmen_map_graphics_draw_arc(GtkMapViewer *map_view, GdkColor *colour, 
				  int filled, carmen_world_point_p world_point,
				  double radius,int start, int delta);

void carmen_map_graphics_draw_point(GtkMapViewer *map_view, GdkColor *colour,
				    carmen_world_point_p world_point);

void carmen_map_graphics_draw_line(GtkMapViewer *map_view, GdkColor *colour, 
				   carmen_world_point_p start, 
				   carmen_world_point_p end);
void carmen_map_graphics_draw_polygon(GtkMapViewer *map_view, GdkColor *colour,
				      carmen_world_point_t *points, 
				      int num_points, int filled);
void carmen_map_graphics_draw_ellipse(GtkMapViewer *map_view, GdkColor *colour,
				      carmen_world_point_p mean, 
				      double x_variance,
				      double covariance, double y_variance, 
				      double k);

void carmen_map_graphics_draw_circle(GtkMapViewer *map_view, GdkColor *colour, 
				     int filled, 
				     carmen_world_point_p world_point, 
				     double radius);

void carmen_map_graphics_draw_rectangle(GtkMapViewer *map_view, 
					GdkColor *colour, 
					int filled, carmen_world_point_p start,
					carmen_world_point_p end);

void carmen_map_graphics_draw_string(GtkMapViewer *map_view, GdkColor *colour, 
				     GdkFont *font, int x, int y, 
				     const char *string);
  
void carmen_map_graphics_add_motion_event(GtkMapViewer *internal_map_view, 
					  GtkSignalFunc motion_handler);
void carmen_map_graphics_add_button_release_event(GtkMapViewer 
						  *internal_map_view, 
						  GtkSignalFunc 
						  button_release_handler);

void 
carmen_map_graphics_add_button_press_event(GtkMapViewer *map_view, 
					   GtkSignalFunc button_press_handler);

carmen_map_p carmen_map_image_to_map( GdkImlibImage* im, double resolution);
carmen_map_p carmen_map_imagefile_to_map(char *filename, double resolution);

#ifdef __cplusplus
}
#endif

#endif
