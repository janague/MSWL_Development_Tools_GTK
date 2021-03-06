/*-
 * Copyright (c) 2013 janague <janague@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $gPad$
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

//_____________________________________________________________________________
// Configuration
//
// Version
#define GP_VERSION "0.2.0"

// Set default values
#define GP_DEFAULT_WIDTH 600
#define GP_DEFAULT_HEIGHT 400

// set the buffer size that it uses for reading the file.
#define GP_BUFFER_SIZE 1024

//_____________________________________________________________________________
// Evente handler
//
// on_delete_event: Manage delete event
static gboolean
on_delete_event (   GtkWidget *widget,
                    GdkEvent *event,
                    gpointer user_data);

// New file
void
on_new_file_menu (GtkAction *action,
                  GtkWidget *window);

// Open a file    
void
on_open_file_menu (GtkAction *action,
                   GtkWidget *window);

// save as file    
void
on_save_as_file_menu (GtkAction *action,
                   GtkWidget *window);


// TODO: Remove this global variable
GtkTextBuffer *gBuffer;

// Show about window
static void about_cb (  GtkAction *action,
                        GtkWidget *window);


//_____________________________________________________________________________
// Widget configuration
//
// Menus
static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "_File" },               /* name, stock id, label */
  { "HelpMenu", NULL, "_Help" },               /* name, stock id, label */
  { "New", GTK_STOCK_NEW,                      /* name, stock id */
    "_New", "<control>N",                      /* label, accelerator */
    "Create a new file",                       /* tooltip */
    G_CALLBACK (on_new_file_menu) },
  { "Open", GTK_STOCK_OPEN,                    /* name, stock id */
    "_Open","<control>O",                      /* label, accelerator */
    "Open a file",                             /* tooltip */
    G_CALLBACK (on_open_file_menu) },
  { "Save", GTK_STOCK_SAVE,                    /* name, stock id */
    "_Save","<control>S",                      /* label, accelerator */
    "Save current file",                       /* tooltip */
    G_CALLBACK (on_save_as_file_menu) },
  { "SaveAs", GTK_STOCK_SAVE,                  /* name, stock id */
    "Save _As...", NULL,                       /* label, accelerator */
    "Save to a file",                          /* tooltip */
    G_CALLBACK (on_save_as_file_menu) },
  { "Quit", GTK_STOCK_QUIT,                    /* name, stock id */
    "_Quit", "<control>Q",                     /* label, accelerator */
    "Quit",                                    /* tooltip */
    G_CALLBACK (on_delete_event) },
  { "About", NULL,                             /* name, stock id */
    "_About", "<control>A",                    /* label, accelerator */
    "About",                                   /* tooltip */
    G_CALLBACK (about_cb) },
};
static guint n_entries = G_N_ELEMENTS (entries);

// User interface definition
static const gchar *ui_info =
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <menuitem action='New'/>"
"      <menuitem action='Open'/>"
"      <menuitem action='Save'/>"
"      <menuitem action='SaveAs'/>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='HelpMenu'>"
"      <menuitem action='About'/>"
"    </menu>"
"  </menubar>"
"</ui>";


// read_file: Read a file using standard io library
void read_file ( char *filename,
                 GtkTextBuffer *buffer); 

// Main function
int 
main(int argc, char **argv)
{
    gtk_init (&argc, &argv);

    GtkWidget *window;
    GtkWidget *textView;
    GtkWidget *sWindow; // scrolled window
    GtkWidget *box;
    GtkUIManager *ui; // user interface
    GtkActionGroup *actions; // actions
    GError *error = NULL;

    GtkTextBuffer *buffer;

    // Create a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Set title
    gtk_window_set_title(   GTK_WINDOW(window),
                            "gPad");
    // Set default size of the window
    gtk_window_set_default_size (GTK_WINDOW (window),
                                GP_DEFAULT_WIDTH,
                                GP_DEFAULT_HEIGHT);

    // Set menu actions
    actions = gtk_action_group_new ("Actions");
    gtk_action_group_add_actions (actions, entries, n_entries, NULL);

    // Create user interface manager
    ui = gtk_ui_manager_new ();
    gtk_ui_manager_insert_action_group (ui, actions, 0);
    g_object_unref (actions);
    gtk_window_add_accel_group (GTK_WINDOW (window),
                                gtk_ui_manager_get_accel_group (ui));

    if (!gtk_ui_manager_add_ui_from_string (ui, ui_info, -1, &error))
    {
        g_message ("building menus failed: %s", error->message);
        g_error_free (error);
    }

    // Create a box
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (window), box);

    gtk_box_pack_start (GTK_BOX (box),
                        gtk_ui_manager_get_widget (ui, "/MenuBar"),
                        FALSE, FALSE, 0);

    // Connect signal when destroy event is threw
    g_signal_connect (  window,
                        "destroy",
                        G_CALLBACK (gtk_widget_destroyed), &window);

    // Connect signal when delete-event is threw
    g_signal_connect (  window,
                        "delete-event",
                        G_CALLBACK (on_delete_event),
                        NULL);

    // Create a text view
    textView = gtk_text_view_new ();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textView));
    gBuffer = buffer;

    // Create a scrolled window
    sWindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sWindow),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    // Add scrolled window in application window
    // gtk_container_add(GTK_CONTAINER (window), sWindow);
    // Add scrolled window in box
    gtk_box_pack_start (GTK_BOX (box), sWindow, TRUE, TRUE, 0);
    // Add text view in scrolled window
    gtk_container_add(GTK_CONTAINER (sWindow), textView);

    // Read the file
    // TODO: Include management of arguments.
    read_file(argv[1], buffer);

    // Show widgets
    // Show menus
    // gtk_widget_show(ui);
    // Show text view
    gtk_widget_show (textView);
    // Show scrolled window
    gtk_widget_show (sWindow);
    // Show window
    gtk_widget_show (window);

    gtk_widget_show (box);
   // gtk_widget_show_all (window);
   g_object_unref (ui);

    gtk_main();

    return 0;
}


//_____________________________________________________________________________
// read_file - Read text file and write in GtkTextBuffer
//      filename: path + file name
//      buffer: pointer to a buffer
void read_file ( char *filename,
                 GtkTextBuffer *buffer)
{
    char buf[GP_BUFFER_SIZE];
    FILE *inFile;

    GtkTextMark *mark;
    GtkTextIter iter;

    // Open the file for reading
    inFile=fopen(filename,"rw");

    if(inFile==NULL)
    {
        printf("An error occoured!");
        return;
    }

    // Read file in block of GP_BUFFER_SIZE
    while (fgets(buf, sizeof buf, inFile) != NULL)
    {
        mark = gtk_text_buffer_get_insert (buffer);
        gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
        gtk_text_buffer_insert (buffer, &iter, buf, -1);
    }

    // Close the file
    fclose(inFile);
}


//_____________________________________________________________________________
static void
about_cb (GtkAction *action,
          GtkWidget *window)
{
  const gchar *authors[] = {
    "janague",
    NULL
  };

  const gchar *documentors[] = {
    "janague",
    NULL
  };

  // Show about dialog
  gtk_show_about_dialog (GTK_WINDOW (window),
                         "program-name", "gPad",
                         "version", g_strdup_printf ("%s,\nRunning against GTK+ %d.%d.%d",
                                                     GP_VERSION,
                                                     gtk_get_major_version (),
                                                     gtk_get_minor_version (),
                                                     gtk_get_micro_version ()),
                         "copyright", "(C) 2013 janague",
                         "license-type",GTK_LICENSE_BSD,
                         "website", "http://www.gPad.org",
                         "comments", "Program to demonstrate GTK+ functions.",
                         "authors", authors,
                         "documenters", documentors,
                         "logo-icon-name", GTK_STOCK_EDIT,
                         "title", "About GTK+ gPad",
                         NULL);

}


//_____________________________________________________________________________
// Evente handler
//
// on_delete_event: Manage delete event
static gboolean
on_delete_event (   GtkWidget *widget,
                    GdkEvent *event,
                    gpointer user_data)
{
    gtk_main_quit ();

    return GDK_EVENT_PROPAGATE;
}


// New file
void
on_new_file_menu (GtkAction *action,
                   GtkWidget *window)
{
    GtkTextIter start, end;

    // Delete information in the buffer
    gtk_text_buffer_get_start_iter (gBuffer, &start);
    gtk_text_buffer_get_end_iter (gBuffer, &end);
    gtk_text_buffer_delete(gBuffer, &start, &end);
}


// Open a file
void
on_open_file_menu (GtkAction *action,
                   GtkWidget *window)
{
    gchar       *filename;
    GtkWidget   *chooser;
    GtkTextIter start, end;

    chooser = gtk_file_chooser_dialog_new ( "Open File...",
                                            GTK_WINDOW (window),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN,
                                            GTK_RESPONSE_OK,
                                            NULL);
                                               
    if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_OK)
    {
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));
                
        gtk_widget_destroy (chooser);

        // Delete information in the buffer
        gtk_text_buffer_get_start_iter (gBuffer, &start);
        gtk_text_buffer_get_end_iter (gBuffer, &end);
        gtk_text_buffer_delete(gBuffer, &start, &end);

        if (filename != NULL)
        {
            read_file (filename, gBuffer); 
        }
    }
}


// Save file
void
on_save_as_file_menu (GtkAction *action,
                   GtkWidget *window)
{
    GtkTextIter start, end;
    GError      *err=NULL;
    gchar       *text;
    GtkWidget   *chooser;
    gchar       *filename=NULL;

    // Delete information in the buffer
    gtk_text_buffer_get_start_iter (gBuffer, &start);
    gtk_text_buffer_get_end_iter (gBuffer, &end);
    text = gtk_text_buffer_get_text (gBuffer, &start, &end, FALSE);

    chooser = gtk_file_chooser_dialog_new ("Save File...",
                                           GTK_WINDOW (window),
                                           GTK_FILE_CHOOSER_ACTION_SAVE,
                                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                           GTK_STOCK_SAVE, GTK_RESPONSE_OK,
                                           NULL);
                                                                   
    if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_OK)
    {
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));

        if (filename != NULL)
        {
            g_file_set_contents (filename, text, -1, &err);
        }
    }

    // Free the memory
    g_free (text);
                           
    gtk_widget_destroy (chooser);
}
