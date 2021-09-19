var uniqueInOrder = function (iterable){
    var array=[];
    for(var element1 =0;element1<iterable.length;element1++){
        var calc=0;
        for(var element2 =0;element2<iterable.length;element2++){
            if (iterable[element1]==iterable[element2]){
                calc++;
            }
        };
        if(iterable[element1+1]!=iterable[element1]){
            array.push([iterable[element1]]);
        }
    };
    return array;
}
console.log(typeof(uniqueInOrder(['a','s','d'])));

  