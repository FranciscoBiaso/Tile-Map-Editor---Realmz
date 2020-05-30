#include "GraphicsTool.h"

GObject* ui::GraphicsTool::gtkFileChooserButtonImg = nullptr;
GObject* ui::GraphicsTool::gtkImgInput = nullptr;
GObject* ui::GraphicsTool::gtkFrameImg = nullptr;
GObject* ui::GraphicsTool::gtkFrameImgView = nullptr;
GObject* ui::GraphicsTool::gtkEntryImgName = nullptr;
GObject* ui::GraphicsTool::gtkButtonAddImgName = nullptr;
GObject* ui::GraphicsTool::gtkTreeViewImgObj = nullptr;
GObject* ui::GraphicsTool::gtkButtonCreateImg = nullptr;
GObject* ui::GraphicsTool::gtkScrolledWindowTileset = nullptr;
GObject* ui::GraphicsTool::gtkCheckButtonGrid = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton32x32 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton32x64 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton64x32 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton64x64 = nullptr;
GObject* ui::GraphicsTool::gtkViewportImgSrc = nullptr;

GtkWidget* ui::GraphicsTool::drawingAreaImgSrc = nullptr;
GtkWidget* ui::GraphicsTool::drawingAreaImgDst = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBufImgSrc = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBufImgDest = nullptr;

int ui::GraphicsTool::xPosHighlightSquare = 0;
int ui::GraphicsTool::yPosHighlightSquare = 0;
int ui::GraphicsTool::MAX_IMG_WIDGET_WIDTH = 320;
int ui::GraphicsTool::MAX_IMG_WIDGET_HEIGHT = 320;
int ui::GraphicsTool::imgCursor = 0;
bool ui::GraphicsTool::canDrawSelectedSquare = false;

cairo_surface_t*  ui::GraphicsTool::surfaceScr = nullptr;
cairo_surface_t* ui::GraphicsTool::surfaceDst = nullptr;

namespace GtkUserInterface { extern GtkBuilder* builder;}

ui::GraphicsTool::GraphicsTool()
{
    // initialize timer //
    g_timeout_add(600, timerChangeSquareData, NULL);

    gtkFileChooserButtonImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkFileChooserButtonImg");
    gtkImgInput = gtk_builder_get_object(GtkUserInterface::builder, "gtkImgInput");
    gtkFrameImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkFrameImg");
    gtkFrameImgView = gtk_builder_get_object(GtkUserInterface::builder, "gtkFrameImgView");
    gtkCheckButtonGrid = gtk_builder_get_object(GtkUserInterface::builder, "gtkCheckButtonGrid");
    gtkViewportImgSrc = gtk_builder_get_object(GtkUserInterface::builder, "gtkViewportImgSrc");
    /* toggle buttons */
    gtkToggleButton32x32 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton32x32");
    gtkToggleButton32x64 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton32x64");
    gtkToggleButton64x32 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton64x32");
    gtkToggleButton64x64 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton64x64");
    /* preset toggle buttons state */
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
    /* connect toggle buttons state */
    g_signal_connect(gtkToggleButton32x32, "toggled", G_CALLBACK(cb_signalGtkToggleButton32x32), gtkToggleButton32x32);
    g_signal_connect(gtkToggleButton32x64, "toggled", G_CALLBACK(cb_signalGtkToggleButton32x64), gtkToggleButton32x64);
    g_signal_connect(gtkToggleButton64x32, "toggled", G_CALLBACK(cb_signalGtkToggleButton64x32), gtkToggleButton64x32);
    g_signal_connect(gtkToggleButton64x64, "toggled", G_CALLBACK(cb_signalGtkToggleButton64x64), gtkToggleButton64x64);

    // alocate memory for img pixel buf
    pixelBufImgDest = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE * 2, REALMZ_GRID_SIZE * 2);

    setDrawingAreaImgDst(GTK_WIDGET(gtkFrameImgView));

    std::string path = g_win32_get_package_installation_directory_of_module(NULL);
    path += "\\imgs\\";
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), path.c_str());
    gtk_file_chooser_set_action(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), GTK_FILE_CHOOSER_ACTION_OPEN);
    //gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), TRUE);

    g_signal_connect(gtkFileChooserButtonImg, "file-set", G_CALLBACK(cb_onFileSet), gtkViewportImgSrc);

    /* bind check button to grid action (show or hide) */
    g_signal_connect(gtkCheckButtonGrid, "toggled", G_CALLBACK(cb_toggleButtonChangeGrid), NULL);

}

gboolean ui::GraphicsTool::cb_MotionNotify(GtkWidget* widget, GdkEventMotion* e, gpointer   user_data)
{
    xPosHighlightSquare = (int)(e->x/REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    yPosHighlightSquare = (int)(e->y/REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    if (drawingAreaImgSrc != nullptr)
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgSrc));
    return TRUE;    
}

gboolean ui::GraphicsTool::cb_clickNotify(GtkWidget* widget, GdkEventButton* event, gpointer   user_data)
{
    // double click //
    if (event->type == GDK_DOUBLE_BUTTON_PRESS)
    {      
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2);
            setDstSurfaceFromDstPixelbuf();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2,imgCursor * REALMZ_GRID_SIZE);
            setDstSurfaceFromDstPixelbuf();
            rightShiftCursor();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2);
            setDstSurfaceFromDstPixelbuf();
            rightShiftCursor();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        {
            if(imgCursor <= 1)
                gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, imgCursor * REALMZ_GRID_SIZE, 0);
            else
                gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, (imgCursor-2) * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
            setDstSurfaceFromDstPixelbuf();
            rightShiftCursor();
        }

        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
    }

    return TRUE;
}

void ui::GraphicsTool::cb_toggleButtonChangeGrid(GtkToggleButton* togglebutton, gpointer data)
{
    if (drawingAreaImgSrc != nullptr)
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgSrc));
}

gboolean ui::GraphicsTool::cb_draw_callback_img_src(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    // highlight Square color //
    GdkRGBA HLSColor; HLSColor.red = 0; HLSColor.green = 0; HLSColor.blue = 0.85; HLSColor.alpha = 0.25;
    // backgrond color //
    GdkRGBA bgColor; bgColor.red = 0.5; bgColor.green = 0.5; bgColor.blue = 0.5; bgColor.alpha = 0.5;

    // draw background //
    cairo_set_source_rgba(cr, bgColor.red, bgColor.green,bgColor.blue,bgColor.alpha);
    cairo_paint(cr);

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);
    
    //int w = cairo_image_surface_get_width(surface);
    //int h = cairo_image_surface_get_height(surface);

    if (surfaceScr != nullptr)
    {
        // draw surface into widget region //
        cairo_set_source_surface(cr, surfaceScr, 0, 0);
        cairo_paint(cr);
    }

    // Draw the highlighted square //
    drawSquare(cr, xPosHighlightSquare, yPosHighlightSquare,REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);

    // draw grid //
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkCheckButtonGrid)) == TRUE)
        drawGrid(cr, width, height, REALMZ_GRID_SIZE);

    return FALSE;
}

gboolean ui::GraphicsTool::cb_draw_callback_img_dst(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    // highlight Square color //
    GdkRGBA HLSColor; HLSColor.red = 0; HLSColor.green = 0; HLSColor.blue = 0.85; HLSColor.alpha = 0.25;
    // backgrond color //
    GdkRGBA bgColor; bgColor.red = 0.5; bgColor.green = 0.5; bgColor.blue = 0.5; bgColor.alpha = 0.5;
    // square color //
    GdkRGBA sColor; sColor.red = 0.35; sColor.green = 0.35; sColor.blue = 0.35; sColor.alpha = 0.35;

    // draw background //
    cairo_set_source_rgba(cr, bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
    cairo_paint(cr);

    // draw background square (32x32,32x64,64x32 or 64x64)
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)       drawImg32x32(cr, sColor);
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)  drawImg32x64(cr, sColor);
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)  drawImg64x32(cr, sColor);
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)  drawImg64x64(cr, sColor);
    
    if (surfaceDst != nullptr)
    {
        cairo_set_source_surface(cr, surfaceDst, 0, 0);
        cairo_paint(cr);
    }

    // highligh square //
    if (canDrawSelectedSquare)
    {
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
        {
            drawSquare(cr, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
        {
            drawSquare(cr, REALMZ_GRID_SIZE / 2, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        {
            drawSquare(cr, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        {
            if (imgCursor <= 1)
                drawSquare(cr, imgCursor * REALMZ_GRID_SIZE, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
            else
                drawSquare(cr, (imgCursor - 2) * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
    }
    return FALSE;
}

ui::GraphicsTool::FileReturnMsg ui::GraphicsTool::loadImgFromFile()
{
    gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtkFileChooserButtonImg));
    if (filename == NULL)
        return NO_FILE_iS_SELECTED;

    pixelBufImgSrc = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(filename)));
    if (pixelBufImgSrc == NULL)
        return IMAGE_IS_EMPTY;

    return FILE_OK;
}

void ui::GraphicsTool::setSrcSurfaceFromScrPixelbuf()
{
    surfaceScr = gdk_cairo_surface_create_from_pixbuf(pixelBufImgSrc, 0, NULL);
}

void ui::GraphicsTool::setDstSurfaceFromDstPixelbuf()
{
    surfaceDst = gdk_cairo_surface_create_from_pixbuf(pixelBufImgDest, 0, NULL);
}

void ui::GraphicsTool::setDrawingAreaImgDst(GtkWidget* widget)
{
    if (drawingAreaImgDst == nullptr)
    {
        drawingAreaImgDst = gtk_drawing_area_new();
        gtk_container_add(GTK_CONTAINER(widget), drawingAreaImgDst);

        g_signal_connect(G_OBJECT(drawingAreaImgDst), "draw", G_CALLBACK(cb_draw_callback_img_dst), NULL);
    }
}

void ui::GraphicsTool::setDrawingAreaImgScr(GtkWidget * widget)
{
    drawingAreaImgSrc = gtk_drawing_area_new();
    gtk_widget_add_events(drawingAreaImgSrc, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(drawingAreaImgSrc, GDK_BUTTON_PRESS_MASK);

    gtk_container_add(GTK_CONTAINER(widget), drawingAreaImgSrc);

    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "draw", G_CALLBACK(cb_draw_callback_img_src), NULL);

    /* grab motion event */
    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "motion-notify-event", G_CALLBACK(cb_MotionNotify), NULL);
    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "button-press-event", G_CALLBACK(cb_clickNotify), NULL);

    gtk_widget_show_all(GTK_WIDGET(gtk_widget_get_parent(drawingAreaImgSrc)));
}

void ui::GraphicsTool::cb_onFileSet(GtkFileChooserButton* widget, gpointer data)
{
    if (loadImgFromFile() != FILE_OK)
    {
        std::cout << "Error to load img from file!"<<std::endl;// used until create an txt msg manipulator //
        return;
    }

    setSrcSurfaceFromScrPixelbuf();
    if (drawingAreaImgSrc == nullptr)
        setDrawingAreaImgScr(GTK_WIDGET(data));
    int width = gdk_pixbuf_get_width(pixelBufImgSrc);
    int height = gdk_pixbuf_get_height(pixelBufImgSrc);
    gtk_widget_set_size_request(GTK_WIDGET(drawingAreaImgSrc), width, height);
    gtk_widget_set_size_request(GTK_WIDGET(gtkFrameImg), std::min(width,MAX_IMG_WIDGET_WIDTH), std::min(height,MAX_IMG_WIDGET_HEIGHT));
    gtk_widget_show_all(GTK_WIDGET(gtk_widget_get_parent(drawingAreaImgSrc)));
}

void ui::GraphicsTool::cb_signalGtkToggleButton32x32(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
       
    }
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::cb_signalGtkToggleButton32x64(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);        
    }
    
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::cb_signalGtkToggleButton64x32(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
    }
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::cb_signalGtkToggleButton64x64(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
    }
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::drawGrid(cairo_t* cr, int w, int h, int gridSize)
{
    cairo_set_line_width(cr, 2);
    //double dashs[2] = { 1.0, 3.0 };
    //cairo_set_dash(cr, dashs, 2, 0);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_set_source_rgba(cr, 0.95,0.95,0.1,0.95);

    int countRows = h / gridSize;
    int countCols = w / gridSize;
    for (int l = 0; l <= countRows; l++)
    {
        // draw vertical line //
        cairo_move_to(cr, 0, l * gridSize);
        cairo_line_to(cr, w, l * gridSize);

        for (int c = 0; c <= countCols; c++)
        {
            cairo_move_to(cr, c * gridSize, 0);
            cairo_line_to(cr, c * gridSize, h);
        }

    }
    cairo_stroke(cr);
    cairo_fill(cr);
}

void ui::GraphicsTool::drawSquare(cairo_t* cr, int x, int y, int w, int h, GdkRGBA color)
{
    cairo_rectangle(cr, x, y, w, h);
    cairo_set_source_rgba(cr, color.red, color.green, color.blue , color.alpha);
    cairo_fill(cr);
}

void ui::GraphicsTool::drawImg32x32(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, REALMZ_GRID_SIZE/2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::drawImg32x64(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, REALMZ_GRID_SIZE / 2,0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::drawImg64x32(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, 0, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::drawImg64x64(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::rightShiftCursor()
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        imgCursor = (imgCursor+1) % 2;  // [0,1]
    else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        imgCursor = (imgCursor + 1) % 4; // [0,1,2,3]
}

void ui::GraphicsTool::leftShiftCursor()
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
    {
        if (imgCursor == 0)
            imgCursor = 1;
        else
            imgCursor = (imgCursor - 1) % 2;  // [0,1]
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
    {
        if (imgCursor == 0)
            imgCursor = 3;
        else
            imgCursor = (imgCursor - 1) % 2;  // [0,1]
    }       
}

void ui::GraphicsTool::resetCursor()
{
    imgCursor = 0;
}

gboolean ui::GraphicsTool::timerChangeSquareData(gpointer data)
{
    if (canDrawSelectedSquare)
        canDrawSelectedSquare = false;
    else
        canDrawSelectedSquare = true;
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));

    return TRUE;
}