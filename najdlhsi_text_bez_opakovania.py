pole=['a','c','d','r','u','y']
zaciatok=0
koniec=0
for j in range(0,6):
    for pismeno in pole:
        if pismeno == pole[j]:
            zaciatok=j
        else:
            koniec=j
print(zaciatok,koniec)
