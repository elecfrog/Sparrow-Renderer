import json
import os

# Get the current directory (where config.json is located)
current_dir = os.path.abspath(os.path.dirname(__file__))
parent_dir = os.path.dirname(current_dir)
print(f"current_dir:{current_dir}")
print(f"parent_dir:{parent_dir}")

# Read the config file
config_file_path = os.path.join(current_dir, 'config.json')

with open(config_file_path, 'r') as f:
    config = json.load(f)

# Convert paths to use forward slashes for consistency with C++ code
def normalize_path(path):
    return path.replace('\\', '/')

# Update the paths
config['directories']['config']['path'] = normalize_path(current_dir)
config['directories']['asset']['path']  = normalize_path(os.path.join(parent_dir, '_assets'))
config['directories']['shader']['path'] = normalize_path(os.path.join(parent_dir, '_assets', "shaders"))

# Write the updated config back to file
with open(config_file_path, 'w') as f:
    json.dump(config, f, indent=4)

print("配置文件已更新：")
print(f"Config路径: {config['directories']['config']['path']}")
print(f"Asset路径: {config['directories']['asset']['path']}")
print(f"Shader路径: {config['directories']['shader']['path']}")

# 验证路径是否存在
def verify_path(path):
    if os.path.exists(path):
        print(f"✓ 路径存在: {path}")
    else:
        print(f"✗ 路径不存在: {path}")

print("\n验证路径:")
verify_path(config['directories']['config']['path'])
verify_path(config['directories']['asset']['path'])
verify_path(config['directories']['shader']['path']) 