def isN555etc(num):
    calc=0
    for item in str(num):
        if(item=="5"):
            calc=calc+1
    if(calc==len(str(num))):
        return False
    else:
        return True
num=int(input())
nas=1
while(isN555etc(num*nas)):
    nas=nas+1
print(nas)
        
             
    