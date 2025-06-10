# oslab6

### 代码实现
```cpp
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
```

运行结果如下：
![alt text](image.png)
![alt text](image-1.png)

### 算法比较
1、先来先服务算法（FCFS）
先来先服务算法根据磁道访问请求到来的先后顺序完成请求。实现简单，但在请求分布不均匀时可能导致磁头移动距离较长。
适用场景：请求顺序接近于磁头当前位置的顺序，或者对公平性要求较高的场景。
2、最短寻道时间优先算法（SSTF）
最短寻道时间优先算法总是优先满足距离磁头当前位置最近的访问请求。可以减少磁头移动的总距离，提高性能，但可能导致“饥饿”现象。SSTF算法会产生饥饿的原因在于：磁头有可能在一个小区域内来回来去地移动。
适用场景：请求较为分散，且对响应时间有较高要求的场景。
3、电梯调度算法（扫描算法SCAN）
对于先后到达的磁盘访问请求，电梯调度算法首先选择移臂方向，磁臂在该方向上移动的过程中依次处理途经的各个访问请求，直到该方向上再无请求时，改变移臂方向，依次处理相反方向上遇到的各个请求。如果同一柱面上有多个请求，还需进行旋转优化。可以减少磁头移动距离，提高效率。
适用场景：请求在磁盘两端较为集中的场景，可以提高磁头移动的效率。
4、循环扫描算法（C-SCAN）
在该算法中，磁头仅在一个移动方向上提供访问服务。磁臂从磁盘开始端柱面至结束端柱面移动的过程中依次处理途经请求，然后，直接返回开始端柱面重复进行，归途中并不响应请求。开始端与结束端柱面构成了一个循环。C-SCAN 是 SCAN 算法的改进，它在到达磁盘末端后不是立即反向，而是在磁盘的另一端等待新的请求，这样可以减少磁头在磁盘末端的空闲时间。
适用场景：请求较为均匀分布在磁盘上的场景，可以提高磁盘的整体利用率。
5.LOOK调度算法
LOOK算法基于SCAN算法，但‌在磁头移动方向上仅服务到最后一个请求，无需移动到磁盘边缘‌。
适用场景：请求分布不规律，且需要减少磁头回溯移动的场景。