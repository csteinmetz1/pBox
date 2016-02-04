//
//  main.c
//  pBox
//
//  Created by Christian Steinmetz on 1/23/16.
//  Copyright (c) 2016 Christian Steinmetz. All rights reserved.
//  This is an edit to the comment header

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

typedef struct Credentials {
    
    char name[64];
    char username[64];
    char password[64];

} Credential;


    
int AlphanumCheck (char password[64]) {
    
    int i = 0;
    int alpha = 0;
    int num = 0;
    int check = 0;
    int passlen = (int)strlen(password);
    
    // check for a number
    while(i < passlen) {
        if (password[i] > 47 && password[i] < 58)
            alpha = 1;
        i++;
    }
    
    // check for a letter
    i = 0;
    while(i < passlen) {
        if ((password[i] > 64 && password[i] < 91) || (password[i] > 96 && password[i] < 123))
            num = 1;
        i++;
    }
    
    if (alpha == 0 || num == 0) {
        check = 0;
    }
    else if (alpha == 1 && num == 1) {
        check = 1;
        }

    return check;
}


int PasswordCheck (char password[64], char password_confirm[64]) {
    
    // check if password entries match
    while ((strcmp(password, password_confirm) != 0) || (strlen(password) < 13) || (AlphanumCheck(password)) == 0) {
        
        if (strcmp(password, password_confirm) != 0) {
            
            printf("\nYour passwords did not match,\n");
            
            printf("Please re-enter your password: ");
            fgets(password, 64, stdin);
            printf("Confirm your password: ");
            fgets(password_confirm, 64, stdin);
        }
        
        else if (strlen(password) < 13) {
            
            printf("\nYour password was less than 12 characters,\n");
            
            printf("Please enter another password: ");
            fgets(password, 64, stdin);
            printf("Confirm your password: ");
            fgets(password_confirm, 64, stdin);
        }
        else if (AlphanumCheck(password) == 0) {
            
            printf("\nYour password did not contain both numbers and letters,\n");
            
            printf("Please enter another password: ");
            fgets(password, 64, stdin);
            printf("Confirm your password: ");
            fgets(password_confirm, 64, stdin);
        }
    }
    return 0;
}

// To check length of user entires (shall not exceed 16)
char* CheckLength (char user_input[64]) {
    
    while (strlen(user_input) > 17) {
        
        printf("Your input was too long!\nAll inputs must be less than 16 characters.\n");
        printf("Please re-enter your input: ");
    
        fgets(user_input, 64, stdin);
    }
              
    return user_input;
}


// To add user credentials and write them to file
char AddCredential () {
    
    Credential credential = {0};
    
    char response[3] = {0};
    
    printf("Enter the name of the credential (ex: Gmail): ");
    fgets(credential.name, 64, stdin);
    
    CheckLength(credential.name);
    
    printf("Enter the username: ");
    fgets(credential.username, 64, stdin);
    
    CheckLength(credential.username);
    
    printf("Enter the password: ");
    fgets(credential.password, 64, stdin);
    
    CheckLength(credential.password);
    
    //remove \n
    credential.name[(strlen(credential.name)) - 1] = '\0';
    credential.username[(strlen(credential.username)) - 1] = '\0';
    credential.password[(strlen(credential.password)) - 1] = '\0';
    
    printf("\n\n");
    printf("+------------------+------------------+------------------+\n");
    printf("| Name             | Username         | Password         |\n");
    printf("+------------------+------------------+------------------+\n");
    printf("| %-16s | %-16s | %-16s |\n", credential.name, credential.username, credential.password);
    printf("+------------------+------------------+------------------+\n");
    
    printf("\nIs the following information is correct? y/n: ");
    fgets(response, 3, stdin);
    
    if (strcmp(response, "y\n") == 0) {
      
       //add \n
       credential.name[(strlen(credential.name))] = '\n';
       credential.username[(strlen(credential.username))] = '\n';
       credential.password[(strlen(credential.password))] = '\n';
        
       // create cred file and write user data
       FILE *filePtr = fopen("pbox_cred.txt", "a");
       fputs(credential.name, filePtr);
       fputs(credential.username, filePtr);
       fputs(credential.password, filePtr);
       fclose(filePtr);
        
       printf("\nWould you like to add another credential? y/n: ");
       fgets(response, 3, stdin);
    }
    
    return response[0];
}

// To check user pass and then allow them to modify and write to config file
int ChangePassword(char name[64], char password[64]) {
    
    
    char user_pass[64] = {0};
    char new_pass[64] = {0};
    char check_pass[64] = {0};
    
    printf("\nPlease enter your current password: ");
    fgets(user_pass, 64, stdin);
    
    while (strcmp(user_pass, password) != 0) {
        
        printf("\nThe password you entered was incorrect!");
        printf("\nPlease re-enter your password: ");
        fgets(user_pass, 64, stdin);
    }
    
    if (strcmp(password,user_pass) == 0) {
        
        printf("Please enter your new password: ");
        fgets(new_pass, 64, stdin);
        CheckLength(new_pass);
        printf("Please confirm your new password: ");
        fgets(check_pass, 64, stdin);
        
        
        
        while (strcmp(new_pass, check_pass) != 0) {
            
            printf("\nYour passwords did not match!");
            printf("\nPlease enter your new password: ");
            fgets(new_pass, 64, stdin);
            CheckLength(new_pass);
            printf("\nPlease confirm your new password: ");
            fgets(check_pass, 64, stdin);
        }
        
        // write new password to file
        FILE *filePtr = fopen("pbox_config.txt", "w");
        fputs(name, filePtr);
        fputs(new_pass, filePtr);
        fclose(filePtr);
        
    }
    return 0;
}

// reads in credentials and then prints them to terminal
void ViewCredentials() {
    
    Credential credential = {0};
    char anykey[3] = {0};
    
    // open file to read in credentials
    FILE *filePtr = fopen("pbox_cred.txt", "r");
    
    printf("\n\n");
    printf("+------------------+------------------+------------------+\n");
    printf("| [Name]           | [Username]       | [Password]       |\n");
    
    
    while (fgets(credential.name, 64, filePtr) != NULL) {
        
        fgets(credential.username, 64, filePtr) ;
        fgets(credential.password, 64, filePtr);
        
        
        credential.name[(strlen(credential.name)) - 1] = '\0';
        credential.username[(strlen(credential.username)) - 1] = '\0';
        credential.password[(strlen(credential.password)) - 1] = '\0';

        printf("+------------------+------------------+------------------+\n");
        printf("| %-16s | %-16s | %-16s |\n", credential.name, credential.username, credential.password);
        
    
        }
    
    fclose(filePtr);
    printf("+------------------+------------------+------------------+\n");
    
    printf("\nPress enter to return to the main menu... ");
    fgets(anykey, 3, stdin);
}

// to modify any credential stored by the user and save out the new data
char ModifyCredential() {
    
    Credential credential = {0};
    Credential new_cred = {0};
    Credential cred_table[100];
    
    char response[3] = {0};
    int editnum = 0;
    int i = 1;
    int j = 0;

    
    // open file to read in credentials
    FILE *filePtr = fopen("pbox_cred.txt", "r");
    
    printf("\n\n");
    printf("+-----+------------------------+-------------------------+\n");
    printf("| [#] | [Name]                 | [Username]              |\n");
    
    // read in the data until end of file
    while (fgets(credential.name, 64, filePtr) != NULL) {
        
        fgets(credential.username, 64, filePtr) ;
        fgets(credential.password, 64, filePtr);
        
        // copy entries into array cred_table
        strcpy(cred_table[j].name, credential.name);
        strcpy(cred_table[j].username, credential.username);
        strcpy(cred_table[j].password, credential.password);
        
        // remove newline character
        credential.name[(strlen(credential.name)) - 1] = '\0';
        credential.username[(strlen(credential.username)) - 1] = '\0';
        credential.password[(strlen(credential.password)) - 1] = '\0';
        
        // print table of all entires along with ref numbers
        printf("+-----+------------------------+-------------------------+\n");
        printf("|  %-2d | %-22s | %-23s |\n", i, credential.name, credential.username);
        
        i++;
        j++;
    }
    
    
    printf("+-----+------------------------+-------------------------+\n");
    
    // prompt user for which credential to modify
    printf("Enter the number of the credential you would like to modify: ");
    
    char buff[4] = {0};
    char *ptr;
    long num;
    fgets(buff, 4, stdin);
    num = strtol(buff, &ptr, 10);
    editnum = (int) num;
    
    // add get new input and check for length
    printf("Enter the modified name: ");
    fgets(new_cred.name, 64, stdin);
    CheckLength(new_cred.name);
    printf("Enter the modified username: ");
    fgets(new_cred.username, 64, stdin);
    CheckLength(new_cred.username);
    printf("Enter the modified password: ");
    fgets(new_cred.password, 64, stdin);
    CheckLength(new_cred.password);
    
    //remove \n
    new_cred.name[(strlen(new_cred.name)) - 1] = '\0';
    new_cred.username[(strlen(new_cred.username)) - 1] = '\0';
    new_cred.password[(strlen(new_cred.password)) - 1] = '\0';
    
    printf("\n\n");
    printf("+------------------+------------------+------------------+\n");
    printf("| Name             | Username         | Password         |\n");
    printf("+------------------+------------------+------------------+\n");
    printf("| %-16s | %-16s | %-16s |\n", new_cred.name, new_cred.username, new_cred.password);
    printf("+------------------+------------------+------------------+\n");
    
    printf("\nIs the following information is correct? y/n: ");
    fgets(response, 3, stdin);
    
    if (strcmp(response, "y\n") == 0) {
        
        //add \n
        new_cred.name[(strlen(new_cred.name))] = '\n';
        new_cred.username[(strlen(new_cred.username))] = '\n';
        new_cred.password[(strlen(new_cred.password))] = '\n';
        
        // place modified credentials into table
        strcpy(cred_table[editnum - 1].name, new_cred.name);
        strcpy(cred_table[editnum - 1].username, new_cred.username);
        strcpy(cred_table[editnum - 1].password, new_cred.password);
        
        // create cred file and write user data
        FILE *filePtr = fopen("pbox_cred.txt", "w");
        
        j = 0;
        
        while (j < i) {
            fputs(cred_table[j].name, filePtr);
            fputs(cred_table[j].username, filePtr);
            fputs(cred_table[j].password, filePtr);
            j++;
        }
     
    fclose(filePtr);
    
    }
    
    return response[0];
}


int main(int argc, const char * argv[]) {


    // Startup Msg - add a fancy asci msg here
    printf("+--------------------------------------------------------+\n");
    printf("|                       pBox v1.0                        |\n");
    printf("+--------------------------------------------------------+\n");
    

   
    // check to see if config file exists, open file and load info
    FILE *filePtr = fopen("pbox_config.txt", "r+");
    
    if (filePtr) {
        
        char name[64] = {0};
        char pass[64] = {0};
        char user_pass[64] = {0};
        
        fgets(name, 64, filePtr);
        fgets(pass, 64, filePtr);
        fclose(filePtr);
        
        // authenticate user
        while (strcmp(pass, user_pass) != 0) {
            
            printf("Please enter your password: ");
            fgets(user_pass, 64, stdin);
            
            if (strcmp(pass, user_pass) == 0) {
                
                char state[2] = {0};
        
                while (strcmp(state, "e") != 0) {
                
                       printf("\n             Welcome Back to pBox %s\n", name);
                       printf("                What would you like to do?\n");
                       printf("+--------------------------------------------------------+\n");
                       printf("| a - Add credential                                     |\n");
                       printf("+--------------------------------------------------------+\n");
                       printf("| c - Change password                                    |\n");
                       printf("+--------------------------------------------------------+\n");
                       printf("| m - Modify credentials                                 |\n");
                       printf("+--------------------------------------------------------+\n");
                       printf("| v - View credentials                                   |\n");
                       printf("+--------------------------------------------------------+\n");
                       printf("| e - Exit Program                                       |\n");
                       printf("+--------------------------------------------------------+\n\n");
                
                       fgets(state, 3, stdin);
                
                       // replace \n with \0
                       state[1] = '\0';
                
                       // Add credential
                       if (strcmp(state, "a") == 0) {
                           
                           char response[] = "y";
                           
                           while (strcmp(response, "y") == 0) {
                               
                               response[0] = AddCredential();
                           }

                       }
                
                       // Change password
                       else if (strcmp(state, "c") == 0) {
                    
                           ChangePassword(name, pass);
                       }

                       // Modify credentials
                       else if (strcmp(state, "m") == 0) {
                           
                           ModifyCredential();
                       }
                    
                       // View credentials
                       else if (strcmp(state, "v") == 0) {
                           
                            ViewCredentials();
                       }
                    
                       // Exit
                       else if (strcmp(state, "e") == 0) {
                           
                           printf("Thank you for using pBox.\n");
                           exit(0);
                       }
                
                
                }
                
            }
            
            
            else if (strcmp(pass, user_pass) != 0) {
                  printf("Incorrect Pasword!\n");
            }
            
            
        }
        
        
        
        
        
        
    }
    
    // if the file doesnt exist get info from user and write to file (for first time setup)
    else {
        
        char name[64] = {0};
        char pass[64] = {0};
        char pass_check[64] = {0};
       
        printf("It seems this is your first time using pBox, we will\nask you some questions to setup your account.\n\n");
        
        printf("Enter your name: ");
        fgets(name, 64, stdin);
        printf("\n");
        
        printf("+--------------------------------------------------------+\n");
        printf("| Password Entry                                         |\n");
        printf("+--------------------------------------------------------+\n");
        printf("| Next you will select a password which will be used to  |\n");
        printf("| encrypt your exisiting passwords and therfore we       |\n");
        printf("| suggest it be both complex and unique. Your password   |\n");
        printf("| must be alteast 12 characters in length and should     |\n");
        printf("| include both letters and numbers at the least.         |\n");
        printf("+--------------------------------------------------------+\n\n");
        
        printf("What password do you want to use?: ");
        fgets(pass, 64, stdin);
        printf("Confirm your password: ");
        fgets(pass_check, 64, stdin);
    
        // check if password entries match
        PasswordCheck(pass, pass_check);
        
        char response[] = "n";
        
        // if not then ask user again
        while (strcmp(response, "n") == 0) {
            
            printf("\n\n");
            printf("+--------------------------------------------------------+\n");
            printf("| Name                                                   |\n");
            printf("+--------------------------------------------------------+\n");
            printf("  %s", name);
            printf("\n");
            printf("+--------------------------------------------------------+\n");
            printf("| Password                                               |\n");
            printf("+--------------------------------------------------------+\n");
            printf("  %s\n", pass);
            printf("\nIs the following information is correct? y/n: ");
            
            fgets(response, 3, stdin);
            
            // replace \n with NULL
            response[1] = '\0';
            
            // and if they answer no then ask them for a new pass and check it
            if (strcmp(response, "n") == 0) {
                
                printf("\nEnter your name: ");
                fgets(name, 64, stdin);
                printf("\n");
            
                printf("What password do you want to use?: ");
                fgets(pass, 64, stdin);
                printf("Confirm your password: ");
                fgets(pass_check, 64, stdin);
            
                // check if password entries match
                PasswordCheck(pass, pass_check);
            }
        }
        
        
        
        // this finds the current directory and prints it
        char* cwd;
        char buff[200];
        
        cwd = getcwd(buff, 200);
        if( cwd != NULL ) {
            printf("\nYour configuration file will be saved to %s\n\n", cwd );
        }
        
        // create config file and write user data
        FILE *filePtr = fopen("pbox_config.txt", "w");
        fputs(name, filePtr);
        fputs(pass, filePtr);
        fclose(filePtr);
        
        // now prompt user to add inital credentials
        printf("Would you like to add credentials to be stored now?\n");
        printf("NOTE: New credentials can always be added at the main menu.\n");
        printf("y/n: ");
        
        fgets(response, 3, stdin);
        
        // replace \n with NULL
        response[1] = '\0';
        
        // if they answer no then exit the program
        if (strcmp(response, "n") == 0) {
            exit(0);
        }
        
        // if answer yes then ask them to enter credentials
        else if (strcmp(response, "y") == 0) {
            
            while (strcmp(response, "y") == 0) {
                
               response[0] = AddCredential();
                
            }
        }
    }
    return 0;
}

