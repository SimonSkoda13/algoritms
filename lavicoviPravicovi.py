while(True):
    numberOfVotes=int(input())
    votesNumbersString=input() 
    numberOfVotes=votesNumbersString.split(" ")
    pravica=0
    lavica=0
    for item in numberOfVotes:
        if(int(item)>=0):
            pravica+=1
        else:
            lavica+=1
    if(lavica>pravica):
        print("lavica")
    elif(lavica<pravica):
        print("pravica")
    else:
        print("lavopravostred")