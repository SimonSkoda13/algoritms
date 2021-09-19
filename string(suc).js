function duplicateEncode(word){
    var string="";
    for(var element1 =0;element1<word.length;element1++){
        var calc=0;
        for(var element2 =0;element2<word.length;element2++){
            if (word[element1]==word[element2]){
                calc++;
            }
        };
        if(calc>1){
            string+=")"
        }
        else{
            string+="("
        }
    };
}

  