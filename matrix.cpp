#include <iostream>       // For input/output operations
#include <fstream>        // For file operations
#include <iomanip>        // For output formatting
#include <vector>         // For using vectors
#include <type_traits>    
#include <limits>         // For input validation

using namespace std;      // Setting the standard namespace for the rest of the file

// Template class to represent a square matrix of any numeric type
template<typename T>
class Matrix {
private:
    int size;             // Size of the matrix (n x n)
    vector<vector<T>> data; // nested vector to store matrix elements

public:
    // Matrix constructor creates an n x n matrix initialized with default values
    Matrix(int n) : size(n), data(n, vector<T>(n)) {}

    // Reads matrix elements from an input file
    void readFromFile(ifstream& in) {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                in >> data[i][j];  // Read each element from file
    }

    // Displays the matrix with formatted output
    void display() const {
        for (const auto& row : data) {         // For each row
            for (const auto& elem : row)       // For each element in row
                // Format output: width 10, 2 decimal places, convert to double for consistent display
                cout << setw(10) << fixed << setprecision(2) << static_cast<double>(elem);
            cout << "\n";  // New line after each row
        }
    }

    // Overloading operator for matrix addition
    Matrix<T> operator+(const Matrix<T>& other) const {
        Matrix<T> result(size);  // Create result matrix
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];  // Element-wise addition
        return result;
    }

    // Overloading operator for matrix multiplication
    Matrix<T> operator*(const Matrix<T>& other) const {
        Matrix<T> result(size);  // Create result matrix
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                for (int k = 0; k < size; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];  // Dot product
        return result;
    }

    // Calculates and prints sums of main and secondary diagonals
    void printDiagonalSums() const {
        T mainDiag = 0, secDiag = 0;  // Initialize sums
        for (int i = 0; i < size; ++i) {
            mainDiag += data[i][i];                   // Main diagonal (i,i)
            secDiag += data[i][size - i - 1];         // Secondary diagonal (i, n-i-1)
        }
        cout << "Main Diagonal Sum: " << mainDiag << endl;
        cout << "Secondary Diagonal Sum: " << secDiag << endl;
    }

    // Swaps two rows if indices are valid
    void swapRows(int r1, int r2) {
        if (r1 >= 0 && r2 >= 0 && r1 < size && r2 < size)
            swap(data[r1], data[r2]);  // Standard library swap
    }

    // Swaps two columns if indices are valid
    void swapCols(int c1, int c2) {
        if (c1 >= 0 && c2 >= 0 && c1 < size && c2 < size)
            for (int i = 0; i < size; ++i)
                swap(data[i][c1], data[i][c2]);  // Swap elements in each row
    }

    // Updates an element if indices are valid
    void updateElement(int r, int c, T val) {
        if (r >= 0 && c >= 0 && r < size && c < size)
            data[r][c] = val;  // Assign new value
    }
};

// Template function to swap two vectors (used for rows)
template<typename T>
void polymorphicSwap(vector<T>& row1, vector<T>& row2) {
    swap(row1, row2);  // Standard library swap
}

// Template function to swap columns in a matrix
template<typename T>
void polymorphicSwap(vector<vector<T>>& matrix, int c1, int c2, bool isColumn) {
    if (isColumn) {  // Only proceed if it's a column swap
        for (auto& row : matrix) {  // For each row in matrix
            if (c1 >= 0 && c2 >= 0 && c1 < row.size() && c2 < row.size())
                swap(row[c1], row[c2]);  // Swap elements in the specified columns
        }
    }
}

int main() {
    // Get filename from user
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    // Open file and check if successful
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file.\n";
        return 1;  // Exit with error code
    }

    // Read matrix size and type flag from file
    int n, typeFlag;
    file >> n >> typeFlag;  // First line contains size and type (0=int, 1=double)

    // Process integer matrices
    if (typeFlag == 0) {
        Matrix<int> m1(n), m2(n);  // Create two integer matrices
        m1.readFromFile(file);     // Read data from file
        m2.readFromFile(file);

        // Display matrices
        cout << "Matrix 1:\n"; m1.display();
        cout << "Matrix 2:\n"; m2.display();

        // Perform and display operations
        cout << "\nMatrix Addition:\n"; (m1 + m2).display();
        cout << "\nMatrix Multiplication:\n"; (m1 * m2).display();
        cout << "\nDiagonal Sums for Matrix 1:\n"; m1.printDiagonalSums();

        // Row swap with input validation
        int r1, r2;
        bool validInput = false;
        do {
            cout << "\nEnter two row indices to swap (Example: '0 2'): ";
            if (cin >> r1 >> r2) {  // If input is successful
                if (r1 >= 0 && r1 < n && r2 >= 0 && r2 < n) {  // Check bounds
                    validInput = true;
                } else {
                    cout << "Row indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {  // Input failed (wrong type)
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();  // Clear error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard bad input
            }
        } while (!validInput);
        m1.swapRows(r1, r2);
        cout << "After Swapping Rows:\n"; m1.display();

        // Column swap with input validation (similar to row swap)
        int c1, c2;
        validInput = false;
        do {
            cout << "\nEnter two column indices to swap (Example: '1 0'): ";
            if (cin >> c1 >> c2) {
                if (c1 >= 0 && c1 < n && c2 >= 0 && c2 < n) {
                    validInput = true;
                } else {
                    cout << "Column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapCols(c1, c2);
        cout << "After Swapping Columns:\n"; m1.display();

        // Element update with input validation
        int ur, uc, newVal;
        validInput = false;
        do {
            cout << "\nEnter row, column, and new value to update (Example: '0 0 99'): ";
            if (cin >> ur >> uc >> newVal) {
                if (ur >= 0 && ur < n && uc >= 0 && uc < n) {
                    validInput = true;
                } else {
                    cout << "Row and column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter three integers (row, column, value).\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.updateElement(ur, uc, newVal);
        cout << "After Updating Element:\n"; m1.display();

    } else {  // Process double matrices (similar to integer case)
        Matrix<double> m1(n), m2(n);
        m1.readFromFile(file);
        m2.readFromFile(file);

        cout << "Matrix 1:\n"; m1.display();
        cout << "Matrix 2:\n"; m2.display();

        cout << "\nMatrix Addition:\n"; (m1 + m2).display();
        cout << "\nMatrix Multiplication:\n"; (m1 * m2).display();
        cout << "\nDiagonal Sums for Matrix 1:\n"; m1.printDiagonalSums();

        int r1, r2;
        bool validInput = false;
        do {
            cout << "\nEnter two row indices to swap (Example: '0 2'): ";
            if (cin >> r1 >> r2) {
                if (r1 >= 0 && r1 < n && r2 >= 0 && r2 < n) {
                    validInput = true;
                } else {
                    cout << "Row indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapRows(r1, r2);
        cout << "After Swapping Rows:\n"; m1.display();

        int c1, c2;
        validInput = false;
        do {
            cout << "\nEnter two column indices to swap (Example: '1 0'): ";
            if (cin >> c1 >> c2) {
                if (c1 >= 0 && c1 < n && c2 >= 0 && c2 < n) {
                    validInput = true;
                } else {
                    cout << "Column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapCols(c1, c2);
        cout << "After Swapping Columns:\n"; m1.display();

        int ur, uc;
        double newVal;
        validInput = false;
        do {
            cout << "\nEnter row, column, and new value to update (Example: '0 0 99'): ";
            if (cin >> ur >> uc >> newVal) {
                if (ur >= 0 && ur < n && uc >= 0 && uc < n) {
                    validInput = true;
                } else {
                    cout << "Row and column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers followed by a double (row column value).\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.updateElement(ur, uc, newVal);
        cout << "After Updating Element:\n"; m1.display();
    }

    return 0;
}