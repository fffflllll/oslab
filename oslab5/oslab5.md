# oslab5
github仓库地址 https://github.com/fffflllll/oslab/tree/main/oslab5
### 时钟置换算法
```cpp
void Replace::Clock(void) {
    InitSpace("Clock");
    int pointer = 0;                     // 指向下一个要替换的页面
    bool* used = new bool[FrameNumber];  // 用于记录页面是否被访问过
    int eliminateIndex = 0;              // 用于记录淘汰页的索引

    for (int i = 0; i < FrameNumber; i++) {
        used[i] = false;
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;

        // 检查页面是否在帧中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                found = true;
                used[i] = true;  // 将页面标记为已访问
                break;
            }
        }

        if (!found) {
            FaultNumber++;
            // 找到一个没有被访问过的页面进行替换
            while (used[pointer]) {
                used[pointer] = false;
                pointer = (pointer + 1) % FrameNumber;
            }

            // 记录被淘汰的页面
            if (PageFrames[pointer] != -1) {
                EliminatePage[eliminateIndex++] = PageFrames[pointer];
            }

            // 进行页面替换
            PageFrames[pointer] = next;
            used[pointer] = true;
            pointer = (pointer + 1) % FrameNumber;
        }

        // 报告当前实存中页号
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0)
                cout << PageFrames[j] << " ";
        }
        if (!found && eliminateIndex > 0 &&
            EliminatePage[eliminateIndex - 1] > 0)
            cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
        else
            cout << endl;
    }

    delete[] used;
    Report();
}
```
### 增强二次机会置换算法
```cpp
void Replace::Eclock(void) {
    InitSpace("EClock");
    int pointer = 0;                     // 指向下一个要替换的页面
    bool* used = new bool[FrameNumber];  // 用于记录页面是否被访问过
    bool* modified = new bool[FrameNumber];  // 用于记录页面是否被修改过
    int eliminateIndex = 0;                  // 用于记录淘汰页的索引

    for (int i = 0; i < FrameNumber; i++) {
        used[i] = false;
        modified[i] = false;
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;

        // 检查页面是否在帧中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                found = true;
                used[i] = true;      // 将页面标记为已访问
                modified[i] = true;  // 假设页面被访问时也被修改
                break;
            }
        }

        if (!found) {
            FaultNumber++;  // 缺页次数 +1
            // 寻找替换页面：优先选择 (used=false, modified=false) 的页面
            while (used[pointer] || modified[pointer]) {
                if (used[pointer]) {
                    used[pointer] = false;  // 清除访问位，给予第二次机会
                } else if (modified[pointer]) {
                    modified[pointer] = false;  // 清除修改位，但仍保留访问位
                    used[pointer] = true;       // 转为“已访问但未修改”状态
                }
                pointer = (pointer + 1) % FrameNumber;  // 指针移动
            }

            // 记录被淘汰的页面
            if (PageFrames[pointer] != -1) {
                EliminatePage[eliminateIndex++] = PageFrames[pointer];
            }

            // 进行页面替换
            PageFrames[pointer] = next;
            used[pointer] = true;
            modified[pointer] = false;  // 新页面假设未被修改
            pointer = (pointer + 1) % FrameNumber;
        }

        // 报告当前实存中页号
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0)
                cout << PageFrames[j] << " ";
        }
        if (!found && eliminateIndex > 0 &&
            EliminatePage[eliminateIndex - 1] > 0)
            cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
        else
            cout << endl;
    }

    delete[] used;
    delete[] modified;
    Report();
}
```

### 最不经常使用置换算法
```cpp
void Replace::Lfu(void) {
    InitSpace("LFU");
    int* frequency = new int[FrameNumber];  // 记录每个页面的使用频率
    int eliminateIndex = 0;                 // 用于记录淘汰页的索引

    for (int i = 0; i < FrameNumber; i++) {
        frequency[i] = 0;
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;

        // 检查页面是否在帧中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                found = true;
                frequency[i]++;  // 增加页面的使用频率
                break;
            }
        }

        if (!found) {
            FaultNumber++;
            // 找到使用频率最低的页面进行替换
            int minFreqIndex = 0;
            for (int i = 1; i < FrameNumber; i++) {
                if (frequency[i] < frequency[minFreqIndex]) {
                    minFreqIndex = i;
                }
            }

            // 记录被淘汰的页面
            if (PageFrames[minFreqIndex] != -1) {
                EliminatePage[eliminateIndex++] = PageFrames[minFreqIndex];
            }

            // 进行页面替换
            PageFrames[minFreqIndex] = next;
            frequency[minFreqIndex] = 1;  // 新页面初始使用频率为1
        }

        // 报告当前实存中页号
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0)
                cout << PageFrames[j] << " ";
        }
        if (!found && eliminateIndex > 0 &&
            EliminatePage[eliminateIndex - 1] > 0)
            cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
        else
            cout << endl;
    }

    delete[] frequency;
    Report();
}

```
### 最经常使用置换算法
```cpp
void Replace::Mfu(void) {
    InitSpace("MFU");
    int* frequency = new int[FrameNumber];  // 记录每个页面的使用频率
    int eliminateIndex = 0;                 // 用于记录淘汰页的索引

    // 初始化频率数组和页面帧
    for (int i = 0; i < FrameNumber; i++) {
        frequency[i] = 0;
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;

        // 检查页面是否在帧中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                found = true;
                frequency[i]++;  // 增加页面的使用频率
                break;
            }
        }

        if (!found) {
            FaultNumber++;
            // 找到使用频率最高的页面进行替换
            int maxFreqIndex = -1;
            int maxFrequency = -1;

            for (int i = 0; i < FrameNumber; i++) {
                if (PageFrames[i] == -1) {  // 如果有空位，直接使用
                    maxFreqIndex = i;
                    break;
                }
                if (frequency[i] > maxFrequency) {
                    maxFrequency = frequency[i];
                    maxFreqIndex = i;
                }
            }

            // 记录被淘汰的页面
            if (PageFrames[maxFreqIndex] != -1) {
                EliminatePage[eliminateIndex++] = PageFrames[maxFreqIndex];
            }

            // 进行页面替换
            PageFrames[maxFreqIndex] = next;
            frequency[maxFreqIndex] = 1;  // 新页面初始使用频率为1
        }

        // 报告当前实存中页号
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0)
                cout << PageFrames[j] << " ";
        }
        if (!found && eliminateIndex > 0 &&
            EliminatePage[eliminateIndex - 1] > 0)
            cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
        else
            cout << endl;
    }

    delete[] frequency;
    Report();
}
```
### 随机生成内存页面引用串
```sh
#!/bin/bash

n=$((RANDOM % 100 + 1))

numbers=()
for (( i=0; i<n; i++ ))
do
    numbers+=($((RANDOM % 20 + 1)))
done

echo "100以内随机正整数 n: $n"
echo "随机内存引用串: ${numbers[@]}"

./vmrp <<EOF
$n
${numbers[@]}
3
EOF
```
### 运行结果如下
```cpp

fl@dut:~/oslab/oslab5$ bash random.sh 
100以内随机正整数 n: 52
随机内存引用串: 7 20 14 5 18 2 17 14 2 8 12 14 14 7 16 17 2 10 20 2 20 16 11 19 14 12 8 17 14 1 18 19 8 4 8 6 18 10 5 19 10 14 13 8 10 16 14 18 3 18 14 16
Please input page numbers :Please input reference page string :Please input page frames :
FIFO
7 
7 20 
7 20 14 
5 20 14 ->7
5 18 14 ->20
5 18 2 ->14
17 18 2 ->5
17 14 2 ->18
17 14 2 
17 14 8 ->2
12 14 8 ->17
12 14 8 
12 14 8 
12 7 8 ->14
12 7 16 ->8
17 7 16 ->12
17 2 16 ->7
17 2 10 ->16
20 2 10 ->17
20 2 10 
20 2 10 
20 16 10 ->2
20 16 11 ->10
19 16 11 ->20
19 14 11 ->16
19 14 12 ->11
8 14 12 ->19
8 17 12 ->14
8 17 14 ->12
1 17 14 ->8
1 18 14 ->17
1 18 19 ->14
8 18 19 ->1
8 4 19 ->18
8 4 19 
8 4 6 ->19
18 4 6 ->8
18 10 6 ->4
18 10 5 ->6
19 10 5 ->18
19 10 5 
19 14 5 ->10
19 14 13 ->5
8 14 13 ->19
8 10 13 ->14
8 10 16 ->13
14 10 16 ->8
14 18 16 ->10
14 18 3 ->16
14 18 3 
14 18 3 
16 18 3 ->14

Eliminate page:7 20 14 5 18 2 17 14 8 12 7 16 17 2 10 20 16 11 19 14 12 8 17 14 1 18 19 8 4 6 18 10 5 19 14 13 8 10 16 14 
Number of page faults = 43
Rate of page faults = 82.7%

LRU
7 
20 7 
14 20 7 
5 14 20 ->7
18 5 14 ->20
2 18 5 ->14
17 2 18 ->5
14 17 2 ->18
2 14 17 
8 2 14 ->17
12 8 2 ->14
14 12 8 ->2
14 12 8 
7 14 12 ->8
16 7 14 ->12
17 16 7 ->14
2 17 16 ->7
10 2 17 ->16
20 10 2 ->17
2 20 10 
20 2 10 
16 20 2 ->10
11 16 20 ->2
19 11 16 ->20
14 19 11 ->16
12 14 19 ->11
8 12 14 ->19
17 8 12 ->14
14 17 8 ->12
1 14 17 ->8
18 1 14 ->17
19 18 1 ->14
8 19 18 ->1
4 8 19 ->18
8 4 19 
6 8 4 ->19
18 6 8 ->4
10 18 6 ->8
5 10 18 ->6
19 5 10 ->18
10 19 5 
14 10 19 ->5
13 14 10 ->19
8 13 14 ->10
10 8 13 ->14
16 10 8 ->13
14 16 10 ->8
18 14 16 ->10
3 18 14 ->16
18 3 14 
14 18 3 
16 14 18 ->3

Eliminate page:7 20 14 5 18 17 14 2 8 12 14 7 16 17 10 2 20 16 11 19 14 12 8 17 14 1 18 19 4 8 6 18 5 19 10 14 13 8 10 16 3 
Number of page faults = 44
Rate of page faults = 84.6%

Clock
7 
7 20 
7 20 14 
5 20 14 ->7
5 18 14 ->20
5 18 2 ->14
17 18 2 ->5
17 14 2 ->18
17 14 2 
17 14 8 ->2
12 14 8 ->17
12 14 8 
12 14 8 
12 7 8 ->14
12 7 16 ->8
17 7 16 ->12
17 2 16 ->7
17 2 10 ->16
20 2 10 ->17
20 2 10 
20 2 10 
20 16 10 ->2
20 16 11 ->10
19 16 11 ->20
19 14 11 ->16
19 14 12 ->11
8 14 12 ->19
8 17 12 ->14
8 17 14 ->12
1 17 14 ->8
1 18 14 ->17
1 18 19 ->14
8 18 19 ->1
8 4 19 ->18
8 4 19 
8 4 6 ->19
18 4 6 ->8
18 10 6 ->4
18 10 5 ->6
19 10 5 ->18
19 10 5 
19 10 14 ->5
19 13 14 ->10
8 13 14 ->19
8 13 10 ->14
8 16 10 ->13
14 16 10 ->8
14 16 18 ->10
14 3 18 ->16
14 3 18 
14 3 18 
14 3 16 ->18

Eliminate page:7 20 14 5 18 2 17 14 8 12 7 16 17 2 10 20 16 11 19 14 12 8 17 14 1 18 19 8 4 6 18 5 10 19 14 13 8 10 16 18 
Number of page faults = 43
Rate of page faults = 82.7%

EClock
7 
7 20 
7 20 14 
5 20 14 ->7
5 18 14 ->20
5 18 2 ->14
17 18 2 ->5
17 14 2 ->18
17 14 2 
8 14 2 ->17
8 12 2 ->14
8 12 14 ->2
8 12 14 
7 12 14 ->8
7 16 14 ->12
17 16 14 ->7
17 2 14 ->16
17 2 10 ->14
20 2 10 ->17
20 2 10 
20 2 10 
20 2 16 ->10
20 11 16 ->2
20 11 19 ->16
14 11 19 ->20
14 12 19 ->11
14 12 8 ->19
17 12 8 ->14
17 14 8 ->12
17 14 1 ->8
18 14 1 ->17
18 19 1 ->14
18 19 8 ->1
4 19 8 ->18
4 19 8 
4 6 8 ->19
18 6 8 ->4
18 10 8 ->6
18 10 5 ->8
19 10 5 ->18
19 10 5 
19 10 14 ->5
13 10 14 ->19
13 8 14 ->10
13 8 10 ->14
16 8 10 ->13
16 14 10 ->8
16 14 18 ->10
3 14 18 ->16
3 14 18 
3 14 18 
16 14 18 ->3

Eliminate page:7 20 14 5 18 17 14 2 8 12 7 16 14 17 10 2 16 20 11 19 14 12 8 17 14 1 18 19 4 6 8 18 5 19 10 14 13 8 10 16 3 
Number of page faults = 44
Rate of page faults = 84.6%

LFU
7 
7 20 
7 20 14 
5 20 14 ->7
18 20 14 ->5
2 20 14 ->18
17 20 14 ->2
17 20 14 
2 20 14 ->17
8 20 14 ->2
12 20 14 ->8
12 20 14 
12 20 14 
7 20 14 ->12
16 20 14 ->7
17 20 14 ->16
2 20 14 ->17
10 20 14 ->2
10 20 14 
2 20 14 ->10
2 20 14 
16 20 14 ->2
11 20 14 ->16
19 20 14 ->11
19 20 14 
12 20 14 ->19
8 20 14 ->12
17 20 14 ->8
17 20 14 
1 20 14 ->17
18 20 14 ->1
19 20 14 ->18
8 20 14 ->19
4 20 14 ->8
8 20 14 ->4
6 20 14 ->8
18 20 14 ->6
10 20 14 ->18
5 20 14 ->10
19 20 14 ->5
10 20 14 ->19
10 20 14 
13 20 14 ->10
8 20 14 ->13
10 20 14 ->8
16 20 14 ->10
16 20 14 
18 20 14 ->16
3 20 14 ->18
18 20 14 ->3
18 20 14 
16 20 14 ->18

Eliminate page:7 5 18 2 17 2 8 12 7 16 17 2 10 2 16 11 19 12 8 17 1 18 19 8 4 8 6 18 10 5 19 10 13 8 10 16 18 3 18 
Number of page faults = 42
Rate of page faults = 80.8%

MFU
7 
7 20 
7 20 14 
5 20 14 ->7
18 20 14 ->5
2 20 14 ->18
17 20 14 ->2
17 20 14 
17 20 2 ->14
8 20 2 ->17
12 20 2 ->8
14 20 2 ->12
14 20 2 
7 20 2 ->14
16 20 2 ->7
17 20 2 ->16
17 20 2 
17 20 10 ->2
17 20 10 
17 2 10 ->20
20 2 10 ->17
16 2 10 ->20
11 2 10 ->16
19 2 10 ->11
14 2 10 ->19
12 2 10 ->14
8 2 10 ->12
17 2 10 ->8
14 2 10 ->17
1 2 10 ->14
18 2 10 ->1
19 2 10 ->18
8 2 10 ->19
4 2 10 ->8
8 2 10 ->4
6 2 10 ->8
18 2 10 ->6
18 2 10 
18 2 5 ->10
19 2 5 ->18
10 2 5 ->19
14 2 5 ->10
13 2 5 ->14
8 2 5 ->13
10 2 5 ->8
16 2 5 ->10
14 2 5 ->16
18 2 5 ->14
3 2 5 ->18
18 2 5 ->3
14 2 5 ->18
16 2 5 ->14

Eliminate page:7 5 18 2 14 17 8 12 14 7 16 2 20 17 20 16 11 19 14 12 8 17 14 1 18 19 8 4 8 6 10 18 19 10 14 13 8 10 16 14 18 3 18 14 
Number of page faults = 47
Rate of page faults = 90.4%
```
### 绘图分析
```py
import matplotlib.pyplot as plt

# 页面访问序列
reference_string = [0, 1, 2, 0, 3, 4, 2, 1, 2, 0, 3, 4]

# 页面数量
num_frames = [2, 3, 4]

# 定义页面置换算法
def fifo(reference_string, frames):
    page_faults = 0
    current_frames = []
    for page in reference_string:
        if page not in current_frames:
            page_faults += 1
            if len(current_frames) < frames:
                current_frames.append(page)
            else:
                current_frames.pop(0)
                current_frames.append(page)
    return page_faults

def lru(reference_string, frames):
    page_faults = 0
    current_frames = []
    for page in reference_string:
        if page not in current_frames:
            page_faults += 1
            if len(current_frames) < frames:
                current_frames.append(page)
            else:
                current_frames.pop(-1)  # 移除最近最少使用的页面
                current_frames.insert(0, page)
        else:
            current_frames.remove(page)
            current_frames.insert(0, page)
    return page_faults

def clock(reference_string, frames):
    page_faults = 0
    current_frames = [-1] * frames
    used = [False] * frames
    pointer = 0
    for page in reference_string:
        if page not in current_frames:
            page_faults += 1
            while used[pointer]:
                used[pointer] = False
                pointer = (pointer + 1) % frames
            current_frames[pointer] = page
            used[pointer] = True
            pointer = (pointer + 1) % frames
        else:
            used[current_frames.index(page)] = True
    return page_faults

# 计算每种算法的页面置换次数
results = {"FIFO": [], "LRU": [], "Clock": []}
for frames in num_frames:
    results["FIFO"].append(fifo(reference_string, frames))
    results["LRU"].append(lru(reference_string, frames))
    results["Clock"].append(clock(reference_string, frames))

# 绘制曲线图
plt.figure(figsize=(10, 6))
for algorithm, faults in results.items():
    plt.plot(num_frames, faults, marker='o', label=algorithm)

plt.title('Page Faults vs Number of Frames')
plt.xlabel('Number of Frames')
plt.ylabel('Number of Page Faults')
plt.grid(True)
plt.legend()
plt.show()
```
![alt text](image.png)

### 模拟过程
clock
1. `InitSpace("Clock");`：用于初始化页面帧数组`PageFrames`
2. `int pointer = 0;`：`pointer`变量用于指向下一个可能被替换的页面
3. `bool* used = new bool[FrameNumber];`：这个布尔数组用于记录页面是否被访问过
4. `int eliminateIndex = 0;`：用于记录被淘汰页面的索引。
5. 循环初始化`used`数组，将所有页面标记为未访问
6. 外层循环遍历所有页面引用（`PageNumber`）：

   - 内层循环检查当前页面是否已经在帧中（即是否已经被加载）
   - 如果页面在帧中，将其对应的`used`标记为`true`
7. 如果页面不在帧中（`!found`），则发生缺页，需要进行页面置换：
   - 使用`pointer`指针寻找一个未被访问过的页面进行替换
   - 循环直到找到一个`used`为`false`的页面，将其标记为已访问（`used[pointer] = true`）
8. 在找到要替换的页面后，记录被淘汰的页面，并更新`PageFrames`数组
9. 每次页面访问后，打印当前帧中的页面号
10. 如果发生了页面置换，打印被淘汰的页面号
11. 程序结束时，释放`used`数组，并调用`Report()`函数报告结果增强二次机会算法

eclock
1. `InitSpace("EClock");`：用于初始化页面帧数组`PageFrames`

2. `int pointer = 0;`：`pointer`变量用于指向下一个可能被替换的页面

3. `bool* used = new bool[FrameNumber];` 和 `bool* modified = new bool[FrameNumber];`：这两个布尔数组分别用于记录页面是否被访问过（`used`）和是否被修改过（`modified`）

4. `int eliminateIndex = 0;`：用于记录被淘汰页面的索引

5. 循环初始化`used`和`modified`数组，将所有页面标记为未访问和未修改

6. 外层循环遍历所有页面引用（`PageNumber`）：

   - 内层循环检查当前页面是否已经在帧中（即是否已经被加载）
   - 如果页面在帧中，将其对应的`used`标记为`true`，并假设页面被修改（`modified`也设置为`true`）。

7. 如果页面不在帧中（`!found`），则发生缺页，需要进行页面置换：
   - 使用`pointer`指针寻找一个未被访问过或未被修改过的页面进行替换
   - 如果`pointer`指向的页面被访问过，将其`used`标记为`false`
   - 如果页面被修改过，将其`modified`标记为`false`，并给予第二次机会（即将`used`标记为`true`）
   - 当找到一个合适的页面进行替换时，记录被淘汰的页面，并更新`PageFrames`数组

8. 每次页面替换后，打印当前帧中的页面号
9. 程序结束时，释放`used`和`modified`数组，并调用`Report()`函数报告结果

lfu
1. `InitSpace("LFU");`：用于初始化页面帧数组`PageFrames`

2. `int* frequency = new int[FrameNumber];`：创建一个整型数组来记录每个页面的使用频率

3. `int eliminateIndex = 0;`：用于记录被淘汰页面的索引

4. 初始化`frequency`数组，将所有页面的使用频率设置为0

5. 外层循环遍历所有页面引用（`PageNumber`）：

   - 内层循环检查当前页面是否已经在帧中（即是否已经被加载）
   - 如果页面在帧中，将其对应的使用频率`frequency[i]`增加1

6. 如果页面不在帧中（`!found`），则发生缺页，需要进行页面置换：
   - 遍历`frequency`数组，找到使用频率最低的页面索引`minFreqIndex`
   - 记录被淘汰页面的编号，如果该页面编号不是-1

7. 进行页面替换：
   - 将`minFreqIndex`位置的页面替换为新页面`next`
   - 将新页面的使用频率设置为1，因为新页面被访问了一次

8. 每次页面访问后，打印当前帧中的页面号

9. 如果发生了页面置换，打印被淘汰的页面号

10. 程序结束时，释放`frequency`数组，并调用`Report()`函数报告结果

mfu
1. `InitSpace("MFU");`：用于初始化页面帧数组`PageFrames`

2. `int* frequency = new int[FrameNumber];`：创建一个整型数组来记录每个页面的使用频率

3. `int eliminateIndex = 0;`：用于记录被淘汰页面的索引

4. 初始化`frequency`数组，将所有页面的使用频率设置为0

5. 外层循环遍历所有页面引用（`PageNumber`）：

   - 内层循环检查当前页面是否已经在帧中（即是否已经被加载）
   - 如果页面在帧中，将其对应的使用频率`frequency[i]`增加1。

6. 如果页面不在帧中（`!found`），则发生缺页，需要进行页面置换：
   - 遍历`frequency`数组，找到使用频率最高的页面索引`maxFreqIndex`

7. 记录被淘汰页面的编号，如果该页面编号不是-1

8. 进行页面替换：
   - 将`maxFreqIndex`位置的页面替换为新页面`next`
   - 将新页面的使用频率设置为1，因为新页面被访问了一次

9. 每次页面访问后，打印当前帧中的页面号

10. 如果发生了页面置换，打印被淘汰的页面号

11. 程序结束时，释放`frequency`数组，并调用`Report()`函数报告结果

### 适用场景
1. FIFO（先进先出）​
适用场景​：
嵌入式系统或资源受限环境。
页面访问模式无局部性​（如顺序访问流）。
教学示例或算法对比基准。

以下是各个页面置换算法的特点及其典型使用场景的总结，帮助您根据实际需求选择合适的算法：

​1. FIFO（先进先出）​​
​特点​：替换最早进入内存的页面，实现简单（队列结构）。
​优点​：低开销，适合硬件实现。
​缺点​：可能出现 ​Belady 异常​（分配更多物理帧时缺页率反而升高）。
​适用场景​：
嵌入式系统或资源受限环境。
页面访问模式无局部性​（如顺序访问流）。
教学示例或算法对比基准。
​2. LRU（最近最少使用）​
通用操作系统（如 Linux 的页面缓存）。
访问模式有强时间局部性​（如循环访问、热点数据）。
需要高缓存命中率的应用（数据库、Web 服务器）。

3. Clock
操作系统的实际实现（如 Windows 的页面置换）。
需要折中性能与复杂度的场景。
中等规模的内存管理。

4. Eclock
写操作频繁的系统（如文件系统缓存）。
需要优化磁盘写入的场景（如虚拟内存管理）。
Linux 的 kswapd 守护进程。

​5. LFU（最不经常使用）
长期运行的服务器应用（如缓存代理）。
访问模式高度稳定​（如静态内容服务）。
需要历史频率统计的场景（推荐系统底层缓存）。

​6. MFU（最常使用）
特殊缓存清理需求（如防止某些页面垄断内存）。
实验性场景或算法研究。
极少在实际系统中直接使用。
