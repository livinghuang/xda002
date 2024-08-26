import json
import csv
from datetime import datetime

# Read JSON data from the file
with open('device.json', 'r') as file:
    data = json.load(file)

# Extract publishedAt timestamps
timestamps = [entry['payload']['publishedAt'] for entry in data]

# Define the datetime format, ignoring the last digits
datetime_format = "%Y-%m-%dT%H:%M:%S.%f"

# Convert to datetime objects
datetime_objects = [datetime.strptime(ts[:26], datetime_format) for ts in timestamps]

# Reverse the datetime objects to process in ascending order
datetime_objects.reverse()
timestamps.reverse()

# Calculate time intervals between consecutive timestamps
intervals = []
for i in range(1, len(datetime_objects)):
    time_interval = (datetime_objects[i] - datetime_objects[i - 1]).total_seconds()
    intervals.append({
        "Start Timestamp": timestamps[i-1],
        "End Timestamp": timestamps[i],
        "Interval (seconds)": time_interval
    })

# Write the intervals to a CSV file
csv_file = 'time_intervals.csv'
csv_columns = ["Start Timestamp", "End Timestamp", "Interval (seconds)"]

try:
    with open(csv_file, 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=csv_columns)
        writer.writeheader()
        for data in intervals:
            writer.writerow(data)
    print(f"Intervals successfully written to {csv_file}")
except IOError:
    print("I/O error")