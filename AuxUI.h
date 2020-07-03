#pragma once
#include <gtk/gtk.h>
#include "MapResources.h"
#include "StuffBookUI.h"
#include "ImgPackUI.h"
#include <string>

namespace ui {
	/*!
		Auxiliary graphics module
		=========================

		This class represents all auxiliary graphical interface.
	 */
	class AuxUI
	{
	private:
		static GObject* gtkEntryThingSearch;
		static GObject* gtkEntryImgSearch;
		static GObject* gtkStatusBar;
		GObject* gtkProgressBar;

		GtkCssProvider* provider;
		GdkDisplay* display;
		GdkScreen* screen;
		GtkStyleContext* context;

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static gboolean static_cb_removeMsg(gpointer data);
	public:
		
		/**
		 *  constructor
		 */
		AuxUI();

		/**
		 *  @brief This method gets GtkStatusbar.
		 */
		static GtkStatusbar* getStatusBar() { return GTK_STATUSBAR(gtkStatusBar); }
		
		/**
		 *  @brief This method searches for a thing by name.
		 *  If find then select it into user interface GtkTreeView
		 *  @param name name to be searched.
		 *  @return true if found and false otherwise.
		 */
		static bool searchThingByName(std::string name);

		/**
		 *  @brief This method cleans text input when focused.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		static void cb_onFocusInEvent(GtkWidget* widget, gpointer data);
		
		/**
		 *  @brief This method cleans text on imgByName user interface when focused..
		 */
		static void cb_onFocusInEventImgByNameTextEntry(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method add text input "..." when not focused.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		static void cb_onFocusOutEvent(GtkWidget* widget, gpointer data);


		/**
		 *  @brief This method add text input on imgByName user interface "..." when not focused.
		 */
		static void cb_onFocusOutEventImgByNameTextEntry(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method call searchThingByName when activated.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 *  @see searchThingByName
		 */
		static void cb_onActive(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method call searchImgByName when activated.
		 */
		static void cb_onActiveImgSearch(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method searches for a ImgObj into the ImgPack user interface.
		 *  If found then select.
		 */
		static bool searchImgByName(std::string name);

		/**
		 *  @brief This method print msg into the GtkStatusBar widget.
		 */
		void printMsg(std::string msg);

		/**
		 *  @brief This method removes the last msg from stack.
		 */
		gboolean removeMsg(gpointer data);

		/**
		 *  @brief This method updates progress bar widget with value from [0,100].
		 */
		void updateProgressBar(double percentage);
	};

}
