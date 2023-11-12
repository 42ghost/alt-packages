import requests
import json
import sys

api_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + sys.argv[1]

response = requests.get(api_url)
response = response.json()

print(api_url)
with open(sys.argv[1] + ".json", 'w') as out:
    out.write(json.dumps(response, indent=4, sort_keys=True))

