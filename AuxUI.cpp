#include "AuxUI.h"


// class static members //
GObject* ui::AuxUI::gtkEntryThingSearch = nullptr;
GObject* ui::AuxUI::gtkEntryImgSearch = nullptr;
GObject* ui::AuxUI::gtkStatusBar = nullptr;

// externs //
namespace GtkUserInterface { extern GtkBuilder* builder; }
extern data::MapResources* gResources;
extern ui::StuffBookUI* gStuffBook;
extern ui::ImgPackUI* gImgPackUI;

ui::AuxUI::AuxUI()
{
    // loading interfaces from xml //
    gtkEntryThingSearch = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryThingSearch");
    gtkEntryImgSearch = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryImgSearch");
    gtkStatusBar = gtk_builder_get_object(GtkUserInterface::builder, "gtkStatusBar");
    // callbacks //
    g_signal_connect(gtkEntryThingSearch, "activate", G_CALLBACK(cb_onActive), NULL);
    g_signal_connect(gtkEntryImgSearch, "activate", G_CALLBACK(cb_onActiveImgSearch), NULL);
    g_signal_connect(gtkEntryThingSearch, "focus-in-event", G_CALLBACK(cb_onFocusInEvent), NULL);
    g_signal_connect(gtkEntryThingSearch, "focus-out-event", G_CALLBACK(cb_onFocusOutEvent), NULL);
    g_signal_connect(gtkEntryImgSearch, "focus-in-event", G_CALLBACK(cb_onFocusInEventImgByNameTextEntry), NULL);
    g_signal_connect(gtkEntryImgSearch, "focus-out-event", G_CALLBACK(cb_onFocusOutEventImgByNameTextEntry), NULL);
}


void ui::AuxUI::cb_onActive(GtkWidget* widget, gpointer data)
{
    searchThingByName(std::string(gtk_entry_get_text(GTK_ENTRY(gtkEntryThingSearch))));
}

void ui::AuxUI::cb_onActiveImgSearch(GtkWidget* widget, gpointer data)
{
    searchImgByName(std::string(gtk_entry_get_text(GTK_ENTRY(gtkEntryImgSearch))));
}


void ui::AuxUI::cb_onFocusInEvent(GtkWidget* widget, gpointer data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryThingSearch));
    gtk_entry_buffer_delete_text(gtkEntryBuffer, 0, gtk_entry_buffer_get_length(gtkEntryBuffer));    
}

void ui::AuxUI::cb_onFocusInEventImgByNameTextEntry(GtkWidget* widget, gpointer data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryImgSearch));
    gtk_entry_buffer_delete_text(gtkEntryBuffer, 0, gtk_entry_buffer_get_length(gtkEntryBuffer));
}

void ui::AuxUI::cb_onFocusOutEvent(GtkWidget* widget, gpointer data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryThingSearch));
    std::string buffer = "...";
    gtk_entry_buffer_set_text(gtkEntryBuffer, buffer.c_str(), buffer.size());
}

void ui::AuxUI::cb_onFocusOutEventImgByNameTextEntry(GtkWidget* widget, gpointer data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryImgSearch));
    std::string buffer = "...";
    gtk_entry_buffer_set_text(gtkEntryBuffer, buffer.c_str(), buffer.size());
}

bool ui::AuxUI::searchThingByName(std::string name)
{  
    size_t ret = name.find(',');
    std::string partThingType; // Thing Type
    bool useSpecification = false;
    // in this case the user wants to find using char ',' //
    if (ret != std::string::npos)
    {
        partThingType = name.substr(ret + 1, name.size());
        name = name.substr(0, ret);
        useSpecification = true;
    }

    gint row_count = 0; // count rows, not used  //
    gboolean valid; // check error //
    GtkTreeIter iter; // our node to iterate through the model //
    GtkTreeModel* model = gtk_tree_view_get_model(gStuffBook->getTreeViewStuffBook()); // grab model //

    /* Get the first iter in the list */
    valid = gtk_tree_model_get_iter_first(model, &iter);

    bool founded = false; // start as not founded //
    gchar* str_data;
    while (valid && !founded) // while iterating and not founded //
    {
        GtkTreeIter iterChildren;
        gboolean validChildren;

        if (useSpecification)
        {
            gtk_tree_model_get(model, &iter, 0, &str_data, -1); // grab data from model, check root //
            
            if (std::string(str_data) != partThingType)
            {
                valid = gtk_tree_model_iter_next(model, &iter); // jump to next data //
                row_count++; // increment row //
                continue;
            }
        }
        validChildren = gtk_tree_model_iter_children(model, &iterChildren, &iter);
        while (validChildren && !founded) // iterate through children //
        {
            gtk_tree_model_get(model, &iterChildren, 0, &str_data, -1); // grab data from model //

            if (std::string(str_data) == name) // compare //
            {
                GtkTreePath* path = gtk_tree_model_get_path(model, &iter);
                gtk_tree_view_expand_row(gStuffBook->getTreeViewStuffBook(), path, false); // expand row //
                GtkTreePath* pathCell = gtk_tree_model_get_path(model, &iterChildren);
                gtk_tree_selection_select_iter(gtk_tree_view_get_selection(gStuffBook->getTreeViewStuffBook()), &iterChildren); // select item //
                gtk_tree_view_scroll_to_cell(gStuffBook->getTreeViewStuffBook(), pathCell, NULL, false, 1.0, 1.0); // adjust scroll window //                
                gtk_tree_path_free(path);
                founded = true; // we founded //
            }
            g_free(str_data); // clean //

            validChildren = gtk_tree_model_iter_next(model, &iterChildren); // jump to next data //
        }

        valid = gtk_tree_model_iter_next(model, &iter); // jump to next data //
        row_count++; // increment row //
    }

    // print msg //
    if (!founded)
    {
        std::string statusMsg = "Thing [" + name + "] not founded!";
        gtk_statusbar_push(GTK_STATUSBAR(gtkStatusBar), 0, statusMsg.c_str());
    }

    return founded;
}

bool ui::AuxUI::searchImgByName(std::string name)
{
    gint row_count = 0; // count rows, not used  //
    GtkTreeIter iter; // our node to iterate through the model //
    GtkTreeModel* model = gtk_tree_view_get_model(gImgPackUI->getTreeView()); // grab model //
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter); // check error //
    bool founded = false; // start as not founded //
    
    gchar* str_data;
    while (valid && !founded) // while iterating and not founded //
    {
        gtk_tree_model_get(model, &iter, 0, &str_data, -1); // grab data from model //
        if (std::string(str_data) == name)
        {
            GtkTreePath* path = gtk_tree_model_get_path(model, &iter);
            GtkTreePath* pathCell = gtk_tree_model_get_path(model, &iter);
            gtk_tree_selection_select_iter(gtk_tree_view_get_selection(gImgPackUI->getTreeView()),&iter); // select item //
            gtk_tree_view_scroll_to_cell(gImgPackUI->getTreeView(), pathCell, NULL, false, 1.0, 1.0); // adjust scroll window //
            gtk_tree_path_free(path);
            founded = true;
        }
        valid = gtk_tree_model_iter_next(model, &iter); // jump to next data //
        row_count++; // increment row //       
    }

    // print msg //
    if (!founded)
    {
        std::string statusMsg = "ImgObj [" + name + "] not founded!";
        gtk_statusbar_push(GTK_STATUSBAR(gtkStatusBar), 0, statusMsg.c_str());
    }

    return founded;
}

void ui::AuxUI::printMsg(std::string msg)
{
    gtk_statusbar_push(GTK_STATUSBAR(gtkStatusBar), 0, msg.c_str());
    g_timeout_add(MSG_VISIBLE_TIME, static_cb_removeMsg, this);
}

gboolean ui::AuxUI::static_cb_removeMsg(gpointer data)
{
    return reinterpret_cast<AuxUI*>(data)->removeMsg(data);
}

gboolean ui::AuxUI::removeMsg(gpointer data)
{    
    gtk_statusbar_pop(GTK_STATUSBAR(gtkStatusBar), 0);
    return FALSE; // only once //
}
