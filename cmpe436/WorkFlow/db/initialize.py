import os
import json

ret = []
arr = next(os.walk('.'))[2]
for x in arr:
	if not x.startswith("init"):
		ff = {}
		ff["name"] = x
		ff["version"] = 1
		ret.append(ff)

fout = open("init.json", "w")
fout.write(json.dumps(ret))
fout.close()