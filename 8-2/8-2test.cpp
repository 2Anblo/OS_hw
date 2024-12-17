#include <iostream>
#include <fstream>
using namespace std;

struct operation {
    int time;
    int block;
    int oper;
    int protection;
};

int main() {
    ifstream file("opfile", ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    operation op;
    int index = 0;
    while (file.read(reinterpret_cast<char*>(&op), sizeof(op))) {
        cout << "Record " << index++ << ": ";
        cout << "time=" << op.time << ", block=" << op.block
             << ", oper=" << op.oper << ", protection=" << op.protection << endl;
    }

    file.close();
    system("pause");
    return 0;
}
