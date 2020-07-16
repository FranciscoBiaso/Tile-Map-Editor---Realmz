#pragma once

#include <gtk/gtk.h>
#include <string>
#include "Thing.h"
#include "MapResources.h"
#include "StuffBookUI.h"
#include <json/json.h>
#include <iostream>

namespace data { class ImgObj; }

namespace ui {
	/*!
		THING CREATOR MODULE
		====================

		This class represents all graphical interface necessary to create a thing obj.
	 */
	class ThingCreatorTool
	{
		enum ThingIsReady : int {
			THING_EMPTY = 0,
			THING_TYPE_IS_READY = 1,
			THING_NAME_IS_READY = 2,
			THING_PIXELBUF_IS_READY = 4,
			THING_IS_READY = 7
		};

	private:
		static GObject* gtkEntryThingName;        /* label to enter the name of thing [creating a new name] */
		//static GObject* gtkEntryThingImg;         /* label to enter the name of img from imgBook */	
		static GObject* gtkTreeViewThingType;     /* shows thing types */
		static GObject* gtkTreeViewThingObj;      /* show thing atributes */
		static GObject* gtkButtonCreateThing;     /* create thing [add to stuffBook] */
		static GObject* gtkFrameThingImgView;     /* gtkFrame */
		

		static data::Thing thing; // thing structure 

		static GdkPixbuf* pixelRegion;   
		static GdkPixbuf* pixelRegionBackground;		
		static GtkWidget* drawingArea;

		void createTreeViewThingObj();
		void createTreeViewThingType();
		static GtkTreeModel* fillTreeThingObj();
		static GtkTreeModel* fillTreeThingType();

		int thingIsReadyToSelect;
		bool isThingReadyToSelect();

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static void static_cb_createThing(GtkWidget* widget, gpointer data);
		static void static_cb_updateThingType(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);
		static void static_cb_updateThingName(GtkWidget* widget, gpointer data);
	public:
		/*
		 * constructor
		*/
		ThingCreatorTool();

		/**
		 *  @brief This method update user interface tree view thing obj.
		 */
		static void updateTreeThingObj();

		/**
		 *  @brief This method update user interface tree view thing type.
		 */
		static void updateTreeThingType();

		/**
		 *  @brief This method create a thing, update user interface then select the new thing into the tree view.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		void cb_createThing(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method change a thing name when key are pressed from user interface thing name.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		void cb_updateThingName(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method change a thing type when 2x click into user interface tree view thing type.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		void cb_updateThingType(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);

		/**
		 *  @brief This method represents a redenring callback of Thing ImgObj.
		 */
		static  gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);

		/**
		 *  @brief This method updates ImgObj pixel area of a Thing.
		 */
		void updateImgPixelArea(data::ImgObj*);
	};
}

