/*-------------------------------------------------------*/
/*  This file contains different callbacks functions used
    with corresponding signals.
*/
/*-------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>

#include "callbacks.h"
#include "calc.h"
#include "stack.h"

Stack_STR s = NULL;
int cursor_position;

// Add the icon to the main window:
GdkPixbuf *create_pixbuf(const gchar *filename)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if (!pixbuf)
    {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

// Print the text entered with the buttons on the entry:
void main_buttons_clicked(GtkButton *button, gpointer data)
{
    gtk_label_set_label(GTK_LABEL(label), "");

    gtk_widget_set_can_focus(entry, TRUE);
    gtk_widget_grab_focus(entry);

    if ((strcmp("sqrt", (const char *)data) == 0))
    {
        gtk_editable_insert_text(GTK_EDITABLE(entry), "sqrt(", 5, &(cursor_position));
        gtk_editable_set_position(GTK_EDITABLE(entry), cursor_position);
    }
    else
    {
        gtk_editable_insert_text(GTK_EDITABLE(entry), (const char *)data, 1, &(cursor_position));
        gtk_editable_set_position(GTK_EDITABLE(entry), cursor_position);
    }
}

// Handle the clear "C" button:
void button_clear_clicked(GtkButton *button, gpointer data)
{
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    gtk_label_set_label(GTK_LABEL(label), "");
    gtk_widget_set_can_focus(entry, TRUE);
    gtk_widget_grab_focus(entry);
}

// Handle the entred expression:
void button_equal_clicked(GtkButton *button, gpointer data)
{
    const char *text_entred;
    char output[MAX_SIZE];

    int status_error = 0;

    text_entred = gtk_entry_get_text(GTK_ENTRY(entry));

    double final_result = calculate(text_entred, &status_error);

    // If a syntax error is found:
    if (isnan(final_result) && (status_error == -1))
    {
        gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
        gtk_label_set_text(GTK_LABEL(label), "Malformed Expression");
    }
    // If the expression contains a division by zero or sqrt(-a) such as a > 0:
    else if ((isnan(final_result) || isinf(final_result)) && (status_error == 0))
    {
        gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
        gtk_label_set_text(GTK_LABEL(label), "Math Error");
    }
    else
    {
        push_STR(text_entred, &s);  /*  Push the expression entred into the  
                                        history stack   */

        snprintf(output, MAX_SIZE, "%.10g", final_result);
        
        gtk_label_set_label(GTK_LABEL(label), "");
        gtk_entry_set_text(GTK_ENTRY(entry), "");
        
        gtk_entry_set_text(GTK_ENTRY(entry), output);
    }

    gtk_widget_set_can_focus(entry, TRUE);
    gtk_widget_grab_focus(entry);

    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    cursor_position = strlen(text);
    gtk_editable_set_position(GTK_EDITABLE(entry), cursor_position);
}

// Handle the "About" button:
void button_about_clicked(GtkButton *button, gpointer data)
{
    GtkWidget *about = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about), "rCalc"); gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), NULL);
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(about), "GitHub");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about), "https://github.com/ilrffy");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about), "(c) ilrffy");

    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about), "A simple calculator");

    gtk_dialog_run(GTK_DIALOG(about));
    gtk_widget_destroy(about);

    gtk_widget_set_can_focus(entry, TRUE);
    gtk_widget_grab_focus(entry);
}


void clear_label(GtkWidget *wid, gpointer data)
{
    gtk_label_set_label(GTK_LABEL(label), "");
}


void get_cursor_position(GtkWidget *widget)
{
    cursor_position = gtk_editable_get_position(GTK_EDITABLE(entry));
}

// Handle the undo "⟲" button:
void button_undo_clicked(GtkButton *button, gpointer data)
{
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));

    gtk_widget_set_can_focus(entry, TRUE);
    gtk_widget_grab_focus(entry);

    if ((strcmp(text, "") == 0))
    {
        const char *text_undo = pop_STR(&s);
        
        if (text_undo == NULL)
        {
            gtk_label_set_label(GTK_LABEL(label), "No undo history");
        }
        else
        {
            gtk_entry_set_text(GTK_ENTRY(entry), "");
            gtk_label_set_label(GTK_LABEL(label), "");

            gtk_editable_insert_text(GTK_EDITABLE(entry), (const char *)text_undo, strlen(text_undo), &(cursor_position));
            text_undo = gtk_entry_get_text(GTK_ENTRY(entry));
            gtk_editable_set_position(GTK_EDITABLE(entry), cursor_position);
        }
    }
    else 
    {
        gtk_editable_delete_text(GTK_EDITABLE(entry), cursor_position-1, -1);
        cursor_position--;
        gtk_editable_set_position(GTK_EDITABLE(entry), cursor_position);
    }
}
