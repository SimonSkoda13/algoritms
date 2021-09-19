function rowSumOddNumbers(n) {
    var start=1;
    for (let i = 1; i < n; i++) {       
        start+=2*i;
    }
    var number=start;
    for (let j = 1; j < n; j++) {       
        number+= start+2*j;
    }
    return number;
    
}