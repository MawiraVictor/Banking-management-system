#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

// Global variables for widgets
GtkWidget *login_dialog = NULL;
GtkWidget *entry_account = NULL;
GtkWidget *entry_pin = NULL;
GtkWidget *main_window = NULL;

// Function to show error message
void show_error(GtkWindow *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message
    );
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to show info message
void show_info(GtkWindow *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message
    );
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Handle Login button click in dialog
void on_login_submit_clicked(GtkButton *button, gpointer user_data) {
    const char *account = gtk_entry_get_text(GTK_ENTRY(entry_account));
    const char *pin = gtk_entry_get_text(GTK_ENTRY(entry_pin));
    
    // Simple validation
    if (strlen(account) == 0 || strlen(pin) == 0) {
        show_error(GTK_WINDOW(login_dialog), "Please enter both account number and PIN");
        return;
    }
    
    // For demo purposes - hardcoded credentials
    if (strcmp(account, "1234") == 0 && strcmp(pin, "5678") == 0) {
        show_info(GTK_WINDOW(login_dialog), "Login successful! Welcome to your account.");
        
        // Clear entries and hide dialog
        gtk_entry_set_text(GTK_ENTRY(entry_account), "");
        gtk_entry_set_text(GTK_ENTRY(entry_pin), "");
        gtk_widget_hide(login_dialog);
        
        // Here you would show the main account dashboard
    } else {
        show_error(GTK_WINDOW(login_dialog), "Invalid account number or PIN");
        
        // Clear only the PIN field for retry
        gtk_entry_set_text(GTK_ENTRY(entry_pin), "");
    }
}

// Handle Cancel button click in dialog
void on_login_cancel_clicked(GtkButton *button, gpointer user_data) {
    // Clear entries
    gtk_entry_set_text(GTK_ENTRY(entry_account), "");
    gtk_entry_set_text(GTK_ENTRY(entry_pin), "");
    
    // Hide the dialog
    gtk_widget_hide(login_dialog);
}

// Handle main window Login button click
void on_login_clicked(GtkButton *button, gpointer data) {
    if (login_dialog == NULL) {
        printf("Error: Login dialog not loaded!\n");
        return;
    }
    
    // Show the dialog
    gtk_widget_show_all(login_dialog);
}

// Handle Register button click
void on_register_clicked(GtkButton *button, gpointer data) {
    show_info(GTK_WINDOW(main_window), 
              "Registration feature coming soon!\n\n"
              "You will be able to create new accounts here.");
}

// Load the login dialog from its UI file
void load_login_dialog() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "ui/login.ui", &error)) {
        printf("Error loading login dialog: %s\n", error->message);
        g_error_free(error);
        return;
    }
    
    // Get the dialog
    login_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "loginDialog"));
    
    // Get the entry fields
    entry_account = GTK_WIDGET(gtk_builder_get_object(builder, "entryAccount"));
    entry_pin = GTK_WIDGET(gtk_builder_get_object(builder, "entryPin"));
    
    // Get the buttons
    GtkWidget *login_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnLoginSubmit"));
    GtkWidget *cancel_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnLoginCancel"));
    
    // Connect signals
    if (login_btn) {
        g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_submit_clicked), NULL);
    }
    
    if (cancel_btn) {
        g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_login_cancel_clicked), NULL);
    }
    
    // Connect the delete event to hide instead of destroy
    g_signal_connect(login_dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *login_btn;
    GtkWidget *register_btn;
    GError *error = NULL;
    
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Load the login dialog first
    load_login_dialog();
    
    // Load the main window
    builder = gtk_builder_new();
    
    if (!gtk_builder_add_from_file(builder, "ui/banking.ui", &error)) {
        printf("Error loading main UI: %s\n", error->message);
        return 1;
    }
    
    // Get pointers to widgets
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "welcomeWindow"));
    login_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnLogin"));
    register_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnRegister"));
    
    // Connect signals
    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_clicked), NULL);
    g_signal_connect(register_btn, "clicked", G_CALLBACK(on_register_clicked), NULL);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Show the main window
    gtk_widget_show_all(main_window);
    
    // Start the main loop
    gtk_main();
    
    return 0;
}