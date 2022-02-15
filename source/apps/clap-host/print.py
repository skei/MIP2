import json
import pprint

json_data = None
with open('test.json', 'r') as f:
    data = f.read()
    json_data = json.loads(data)

pprint.pprint(json_data)

