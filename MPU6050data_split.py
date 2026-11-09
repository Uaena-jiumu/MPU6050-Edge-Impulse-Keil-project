import os

# ================= 配置区 =================
# 你的原始 txt 文件名（记得改名）
input_file = "stm32data_sway10081600.txt"
# 生成的文件保存到哪个文件夹
output_folder = "sway"
# 每个窗口包含多少行数据
window_size = 200
# 假设你是 100Hz 采样率，每个点间隔 10ms
time_increment = 10
# =========================================

# 确保输出文件夹存在
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# 读取原始数据
with open(input_file, 'r') as f:
    lines = f.readlines()

# 清理数据：只保留纯数字行，过滤掉空行、分隔符以及乱码
data_lines = []
for line in lines:
    line = line.strip()
    # 排除空行、分隔符
    if line and "WINDOW" not in line:
        # 终极防线：过滤掉包含奇怪符号的乱码行
        # 允许的字符只有数字、逗号、负号、空格
        if all(c in "0123456789,- " for c in line):
            data_lines.append(line)

# 开始切分
file_count = 1
for i in range(0, len(data_lines), window_size):
    # 取出 200 行作为一个小窗口
    chunk = data_lines[i: i + window_size]

    # 如果最后一段不够 200 行，就舍弃
    if len(chunk) < window_size:
        break

    # 生成小文件名，比如 vibration/vib_01.csv
    out_name = os.path.join(output_folder, f"sway_{file_count:02d}.csv")

    # 写入文件
    with open(out_name, 'w') as out_f:
        # 1. 写入 Edge Impulse 要求的表头
        out_f.write("timestamp,accX,accY,accZ\n")

        current_time = 0  # 每个文件的起始时间戳从 0 开始
        for row in chunk:
            # 2. 拼接时间戳和 XYZ 数据
            out_f.write(f"{current_time},{row}\n")
            current_time += time_increment

    file_count += 1

# 打印结果
print(f"一共切出了 {file_count - 1} 个文件，保存在 {output_folder} 文件夹里。")