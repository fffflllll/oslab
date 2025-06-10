#include "dask.h"

// Ensure the class name matches the definition in the header file
using namespace std;
DiskArm::DiskArm() {
    srand(static_cast<unsigned>(time(0)));
    // 自动参数设置
    CurrentCylinder = 50;      // 当前磁头初始位置
    SeekDirection = 1;         // 1为向大号方向，0为向小号方向
    RequestNumber = 10;        // 请求数
    int max_cylinder = 199;    // 最大柱面号

    Request = new int[RequestNumber];
    Cylinder = new int[RequestNumber];
    for (int i = 0; i < RequestNumber; i++) {
        Request[i] = rand() % (max_cylinder + 1);
    }
    cout << "Current Cylinder: " << CurrentCylinder << endl;
    cout << "Seek Direction: " << SeekDirection << endl;
    cout << "Request Sequence: ";
    for (int i = 0; i < RequestNumber; i++) {
        cout << Request[i] << " ";
    }
    cout << endl;
}
DiskArm::~DiskArm() {
    delete[] Request;
    delete[] Cylinder;
}
void DiskArm::InitSpace(char* MethodName) {
    cout << endl << "==== " << MethodName << " ====" << endl;
    SeekNumber = 0;
    SeekChange = 0;
    for (int i = 0; i < RequestNumber; i++)
        Cylinder[i] = Request[i];
}
void DiskArm::Report(void) {
    cout << endl;
    cout << "Seek Number: " << SeekNumber << endl;
    cout << "Change Direction: " << SeekChange << endl;
    cout << "AVG Seek: " << (float)SeekNumber / RequestNumber << endl;
}
void DiskArm::FCFS(void) {  
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace((char*)"FCFS");
    cout << Current;
    for (int i = 0; i < RequestNumber; i++) {
        if (((Cylinder[i] >= Current) && !Direction) ||
            ((Cylinder[i] < Current) && Direction)) {
            SeekChange++;
            Direction = !Direction;
            cout << endl << Current << " -> " << Cylinder[i];
        } else
            cout << " -> " << Cylinder[i];
        SeekNumber += abs(Current - Cylinder[i]);
        Current = Cylinder[i];
    }
    Report();
}

void DiskArm::SSTF(void) {  
    int Shortest = 0;
    int Direction = SeekDirection;
    int Current = CurrentCylinder;
    InitSpace((char*)"SSTF");
    cout << Current;
    for (int i = 0; i < RequestNumber; i++) {
        int Distance = 999999;
        for (int j = 0; j < RequestNumber; j++) {
            if (Cylinder[j] == -1)
                continue;
            if (Distance > abs(Current - Cylinder[j])) {
                Distance = abs(Current - Cylinder[j]);
                Shortest = j;
            }
        }
        if (((Cylinder[Shortest] >= Current) && !Direction) ||
            ((Cylinder[Shortest] < Current) && Direction)) {
            SeekChange++;
            Direction = !Direction;
            cout << endl << Current << " -> " << Cylinder[Shortest];
        } else
            cout << " -> " << Cylinder[Shortest];
        SeekNumber += abs(Current - Cylinder[Shortest]);
        Current = Cylinder[Shortest];
        Cylinder[Shortest] = -1;
    }
    Report();
}

void sort(int* a, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int temp = a[j + 1];
                a[j + 1] = a[j];
                a[j] = temp;
            }
        }
    }
}
void DiskArm::SCAN(void) {  
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace((char*)"SCAN");
    int point = 0;
    for (int i = 0; i < RequestNumber; i++) {
        if (Cylinder[i] <= Current)
            point++;
    } //统计有多少请求小于当前磁头
    sort(Cylinder, RequestNumber);
    cout << Current << " ";
    if (Direction == 0) {
        for (int i = point - 1; i >= 0; i--) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
        cout << "-> 0";
        SeekChange++;
        SeekNumber += abs(Current - 0);
        Current = 0;
        cout << endl;
        for (int i = point; i < RequestNumber; i++) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    } else if (Direction == 1) {
        for (int i = point; i < RequestNumber; i++) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
        cout << "-> 199";
        SeekNumber += abs(Current - 199);
        Current = 199;
        SeekChange++;
        cout << endl;
        for (int i = point - 1; i >= 0; i--) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    }
    Report();
}

void DiskArm::CSCAN(void) {  
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace((char*)"CSCAN");
    int point = 0;
    for (int i = 0; i < RequestNumber; i++) {
        if (Cylinder[i] <= Current)
            point++;
    }
    sort(Cylinder, RequestNumber);
    cout << Current << " ";
    if (Direction == 0) {
        // 向0方向
        for (int i = point - 1; i >= 0; i--) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
        // 到0，统计寻道
        cout << "-> 0";
        SeekNumber += abs(Current - 0);
        Current = 0;
        SeekChange++;
        // 跳转到199，统计寻道
        cout << endl << "Jump to 199";
        SeekNumber += abs(Current - 199);
        Current = 199;
        SeekChange++;
        for (int i = RequestNumber - 1; i >= point; i--) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    } else if (Direction == 1) {
        // 向199方向
        for (int i = point; i < RequestNumber; i++) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
        // 到199，统计寻道
        cout << "-> 199";
        SeekNumber += abs(Current - 199);
        Current = 199;
        SeekChange++;
        // 跳转到0，统计寻道
        cout << endl << "Jump to 0";
        SeekNumber += abs(Current - 0);
        Current = 0;
        SeekChange++;
        for (int i = 0; i < point; i++) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    }
    Report();
}

void DiskArm::LOOK(void) {  
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace((char*)"LOOK");
    int point = 0;
    for (int i = 0; i < RequestNumber; i++) {
        if (Cylinder[i] <= Current)
            point++;
    }
    sort(Cylinder, RequestNumber);
    cout << Current << " ";
    if (Direction == 0) {
        for (int i = point - 1; i >= 0; i--) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
        SeekChange++;
        cout << endl;
        for (int i = point; i < RequestNumber; i++) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    } else if (Direction == 1) {
        for (int i = point; i < RequestNumber; i++) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
        SeekChange++;
        cout << endl;
        for (int i = point - 1; i >= 0; i--) {
            cout << "-> " << Cylinder[i] << " ";
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    }
    Report();
}

int main() {
    DiskArm disk;
    disk.FCFS();
    disk.SSTF();
    disk.SCAN();
    disk.CSCAN();
    disk.LOOK();
    return 0;
}
