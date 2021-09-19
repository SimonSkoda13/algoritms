function divisors(integer) {
    var count=[];
    for(var i=2;i<integer;i++){
        if(integer%i==0){
            count.push(i);
        }
    }
    if(count.length==2){
        return integer+' is prime'
    }
    return count
  };