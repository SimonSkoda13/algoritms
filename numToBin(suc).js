var countBits = function(n) {
    var count=0;
    var newNum=(n >>> 0).toString(2);
    for(var element of newNum){
        if(element=="1"){
            count++
        }
    }
    return count
  };