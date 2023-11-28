import re

# 打开原始文件并读取内容
with open("1_index.TXT", "r") as file:
    content = file.read()

# 使用正则表达式删除数字、括号和空格
filtered_content = re.sub(r'[\d() \n]', '', content)

# 保存到新文件
with open("Index.TXT", "w") as new_file:
    new_file.write(filtered_content)
