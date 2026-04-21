#!/usr/bin/env python3

import os
import sys
from urllib.parse import parse_qs

print("Content-Type: text/plain")
print()

# ---- GET data ----
query_string = os.environ.get("QUERY_STRING", "")
get_params = parse_qs(query_string)

# ---- POST data ----
content_length = int(os.environ.get("CONTENT_LENGTH", 0))
post_data_raw = sys.stdin.read(content_length) if content_length > 0 else ""
post_params = parse_qs(post_data_raw)

# ---- Example usage ----
name_get = get_params.get("name", [""])[0]
name_post = post_params.get("name", [""])[0]

print("GET params:", get_params)
print("POST params:", post_params)

print("\nExtracted values:")
print("name (GET):", name_get)
print("name (POST):", name_post)