
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <tuple> 
#include <iostream>
#include <regex>
#include <ctime>
#include <chrono> 
using namespace std;

// Struct dai dien cho du lieu Disney
struct Disney {
    string Date;
    float Open_Price;
    float High_Price;
    float Low_Price;
    float Close_Price;
    int Volume;
};

// Struct dai dien cho node trong cay nhi phan
struct TreeNode {
    Disney data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Disney x) : data(x), left(nullptr), right(nullptr) {}
};

// Ham tao doi tuong Disney tu mot chuoi du lieu
Disney CreateObject(const string& line) {
    Disney d;
    try {
        stringstream ss(line);
        string temp;

        getline(ss, d.Date, ',');
        getline(ss, temp, ','); d.Open_Price = stof(temp);
        getline(ss, temp, ','); d.High_Price = stof(temp);
        getline(ss, temp, ','); d.Low_Price = stof(temp);
        getline(ss, temp, ','); d.Close_Price = stof(temp);
        getline(ss, temp, ','); d.Volume = stoi(temp);
    }
    catch (const exception& e) {
        cout << "Loi khi phan tich dong: " << line << ". Ngoai le: " << e.what() << endl;
    }
    return d;
}
// Ham chen du lieu vao cay nhi phan tim kiem theo ngay (Date)
TreeNode* insert(TreeNode* root, Disney x) {
    if (root == nullptr) {
        return new TreeNode(x); // Neu cay rong k them not moi
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (x.Date == current->data.Date) {
            //Ngay da ton tai
            cout << "Ngay " << x.Date << " da ton tai. Khong the them." << endl;
            return nullptr; // khong them nut moi
        }
        if (x.Date < current->data.Date) {
            current = current->left; // Duyet trai
        }
        else {
            current = current->right; //Duyet phai
        }
    }

    // Them nut moi
    if (x.Date < parent->data.Date) {
        parent->left = new TreeNode(x);
    }
    else {
        parent->right = new TreeNode(x);
    }
    return root;
}

// Ham doc du lieu tu file
void readFromFile(TreeNode*& root, const string& filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }

    string line;
    bool isFirstLine = true; // Bo qua dong dau (header)
    while (getline(input, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (!line.empty()) {
            try {
                Disney data = CreateObject(line);
                root = insert(root, data);
            }
            catch (const exception& e) {
                cout << "Bo qua dong khong hop le: " << line << endl;
            }
        }
    }
    input.close();
    cout << "Doc file thanh cong." << endl;
}
// Tim ngay cao nhat theo tieu chi
Disney findMax(TreeNode* root, const string& type) {
    if (root == NULL) {
        cout << "Cay rong" << endl;
        return Disney();
    }

    stack<TreeNode*> s;
    TreeNode* current = root;
    Disney maxDisney = root->data;

    // Duyet cay theo thu tu (Left - Node - Right)
    while (current != NULL || !s.empty()) {
        while (current != NULL) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        // So sanh cac gia tri theo tieu chi
        if ((type == "Open_Price" && current->data.Open_Price > maxDisney.Open_Price) ||
            (type == "High_Price" && current->data.High_Price > maxDisney.High_Price) ||
            (type == "Low_Price" && current->data.Low_Price > maxDisney.Low_Price) ||
            (type == "Close_Price" && current->data.Close_Price > maxDisney.Close_Price) ||
            (type == "Volume" && current->data.Volume > maxDisney.Volume)) {
            maxDisney = current->data; //Cap nhat gia tri max
        }

        // Duyệt qua nút bên phải
        current = current->right;
    }

    return maxDisney;
}
//Kiem tra dau vao
float checkFloat(const string& prompt)
{
    float value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        //Kiem tra neu nguoi dung nhap sai
        if (cin.fail() || value < 0) {
            cout << "Vui long nhap lai so hop le (Khong duoc nhap chu cai, ky tu va so am" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //xoa bo nho dem
        }
        else {
            break; //neu hop le thoat khoi vong lap
        }
    }
    return value;
}
//Kiem tra dau vao
int checkInt(const string& prompt)
{
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cout << "Vui long nhap so nguyen duong (khong duoc nhap chu cai hoac so am)." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    return value;
}
//Tim Node nho nhat 
TreeNode* minNode(TreeNode* root)
{
    //Duyet cay den nut cuoi cung ben trai
    TreeNode* current = root;
    while (current and current->left != NULL)
    {
        current = current->left;
    }
    return current;
}
//Xoa 1 Node theo Date 
TreeNode* deleteNodeByDate(TreeNode* root, const string& date)
{
    if (root == NULL)
    {
        return root;
    }
    //Neu ngay can xoa nho hon
    if (date < root->data.Date)
    {
        root->left = deleteNodeByDate(root->left, date);
    }
    //Neu ngay can xoa lon hon 
    if (date > root->data.Date)
    {
        root->right = deleteNodeByDate(root->right, date);
    }
    //Neu ngay cua Node hien tai la Node can xoa 
    else {
        //TH1: Node khong co con
        if (root->left == NULL and root->right == NULL)
        {
            delete root;
            return NULL;
        }
        //TH2: Node co mot con 
        else if (root->left == NULL)
        {
            TreeNode* temp = root;
            root = root->right;
            delete temp;
        }
        else if (root->right == NULL)
        {
            TreeNode* temp = root;
            root = root->left;
            delete temp;
        }
        //TH3: Cay co ca con trai va con phai
        else {
            TreeNode* temp = minNode(root->right);
            root->data = temp->data;
            root->right = deleteNodeByDate(root->right, temp->data.Date);
        }
    }
    return root;
}
//Thay doi thong tin theo Date 
bool updateNodeByDate(TreeNode* root, const string& dateStr)
{
    if (root == NULL)
    {
        cout << "Cay rong!" << endl; // In thong bao khi cay rong
        return false; // Tra ve false khi cay rong
    }

    TreeNode* current = root;
    stack<TreeNode*> s; // Stack de duyet cay theo LNR
    bool found = false; // Bien kiem tra xem co tim thay node khong

    // Duyet cay theo thu tu trung vi (In-order)
    while (current != NULL || !s.empty())
    {
        // Duyet sang trai
        while (current != NULL) {
            s.push(current); // Day node hien tai vao stack
            current = current->left; // Di chuyen sang node con trai
        }

        // Lay node tiep theo tu stack
        current = s.top();
        s.pop();

        // Kiem tra ngay cua node hien tai
        if (current->data.Date == dateStr)
        {
            found = true; // Tim thay node co ngay khop
            cout << "Da tim thay ngay: " << dateStr << endl;
            cout << "Thong tin hien tai: \n";
            cout << "Open Price: " << current->data.Open_Price << "\n"
                << "High Price: " << current->data.High_Price << "\n"
                << "Low Price: " << current->data.Low_Price << "\n"
                << "Close Price: " << current->data.Close_Price << "\n"
                << "Volume: " << current->data.Volume << "\n";

            // Nhap du lieu moi va kiem tra dau vao
            float newOpen = checkFloat("Nhap Open Price: ");
            float newHigh = checkFloat("Nhap High Price: ");
            float newLow = checkFloat("Nhap Low Price: ");
            float newClose = checkFloat("Nhap Close Price: ");
            int newVolume = checkInt("Nhap Volume: ");

            // Cap nhat gia tri moi cho node
            current->data.Open_Price = newOpen;
            current->data.High_Price = newHigh;
            current->data.Low_Price = newLow;
            current->data.Close_Price = newClose;
            current->data.Volume = newVolume;

            // Thong bao cap nhat thanh cong
            cout << "Da cap nhat thanh cong!" << endl;
            return true; // Tra ve true khi cap nhat thanh cong
        }

        // Duyet sang phai
        current = current->right;
    }

    // Neu khong tim thay node co ngay khop
    if (!found) {
        cout << "Khong tim thay ngay: " << dateStr << endl;
    }

    return false; // Tra ve false khi khong tim thay node
}
// Paginate and display data
void printPage(const vector<Disney>& data, int pageNumber, int itemsPerPage) {
    int startIdx = (pageNumber - 1) * itemsPerPage;
    int endIdx = min(startIdx + itemsPerPage, (int)data.size());

    if (startIdx >= data.size()) {
        cout << "Khong co du lieu de hien thi!" << endl;
        return;
    }

    // Print the data on the current page
    for (int i = startIdx; i < endIdx; ++i) {
        cout << "Disney Data " << i + 1 << ":\n";
        cout << "Open Price: " << data[i].Open_Price << "\n"
            << "High Price: " << data[i].High_Price << "\n"
            << "Low Price: " << data[i].Low_Price << "\n"
            << "Close Price: " << data[i].Close_Price << "\n"
            << "Volume: " << data[i].Volume << "\n";
        cout << "------------------------------------------\n";
    }
}

// Pagination functionality
void paginate(vector<Disney>& data, int itemsPerPage) {
    int totalPages = (data.size() + itemsPerPage - 1) / itemsPerPage;
    int currentPage = 1;

    while (true) {
        cout << "Trang " << currentPage << " / " << totalPages << "\n";
        printPage(data, currentPage, itemsPerPage);

        // User input for navigating between pages
        cout << "Nhap '1' de di toi trang tiep theo, '2' de di den trang truoc, '0' de thoat: ";
        char choice;
        cin >> choice;

        if (choice == '1' && currentPage < totalPages) {
            currentPage++; // Go to next page
        }
        else if (choice == '2' && currentPage > 1) {
            currentPage--; // Go to previous page
        }
        else if (choice == '0') {
            break; // Exit
        }
        else {
            cout << "Invalid choice, please try again!" << endl;
        }
    }
}
//Ham tim thong thong theo Date
TreeNode* searchNodeByDate(TreeNode* root, const string& dateStr)
{
    if (root == NULL)
    {
        return NULL;
    }

  
    if (root->data.Date == dateStr)
    {
        return root;
    }
   
    else if (dateStr < root->data.Date)
    {
        return searchNodeByDate(root->left, dateStr);
    }
    else
    {
        return searchNodeByDate(root->right, dateStr);
    }
}
//Sap xep su dung selection sort 
void selectionSort(vector<Disney>& data, const string& criteria)
{
    for (int i = 0; i < data.size() - 1;i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < data.size(); j++)
        {
            if (criteria == "Open Price" && data[j].Open_Price < data[minIndex].Open_Price) {
                minIndex = j;
            }
            else if (criteria == "Close Price" && data[j].Close_Price < data[minIndex].Close_Price)
            {
                minIndex = j;
            }
            else if (criteria == "Volume" && data[j].Close_Price < data[minIndex].Close_Price)
            {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(data[i], data[minIndex]);
        }
    }
}
//Sap xep theo Insertion sort 
void insertionSort(vector<Disney>& data, const string& criteria)
{
    for (int i = 1; i < data.size(); ++i) {
        Disney key = data[i];
        int j = i - 1;

        while (j >= 0) {
            if (criteria == "Open Price" && key.Open_Price < data[j].Open_Price) {
                data[j + 1] = data[j];
            }
            else if (criteria == "Close Price" && key.Close_Price < data[j].Close_Price) {
                data[j + 1] = data[j];
            }
            else if (criteria == "Volume" && key.Volume < data[j].Volume) {
                data[j + 1] = data[j];
            }
            else {
                break;
            }
            --j;
        }
        data[j + 1] = key;
    }
}
//Ve Bieu do 
void drawLineChart(const vector<Disney>& data, const string& criteria) {
    if (data.empty()) {
        cout << "Khong co du lieu de ve bieu do.\n";
        return;
    }

    // Xac dinh gia tri lon nhat de chuan hoa
    float maxValue = 0;
    for (const auto& item : data) {
        if (criteria == "Open_Price") {
            maxValue = max(maxValue, item.Open_Price);
        }
        else if (criteria == "Close_Price") {
            maxValue = max(maxValue, item.Close_Price);
        }
        else if (criteria == "Volume") {
            maxValue = max(maxValue, static_cast<float>(item.Volume));
        }
        else {
            cout << "Tieu chi khong hop le.\n";
            return;
        }
    }

    // Ve Bieu Do
    cout << "Bieu do duong: " << criteria << "\n";
    cout << "-----------------------------------------\n";

    for (const auto& item : data) {
        //Tinh so luong * tuong tung voi gia tri
        int numStars = 0;
        if (criteria == "Open_Price") {
            numStars = static_cast<int>((item.Open_Price / maxValue) * 50); // Chuan hoa den 50 ky tu
        }
        else if (criteria == "Close_Price") {
            numStars = static_cast<int>((item.Close_Price / maxValue) * 50);
        }
        else if (criteria == "Volume") {
            numStars = static_cast<int>((item.Volume / maxValue) * 50);
        }

        //In so dong cua bieu do
        cout << item.Date << " | ";
        for (int i = 0; i < numStars; ++i) {
            cout << "*";
        }
        cout << " ("; // Gia tri cuoi dung de hien thi
        if (criteria == "Open_Price") {
            cout << item.Open_Price;
        }
        else if (criteria == "Close_Price") {
            cout << item.Close_Price;
        }
        else if (criteria == "Volume") {
            cout << item.Volume;
        }
        cout << ")\n";
    }

    cout << "-----------------------------------------\n";
}

void writeFile(TreeNode* root, const string filename)
{
    ofstream output(filename);
    if (!output.is_open()) {
        cout << "Khong the mo file" << endl;
        return;
    }
    stack<TreeNode*> s;
    TreeNode* current = root;
    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }
        current = s.top();
        s.pop();

        // Ghi du lieu vao file theo dinh dang "Date,Open_Price,High_Price,Low_Price,Close_Price,Volume"
        output << current->data.Date << ","
            << current->data.Open_Price << ","
            << current->data.High_Price << ","
            << current->data.Low_Price << ","
            << current->data.Close_Price << ","
            << current->data.Volume << endl;

        current = current->right; // Duyet sang phai
    }
    output.close();
    cout << "Du lieu da duoc ghi vao file." << endl;
}
void displayMenu()
{
    cout << "~~~~~~~~~~~~~~~~~****Menu***~~~~~~~~~~~~~~~~~\n";
    cout << "1.Xem danh sach du lieu\n";
    cout << "2.Tim kiem theo ngay\n";
    cout << "3.Them du lieu \n";
    cout << "4.Xoa du lieu theo ngay\n";
    cout << "5.Cap nhat du lieu theo ngay\n";
    cout << "6.Tim gia tri cao nhat theo tieu chi\n";
    cout << "7. Sap xep du lieu (Insertion Sort va Selection Sort) theo tieu chi\n";
    cout << "8.Ve Bieu Do\n";
    cout << "0.Thoat\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "Nhap lua chon(1-8): ";
}
int main()
{
    TreeNode* root = nullptr;
    readFromFile(root, "C:/DSA/DisneyList.TXT");

    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        switch (choice) {
        case 1: {
            vector<Disney> data;
            stack<TreeNode*> s;
            TreeNode* current = root;
            while (current != nullptr || !s.empty()) {
                while (current != nullptr) {
                    s.push(current);
                    current = current->left;
                }
                current = s.top();
                s.pop();
                data.push_back(current->data);
                current = current->right;
            }
            int itemsPerPage = 5; // so trang
            paginate(data, itemsPerPage);
            break;

        }
        case 2:
        {

            string dateStr;
            cout << "Nhap ngay can tim (yyyy-mm-dd): ";
            cin >> dateStr;


            TreeNode* result = searchNodeByDate(root, dateStr);

            if (result != NULL) {

                cout << "Du lieu tim thay: \n";
                cout << "Ngay: " << result->data.Date << endl;
                cout << "Open Price: " << result->data.Open_Price << endl;
                cout << "High Price: " << result->data.High_Price << endl;
                cout << "Low Price: " << result->data.Low_Price << endl;
                cout << "Close Price: " << result->data.Close_Price << endl;
                cout << "Volume: " << result->data.Volume << endl;
            }
            else {

                cout << "Khong tim thay du lieu voi ngay da nhap.\n";
            }
            break;
        }
        case 3: {
            Disney newData;
            cout << "Nhap ngay (yyyy-mm-dd): ";
            cin >> newData.Date;
            newData.Open_Price = checkFloat("Nhap gia Open Price: ");
            newData.High_Price = checkFloat("Nhap gia High Price: ");
            newData.Low_Price = checkFloat("Nhap gia Low Price: ");
            newData.Close_Price = checkFloat("Nhap gia Close Price: ");
            newData.Volume = checkInt("Nhap Volume: ");
            root = insert(root, newData);
            writeFile(root, "C:/DSA/DisneyList.TXT");
            break;
        }
        case 4:
        {
            string dateStr;
            cout << "Nhap ngay can xoa (yyyy-mm-dd): ";
            cin >> dateStr;
            root = deleteNodeByDate(root, dateStr);
            writeFile(root, "C:/DSA/DisneyList.TXT");
            cout << "Du lieu da duoc xoa thanh cong" << endl;
            break;
        }
        case 5:
        {
            string dateStr;
            cout << "Nhap nam muon thay doi thong tin (yyyy-mm-dd): ";
            cin >> dateStr;
            bool result = updateNodeByDate(root, dateStr);

            if (result)
            {
                writeFile(root, "C:/DSA/DisneyList.TXT");
                cout << "Du lieu da duoc cap nhat thanh cong" << endl;
            }
            else
            {
                cout << "Khong the cap nhat du lieu" << endl;
            }
            break;
        }
        case 6:
        {
            int subChoice;
            cout << "Nhap lua chon de in ra theo tieu chi (1. Open_Price, 2. Close_Price, 3. Volume): ";
            cin >> subChoice;

            try {
                switch (subChoice) {
                case 1: {
                    Disney maxOpen = findMax(root, "Open_Price");
                    cout << "Ngay co Open Price cao nhat:\n";
                    cout << "Date: " << maxOpen.Date
                        << ", Open Price: " << maxOpen.Open_Price << endl;
                    break;
                }
                case 2: {
                    Disney maxClose = findMax(root, "Close_Price");
                    cout << "Ngay co Close Price cao nhat:\n";
                    cout << "Date: " << maxClose.Date
                        << ", Close Price: " << maxClose.Close_Price << endl;
                    break;
                }
                case 3: {
                    Disney maxVolume = findMax(root, "Volume");
                    cout << "Ngay co Volume lon nhat:\n";
                    cout << "Date: " << maxVolume.Date
                        << ", Volume: " << maxVolume.Volume << endl;
                    break;
                }
                default:
                    cout << "Lua chon khong hop le. Vui long thu lai.\n";
                    break;
                }
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << endl; 
            }

            break;
        }

        case 7:
        {
            int year;
            cout << "Nhap nam de sap xep du lieu: ";
            cin >> year;
            if (year < 2010 || year>2024)
            {
                cout << "Nam khong hop le. Vui long nhap lai" << endl;
                break;
            }
            vector<Disney> data;
            stack<TreeNode*> s;
            TreeNode* current = root;
            while (current != nullptr || !s.empty()) {
                while (current != nullptr) {
                    s.push(current);
                    current = current->left;
                }
                current = s.top();
                s.pop();

               
                string yearStr = to_string(year);
                if (current->data.Date.substr(0, 4) == yearStr) {
                    data.push_back(current->data);
                }

                current = current->right;
            }

      
            if (data.empty()) {
                cout << "Khong co du lieu nao trong nam " << year << ".\n";
                break;
            }

          
            cout << "Chon tieu chi sap xep:\n";
            cout << "1. Open Price\n";
            cout << "2. Close Price\n";
            cout << "3. Volume\n";
            cout << "Nhap lua chon (1-3): ";
            int option;
            cin >> option;

            string criteria;
            if (option == 1) {
                criteria = "Open Price";
            }
            else if (option == 2) {
                criteria = "Close Price";
            }
            else if (option == 3) {
                criteria = "Volume";
            }
            else {
                cout << "Lua chon khong hop le.\n";
                break;
            }

            // Nguoi dung chon phuong phap sap xep
            cout << "Chon phuong phap sap xep:\n";
            cout << "1. Selection Sort\n";
            cout << "2. Insertion Sort\n";
            cout << "Nhap lua chon (1-2): ";
            int sortOption;
            cin >> sortOption;
            auto start = chrono::high_resolution_clock::now();
            if (sortOption == 1) {
                selectionSort(data, criteria); 
            }
            else if (sortOption == 2) {
                insertionSort(data, criteria); 
            }
            else {
                cout << "Lua chon khong hop le.\n";
                break;
            }
            auto end = chrono::high_resolution_clock::now(); 
            chrono::duration<double> elapsed = end - start;

            cout << "Thoi gian thuc thi sap xep: " << elapsed.count() << " giay.\n";

            cout << "Du lieu trong nam " << year << " sau khi sap xep (" << criteria << "):\n";
            for (const auto& item : data) {
                cout << "Date: " << item.Date << ", Open Price: " << item.Open_Price
                    << ", Close Price: " << item.Close_Price << ", Volume: " << item.Volume << endl;
            }
            break;
        }
        case 8: {
            int year;
            cout << "Nhap nam de ve bieu do: ";
            cin >> year;

            if (year < 2010 || year > 2024) {
                cout << "Nam khong hop le. Vui long nhap trong khoang 2010-2024.\n";
                break;
            }

            vector<Disney> data;
            stack<TreeNode*> s;
            TreeNode* current = root;

            
            while (current != nullptr || !s.empty()) {
                while (current != nullptr) {
                    s.push(current);
                    current = current->left;
                }
                current = s.top();
                s.pop();

                string yearStr = to_string(year);
                if (current->data.Date.substr(0, 4) == yearStr) {
                    data.push_back(current->data);
                }

                current = current->right;
            }

            if (data.empty()) {
                cout << "Khong co du lieu trong nam " << year << ".\n";
                break;
            }

            cout << "Chon tieu chi de ve bieu do:\n";
            cout << "1. Open Price\n";
            cout << "2. Close Price\n";
            cout << "3. Volume\n";
            cout << "Nhap lua chon (1-3): ";
            int option;
            cin >> option;

            string criteria;
            if (option == 1) {
                criteria = "Open_Price";
            }
            else if (option == 2) {
                criteria = "Close_Price";
            }
            else if (option == 3) {
                criteria = "Volume";
            }
            else {
                cout << "Lua chon khong hop le.\n";
                break;
            }

            drawLineChart(data, criteria);
            break;
        }
        case 0:
        {
            cout << "Chuong trinh ket thuc. Cam on ban da su dung!\n";
            exit(0); 
            default:
            cout << "Lua chon khong hop le. Vui long nhap lai.\n";
            break;
        }
     }
  }
}