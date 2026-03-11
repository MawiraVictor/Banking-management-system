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
GtkWidget *deposit_dialog = NULL;
GtkWidget *withdraw_dialog = NULL;
GtkWidget *transfer_dialog = NULL;
GtkWidget *main_window = NULL;

// Transaction dialogs entries
GtkWidget *enrty_deposit_amount = NULL;
GtkWidget *enrty_withdraw_amount = NULL;
GtkWidget *enrty_transfer_money = NULL;
GtkWidget *entry_transfer_account = NULL;
GtkWidget *entry_transfer_description = NULL;
GtkWidget *entry_deposit_amount = NULL;
GtkWidget *entry_withdraw_amount = NULL;
GtkWidget *entry_transfer_amount = NULL;


//Current logged in user
char current_account[15] = "";
char current_fullname[50] ="";
float current_balance = 0;

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
void load_transactions(const char *account_number, char *transactions, int size);
void save_transactions(const char *account_number, const char *type, float amount, char *description);
void update_account_balance(const char*account_number, float new_balance);
void refresh_dashboard();
void load_deposit_dialog();
void load_withdraw_dialog();
void load_tranfer_dialog();
void on_transfer_clicked(GtkButton *button, gpointer data);
void on_deposit_submit_clicked(GtkButton *button, gpointer data);
void on_deposit_clicked(GtkButton *button, gpointer data);
void on_withdraw_clicked(GtkButton *button, gpointer data);
void on_transfer_clicked(GtkButton *button, gpointer data);void on_deposit_submit_clicked(GtkButton *button, gpointer data);
void on_withdraw_submit_clicked(GtkButton *button, gpointer data);
void on_withdraw_submit_clicked(GtkButton *button, gpointer data);
void on_transfer_submit_clicked(GtkButton *button, gpointer data);
void on_dialog_cancel_clicked(GtkButton *button, gpointer data);
void on_logout_clicked(GtkButton *button, gpointer data);

// Function to load last 5 transactions for an account
void load_transactions(const char *account_number, char *transactions, int size) {
    FILE *file = fopen("transactions.txt", "r");
    if (!file) {
        strcpy(transactions, "No transactions found");
        return;
    }
    
    // Temporary array to store all transactions for this account
    char all_transactions[100][256];
    int count = 0;
    char line[256];
    
    // Read all transactions for this account
    while (fgets(line, sizeof(line), file)) {
        char acc[15];
        sscanf(line, "%[^,]", acc);
        
        if (strcmp(acc, account_number) == 0) {
            // Store this transaction (remove the newline at the end)
            line[strcspn(line, "\n")] = 0;
            strcpy(all_transactions[count], line);
            count++;
        }
    }
    fclose(file);
    
    if (count == 0) {
        strcpy(transactions, "No transactions yet");
        return;
    }
    
    // Clear the transactions string
    transactions[0] = '\0';
    
    // Show last 5 transactions (most recent first)
    int start = (count > 5) ? count - 5 : 0;
    for (int i = count - 1; i >= start; i--) {
        char type[20], amount[20], desc[50];
        float amt;
        
        // Parse transaction line: account,type,amount,description,date
        sscanf(all_transactions[i], "%*[^,],%[^,],%f,%[^,],%s", type, &amt, desc);
        
        char transaction_line[200];
        if (strcmp(type, "deposit") == 0) {
            snprintf(transaction_line, sizeof(transaction_line), "+ KES %.2f %s\n", amt, desc);
        } else if (strcmp(type, "withdraw") == 0) {
            snprintf(transaction_line, sizeof(transaction_line), "- KES %.2f %s\n", amt, desc);
        } else if (strcmp(type, "transfer") == 0) {
            snprintf(transaction_line, sizeof(transaction_line), "→ KES %.2f %s\n", amt, desc);
        }
        
        strcat(transactions, transaction_line);
    }
}

// Function to save a transaction
void save_transaction(const char *account_number, const char *type, float amount, const char *description) {
    FILE *file = fopen("transactions.txt", "a");
    if (!file) {
        printf("Error opening transactions.txt for writing\n");
        return;
    }
    
    // Get current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M", t);
    
    fprintf(file, "%s,%s,%.2f,%s,%s\n", account_number, type, amount, description, date);
    fclose(file);
}
 
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
    
    // Get dashboard buttons
    GtkWidget *btn_deposit = GTK_WIDGET(gtk_builder_get_object(builder, "btnDeposit"));
    GtkWidget *btn_withdraw = GTK_WIDGET(gtk_builder_get_object(builder, "btnWithdraw"));
    GtkWidget *btn_transfer = GTK_WIDGET(gtk_builder_get_object(builder, "btnTransfer"));
    GtkWidget *btn_logout = GTK_WIDGET(gtk_builder_get_object(builder, "btnLogout"));
    
    // Connect button signals
    g_signal_connect(btn_deposit, "clicked", G_CALLBACK(on_deposit_clicked), NULL);
    g_signal_connect(btn_withdraw, "clicked", G_CALLBACK(on_withdraw_clicked), NULL);
    g_signal_connect(btn_transfer, "clicked", G_CALLBACK(on_transfer_clicked), NULL);
    g_signal_connect(btn_logout, "clicked", G_CALLBACK(on_logout_clicked), NULL);
    
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
    // Save to file
    save_account(&new_acc);

    // Save initial deposit transaction
    char deposit_desc[50];
    snprintf(deposit_desc, sizeof(deposit_desc), "Initial deposit");
    save_transaction(new_acc.account_number, "deposit", deposit, deposit_desc);

    
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

    // Store current user info
    strcpy(current_account, acc.account_number);
    strcpy(current_fullname, acc.full_name);
    current_balance = acc.balance;

    // Load REAL transactions from file
    char transactions[1000] = "";
    load_transactions(acc.account_number, transactions, sizeof(transactions));

    gtk_widget_hide(login_dialog);
    show_dashboard(acc.full_name, acc.balance, transactions);
    
    // Clear login fields
    gtk_entry_set_text(GTK_ENTRY(entry_account), "");
    gtk_entry_set_text(GTK_ENTRY(entry_pin), "");
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

// Function to update balance in accounts.txt
void update_account_balance(const char *account_number, float new_balance) {
    FILE *file = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (!file || !temp) {
        printf("Error updating account balance\n");
        return;
    }
    
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char acc[15];
        sscanf(line, "%[^,]", acc);
        
        if (strcmp(acc, account_number) == 0) {
            // Update this account's balance
            Account acc_data;
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%f",
                   acc_data.account_number, acc_data.pin, acc_data.full_name,
                   acc_data.id_number, acc_data.kra_pin, &acc_data.balance);
            
            fprintf(temp, "%s,%s,%s,%s,%s,%.2f\n",
                    acc_data.account_number, acc_data.pin, acc_data.full_name,
                    acc_data.id_number, acc_data.kra_pin, new_balance);
        } else {
            fprintf(temp, "%s", line);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
    
    // Update current balance
    current_balance = new_balance;
}

// Refresh dashboard with latest data
void refresh_dashboard() {
    // Update welcome label
    char welcome_text[100];
    snprintf(welcome_text, sizeof(welcome_text), "Welcome %s", current_fullname);
    gtk_label_set_text(GTK_LABEL(lblWelcome), welcome_text);

    // Update balance
    char balance_text[50];
    snprintf(balance_text, sizeof(balance_text), "KES %.2f", current_balance);
    gtk_label_set_text(GTK_LABEL(lblBalance), balance_text);

    // Update transactions
    char transactions[1000] = "";
    load_transactions(current_account, transactions, sizeof(transactions));
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txtTransactions));
    gtk_text_buffer_set_text(buffer, transactions, -1);
}

// Load deposit dialog
void load_deposit_dialog() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "ui/deposit.ui", &error)) {
        printf("Error loading deposit dialog: %s\n", error->message);
        g_error_free(error);
        return;
    }
    
    deposit_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "depositDialog"));
    entry_deposit_amount = GTK_WIDGET(gtk_builder_get_object(builder, "entryDepositAmount"));
    
    GtkWidget *submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnDepositSubmit"));
    GtkWidget *cancel_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnDepositCancel"));
    
    g_signal_connect(submit_btn, "clicked", G_CALLBACK(on_deposit_submit_clicked), NULL);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_dialog_cancel_clicked), deposit_dialog);
    g_signal_connect(deposit_dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

// Load withdraw dialog
void load_withdraw_dialog() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "ui/withdraw.ui", &error)) {
        printf("Error loading withdraw dialog: %s\n", error->message);
        g_error_free(error);
        return;
    }
    
    withdraw_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "withdrawDialog"));
    entry_withdraw_amount = GTK_WIDGET(gtk_builder_get_object(builder, "entryWithdrawAmount"));
    
    GtkWidget *submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnWithdrawSubmit"));
    GtkWidget *cancel_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnWithdrawCancel"));
    
    g_signal_connect(submit_btn, "clicked", G_CALLBACK(on_withdraw_submit_clicked), NULL);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_dialog_cancel_clicked), withdraw_dialog);
    g_signal_connect(withdraw_dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

// Load transfer dialog
void load_transfer_dialog() {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "ui/transfer.ui", &error)) {
        printf("Error loading transfer dialog: %s\n", error->message);
        g_error_free(error);
        return;
    }
    
    transfer_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "transferDialog"));
    entry_transfer_amount = GTK_WIDGET(gtk_builder_get_object(builder, "entryTransferAmount"));
    entry_transfer_account = GTK_WIDGET(gtk_builder_get_object(builder, "entryTransferAccount"));
    entry_transfer_description = GTK_WIDGET(gtk_builder_get_object(builder, "entryTransferDescription"));
    
    GtkWidget *submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnTransferSubmit"));
    GtkWidget *cancel_btn = GTK_WIDGET(gtk_builder_get_object(builder, "btnTransferCancel"));
    
    g_signal_connect(submit_btn, "clicked", G_CALLBACK(on_transfer_submit_clicked), NULL);
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_dialog_cancel_clicked), transfer_dialog);
    g_signal_connect(transfer_dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

// Dashboard button handlers
void on_deposit_clicked(GtkButton *button, gpointer data) {
    if (deposit_dialog) {
        gtk_entry_set_text(GTK_ENTRY(entry_deposit_amount), "");
        gtk_widget_show_all(deposit_dialog);
    }
}

void on_withdraw_clicked(GtkButton *button, gpointer data) {
    if (withdraw_dialog) {
        gtk_entry_set_text(GTK_ENTRY(entry_withdraw_amount), "");
        gtk_widget_show_all(withdraw_dialog);
    }
}

void on_transfer_clicked(GtkButton *button, gpointer data) {
    if (transfer_dialog) {
        gtk_entry_set_text(GTK_ENTRY(entry_transfer_amount), "");
        gtk_entry_set_text(GTK_ENTRY(entry_transfer_account), "");
        gtk_entry_set_text(GTK_ENTRY(entry_transfer_description), "");
        gtk_widget_show_all(transfer_dialog);
    }
}

void on_logout_clicked(GtkButton *button, gpointer data) {
    gtk_widget_hide(dashboardWindow);
    gtk_widget_show_all(main_window);
    
    // Clear current user info
    current_account[0] = '\0';
    current_fullname[0] = '\0';
    current_balance = 0;
}

// Transaction dialog handlers
void on_deposit_submit_clicked(GtkButton *button, gpointer data) {
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(entry_deposit_amount));
    
    if (strlen(amount_str) == 0) {
        show_error(GTK_WINDOW(deposit_dialog), "Please enter an amount");
        return;
    }
    
    float amount = atof(amount_str);
    if (amount <= 0) {
        show_error(GTK_WINDOW(deposit_dialog), "Please enter a valid amount");
        return;
    }
    
    // Update balance
    float new_balance = current_balance + amount;
    update_account_balance(current_account, new_balance);
    
    // Save transaction
    save_transaction(current_account, "deposit", amount, "Cash Deposit");
    
    // Refresh dashboard
    refresh_dashboard();
    
    // Show success message
    char success_msg[100];
    snprintf(success_msg, sizeof(success_msg), "KES %.2f deposited successfully!", amount);
    show_info(GTK_WINDOW(deposit_dialog), success_msg);
    
    // Clear and close dialog
    gtk_entry_set_text(GTK_ENTRY(entry_deposit_amount), "");
    gtk_widget_hide(deposit_dialog);
}

void on_withdraw_submit_clicked(GtkButton *button, gpointer data) {
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(entry_withdraw_amount));
    
    if (strlen(amount_str) == 0) {
        show_error(GTK_WINDOW(withdraw_dialog), "Please enter an amount");
        return;
    }
    
    float amount = atof(amount_str);
    if (amount <= 0) {
        show_error(GTK_WINDOW(withdraw_dialog), "Please enter a valid amount");
        return;
    }
    
    if (amount > current_balance) {
        show_error(GTK_WINDOW(withdraw_dialog), "Insufficient balance");
        return;
    }
    
    // Update balance
    float new_balance = current_balance - amount;
    update_account_balance(current_account, new_balance);
    
    // Save transaction
    save_transaction(current_account, "withdraw", amount, "Cash Withdrawal");
    
    // Refresh dashboard
    refresh_dashboard();
    
    // Show success message
    char success_msg[100];
    snprintf(success_msg, sizeof(success_msg), "KES %.2f withdrawn successfully!", amount);
    show_info(GTK_WINDOW(withdraw_dialog), success_msg);
    
    // Clear and close dialog
    gtk_entry_set_text(GTK_ENTRY(entry_withdraw_amount), "");
    gtk_widget_hide(withdraw_dialog);
}

void on_transfer_submit_clicked(GtkButton *button, gpointer data) {
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(entry_transfer_amount));
    const char *dest_account = gtk_entry_get_text(GTK_ENTRY(entry_transfer_account));
    const char *description = gtk_entry_get_text(GTK_ENTRY(entry_transfer_description));
    
    if (strlen(amount_str) == 0 || strlen(dest_account) == 0) {
        show_error(GTK_WINDOW(transfer_dialog), "Please fill in all fields");
        return;
    }
    
    float amount = atof(amount_str);
    if (amount <= 0) {
        show_error(GTK_WINDOW(transfer_dialog), "Please enter a valid amount");
        return;
    }
    
    if (amount > current_balance) {
        show_error(GTK_WINDOW(transfer_dialog), "Insufficient balance");
        return;
    }
    
    if (strcmp(dest_account, current_account) == 0) {
        show_error(GTK_WINDOW(transfer_dialog), "Cannot transfer to your own account");
        return;
    }
    
    // Check if destination account exists
    FILE *file = fopen("accounts.txt", "r");
    int dest_exists = 0;
    float dest_balance = 0;
    char line[200];
    char dest_line[200];
    
    if (file) {
        while (fgets(line, sizeof(line), file)) {
            char acc[15];
            sscanf(line, "%[^,]", acc);
            if (strcmp(acc, dest_account) == 0) {
                dest_exists = 1;
                strcpy(dest_line, line);
                break;
            }
        }
        fclose(file);
    }
    
    if (!dest_exists) {
        show_error(GTK_WINDOW(transfer_dialog), "Destination account does not exist");
        return;
    }
    
    // Update sender's balance
    float new_sender_balance = current_balance - amount;
    update_account_balance(current_account, new_sender_balance);
    
    // Update receiver's balance
    Account receiver;
    sscanf(dest_line, "%[^,],%[^,],%[^,],%[^,],%[^,],%f",
           receiver.account_number, receiver.pin, receiver.full_name,
           receiver.id_number, receiver.kra_pin, &receiver.balance);
    
    float new_receiver_balance = receiver.balance + amount;
    update_account_balance(dest_account, new_receiver_balance);
    
    // Save transactions for both accounts
    char sender_desc[100];
    if (strlen(description) > 0) {
        snprintf(sender_desc, sizeof(sender_desc), "Transfer to %s: %s", dest_account, description);
    } else {
        snprintf(sender_desc, sizeof(sender_desc), "Transfer to %s", dest_account);
    }
    save_transaction(current_account, "transfer", amount, sender_desc);
    
    char receiver_desc[100];
    snprintf(receiver_desc, sizeof(receiver_desc), "Transfer from %s", current_account);
    save_transaction(dest_account, "deposit", amount, receiver_desc);
    
    // Refresh dashboard
    refresh_dashboard();
    
    // Show success message
    char success_msg[100];
    snprintf(success_msg, sizeof(success_msg), "KES %.2f transferred successfully to account %s!", 
             amount, dest_account);
    show_info(GTK_WINDOW(transfer_dialog), success_msg);
    
    // Clear and close dialog
    gtk_entry_set_text(GTK_ENTRY(entry_transfer_amount), "");
    gtk_entry_set_text(GTK_ENTRY(entry_transfer_account), "");
    gtk_entry_set_text(GTK_ENTRY(entry_transfer_description), "");
    gtk_widget_hide(transfer_dialog);
}

void on_dialog_cancel_clicked(GtkButton *button, gpointer dialog) {
    gtk_widget_hide(GTK_WIDGET(dialog));
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
    load_deposit_dialog();    
    load_withdraw_dialog();   
    load_transfer_dialog();   
    
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