pole=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25]
l=1
r=24
y=20
vysledok=0
while l<=r:
    s=int((l+r)/2)
    if y==pole[s]:
        vysledok=s
        break;
    elif y>pole[s]:
        l=s+1
    else:
        l=s-1
print(vysledok)