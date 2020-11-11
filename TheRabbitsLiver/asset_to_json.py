import json
import os

path = os.getcwd() + '/Assets'


files = os.listdir(path)

assets = {}

needs = ['gpmesh', 'wav', 'mp3', 'jpg', 'png']

for file in files:
    extension = file.split('.')[1]
    if extension not in needs:
        continue
    if extension not in assets:
        assets[extension] = []
        assets[extension].append(file)
    else:
        assets[extension].append(file)

with open('assets.json', 'w') as f:
    json.dump(assets, f, indent = 2)

# print(os.getcwd())

