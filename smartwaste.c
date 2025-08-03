#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    int choice;

    while (1) {
        printf("\n===== Smart Waste Management System =====\n");
        printf("1. Update Bin Fill Level & Cleaning Time\n");
        printf("2. Check Full Bins & Time Alerts\n");
        printf("3. Display All Bin Status\n");
        printf("4. Save Sanitation Log to File\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
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
                printf("Exiting the system. Thank you!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void updateFillLevel(int count) {
    int id, level, hours, found = 0;

    printf("Enter Bin ID to update (1-%d): ", MAX_BINS);
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (bins[i].binID == id) {
            printf("Enter new fill level (0-100): ");
            scanf("%d", &level);
            if (level < 0 || level > 100) {
                printf("❌ Invalid fill level.\n");
                return;
            }
            bins[i].fillLevel = level;


            printf("Enter hours since last cleaned: ");
            scanf("%d", &hours);
            if (hours < 0) {
                printf("❌ Invalid time.\n");
                return;
            }
            bins[i].hoursSinceCleaned = hours;

            printf("✅ Bin updated successfully.\n");

            if (level >= FULL_THRESHOLD) {
                sendNotification(bins[i]);
            }

            if (hours > MAX_HOURS_BEFORE_CLEANING) {
                printf("⚠️  WARNING: Bin ID %d at '%s' has not been cleaned in over 48 hours!\n", bins[i].binID, bins[i].location);
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("❌ Bin ID not found.\n");
    }
}

void checkFullBins(int count) {
    int flag = 0;
    printf("\n🔔 Checking Bin Alerts:\n");

    for (int i = 0; i < count; i++) {
        if (bins[i].fillLevel >= FULL_THRESHOLD) {
            sendNotification(bins[i]);
            flag = 1;
        }

        if (bins[i].hoursSinceCleaned > MAX_HOURS_BEFORE_CLEANING) {
            printf("⚠️  Bin ID %d at '%s' not cleaned in over 48 hours!\n", bins[i].binID, bins[i].location);
            flag = 1;
        }
    }

    if (!flag) {
        printf("✅ All bins are within safe levels.\n");
    }
}

void displayBins(int count) {
    printf("\n📋 Bin Status Report:\n");
    for (int i = 0; i < count; i++) {
        printf("Bin ID: %d | Location: %s | Fill Level: %d%% | Hours Since Cleaned: %d\n",
               bins[i].binID, bins[i].location, bins[i].fillLevel, bins[i].hoursSinceCleaned);
    }
}

void saveSanitationLog(int count) {
    FILE *file = fopen("sanitation_log.txt", "w");
    if (file == NULL) {
        printf("❌ Could not open file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Bin ID: %d | Location: %s | Fill Level: %d%% | Hours Since Cleaned: %d\n",
                bins[i].binID, bins[i].location, bins[i].fillLevel, bins[i].hoursSinceCleaned);
    }

    fclose(file);
    printf("✅ Sanitation log saved to 'sanitation_log.txt'.\n");
}

void sendNotification(WasteBin bin) {
    printf("🚨 ALERT: Bin ID %d at '%s' is %d%% full. Immediate collection required!\n",
           bin.binID, bin.location, bin.fillLevel);
}
