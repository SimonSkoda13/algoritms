pole=[1,-2,21,4,5,6]
m=0

''' 1.
for i in range(0,7):
    for j in range(i,7):
        s=0
        for k in range(i,j):
            s=s+pole[k]
        m=max(m,s)
print(m)'''


''' 2.
for i in range(0,7):
    s=0
    for j in range(i,6):
        s=s+pole[j]
        m=max(m,s)
print(m)'''

k=0
zaciatok=0
koniec=0
for j in range(0,6):
        k=max(k,0)+pole[j]
        m=max(m,k)
        if k==pole[j]:
            zaciatok=j
        if m==k:
            koniec=j
print(m,zaciatok,koniec)
