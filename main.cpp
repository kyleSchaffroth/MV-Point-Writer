#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;
double calculateArcRadius (double chord, double altitude){
    return (pow(chord, 2) / (8 * altitude)) + (altitude / 2.0);
}

string Mirror(string partName,char direction){
    string upperDirection = "";
    if (direction == 'l'){
        upperDirection = "LENGTH";
    }

    if (direction == 'w'){
        upperDirection = "WIDTH";
    }
    string mirrorPhrase = "PART" + upperDirection + "(MVCP," + '"' + partName +'"' + ")";

    return mirrorPhrase;
}

string changeReference(string point){
    size_t pos = point.find("WIDTH");
    if (pos != std::string::npos){
        point.replace(pos,5,"LENGTH");
    }


}

vector<vector<string>> getPoints() {
    vector<vector<string>> points;
    int pointNum = 1;
    int nextPoint = 2;
    string partName;
    cin.ignore();
    cout << "Please enter the name of your part: ";
    getline(cin, partName);

    vector<string> firstPoint;
    bool firstPointCaptured = false;
    int useFirstPoint = 0;
    cout << "\nOption 1 selected, writing vectors with known values." << endl;
    while (true) {
        vector<string> point(3);
        string inputValue;

        cout << "To reference the part's width, enter a w before your input. EX: w - .9361" << endl;
        cout << "To reference the part's length, enter a l before your input. EX: l - .9361" << endl;
        cout << "\nPlease enter point " << pointNum << "'s X Value: ";
        getline(cin >> ws, inputValue);
        if (inputValue.rfind("w ", 0) == 0) {
            string value = inputValue.substr(2); // Extract the value after "w "
            point[0] = Mirror(partName, 'w') + value; // Encase the value
        } else if (inputValue.rfind("l ", 0) == 0) {
            string value = inputValue.substr(2); // Extract the value after "l "
            point[0] = Mirror(partName, 'l') + value; // Encase the value
        } else {
            point[0] = inputValue;
        }

        cout << "Please enter point " << pointNum << "'s Y Value: ";
        getline(cin >> ws, inputValue);
        if (inputValue.rfind("w ", 0) == 0) {
            string value = inputValue.substr(2);
            point[1] = Mirror(partName, 'w') + value;
        } else if (inputValue.rfind("l ", 0) == 0) {
            string value = inputValue.substr(2);
            point[1] = Mirror(partName, 'l') + value;
        } else {
            point[1] = inputValue;
        }

        cout << "Please enter point " << pointNum << "'s Z Value: ";
        getline(cin >> ws, inputValue);
        if (inputValue.rfind("w ", 0) == 0) {
            string value = inputValue.substr(2);
            point[2] = Mirror(partName, 'w') + "(" + value + ")";
        } else if (inputValue.rfind("l ", 0) == 0) {
            string value = inputValue.substr(2);
            point[2] = Mirror(partName, 'l') + "(" + value + ")";
        } else {
            point[2] = inputValue;
        }

        points.push_back(point);

        if (!firstPointCaptured) {
            firstPoint = point;
            firstPointCaptured = true;
        }

        cout << endl << "Point " << pointNum << " accepted." << endl;
        cout << "Enter 4 to end this line, mirroring across the width of the part" << endl;
        cout << "Enter 3 to end this line, mirroring across the length of the part" << endl;
        cout << "Enter 2 to continue." << endl;
        if (firstPointCaptured) {
            cout << "Enter 1 close the vector, using the first point as the last." << endl;
        }
        cout << "Enter 0 to close the vector: ";

        cin >> nextPoint;

        if (cin.fail()) {
            cout << "Invalid input. Assuming you want to finish (0)." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            nextPoint = 0;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (nextPoint == 2) {
            } else if (nextPoint == 1) {
                if (!firstPointCaptured) {
                    cout << "No first point captured to append. Closing vector." << endl;
                    nextPoint = 0;
                }
            } else if (nextPoint == 0) {

            } else {
                cout << "Invalid choice. Assuming you want to finish (0)." << endl;
                nextPoint = 0;
            }
        }

        if (nextPoint == 2) {
            pointNum++;
        } else {
            break;
        }
    }

    if (nextPoint == 1 && firstPointCaptured) {
        points.push_back(firstPoint);
        useFirstPoint = 1;
        cout << "Appended the first point as the last point." << endl;
    } else if (nextPoint == 1 && !firstPointCaptured) {
        cout << "Attempted to append first point, but no first point was captured. Vector closed." << endl;
    }

    return points;
}

string rectangleWriter(double leadIn, double Width, double Depth) {
    double midX = Width / 2;
    double midY = Depth / 2;
    string points[7];
    stringstream ss;
    ss << fixed << setprecision(3) << noshowpoint; // Set precision to 3, fixed-point notation, and no trailing decimal point if zero

    // Point 0
    ss.str(""); // Clear the stringstream
    ss << "PNT(-"<< leadIn << "," << -midY << ",0)";
    points[0] = ss.str();

    // Point 1
    ss.str("");
    ss << "PNT(0," << -midY << ",L!Material_Thickness)";
    points[1] = ss.str();

    // Point 2
    ss.str("");
    ss << "PNT(" << -midX << "," << -midY << ",L!Material_Thickness)";
    points[2] = ss.str();

    // Point 3
    ss.str("");
    ss << "PNT(" << midX << "," << midY << ",L!Material_Thickness)";
    points[3] = ss.str();

    // Point 4
    ss.str("");
    ss << "PNT(" << midX << "," << midY << ",L!Material_Thickness)";
    points[4] = ss.str();

    // Point 5
    ss.str("");
    ss << "PNT(" << midX << "," << -midY << ",L!Material_Thickness)";
    points[5] = ss.str();

    // Point 6
    points[6] = points[1];

    string concatString = points[0];
    for (int i = 1; i < 7; i++) {
        concatString = concatString + "," + points[i];
    }
    concatString = "=VECTORS(" + concatString + ')';
    return concatString;
}

void Menu(){
    cout << "\n\nEnter the corresponding number to navigate:" << endl;
    cout << "\n1: To write a =VECTOR() with known values" << endl;
    cout << "2: To calculate a bulge based on altitude and chord." << endl;
    cout << "3: Rewrite an existing vector across a different axis" << endl;
    cout << "4: smartWriter" << endl; 
    cout << "5: Rectangle Writer"<<endl;
    cout << "0: Terminate the program"<< endl;

}

int main(){
    int menuOption = 4;
    cout << "\nMicrovellum PNT Writer" << endl;
    cout <<"_________________________" << endl << endl;
    Menu();
    cin >> menuOption;
    while(menuOption != 0){
        switch (menuOption){
            case 1: {
                vector<vector<string>> points = getPoints(); // Get the returned vector
                string formattedPoints = "=VECTORS(";
                for(size_t i = 0;i < points.size();++i){
                    formattedPoints += "PNT(" + points[i][0] + "," + points[i][1] + "," + points[i][2] + ")";
                    if (i < points.size() - 1) {
                        formattedPoints += ",";
                    }
                }
                string firstPoint = "PNT(" + points[0][0] + "," + points [0][1] + "," + points [0][2] + ")";
                cout << firstPoint;
                formattedPoints += ")";
                cout << "\nYour formatted line: " << formattedPoints <<endl;
                break;
            }

            case 2:
            double chordLength;
            double altitude;
            cout << "Chord Length:";
            cin >> chordLength;
            cout <<"\nAltitude:";
            cin >> altitude;
            cout <<"\nBulge:" << calculateArcRadius(chordLength,altitude);
                break;

            case 3:
                cout <<"\nWIP\n";
                break;
            case 4:
            break;

            case 5:
            cin.clear();
            double Width;
            double Depth;
            double leadIn;
            cout<<"Please enter the width (x) of your cutout:";
            cin >> Width;
            cout<<"\nPlease enter the depth (y) of your cutout:";
            cin >> Depth;
            cout<<"\nPlease enter the length of the lead in of your cutout:"<<endl;
            cin >> leadIn;
            cout << endl << rectangleWriter(leadIn,Width,Depth);

            break;

            default:
                break;
        }
    char answer;
        cout << "\n\nContinue? (y,n):";
                cin >> answer;
        while(answer != 'n' && answer != 'N' && answer != 'y' && answer != 'Y'){ // Corrected the while condition
            cout << "Invalid response. Do you want to continue? (y,n):";
            cin >> answer;
        if(answer == 'y' || answer == 'Y'){
            break;
        }
        if(answer == 'n'|| answer == 'N'){
            break;
        }
    }
    cout <<"Program Terminated.";
    return 0;
}