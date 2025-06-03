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