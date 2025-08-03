#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Color codes for enhanced visual experience
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"

#define MAX_BINS 5
#define FULL_THRESHOLD 80
#define MAX_HOURS_BEFORE_CLEANING 48

typedef struct {
    int binID;
    char location[30];
    int fillLevel; // percentage
    int hoursSinceCleaned; // hours since last cleaned
} WasteBin;

// Predefined bins within campus
WasteBin bins[MAX_BINS] = {
    {1, "Library", 0, 0},
    {2, "Hostel", 0, 0},
    {3, "Canteen", 0, 0},
    {4, "Admin Block", 0, 0},
    {5, "Playground", 0, 0}
};

// Function declarations
void updateFillLevel(int count);
void checkFullBins(int count);
void displayBins(int count);
void saveSanitationLog(int count);
void sendNotification(WasteBin bin);
void printHeader();
void printSeparator();

int main() {
    int choice;

    while (1) {
        system("clear || cls"); // Clear screen for better visual experience
        printHeader();
        
        printf(CYAN BOLD "\n+====================================================+\n" RESET);
        printf(CYAN BOLD "|" RESET YELLOW "         SMART WASTE MANAGEMENT SYSTEM         " RESET CYAN BOLD "|\n" RESET);
        printf(CYAN BOLD "+====================================================+\n" RESET);
        printf(CYAN BOLD "|" RESET GREEN " 1. " RESET WHITE "Update Bin Fill Level & Cleaning Time      " RESET CYAN BOLD "|\n" RESET);
        printf(CYAN BOLD "|" RESET GREEN " 2. " RESET WHITE "Check Full Bins & Time Alerts              " RESET CYAN BOLD "|\n" RESET);
        printf(CYAN BOLD "|" RESET GREEN " 3. " RESET WHITE "Display All Bin Status                     " RESET CYAN BOLD "|\n" RESET);
        printf(CYAN BOLD "|" RESET GREEN " 4. " RESET WHITE "Save Sanitation Log to File                " RESET CYAN BOLD "|\n" RESET);
        printf(CYAN BOLD "|" RESET GREEN " 5. " RESET WHITE "Exit                                        " RESET CYAN BOLD "|\n" RESET);
        printf(CYAN BOLD "+====================================================+\n" RESET);
        printf(MAGENTA BOLD ">> Enter your choice: " RESET);
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1:
                updateFillLevel(MAX_BINS);
                break;
            case 2:
                checkFullBins(MAX_BINS);
                break;
            case 3:
                displayBins(MAX_BINS);
                break;
            case 4:
                saveSanitationLog(MAX_BINS);
                break;
            case 5:
                printf(GREEN BOLD "\nExiting the system. Thank you for keeping our campus clean!\n" RESET);
                exit(0);
            default:
                printf(RED BOLD "X Invalid choice. Please try again.\n" RESET);
        }
        
        printf(YELLOW "\nPress Enter to continue..." RESET);
        getchar();
    }

    return 0;
}

void printHeader() {
    printf(BLUE BOLD "+===============================================================+\n" RESET);
    printf(BLUE BOLD "|" RESET GREEN "     ECO-SMART CAMPUS WASTE MANAGEMENT DASHBOARD        " RESET BLUE BOLD "|\n" RESET);
    printf(BLUE BOLD "|" RESET CYAN "              Keeping Our Environment Clean & Safe             " RESET BLUE BOLD "|\n" RESET);
    printf(BLUE BOLD "+===============================================================+\n" RESET);
}

void printSeparator() {
    printf(CYAN "================================================================\n" RESET);
}

void updateFillLevel(int count) {
    int id, level, hours, found = 0;

    printSeparator();
    printf(YELLOW BOLD "\nBIN UPDATE CENTER\n" RESET);
    printSeparator();
    
    printf(CYAN "Enter Bin ID to update (1-%d): " RESET, MAX_BINS);
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (bins[i].binID == id) {
            printf(BLUE "Enter new fill level (0-100): " RESET);
            scanf("%d", &level);
            if (level < 0 || level > 100) {
                printf(RED BOLD "X Invalid fill level. Please enter a value between 0-100.\n" RESET);
                return;
            }
            bins[i].fillLevel = level;

            printf(BLUE "Enter hours since last cleaned: " RESET);
            scanf("%d", &hours);
            if (hours < 0) {
                printf(RED BOLD "X Invalid time. Please enter a positive value.\n" RESET);
                return;
            }
            bins[i].hoursSinceCleaned = hours;

            printf(GREEN BOLD "* Bin updated successfully!\n" RESET);

            if (level >= FULL_THRESHOLD) {
                sendNotification(bins[i]);
            }

            if (hours > MAX_HOURS_BEFORE_CLEANING) {
                printf(BG_RED WHITE BOLD "! CRITICAL WARNING: Bin ID %d at '%s' has not been cleaned in over 48 hours !" RESET "\n", bins[i].binID, bins[i].location);
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED BOLD "X Bin ID not found. Please check the ID and try again.\n" RESET);
    }
}

void checkFullBins(int count) {
    int flag = 0;
    
    printSeparator();
    printf(YELLOW BOLD "\nALERT MONITORING CENTER\n" RESET);
    printSeparator();

    for (int i = 0; i < count; i++) {
        if (bins[i].fillLevel >= FULL_THRESHOLD) {
            sendNotification(bins[i]);
            flag = 1;
        }

        if (bins[i].hoursSinceCleaned > MAX_HOURS_BEFORE_CLEANING) {
            printf(BG_RED WHITE BOLD "! URGENT: Bin ID %d at '%s' not cleaned in over 48 hours !" RESET "\n", bins[i].binID, bins[i].location);
            flag = 1;
        }
    }

    if (!flag) {
        printf(GREEN BOLD "* Excellent! All bins are within safe levels.\n" RESET);
    }
}

void displayBins(int count) {
    printSeparator();
    printf(MAGENTA BOLD "\nCOMPREHENSIVE BIN STATUS REPORT\n" RESET);
    printSeparator();
    
    printf(BOLD "+-----+-----------------+-------------+----------------------+\n" RESET);
    printf(BOLD "| ID  |    Location     | Fill Level  | Hours Since Cleaned  |\n" RESET);
    printf(BOLD "+-----+-----------------+-------------+----------------------+\n" RESET);
    
    for (int i = 0; i < count; i++) {
        char fillColor[10], timeColor[10];
        
        // Color coding based on fill level
        if (bins[i].fillLevel >= 80) strcpy(fillColor, RED);
        else if (bins[i].fillLevel >= 60) strcpy(fillColor, YELLOW);
        else strcpy(fillColor, GREEN);
        
        // Color coding based on cleaning time
        if (bins[i].hoursSinceCleaned > 48) strcpy(timeColor, RED);
        else if (bins[i].hoursSinceCleaned > 24) strcpy(timeColor, YELLOW);
        else strcpy(timeColor, GREEN);
        
        printf("| %s%2d%s  | %-15s | %s%8d%%%s   | %s%17d%s    |\n",
               CYAN, bins[i].binID, RESET,
               bins[i].location,
               fillColor, bins[i].fillLevel, RESET,
               timeColor, bins[i].hoursSinceCleaned, RESET);
    }
    
    printf(BOLD "+-----+-----------------+-------------+----------------------+\n" RESET);
    
    // Legend
    printf(WHITE "\nColor Legend:\n" RESET);
    printf(GREEN "   Green: Good Status\n" RESET);
    printf(YELLOW "   Yellow: Caution\n" RESET);
    printf(RED "   Red: Critical/Alert\n" RESET);
}

void saveSanitationLog(int count) {
    FILE *file = fopen("sanitation_log.txt", "w");
    if (file == NULL) {
        printf(RED BOLD "X Could not open file for writing.\n" RESET);
        return;
    }

    fprintf(file, "===============================================================\n");
    fprintf(file, "           SMART WASTE MANAGEMENT - SANITATION LOG\n");
    fprintf(file, "===============================================================\n\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "Bin ID: %d | Location: %s | Fill Level: %d%% | Hours Since Cleaned: %d\n",
                bins[i].binID, bins[i].location, bins[i].fillLevel, bins[i].hoursSinceCleaned);
    }
    
    fprintf(file, "\n===============================================================\n");
    fprintf(file, "Report generated automatically by Smart Waste Management System\n");

    fclose(file);
    printf(GREEN BOLD "* Sanitation log successfully saved to 'sanitation_log.txt'\n" RESET);
}

void sendNotification(WasteBin bin) {
    printf(BG_RED WHITE BOLD "! URGENT ALERT: Bin ID %d at '%s' is %d%% full. Immediate collection required !" RESET "\n",
           bin.binID, bin.location, bin.fillLevel);
}