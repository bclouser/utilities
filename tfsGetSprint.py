#!/usr/bin/env python3

from tfs import TFSAPI

user="bclouser"
password="demolition5*4"

# Use DefaultCollection
#client = TFSAPI("https://http://tfs02.corp.tsafe.systems:8080/tfs/Viableware", user=user, password=password)

# Use CustomCollection
client = TFSAPI("https://http://tfs02.corp.tsafe.systems:8080/tfs/Viableware", project="Software Development", user=user, password=password)

# Set path to ProjectName in project parameter
#client = TFSAPI("https://tfs.tfs.ru/tfs/", project="DefaultCollection/ProjectName", user=user, password=password)

workitem = client.get_workitem(12870) # Test connection with Workitem id

print(workitem)