#!/usr/bin/python3
import os
import subprocess

print('=========start=========')
dic = {}
with open('ans.data') as file:
    lines=file.readlines()
    for i in range(0,len(lines),2):
        k = str(lines[i])
        k = k[:-2]
        k += 'data'
        v = str(lines[i+1])[:-1]
        dic[str(k)] = str(v)
print('=====init finished=====')
for k, v in dic.items():
    print(k[:-5])
    cmd = "cat riscv-data/testcases/" + k+' | ./cmake-build-debug/code'
    ans = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout.read()
    if len(ans)>0:
        ans=ans[:-1]
        ans=str(int(ans))
    else:
        ans=' NO ANSWER'
    if v != ans:
        print('Wrong : ans=' + v + ',yours=' + ans)
    else:
        print("\tpassed")
print('=========done==========')
