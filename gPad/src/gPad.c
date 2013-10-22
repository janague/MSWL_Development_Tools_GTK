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

// set the buffer size that it uses for reading the file.
#define BUFFER_SIZE 1024

// on_delete_event: Manage delete event
static gboolean
on_delete_event (   GtkWidget *widget,
                    GdkEvent *event,
                    gpointer user_data)
{
    gtk_main_quit ();

    return GDK_EVENT_PROPAGATE;
}

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

    GtkTextBuffer *buffer;

    // Create a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Set title
    gtk_window_set_title(   GTK_WINDOW(window),
                            "gPad");

    // Connect signal when delete-event is threw
    g_signal_connect (  window,
                        "delete-event",
                        G_CALLBACK (on_delete_event),
                        NULL);

    // Create a text view with the
    textView = gtk_text_view_new ();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textView));
    gtk_container_add(GTK_CONTAINER (window), textView);

    // Read the file
    // TODO: Include management of arguments.
    read_file(argv[1], buffer);

    // Show text view
    gtk_widget_show (textView);

    // Show window
    gtk_widget_show (window);

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
    char buf[BUFFER_SIZE];
    FILE *inFile;

    GtkTextMark *mark;
    GtkTextIter iter;

    // Open the file for reading
    inFile=fopen(filename,"r");

    if(inFile==NULL)
    {
        printf("An error occoured!");
        exit(1);
    }

    while (fgets(buf, sizeof buf, inFile) != NULL)
    {
        mark = gtk_text_buffer_get_insert (buffer);
        gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
        gtk_text_buffer_insert (buffer, &iter, buf, -1);
    }

    // Close the file
    fclose(inFile);
}
