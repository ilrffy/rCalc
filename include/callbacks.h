#ifndef CALLBACKS_H
#define CALLBACKS_H

extern GtkWidget *entry;
extern GtkWidget *label;

GdkPixbuf *create_pixbuf(const gchar *filename);
void main_buttons_clicked(GtkButton *button, gpointer data);
void button_equal_clicked(GtkButton *button, gpointer data);
void button_clear_clicked(GtkButton *button, gpointer data);
void button_about_clicked(GtkButton *button, gpointer data);
void clear_label(GtkWidget *wid, gpointer data);
void get_cursor_position(GtkWidget *widget);
void button_undo_clicked(GtkButton *button, gpointer data);



#endif