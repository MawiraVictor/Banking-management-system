#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Global variables
GtkWidget *login_dialog = NULL;
GtkWidget *register_dialog = NULL;
GtkWidget *entry_account = NULL;
GtkWidget *entry_pin = NULL;
GtkWidget *entry_fullname = NULL;
GtkWidget *entry_id = NULL;
GtkWidget *entry_kra = NULL;
GtkWidget *entry_phone = NULL;
GtkWidget *entry_deposit = NULL;
GtkWidget *dashboardWindow = NULL;
GtkWidget *lblWelcome = NULL;
GtkWidget *lblBalance = NULL;
GtkWidget *txtTransactions = NULL;
GtkWidget *main_window = NULL;

// Account structure
typedef struct {
    char account_number[15];
    char pin[5];
    char full_name[50];
    char id_number[20];
    char kra_pin[15];
    char phone[15];
    float balance;
} Account;

// Function prototypes
void show_error(GtkWindow *parent, const char *message);
void show_info(GtkWindow *parent, const char *message);
void generate_account_number(char *acc_no);
void generate_pin(char *pin);
void save_account(Account *acc);
int account_exists(char *id_number);
void load_registration_dialog();
void on_register_submit_clicked(GtkButton *button, gpointer data);
void on_register_cancel_clicked(GtkButton *button, gpointer data);
void on_register_clicked(GtkButton *button, gpointer data);

 
 void show_dashboard(const char *full_name, float balance, const char *transactions) {
    // Update welcome label
    char welcome_text[100];
    snprintf(welcome_text, sizeof(welcome_text), "Welcome %s", full_name);
    gtk_label_set_text(GTK_LABEL(lblWelcome), welcome_text);

    // Update balance - NOW USING LABEL
    char balance_text[50];
    snprintf(balance_text, sizeof(balance_text), "KES %.2f", balance);
    gtk_label_set_text(GTK_LABEL(lblBalance), balance_text);  // This will work now!

    // Update transactions - KEEP AS TEXTVIEW
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txtTransactions));
    gtk_text_buffer_set_text(buffer, transactions, -1);

    // Show dashboard window
    gtk_widget_show_all(dashboardWindow);

    // Optional: hide main window
    gtk_widget_hide(main_window);
}

// Show error message
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

// Show info message
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

// Generate random 8-digit account number
void generate_account_number(char *acc_no) {
    srand(time(NULL));
    sprintf(acc_no, "%d%d%d%d%d%d%d%d",
            rand() % 10, rand() % 10, rand() % 10, rand() % 10,
            rand() % 10, rand() % 10, rand() % 10, rand() % 10);
}

// Generate random 4-digit PIN
void generate_pin(char *pin) {
    srand(time(NULL));
    sprintf(pin, "%d%d%d%d",
            rand() % 10, rand() % 10, rand() % 10, rand() % 10);
}

// Check if ID already exists
int account_exists(char *id_number) {
    FILE *file = fopen("accounts.txt", "r");
    if (!file) return 0;
    
    char line[200];
    char saved_id[20];
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*f", saved_id);
        if (strcmp(saved_id, id_number) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Save account to file
void save_account(Account *acc) {
    FILE *file = fopen("accounts.txt", "a");
    if (!file) {
        printf("Error opening accounts.txt for writing\n");
        return;
    }
    
    fprintf(file, "%s,%s,%s,%s,%s,%.2f\n",
            acc->account_number,
            acc->pin,
            acc->full_name,
            acc->id_number,
            acc->kra_pin,
            acc->balance);
    
    fclose(file);
}

// Load registration dialog
void load_registration_dialog() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "ui/register.ui", &error)) {
        printf("Error loading registration dialog: %s\n", error->message);
        g_error_free(error);
        return;
    }
    
    register_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "registerDialog"));
    entry_fullname = GTK_WIDGET(gtk_builder_get_object(builder, "entryFullName"));
    entry_id = GTK_WIDGET(gtk_builder_get_object(builder, "entryID"));
    entry_kra = GTK_WIDGET(gtk_builder_get_object(builder, "entryKRA"));
    entry_phone = GTK_WIDGET(gtk_builder_get_object(builder, "entryPhone"));
    entry_deposit = GTK_WIDGET(gtk_builder_get_object(builder, "entryDeposit"));
    
    GtkWidget *submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnRegisterSubmit"));
    GtkWidget *cancel_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnRegisterCancel"));
    
    g_signal_connect(submit_btn, "clicked", G_CALLBACK(on_register_submit_clicked), NULL);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_register_cancel_clicked), NULL);
    g_signal_connect(register_dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

void load_dashboard() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (!gtk_builder_add_from_file(builder, "ui/dashboard.ui", &error)) {
        printf("Error loading dashboard: %s\n", error->message);
        g_error_free(error);
        return;
    }

    dashboardWindow = GTK_WIDGET(gtk_builder_get_object(builder, "dashboardWindow"));
    lblWelcome = GTK_WIDGET(gtk_builder_get_object(builder, "lblWelcome"));
    lblBalance = GTK_WIDGET(gtk_builder_get_object(builder, "lblBalance"));
    txtTransactions = GTK_WIDGET(gtk_builder_get_object(builder, "txtTransactions"));

    g_signal_connect(dashboardWindow, "destroy", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

// Handle registration submit
void on_register_submit_clicked(GtkButton *button, gpointer data) {
    const char *fullname = gtk_entry_get_text(GTK_ENTRY(entry_fullname));
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *kra = gtk_entry_get_text(GTK_ENTRY(entry_kra));
    const char *phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));
    const char *deposit_str = gtk_entry_get_text(GTK_ENTRY(entry_deposit));
    
    // Validate fields
    if (strlen(fullname) == 0 || strlen(id) == 0 || strlen(kra) == 0 || 
        strlen(phone) == 0 || strlen(deposit_str) == 0) {
        show_error(GTK_WINDOW(register_dialog), "Please fill in all fields");
        return;
    }
    
    // Check if ID already exists
    if (account_exists((char*)id)) {
        show_error(GTK_WINDOW(register_dialog), "An account with this ID already exists");
        return;
    }
    
    // Parse deposit amount
    float deposit = atof(deposit_str);
    if (deposit < 0) {
        show_error(GTK_WINDOW(register_dialog), "Invalid deposit amount");
        return;
    }
    
    // Create new account
    Account new_acc;
    strcpy(new_acc.full_name, fullname);
    strcpy(new_acc.id_number, id);
    strcpy(new_acc.kra_pin, kra);
    strcpy(new_acc.phone, phone);
    new_acc.balance = deposit;
    
    generate_account_number(new_acc.account_number);
    generate_pin(new_acc.pin);
    
    // Save to file
    save_account(&new_acc);
    
    // Show success message with account details
    char success_msg[300];
    snprintf(success_msg, sizeof(success_msg),
             "Account created successfully!\n\n"
             "Account Number: %s\n"
             "PIN: %s\n\n"
             "Please keep these details safe.\n"
             "You can now login with your account number and PIN.",
             new_acc.account_number, new_acc.pin);
    
    show_info(GTK_WINDOW(register_dialog), success_msg);
    
    // Clear fields and close dialog
    gtk_entry_set_text(GTK_ENTRY(entry_fullname), "");
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_entry_set_text(GTK_ENTRY(entry_kra), "");
    gtk_entry_set_text(GTK_ENTRY(entry_phone), "");
    gtk_entry_set_text(GTK_ENTRY(entry_deposit), "");
    
    gtk_widget_hide(register_dialog);
}

// Handle registration cancel
void on_register_cancel_clicked(GtkButton *button, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(entry_fullname), "");
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_entry_set_text(GTK_ENTRY(entry_kra), "");
    gtk_entry_set_text(GTK_ENTRY(entry_phone), "");
    gtk_entry_set_text(GTK_ENTRY(entry_deposit), "");
    
    gtk_widget_hide(register_dialog);
}

// Handle main window Register button click
void on_register_clicked(GtkButton *button, gpointer data) {
    if (register_dialog) {
        gtk_widget_show_all(register_dialog);
    } else {
        show_error(GTK_WINDOW(main_window), "Registration system not loaded properly");
    }
}

// Login submit handler (update to check file)
void on_login_submit_clicked(GtkButton *button, gpointer user_data) {
    const char *account = gtk_entry_get_text(GTK_ENTRY(entry_account));
    const char *pin = gtk_entry_get_text(GTK_ENTRY(entry_pin));
    
    if (strlen(account) == 0 || strlen(pin) == 0) {
        show_error(GTK_WINDOW(login_dialog), "Please enter both account number and PIN");
        return;
    }
    
    // Check credentials against file
    FILE *file = fopen("accounts.txt", "r");
    if (!file) {
        show_error(GTK_WINDOW(login_dialog), "No accounts found. Please register first.");
        return;
    }
    
    char line[200];
    char saved_acc[15], saved_pin[5];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,]", saved_acc, saved_pin);
        if (strcmp(saved_acc, account) == 0 && strcmp(saved_pin, pin) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);
    
    if (found) {
    // Read full account info from file
    Account acc;
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%f",
           acc.account_number, acc.pin, acc.full_name,
           acc.id_number, acc.kra_pin, &acc.balance);

    // Load last 3 transactions (dummy for now)
    char transactions[500] = 
        "+ KES 2000 Deposit\n"
        "- KES 500 Withdraw\n"
        "+ KES 1000 Transfer\n";

    gtk_widget_hide(login_dialog);
    show_dashboard(acc.full_name, acc.balance, transactions);

} else {
    show_error(GTK_WINDOW(login_dialog), "Invalid account number or PIN");
    gtk_entry_set_text(GTK_ENTRY(entry_pin), "");
}
}

// Login cancel handler
void on_login_cancel_clicked(GtkButton *button, gpointer user_data) {
    gtk_entry_set_text(GTK_ENTRY(entry_account), "");
    gtk_entry_set_text(GTK_ENTRY(entry_pin), "");
    gtk_widget_hide(login_dialog);
}

// Main window Login button click
void on_login_clicked(GtkButton *button, gpointer data) {
    if (login_dialog) {
        gtk_widget_show_all(login_dialog);
    }
}

// Load login dialog
void load_login_dialog() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "ui/login.ui", &error)) {
        printf("Error loading login dialog: %s\n", error->message);
        g_error_free(error);
        return;
    }
    
    login_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "loginDialog"));
    entry_account = GTK_WIDGET(gtk_builder_get_object(builder, "entryAccount"));
    entry_pin = GTK_WIDGET(gtk_builder_get_object(builder, "entryPin"));
    
    GtkWidget *login_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnLoginSubmit"));
    GtkWidget *cancel_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnLoginCancel"));
    
    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_submit_clicked), NULL);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_login_cancel_clicked), NULL);
    g_signal_connect(login_dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *login_btn;
    GtkWidget *register_btn;
    GError *error = NULL;
    
    gtk_init(&argc, &argv);
    
    // Load dialogs
    load_login_dialog();
    load_registration_dialog();
    load_dashboard();
    
    // Load main window
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "ui/banking.ui", &error)) {
        printf("Error loading main UI: %s\n", error->message);
        return 1;
    }
    
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "welcomeWindow"));
    login_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnLogin"));
    register_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnRegister"));
    
    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_clicked), NULL);
    g_signal_connect(register_btn, "clicked", G_CALLBACK(on_register_clicked), NULL);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_widget_show_all(main_window);
    gtk_main();
    
    return 0;
}