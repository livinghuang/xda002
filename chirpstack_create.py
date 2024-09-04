import requests
import csv
import requests

# 设置 API 端点和令牌
CHIRPSTACK_API_URL = "http://lora.vip.tw:8080/api"
API_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcGlfa2V5X2lkIjoiMDRjNjU3YTktODU5MC00Y2ViLTllMzMtZjQ4MDc4MmFkYjM3IiwiYXVkIjoiYXMiLCJpc3MiOiJhcyIsIm5iZiI6MTcyNTMyMjAwNSwic3ViIjoiYXBpX2tleSJ9.R3acdHNHipKnfr5ZLCzpVpHF9LnPIgJyjrRUJ5K5T4I"
APPLICATION_ID = "50"
DEVICE_PROFILE_ID = "8baaf7ca-c2f7-451e-b685-da1552b35efc"  # 替换为您的实际 Device Profile ID

# 设置请求头
headers = {
    "Content-Type": "application/json",
    "Authorization": f"Bearer {API_TOKEN}"
}

# 读取 CSV 文件并逐行创建设备
with open('device_list.csv', mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        # 从每一行中提取信息
        device_name = row['Device Name']
        dev_eui = row['devEUI']
        dev_addr = row['devAddr']
        nwk_skey = row['nwkSKey']
        app_skey = row['appSKey']

        # 设备信息
        device_info = {
            "device": {
                "applicationID": "50",  # 替换为您的实际 Application ID
                "description": f"Device {device_name}",
                "devEUI": dev_eui,
                "deviceProfileID": DEVICE_PROFILE_ID,
                "isDisabled": False,
                "name": device_name,
                "referenceAltitude": 0,
                "skipFCntCheck": True,
                "tags": {},
                "variables": {}
            },
            "keys": {
                "nwkKey": nwk_skey,
                "appKey": app_skey
            }
        }

        # 发送 POST 请求创建设备
        response = requests.post(f"{CHIRPSTACK_API_URL}/devices", json=device_info, headers=headers)

        # 检查响应状态
        if response.status_code in range(200, 300):
            print(f"设备 {device_name} 创建成功!")
        else:
            print(f"设备 {device_name} 创建失败: {response.status_code}")
            print(response.text)