#pragma once

#include "Map.h"
#include "Vec2.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
namespace data { class Thing; }
namespace ui {

	/*!
	MapUI class
	===========

	This class represents the map view user interface.
	*/

	class MapUI: public scene::Map
	{
	private:
		 GtkWidget* drawingArea; // widget to draw //
		 GObject* gtkMapViewPort; // container to draw the map //
		 int viewWidth, viewHeight;
		 data::Thing* drawObj;
		 math::Vec2<int> mousePosition;

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		 static void static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);

		 /**
		  *  @brief Auxiliary function to avoid static members.
		  */
		 static gboolean static_cb_clickNotify(GtkWidget* widget, GdkEventButton* event, gpointer user_data);

		 /**
		  *  @brief Auxiliary function to avoid static members.
		  */
		 static gboolean static_cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer user_data);
	public:
		/**
		 * constructor.
		 */
		MapUI(std::string name, int width, int height);

		/**
		 *  @brief This method represents a redenring callback.
		 */
		gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);

		/**
		 * @brief This method add a Thing into the map where mouse is positioned, when 1x left - click.
		 */
		gboolean cb_clickNotify(GtkWidget* widget, GdkEventButton* event, gpointer user_data);

		/**
		 * @brief This method selects a Thing to be drawn into the map.
		 */
		void setDrawObj(data::Thing* thing);

		/**
		 * @brief This method selects a Thing to be drawn into the map.
		 */
		gboolean cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer user_data);

		/**
		 * @brief This method draws the map grid.
		 */
		void drawGrid(cairo_t* cr, int w, int h, int gridSize);


		static void static_my_getsize(GtkWidget* widget, GtkAllocation* allocation, void* data)
		{

			return reinterpret_cast<MapUI*>(data)->my_getsize(widget, allocation, allocation);
		}

		void my_getsize(GtkWidget* widget, GtkAllocation* allocation, void* data)
		{
			viewWidth = allocation->width;
			viewHeight = allocation->height;
			gtk_widget_queue_draw(drawingArea);
		}
	};
}

