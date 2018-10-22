
/*
 0 |  1 |  2
___|____|____
 3 |  4 |  5
___|____|____
 6 |  7 |  8 
   |    |

*/



$(function(){

//none -->invisivel, block --> visivel
document.getElementById("escolha1").style.display = "block"; // "none" ou "block"
document.getElementById("escolha2").style.display = "none"; // "none" ou "block"
document.getElementById("escolha3").style.display = "none"; // "none" ou "block"
document.getElementById("jogo").style.display = "none"; // "none" ou "block"



var vez = 1;
var vencedor = "";
var tab = new Array(0,0,0,0,0,0,0,0,0);
var computador = 1;
var jogadaMinMax;
var jogadas =0;
var pos;
var JM=1;
var comecar;
var diff;


//JM==1 Jogador vs Maquina

document.getElementById("JM").addEventListener("click", function(){
    document.getElementById("escolha1").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha2").style.display = "block"; // "none" ou "block"
    document.getElementById("escolha3").style.display = "none"; // "none" ou "block"
    document.getElementById("jogo").style.display = "none"; // "none" ou "block"
});

//comecar ==se o jogador deseja começar 1-sim, 0-não

document.getElementById("comecar").addEventListener("click", function(){
    document.getElementById("escolha1").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha2").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha3").style.display = "block"; // "none" ou "block"
    document.getElementById("jogo").style.display = "none"; // "none" ou "block"
    $("#resultado").html("comecar");
});

document.getElementById("Ncomecar").addEventListener("click", function(){
    document.getElementById("escolha1").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha2").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha3").style.display = "block"; // "none" ou "block"
    document.getElementById("jogo").style.display = "none"; // "none" ou "block"
    $("#resultado").html("Ncomecar");

});

//diff == dificuldade 1-fácil, 2- médio, 3-impossível

document.getElementById("dif1").addEventListener("click", function(){
    document.getElementById("escolha1").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha2").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha3").style.display = "none"; // "none" ou "block"
    document.getElementById("jogo").style.display = "block"; // "none" ou "block"
    $("#resultado2").html("dif1");
});

document.getElementById("dif2").addEventListener("click", function(){
    document.getElementById("escolha1").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha2").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha3").style.display = "none"; // "none" ou "block"
    document.getElementById("jogo").style.display = "block"; // "none" ou "block"
    $("#resultado2").html("dif2");
});


document.getElementById("dif3").addEventListener("click", function(){
    document.getElementById("escolha1").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha2").style.display = "none"; // "none" ou "block"
    document.getElementById("escolha3").style.display = "none"; // "none" ou "block"
    document.getElementById("jogo").style.display = "block"; // "none" ou "block"
    $("#resultado2").html("dif3");
});




//jogo jogador vs máquina
if(JM == 1){
        
    $(".coluna").click( function(){

        var t= document.getElementById("resultado").innerHTML;

        if(t=="comecar"){
            comecar=1;

        }
        if(t=="Ncomecar"){
            comecar=0;
        }

        var t2= document.getElementById("resultado2").innerHTML;

        if(t2=="dif1"){
            diff=1;

        }

        if(t2=="dif2"){
            diff=2;

        }

        if(t2=="dif3"){
            diff=3;

        }
        




        //quem é o computador depende de quem começa
        if(comecar == 1 && JM ==1){
            computador=2;//computador não começa
        }


        if((vez == 1 && computador == 2) || (vez == 2 && computador ==1)){ //vez do jogador

            var bg = $(this).css("background-image");

            if(bg == "none" || bg == "")
            {          
                var fig = "url(" + vez.toString() + ".png)";
                $(this).css("background", fig);

                if(this.outerHTML == document.getElementById("casa0").outerHTML ){
                    tab[0]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa1").outerHTML ){
                    tab[1]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa2").outerHTML ){
                    tab[2]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa3").outerHTML ){
                    tab[3]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa4").outerHTML ){
                    tab[4]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa5").outerHTML ){
                    tab[5]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa6").outerHTML ){
                    tab[6]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa7").outerHTML ){
                    tab[7]=(vez== 1? 1 : -1);
                }
                else if(this.outerHTML == document.getElementById("casa8").outerHTML ){
                    tab[8]=(vez== 1? 1 : -1);
                }
                vez=(vez==1?2:1);
            }
            
            
        }else{ //vez do computador
            pos=-1;

            if(diff>=2){ //acrescentar hard rules

                var valor=(computador==1? 1:-1);
                //verificar se a IA tem possibilidade de vitoria
                pos=hardRules(tab.join(','),valor);
                
                //verificar se o adversário tem possibilidade de vitoria
                if (pos==-1){
                    pos=hardRules(tab.join(','), (-1)*valor);
                }

                if(pos==-1 && jogadas==3 && diff==3){
                    //verificar caso especial --> cilada

                    /*
                       |    |  X
                    ___|____|____
                       |  O |   
                    ___|____|____
                     X |    |    
                       |    |

                    */

                    if(tab[4]==valor){
                        if((tab[0]==(-1)*valor && tab[8]==(-1)*valor) || (tab[2]==(-1)*valor && tab[6]==(-1)*valor) ){
                            var aleatoria= Math.floor(Math.random() *4);
                            pos= 2*aleatoria+1
                            
                        }

                    }
                }

                if(pos==-1 && jogadas==3 && diff==3){
                    //verificar caso especial --> cilada 2

                    /*
                     X |    |  
                    ___|____|____
                       |  O |   
                    ___|____|____
                       |  X |    
                       |    |


                     O |    | X 
                    ___|____|____
                       |  O |   
                    ___|____|____
                       |  X |    
                       |    |


                       |  X |  
                    ___|____|____
                       |  O |   
                    ___|____|____
                     X |    |  O  
                       |    |

                    


                    */

                    if(tab[4]==valor){
                        if(tab[1]==(-1)*valor && ( tab[6]==(-1)*valor|| tab[8]==(-1)*valor)){
                           
                            var aleatoria= Math.floor(Math.random() *2);
                            pos= 2*aleatoria; //jogar em 0 ou 2
                            
                        }

                        if(tab[3]==(-1)*valor && ( tab[2]==(-1)*valor|| tab[8]==(-1)*valor)){
                           
                            var aleatoria= Math.floor(Math.random() *2);
                            pos= 6*aleatoria; //jogar em 0 ou 6
                            
                        }

                        if(tab[5]==(-1)*valor && ( tab[0]==(-1)*valor|| tab[6]==(-1)*valor)){
                           
                            var aleatoria= Math.floor(Math.random() *2);
                            pos= 6*aleatoria +2; //jogar em 2 ou 8
                            
                        }

                        if(tab[7]==(-1)*valor && ( tab[0]==(-1)*valor|| tab[2]==(-1)*valor)){
                           
                            var aleatoria= Math.floor(Math.random() *2);
                            pos= 2*aleatoria + 6; //jogar em 6 ou 8
                            
                        }

                    }
                }

            }

            if(pos==-1){
                jogadaMinMax = MinMax(tab.join(','),computador);
            }else{
                jogadaMinMax=pos;
            }
            
            tab[jogadaMinMax]=(vez== 1? 1 : -1);
                      
            var fig = "url(" + vez.toString() + ".png)";
            $(document.getElementById("casa"+jogadaMinMax)).css("background", fig);

            vez=(vez==1?2:1);
        
        }


        verificarFimDeJogo(tab.join(','), computador);
        jogadas=jogadas+1;



    });

}



});




function hardRules(tabuleiro, valor){
    var casa = tabuleiro.split(',');
    var i;
    var j;
    var soma;
    
    //verificar possibilidade de alinhamento
    
    //nas linhas
    for(i=0;i<3;i++){
        soma=0;
        for(j=0;j<3;j++){
            if( parseInt(casa[3*i+j])==(-1)*valor){ //não pode alinhar nessa linha
                break;
            }
            soma+= parseInt(casa[3*i+j]);
        }
        if(soma==2*valor && j==3){
            //pode alinhar
            for(j=0;j<3;j++){
                if(casa[3*i+j]=="0") //nessa posição será feita a jogada
                    return 3*i+j;
            }
        }

    }

    //nas colunas
    for(j=0;j<3;j++){
        soma=0;
        for(i=0;i<3;i++){
            if(parseInt(casa[3*i+j])==(-1)*valor){ //não pode alinhar nessa coluna
                break;
            }
            soma+= parseInt(casa[3*i+j]);
        }
        if(soma==2*valor && i==3){
            //pode alinhar
            for(i=0;i<3;i++){
                if(casa[3*i+j]=="0") //nessa posição será feita a jogada
                    return 3*i+j;
            }
        }
    }

    //na diag. principal
    soma=0;
    for(i=0;i<3;i++){
        if(parseInt(casa[3*i+i])==(-1)*valor){//não pode alinhar nessa coluna
           break; 
        }
        soma+=parseInt(casa[3*i+i]);
    }
    if(soma== 2*valor && i==3){
        for(i=0;i<3;i++){
            if(casa[3*i+i]=="0")
                return 3*i+i;
        }
    }

    //na diagonal secundária
    soma=0;
    for(i=0;i<3;i++){
        if(parseInt(casa[2*i+2])==(-1)*valor){//não pode alinhar nessa coluna
           break; 
        }
        soma+=parseInt(casa[2*i+2]);
    }
    if(soma== 2*valor && i==3){
        for(i=0;i<3;i++){
            if(casa[2*i+2]=="0")
                return 2*i+2;
        }
    }


    return -1;
}




function calcChances(casa, valor){

    var cas = casa.split(',');
    

    for(var i=0;i<9;i++){
        if(cas[i]==0){
            cas[i]=valor;
        }
    }

    var chances=0;
    var soma;
    //chances de alinhamentos nas linhas
    for(var i=0;i<3;i++){
        soma=0;
        for(var j=0;j<3;j++){
            soma=soma+parseInt(cas[3*i+j]);
        }
        if(soma == 3*valor){
            chances++;
        }
        
    }

    //chances de alinhamentos nas colunas
    for(var j=0;j<3;j++){
        soma=0;
        for(var i=0;i<3;i++){
            soma=soma+parseInt(cas[3*i+j]);
        }
        if(soma== 3*valor){
            chances++;
        }

    }
    //chance de alinhamento na diag. princ
    soma=0;
    for(var i=0;i<3;i++){
        soma=soma+parseInt(cas[3*i+i]);
    }
    if(soma== 3*valor){
        chances++;
    }
     
    //chance de alinhamento na diag. sec 
    soma=0;
    for(var i=0;i<3;i++){
        soma=soma+parseInt(cas[2*i+2]); //3*(i+1)- 1 -i == 2*i+2
    }
    if(soma== 3*valor){
        chances++;
    }

    //retorna o número de alinhamentos possiveis
    return chances;

}


function MinMax(tabuleiro,computador){

    var casa = tabuleiro.split(',');

    //esse vetor será a resultante de chances de alinhamento do computador - alinhamento do inimigo
    var chancesPC = new Array(0,0,0,0,0,0,0,0,0); 

    //calcular chances de alinhamento do pc para posição i
    var valor=(computador==1? 1:-1); //valor do computador

    for(var i = 0; i<9 ; i ++){
        if(casa[i]=="0"){//somente devemos calcular as chances para casas disponíveis (vazias)
            casa[i]=valor;
            chancesPC[i]=calcChances(casa.join(',') ,valor) - calcChances(casa.join(',') ,(-1)*valor);
            casa[i]="0";//liberar a casa;
        }
        else{
            chancesPC[i]=-10 ;//isso garante que casas ocupadas não serão selecionadas
        }
    }
    

    //verificar a posição com melhor chance de alinhamento
    var maior=0; //melhor representa a posição

    //buscar a maior chance de vitoria
    for(var i = 1; i<9 ; i++){
        if( chancesPC[i]>chancesPC[maior] ){ 
            maior=i;
        }
    }    
   
    return maior;
}



function casasIguais(a, b, c,tabuleiro){
    var casa = tabuleiro.split(',');

    if(casa[a]=="0" || casa[b] =="0" || casa[c]=="0"){
        return false;
    }


    if(casa[a]==casa[b] && casa[b]==casa[c]){
        vencedor= (casa[a]=="1"? 1 : 2);
        return true;
    }

    return false;
}

function verificarFimDeJogo(tabuleiro,computador){
    
    if( casasIguais(0, 1, 2,tabuleiro) || casasIguais(3, 4, 5,tabuleiro) || casasIguais(6, 7, 8,tabuleiro) ||
        casasIguais(0, 3, 6,tabuleiro) || casasIguais(1, 4, 7,tabuleiro) || casasIguais(2, 5, 8,tabuleiro) ||
        casasIguais(0, 4, 8,tabuleiro) || casasIguais(2, 4, 6,tabuleiro)
        ){
        if( JM==1){
            if((vencedor=="1" && computador==2) || (vencedor=="2" && computador==1)){
                window.alert("Parabéns você venceu o PC");
            }
            else{
                window.alert("Você perdeu haha! Foi derrotado pela IA");
            }
        }
        else{
            if((vencedor=="1" && computador==1) || (vencedor=="2" && computador==2)){
                window.alert("Parabéns sua IA venceu o adversário!");
            }
            else{
                window.alert("Seu computador perdeu haha! Foi derrotado pela outra IA");
            }

        }
        $(".coluna").off("click");
    }
    else{//verificar empate
        var casa= tabuleiro.split(',');
        var contadorZeros=0;
        for(var i=0;i<9;i++){
            if(casa[i]=="0"){
                contadorZeros++;
            }
        }
        if(contadorZeros==0){
            window.alert("Empate!");
            $(".coluna").off("click");
        }
    }
}

