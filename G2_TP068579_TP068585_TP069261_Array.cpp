// G2_TP068579_TP068585_TP069261_Array.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

const int MAX_ITEMS = 1000;

enum class SortColumn { Grams, Calories, Protein, Fat, SatFat, Fiber, Carbs };

struct FoodItem {
    string food = "";
    string measure = "";
    int grams = 0;
    int calories = 0;
    int protein = 0;
    int fat = 0;
    int satFat = 0;
    float fiber = 0.0f;
    float carbs = 0.0f;
    string category = "";
};

string cleanNumericField(string str) {
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    str.erase(remove(str.begin(), str.end(), '\"'), str.end());
    return str.empty() || str.find_first_not_of("0123456789.-") != string::npos ? "0" : str;
}

FoodItem parseCSVLine(const string& line) {
    stringstream ss(line);
    FoodItem item;
    string temp;

    try {
        getline(ss, item.food, ',');
        getline(ss, item.measure, ',');
        getline(ss, temp, ','); item.grams = stoi(cleanNumericField(temp));
        getline(ss, temp, ','); item.calories = stoi(cleanNumericField(temp));
        getline(ss, temp, ','); item.protein = stoi(cleanNumericField(temp));
        getline(ss, temp, ','); item.fat = stoi(cleanNumericField(temp));
        getline(ss, temp, ','); item.satFat = stoi(cleanNumericField(temp));
        getline(ss, temp, ','); item.fiber = stof(cleanNumericField(temp));
        getline(ss, temp, ','); item.carbs = stof(cleanNumericField(temp));
        getline(ss, item.category);
    }
    catch (const std::invalid_argument& e) {
        cerr << "Error parsing line: " << line << "\nException message: " << e.what() << endl;
    }

    return item;
}

void loadDatasetToArray(FoodItem arr[], int& size, const string& filename) {
    ifstream file(filename);
    string line;
    size = 0;

    if (file.is_open()) {
        getline(file, line); // Skip header
        while (getline(file, line) && size < MAX_ITEMS) {
            arr[size++] = parseCSVLine(line);
        }
    }
    else {
        cout << "Unable to open file: " << filename << endl;
    }
    file.close();
}

bool compareFoodItems(const FoodItem& a, const FoodItem& b, SortColumn column, bool ascending) {
    switch (column) {
    case SortColumn::Grams:
        return ascending ? a.grams < b.grams : a.grams > b.grams;
    case SortColumn::Calories:
        return ascending ? a.calories < b.calories : a.calories > b.calories;
    case SortColumn::Protein:
        return ascending ? a.protein < b.protein : a.protein > b.protein;
    case SortColumn::Fat:
        return ascending ? a.fat < b.fat : a.fat > b.fat;
    case SortColumn::SatFat:
        return ascending ? a.satFat < b.satFat : a.satFat > b.satFat;
    case SortColumn::Fiber:
        return ascending ? a.fiber < b.fiber : a.fiber > b.fiber;
    case SortColumn::Carbs:
        return ascending ? a.carbs < b.carbs : a.carbs > b.carbs;
    default:
        return false;
    }
}

void merge(FoodItem arr[], int const left, int const mid, int const right, SortColumn column, bool ascending) {
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    auto* leftArray = new FoodItem[subArrayOne],
        * rightArray = new FoodItem[subArrayTwo];

    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = arr[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = arr[mid + 1 + j];

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (compareFoodItems(leftArray[indexOfSubArrayOne], rightArray[indexOfSubArrayTwo], column, ascending)) {
            arr[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            arr[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        arr[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        arr[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

void mergeSortArray(FoodItem arr[], int const begin, int const end, SortColumn column, bool ascending) {
    if (begin >= end)
        return;

    auto mid = begin + (end - begin) / 2;
    mergeSortArray(arr, begin, mid, column, ascending);
    mergeSortArray(arr, mid + 1, end, column, ascending);
    merge(arr, begin, mid, end, column, ascending);
}

void bubbleSortArray(FoodItem arr[], int size, SortColumn column, bool ascending) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (!compareFoodItems(arr[j], arr[j + 1], column, ascending)) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function to perform insertion sort on the 'Food' column
void insertionSort(FoodItem arr[], int n, SortColumn column, bool ascending) {
    for (int i = 1; i < n; i++) {
        FoodItem key = arr[i];
        int j = i - 1;

        while (j >= 0 && compareFoodItems(arr[j], key, column, !ascending)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void displayArray(const FoodItem arr[], int size) {
    cout << left << setw(20) << "Food" << setw(15) << "Measure" << setw(8) << "Grams"
        << setw(10) << "Calories" << setw(10) << "Protein" << setw(8) << "Fat"
        << setw(10) << "Sat.Fat" << setw(10) << "Fiber" << setw(10) << "Carbs"
        << setw(15) << "Category" << endl;
    cout << string(106, '-') << endl; // Line separator

    for (int i = 0; i < size; ++i) {
        cout << left << setw(20) << arr[i].food.substr(0, 17) + "..."
            << setw(15) << arr[i].measure.substr(0, 12) + "..."
            << setw(8) << arr[i].grams
            << setw(10) << arr[i].calories
            << setw(10) << arr[i].protein
            << setw(8) << arr[i].fat
            << setw(10) << arr[i].satFat
            << setw(10) << fixed << setprecision(1) << arr[i].fiber
            << setw(10) << fixed << setprecision(1) << arr[i].carbs
            << setw(15) << arr[i].category.substr(0, 12) + "..." << endl;
    }
}

int main() {
    FoodItem* foodArray = new FoodItem[MAX_ITEMS];
    int size = 0;
    string filename = "Nutrients_Info_Modified.csv";

    loadDatasetToArray(foodArray, size, filename);

    while (true) {
        system("cls"); // Clear the screen for better readability

        cout << "-----------------------------------" << endl;
        cout << "|        Array Sorting            |" << endl;
        cout << "-----------------------------------" << endl;
        cout << "This program sorts food items in an array based on selected criteria." << endl << endl;

        SortColumn column = SortColumn::Grams;
        bool ascending = true;
        int columnChoice, orderChoice, sortAlgorithmChoice;

        cout << "Select column to sort by (0: Grams, 1: Calories, 2: Protein, 3: Fat, 4: SatFat, 5: Fiber, 6: Carbs): ";
        cin >> columnChoice;
        column = static_cast<SortColumn>(columnChoice);

        cout << "Select sort order (0: Descending, 1: Ascending): ";
        cin >> orderChoice;
        ascending = orderChoice != 0;

        cout << "Select sorting algorithm (0: Bubble Sort, 1: Merge Sort, 2: Insertion Sort): ";
        cin >> sortAlgorithmChoice;

        auto start = chrono::high_resolution_clock::now();
        string sortAlgorithmName;

        if (sortAlgorithmChoice == 0) {
            bubbleSortArray(foodArray, size, column, ascending);
            sortAlgorithmName = "Bubble Sort";
        }
        else if (sortAlgorithmChoice == 1) {
            mergeSortArray(foodArray, 0, size - 1, column, ascending);
            sortAlgorithmName = "Merge Sort";
        }
        else if (sortAlgorithmChoice == 2) {
            insertionSort(foodArray, size, column, ascending);
            sortAlgorithmName = "Insertion Sort";
        }
        else {
            break;
        }

        auto end = chrono::high_resolution_clock::now();
        auto sortingTime = chrono::duration_cast<chrono::duration<double, std::micro>>(end - start).count() / 1000.0;

        cout << "\nSorted Array using " << sortAlgorithmName << ":\n";
        displayArray(foodArray, size);

        cout << "\nTotal sorting time - " << sortAlgorithmName << ": " << fixed << setprecision(7) << sortingTime << " ms\n";

        cout << "\nPress any key to continue...";
        cin.ignore();
        cin.get();
    }

    delete[] foodArray;

    return 0;
}