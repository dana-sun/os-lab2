#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <string>
#include <cstring>
#include <queue>
using namespace std;

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args) {
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); 
}

 int formatting[2] = {15, 25};
queue<int> ready;
int remaining;
int processes[10000][5]; 
int progress[10000][5]; 
int cycle;
int leftoverQ;

void fifo(int n){
    remaining = n;
    cycle = 1;

    memset(progress, 0, n*5*sizeof(int));

    for(int i = 0; i < n; i++) {
        cout << string_format("Please enter the phases for process %d: ", i+1);
        for(int j = 0; j < 4; j++) {
            cin >> processes[i][j];
        }
        processes[i][4] = 1;
        ready.push(i);
    }

    cout << left << setw(formatting[0]) << "Cycle";
    for(int i = 0; i < n; i++) {
        cout << left << setw(formatting[1]) << string_format("P%d State", i+1);
    }
    cout << endl;
    while(remaining != 0){
        bool finished = false;
        cout << left << setw(formatting[0]) << cycle;
        for(int i = 0; i < n; i++) {
            string txt = "";
            for(int j = 0; j < 5; j++) {
                if (progress[i][j] < processes[i][j]) {
                    if (j % 2 == 0) {
                        if(i != ready.front()){
                            txt="Ready";
                        }
                        else {
                            progress[i][j]++;
                            if(progress[i][j] == processes[i][j]) {
                                finished = true;
                            }
                            if(j == 4){
                                txt = "Terminate";
                                remaining--;
                            }
                            else{
                                txt = string_format("Run (%d of %d)", progress[i][j], processes[i][j]);
                            }
                        }
                    }
                    else{
                        progress[i][j]++;
                        if(progress[i][j] == processes[i][j]) {
                            ready.push(i);
                        }
                        txt= string_format("Blocked (%d of %d)", progress[i][j], processes[i][j]);
                    }
                    break;
                }
            }
            cout << left << setw(formatting[1]) << txt;
        }
        cout << endl;
        cycle++;
        if(finished) { ready.pop(); }
    }
}

void rr(int n, int q) {
    remaining = n;
    cycle = 1;
    leftoverQ = q;
    memset(progress, 0, n*5*sizeof(int));

    for(int i = 0; i < n; i++) {
        cout << string_format("Please enter the phases for process %d: ", i+1);
        for(int j = 0; j < 4; j++) {
            cin >> processes[i][j];
        }
        processes[i][4] = 1;
        ready.push(i);
    }   

    cout << left << setw(formatting[0]) << "Cycle";
    for(int i = 0; i < n; i++) {
        cout << left << setw(formatting[1]) << string_format("P%d State", i+1);
    }
    cout << endl;
    while(remaining != 0){
        bool finished = false;
        cout << left << setw(formatting[0]) << cycle;
        for(int i = 0; i < n; i++) {
            string txt = "";
            for(int j = 0; j < 5; j++) {
                if (progress[i][j] < processes[i][j]) {
                    if (j % 2 == 0) {
                        if(i != ready.front()){
                            txt="Ready";
                        }
                        else {
                            progress[i][j]++;
                            leftoverQ--;
                            if(leftoverQ == 0|| progress[i][j] == processes[i][j]) {
                                if(progress[i][j] != processes[i][j]){
                                    ready.push(ready.front());
                                }
                                finished = true;
                                leftoverQ = q;
                            }
                            if(j == 4){
                                txt = "Terminate";
                                remaining--;
                            }
                            else{
                                txt = string_format("Run (%d of %d)", progress[i][j], processes[i][j]);
                            }
                        }
                    }
                    else{
                        progress[i][j]++;
                        if(progress[i][j] == processes[i][j]) {
                            ready.push(i);
                        }
                        txt= string_format("Blocked (%d of %d)", progress[i][j], processes[i][j]);
                    }
                    break;
                }
            }
            cout << left << setw(formatting[1]) << txt;
        }
        cout << endl;
        cycle++;
        if(finished) { ready.pop(); }
    }
}

int main() {
    int n; int SA; int q;

    cout << "Input the number of processes: ";
    cin >> n;
    cout << "Enter '1' for a FIFO algorithm and '2' for RR Scheduling: ";
    cin >> SA;
    if(SA == 1) {
        fifo(n);
    } else if(SA == 2) {
        cout << "Enter the quantum value for your RR scheduling: ";
        cin >> q;
        rr(n, q);
    }
    cout << endl;
}
