import re
import csv

def parse_log_file(log_file_path, csv_file_path):
    # Regular expression to extract the required fields
    pattern = re.compile(r"ESP32ChipID=(\w+)\s+devEUI:(\w+)\s+devAddr:(\w+)\s+nwkSKey:(\w+)\s+appSKey:(\w+)")
    
    with open(log_file_path, 'r') as log_file:
        log_data = log_file.read()
        
    # Find all matches in the log data
    matches = pattern.findall(log_data)
    
    # Write the data to a CSV file
    with open(csv_file_path, 'w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(['ESP32ChipID', 'devEUI', 'devAddr', 'nwkSKey', 'appSKey'])
        
        for match in matches:
            csv_writer.writerow(match)

# Usage
log_file_path = 'raw_log.txt'
csv_file_path = 'device_list.csv'
parse_log_file(log_file_path, csv_file_path)