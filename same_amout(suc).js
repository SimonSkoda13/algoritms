function XO(str) {
    var x=0;
    var o=0;
    var strToLow=str.toLowerCase();
    for(var element of strToLow) {
        if (element=="o") {
            o++;
        }
        if (element=="x") {
            x++;
        }
    };
    if(o==x){
        return true;
    }
    else{
        return false;
    }
}