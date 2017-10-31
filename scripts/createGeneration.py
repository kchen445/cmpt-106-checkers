import os

numNetworks = 30

for i in range(0, numNetworks):
    fname = 'G{}N{}.txt'.format(0, i)
    cmd = "python createNetwork.py > tmp/" + fname
    os.system(cmd)
