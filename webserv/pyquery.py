#!/usr/bin/python3
print("Listing installed Python packages via pip:\n")
import subprocess, sys

try:
    result = subprocess.run([sys.executable, "-m", "pip", "list"], capture_output=True, text=True)
    print(result.stdout)
except Exception as e:
    print("Error:", e)