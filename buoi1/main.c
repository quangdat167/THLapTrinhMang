#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Account
{
    char username[30];
    char password[30];
    int status;
    int isSignIn;
    struct Account *next;

} Account;

int numAccount;
const char MSSV[] = "20205064";
const int ACTIVE = 1;
const int BLOCKED = 0;
const int IDLE = 2;

Account *CreateNewAccount(char *username, char *password, int status);
Account *AddAccount(Account *account, char *username, char *password, int status);
int CheckUser(Account *account, char *username);
int CheckPassword(Account *account, char *password);
int CheckStatus(Account *account, char *username);
void CheckSignIn(Account *account, char *username);
Account *ReadAccount(Account *account);
Account *Register(Account *account);
void UpdateFile(Account *account);
void ChangePassword(Account *account);
void SignInFn(Account *account);
void SearchFn(Account *account);
void SignOutFn(Account *account);
void FreeListFn(Account *account);
void ActivateFn(Account *account);

int main()
{
    int menu;
    Account *acc = NULL;

    acc = ReadAccount(acc);
    while (1 == 1)
    {
        printf("\nUSER MANAGEMENT PROGRAM\n");
        printf("-----------------------------------\n");
        printf("1. Register\n");
        printf("2. Activate\n");
        printf("3. Sign in\n");
        printf("4. Search\n");
        printf("5. Change password\n");
        printf("6. Sign out\n");
        printf("Your choice (1-6, other to quit): ");
        scanf("%d", &menu);
        switch (menu)
        {
        case 1:
            acc = Register(acc);
            break;
        case 2:
            ActivateFn(acc);
            break;
        case 3:
            SignInFn(acc);
            break;
        case 4:
            SearchFn(acc);
            break;
        case 5:
            ChangePassword(acc);
            break;
        case 6:
            SignOutFn(acc);
            break;
        default:
            FreeListFn(acc);
            exit(0);
        }
    }
}

Account *CreateNewAccount(char *username, char *password, int status)
{
    Account *p = (Account *)malloc(sizeof(Account));
    strcpy(p->username, username);
    strcpy(p->password, password);
    p->status = status;
    p->isSignIn = 0;
    p->next = NULL;
    return p;
}

Account *AddAccount(Account *account, char *username, char *password, int status)
{
    if (account == NULL)
    {
        Account *temp = CreateNewAccount(username, password, status);
        return temp;
    }
    else
    {
        if (CheckUser(account, username))
        {
            Account *cur = account;
            while (cur->next != NULL)
            {
                cur = cur->next;
            }
            Account *temp = CreateNewAccount(username, password, status);
            cur->next = temp;
            return account;
        }
        else
        {
            return NULL;
        }
    }
}

int CheckUser(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            return 0;
        }
        cur = cur->next;
    }
    return 1;
}

int CheckPassword(Account *account, char *password)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->password, password) == 0)
        {
            return 0;
        }
        cur = cur->next;
    }
    return 1;
}

int CheckStatus(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            return cur->status;
        }
        cur = cur->next;
    }
}

int isSignIn(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            return cur->isSignIn;
        }
        cur = cur->next;
    }
}

void changeStatus(Account *account, char *username, int status)
{
    if (status == BLOCKED)
    {
        printf("Password is incorrect. Account is blocked\n");
    }
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            cur->status = status;
            return;
        }
        cur = cur->next;
    }
}

void CheckSignIn(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            if (cur->isSignIn == 0)
            {
                printf("This account is not signed in!\n");
            }
            else
            {
                printf("Goodbye %s\n", cur->username);
                cur->isSignIn = 0;
            }
            return;
        }
        cur = cur->next;
    }
}

Account *ReadAccount(Account *acc)
{
    char username[30];
    char password[30];
    int status;
    numAccount = 0;
    FILE *inp = fopen("nguoidung.txt", "r");
    if (!inp)
    {
        printf("Error: Can't open this file! \n");
        return NULL;
    }

    do
    {
        if (fscanf(inp, "%s %s %d", username, password, &status) > 0)
        {
            acc = AddAccount(acc, username, password, status);
            numAccount++;
        }
        else
            break;
    } while (1 == 1);
    fclose(inp);
    return acc;
}

Account *Register(Account *acc)
{
    printf("----Welcome to Register function.----\n");

    char username[30];
    char password[30];
    printf("Username: ");
    scanf("%s", username);
    if (CheckUser(acc, username) != 0)
    {
        printf("Password: ");
        scanf("%s", password);
        acc = AddAccount(acc, username, password, IDLE);
        numAccount++;
        printf("Successful registration. Activation required. \n");
        UpdateFile(acc);
    }
    else
    {
        printf("Account existed! \n");
    }
    return acc;
}

void UpdateFile(Account *acc)
{
    FILE *inp = fopen("nguoidung.txt", "w");
    Account *cur = acc;
    while (cur != NULL)
    {
        fprintf(inp, "%s %s %d\n", cur->username, cur->password, cur->status);
        cur = cur->next;
    }
    fclose(inp);
}

void ActivateFn(Account *acc)
{
    printf("----Welcome to Activate function.----\n");
    char username[30];
    char password[30];
    char code[30];
    int n = 4;

    printf("Username: ");
    scanf("%s", username);

    if (CheckUser(acc, username) != 0)
    {
        printf("Cannot find account\n");
        return;
    }
    if (CheckStatus(acc, username) == BLOCKED)
    {
        printf("Account is blocked\n");
        return;
    }
    else if (CheckStatus(acc, username) == ACTIVE)
    {
        printf("Account is already activated\n");
        return;
    }

    printf("Password: ");
    scanf("%s", password);

    if (CheckPassword(acc, password) != 0)
    {
        printf("Password is incorrect\n");
        return;
    }

    while (n > 0)
    {
        printf("Code: ");
        scanf("%s", code);

        if (strcmp(code, MSSV) == 0)
        {
            changeStatus(acc, username, ACTIVE);
            UpdateFile(acc);
            printf("Account is activated\n");
            return;
        }
        else
        {
            printf("Account is not activated\n");
            n--;
        }
    }
    changeStatus(acc, username, BLOCKED);

    UpdateFile(acc);
    return;
}

void SignInFn(Account *acc)
{
    printf("----Welcome to SignIn function.----\n");
    char username[30];
    char password[30];

    int n = 3;
    printf("Username: ");
    scanf("%s", username);
    if (CheckUser(acc, username) != 0)
    {
        printf("Cannot find account\n");
        return;
    }
    else
    {
        if (CheckStatus(acc, username) == BLOCKED)
        {
            printf("Account is blocked\n");
            return;
        }
        else
        {
            while (n > 0)
            {
                printf("Password: ");
                scanf("%s", password);
                if (CheckPassword(acc, password) != 0)
                {
                    printf("Password is incorrect\n");
                    n--;
                }
                else
                {
                    printf("Hello %s\n", username);
                    Account *cur = acc;
                    while (cur != NULL)
                    {
                        if (strcmp(cur->username, username) == 0)
                        {
                            cur->isSignIn = 1;
                        }
                        cur = cur->next;
                    }
                    printf("%s\n", cur->username);
                    return;
                }
            }
            changeStatus(acc, username, BLOCKED);

            UpdateFile(acc);
            return;
        }
    }
}

void SearchFn(Account *acc)
{
    printf("----Welcome to Search function.----\n");
    char username[30];

    printf("Username: ");
    scanf("%s", username);
    if (CheckUser(acc, username) != 0)
    {
        printf("Account does not exist!\n");
    }
    else
    {
        if (CheckStatus(acc, username) == BLOCKED)
        {
            printf("Account is blocked\n");
        }
        else if (CheckStatus(acc, username) == ACTIVE)
        {
            printf("Account is active\n");
        }
        else
        {
            printf("Account is not active\n");
        }
    }
    return;
}

void ChangePassword(Account *acc)
{
    printf("----Welcome to ChangePassword function.----\n");
    char username[30];
    char password[30];
    char newPassword[30];

    printf("Username: ");
    scanf("%s", username);

    if (CheckUser(acc, username) != 0)
    {
        printf("Cannot find account\n");
        return;
    }

    if (isSignIn(acc, username) == 0)
    {
        printf("This account is not signed in!\n");
        return;
    }
    printf("Password: ");
    scanf("%s", password);

    if (CheckPassword(acc, password) != 0)
    {
        printf("Current password is incorrect. Please try again\n");
        return;
    }
    printf("NewPassword: ");
    scanf("%s", newPassword);

    Account *cur = acc;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            strcpy(cur->password, newPassword);
        }
        cur = cur->next;
    }
    printf("Password is changed\n");

    UpdateFile(acc);
    return;
}

void SignOutFn(Account *acc)
{
    printf("----Welcome to SignOut function.----\n");
    char username[30];

    printf("Username: ");
    scanf("%s", username);
    if (CheckUser(acc, username) != 0)
    {
        printf("Account does not exist!\n");
    }
    else
    {
        CheckSignIn(acc, username);
    }
}

void FreeListFn(Account *head)
{
    Account *tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}