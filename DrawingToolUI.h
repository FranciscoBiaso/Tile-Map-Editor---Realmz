#pragma once
#include "Definitions.h"
#include <gtk/gtk.h>

namespace ui {
	/*!
		Drawing Tool Module
		===================

		This class represents all drawing tool user interface.
	 */

	class DrawingToolUI
	{
	private:
		GObject* brush;
		GObject* eraser;
		def::DrawingMode drawingMode;

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static void static_cb_signalGtkToggleButtonBrush(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static void static_cb_signalGtkToggleButtonEraser(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 *  @brief This methods checks if all toggle button are false.
		 */
		bool areAllToogleFalse();
	public:
		/**
		 * constructor.
		 */
		DrawingToolUI();
		
		/**
		 * @brief This methods gets the drawing mode.
		 */
		def::DrawingMode getDrawingMode() const;

		/**
		 * @brief This method occurs when the toggle button brush is triggered.
		 */
		void cb_signalGtkToggleButtonBrush(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 * @brief This method occurs when the toggle button erase is triggered.
		 */
		void cb_signalGtkToggleButtonEraser(GtkToggleButton* togglebutton, gpointer user_data);
	};

}

