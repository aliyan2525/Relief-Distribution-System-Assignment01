// =====================================================
// Relief Supply Distribution System
// CS-110 Fundamentals of Computer Programming - Assignment 1
// Class: BESE-17 A
//
// What this program does:
//   1. Asks for the starting trucks and supply packs.
//   2. Reads 5 relief requests from the user.
//   3. Calculates a need score for each request.
//   4. Chooses an action (A, B, C or D).
//   5. Updates the trucks and supply packs.
//   6. Prints a summary at the end.
// =====================================================

#include <iostream>
using namespace std;

const int TOTAL_REQUESTS = 5;

// ---------- Input functions ----------

// Reads one whole number from the user.
// If the user types letters, the program clears the error and asks again.
int readNumber()
{
    int number = 0;

    cin >> number;
    while (cin.fail())
    {
        cin.clear();              // clear the error flag
        cin.ignore(1000, '\n');   // throw away the wrong text
        cout << "Please type a whole number: ";
        cin >> number;
    }

    return number;
}

// Asks for the damage level. 1 = Low, 2 = Medium, 3 = High
int askDamageLevel()
{
    int level = 0;

    cout << "Damage level (1 = Low, 2 = Medium, 3 = High): ";
    level = readNumber();

    // Keep asking until the value is 1, 2 or 3
    while (level < 1 || level > 3)
    {
        cout << "Wrong value. Please type 1, 2 or 3: ";
        level = readNumber();
    }

    return level;
}

// Asks for the number of families. Allowed: 10 to 200
int askFamilies()
{
    int families = 0;

    cout << "Families affected (10 to 200): ";
    families = readNumber();

    while (families < 10 || families > 200)
    {
        cout << "Wrong value. Please type a number from 10 to 200: ";
        families = readNumber();
    }

    return families;
}

// Asks for the access route. 1 = Open, 2 = Damaged
int askRouteStatus()
{
    int route = 0;

    cout << "Access route (1 = Open, 2 = Damaged): ";
    route = readNumber();

    while (route != 1 && route != 2)
    {
        cout << "Wrong value. Please type 1 or 2: ";
        route = readNumber();
    }

    return route;
}

// Asks a yes or no question. Returns 'y' or 'n'.
char askYesNo(const char question[])
{
    char answer = 'n';

    cout << question;
    cin >> answer;

    while (answer != 'y' && answer != 'n' && answer != 'Y' && answer != 'N')
    {
        cout << "Please type y or n: ";
        cin >> answer;
    }

    // Change a capital letter to a small letter
    if (answer == 'Y')
    {
        answer = 'y';
    }
    if (answer == 'N')
    {
        answer = 'n';
    }

    return answer;
}

// ---------- Need score ----------

// Adds points for each input. A higher score means more urgent.
int calculateNeedScore(int damageLevel, int families, int routeStatus,
                       char floodWarning, char medicalNeed, char previousAid)
{
    int score = 0;

    // Damage points: Low = 5, Medium = 15, High = 25
    if (damageLevel == 1)
    {
        score = score + 5;
    }
    else if (damageLevel == 2)
    {
        score = score + 15;
    }
    else
    {
        score = score + 25;
    }

    // Family points: 1 point for every 10 families
    score = score + families / 10;

    // Route points
    if (routeStatus == 2)
    {
        score = score + 15;
    }

    // Flood points
    if (floodWarning == 'y')
    {
        score = score + 10;
    }

    // Medical points: people who need medicine come first
    if (medicalNeed == 'y')
    {
        score = score + 30;
    }

    // Previous aid penalty: they already got help before
    if (previousAid == 'y')
    {
        score = score - 10;
    }

    return score;
}

// ---------- Decision rules ----------

// Picks the action we WANT, using only the score.
//   score 70 or more -> A (Full Relief)
//   score 40 to 69   -> B (Standard Relief)
//   score 20 to 39   -> C (Emergency Kit Only)
//   score below 20   -> D (Defer / Monitor)
char chooseWantedAction(int score)
{
    char action = 'D';

    if (score >= 70)
    {
        action = 'A';
    }
    else if (score >= 40)
    {
        action = 'B';
    }
    else if (score >= 20)
    {
        action = 'C';
    }

    return action;
}
// How many trucks does each action need?
int trucksNeeded(char action)
{
    switch (action)
    {
        case 'A':
            return 2;
        case 'B':
            return 1;
        case 'C':
            return 1;
        default:
            return 0;
    }
}
// How many supply packs does each action need?
int packsNeeded(char action)
{
    switch (action)
    {
        case 'A':
            return 50;
        case 'B':
            return 30;
        case 'C':
            return 10;
        default:
            return 0;
    }
}
// If the wanted action does not fit the stock
// Order: A -> B -> C -> D. 
char findActionThatFits(char wantedAction, int trucksLeft, int packsLeft)
{
    char action = wantedAction;

    while (action != 'D')
    {
        if (trucksNeeded(action) <= trucksLeft && packsNeeded(action) <= packsLeft)
        {
            return action;
        }

        // Not enough stock, so go to the next smaller action
        if (action == 'A')
        {
            action = 'B';
        }
        else if (action == 'B')
        {
            action = 'C';
        }
        else
        {
            action = 'D';
        }
    }

    return 'D';
}

// ---------- Text helpers (only used for printing) ----------

const char* damageName(int level)
{
    if (level == 1)
    {
        return "Low";
    }
    if (level == 2)
    {
        return "Medium";
    }
    return "High";
}

const char* routeName(int route)
{
    if (route == 1)
    {
        return "Open";
    }
    return "Damaged";
}

const char* yesNoName(char answer)
{
    if (answer == 'y')
    {
        return "Yes";
    }
    return "No";
}

const char* actionName(char action)
{
    switch (action)
    {
        case 'A':
            return "A - Full Relief";
        case 'B':
            return "B - Standard Relief";
        case 'C':
            return "C - Emergency Kit Only";
        default:
            return "D - Defer / Monitor";
    }
}

// ---------- Main program ----------

int main()
{
    int trucks = 0;
    int packs = 0;

    int handledCount = 0;
    int deferredCount = 0;
    int notServedCount = 0;

    cout << "==========================================\n";
    cout << "    RELIEF SUPPLY DISTRIBUTION SYSTEM\n";
    cout << "==========================================\n\n";

    // Ask for the starting stock
    cout << "Enter starting number of trucks: ";
    trucks = readNumber();
    cout << "Enter starting number of supply packs: ";
    packs = readNumber();

    cout << "\nStarting stock: " << trucks << " trucks and "
         << packs << " supply packs\n";

    // Repeat for every request
    for (int requestNumber = 1; requestNumber <= TOTAL_REQUESTS; requestNumber++)
    {
        cout << "\n------------------------------------------\n";
        cout << "Request " << requestNumber << ": enter the details\n";
        cout << "------------------------------------------\n";

        // Step 1: read the inputs
        int damage = askDamageLevel();
        int families = askFamilies();
        int route = askRouteStatus();
        char flood = askYesNo("Flood warning (y/n): ");
        char medical = askYesNo("Medical need (y/n): ");
        char previousAid = askYesNo("Previous aid received (y/n): ");

        // Step 2: calculate the need score
        int score = calculateNeedScore(damage, families, route, flood, medical, previousAid);

        // Step 3: choose the action we want
        char wanted = chooseWantedAction(score);

        // Step 4: check the stock and choose the action that really fits
        char action = findActionThatFits(wanted, trucks, packs);

        // Step 5: update the trucks and supply packs
        int trucksUsed = trucksNeeded(action);
        int packsUsed = packsNeeded(action);
        trucks = trucks - trucksUsed;
        packs = packs - packsUsed;

        // Step 6: print the result
        cout << "\n*** RESULT FOR REQUEST " << requestNumber << " ***\n";
        cout << "Damage level:\t\t" << damageName(damage) << "\n";
        cout << "Families affected:\t" << families << "\n";
        cout << "Access route:\t\t" << routeName(route) << "\n";
        cout << "Flood warning:\t\t" << yesNoName(flood) << "\n";
        cout << "Medical need:\t\t" << yesNoName(medical) << "\n";
        cout << "Previous aid:\t\t" << yesNoName(previousAid) << "\n";
        cout << "Need score:\t\t" << score << "\n";
        cout << "Selected action:\t" << actionName(action) << "\n";
        cout << "Trucks used:\t\t" << trucksUsed << "\n";
        cout << "Supply packs used:\t" << packsUsed << "\n";
        cout << "Remaining trucks:\t" << trucks << "\n";
        cout << "Remaining supply packs:\t" << packs << "\n";

        // Step 7: explain the decision and count it
        if (action == 'D' && wanted == 'D')
        {
            cout << "Reason: Need score is too low. Request is deferred (monitor).\n";
            deferredCount++;
        }
        else if (action == 'D')
        {
            cout << "Reason: Not enough trucks or supply packs left. Request NOT served.\n";
            notServedCount++;
        }
        else if (action != wanted)
        {
            cout << "Note: Not enough stock for " << actionName(wanted) << ".\n";
            cout << "      A smaller action was used instead.\n";
            handledCount++;
        }
        else
        {
            cout << "Reason: Enough stock. The wanted action was used.\n";
            handledCount++;
        }
    }

    // Summary at the end
    cout << "\n==========================================\n";
    cout << "                  SUMMARY\n";
    cout << "==========================================\n";
    cout << "Total requests:\t\t\t" << TOTAL_REQUESTS << "\n";
    cout << "Successfully handled:\t\t" << handledCount << "\n";
    cout << "Deferred (low need):\t\t" << deferredCount << "\n";
    cout << "Could not be served:\t\t" << notServedCount << "\n";
    cout << "Final trucks left:\t\t" << trucks << "\n";
    cout << "Final supply packs left:\t" << packs << "\n";
    cout << "==========================================\n";
    system("pause");

    return 0;
}
