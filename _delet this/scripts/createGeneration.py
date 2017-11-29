import os

numNetworks = 100

for i in range(0, numNetworks):
    fname = 'G{}N{}.txt'.format(0, i)
    cmd = "python createNetwork.py > ../test/networks/test/" + fname
    os.system(cmd)
