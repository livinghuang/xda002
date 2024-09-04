import csv
import requests

# 设置 API 端点和令牌
CHIRPSTACK_API_URL = "http://lora.vip.tw:8080/api"
API_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcGlfa2V5X2lkIjoiMDRjNjU3YTktODU5MC00Y2ViLTllMzMtZjQ4MDc4MmFkYjM3IiwiYXVkIjoiYXMiLCJpc3MiOiJhcyIsIm5iZiI6MTcyNTMyMjAwNSwic3ViIjoiYXBpX2tleSJ9.R3acdHNHipKnfr5ZLCzpVpHF9LnPIgJyjrRUJ5K5T4I"

# 设置请求头
headers = {
    "Content-Type": "application/json",
    "Authorization": f"Bearer {API_TOKEN}"
}

# 读取 CSV 文件并逐行激活设备
with open('device_list.csv', mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        # 从每一行中提取信息
        dev_eui = row['devEUI']
        dev_addr = row['devAddr']
        app_skey = row['appSKey']
        nwk_skey = row['nwkSKey']

        # 激活信息
        activation_info = {
            "deviceActivation": {
                "aFCntDown": 0,
                "appSKey": app_skey,
                "devAddr": dev_addr,
                "devEUI": dev_eui,
                "fCntUp": 0,
                "fNwkSIntKey": nwk_skey,
                "nFCntDown": 0,
                "nwkSEncKey": nwk_skey,
                "sNwkSIntKey": nwk_skey
            }
        }

        # 发送 POST 请求激活设备
        response = requests.post(f"{CHIRPSTACK_API_URL}/devices/{dev_eui}/activate", json=activation_info, headers=headers)

        # 检查响应状态
        if response.status_code in range(200, 300):
            print(f"设备 {dev_eui} 激活成功!")
        else:
            print(f"设备 {dev_eui} 激活失败: {response.status_code}")
            print(response.text)