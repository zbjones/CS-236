#! /bin/bash
EXAMPLEIN=(example/in21.txt	example/in22.txt	example/in23.txt	example/in24.txt	example/in25.txt	example/in26.txt	example/in27.txt	example/in28.txt	example/in61.txt	example/in62.txt)
EXAMPLEOUT=(example/out21.txt	example/out22.txt	example/out23.txt	example/out24.txt	example/out25.txt	example/out26.txt	example/out27.txt	example/out28.txt	example/out61.txt	example/out62.txt)
PASSOFF80IN=(cases/2-80/input0.txt	cases/2-80/input1.txt	cases/2-80/input2.txt	cases/2-80/input3.txt	cases/2-80/input4.txt	cases/2-80/input5.txt	cases/2-80/input6.txt	cases/2-80/input7.txt	cases/2-80/input8.txt)
PASSOFF80OUT=(cases/2-80/answer0.txt	cases/2-80/answer1.txt	cases/2-80/answer2.txt	cases/2-80/answer3.txt	cases/2-80/answer4.txt	cases/2-80/answer5.txt	cases/2-80/answer6.txt	cases/2-80/answer7.txt	cases/2-80/answer8.txt)
PASSOFF100IN=(cases/2-100/input0.txt  cases/2-100/input1.txt  cases/2-100/input2.txt)
PASSOFF100OUT=(cases/2-100/answer0.txt  cases/2-100/answer1.txt  cases/2-100/answer2.txt)
FILEPATH=""
PROGRAM=""
## Echo Color Variables
RED='\033[0;31m'
BLU='\033[0;34m'
GRN='\033[0;32m'
YEL='\033[1;33m'
GRY='\033[0;37m'
PUR='\033[0;35m'
BRN='\033[0;33m'
BLK='\033[0;30m'
CYN='\033[0;36m'
NC='\033[0m'

BRED='\033[0;41m'
BBLU='\033[0;44m'
BGRN='\033[0;42m'
BYEL='\033[1;43m'
BGRY='\033[0;47m'
BPUR='\033[0;45m'
BBRN='\033[0;43m'
BBLK='\033[0;40m'
BCYN='\033[0;46m'
echo -e "${CYN}Have you already compiled your code? [y , n]${NC}"
read -n 1 -r
echo ""
if [[ ! $REPLY =~ ^[Yy]$ ]]
then 
    echo -e "${CYN}List your cpp filepath(s) i.e \"${GRN}lab2/*.cpp${CYN}\"${NC}"
    read -e FILEPATH
    echo ""
    echo -e "${BLU}Compiling Code using \"${GRN}${FILEPATH}${BLU}\" ${NC}"
    g++ -Wall -Werror -std=c++17 -g $FILEPATH -o run
    PROGRAM="run"
else 
    echo -e "${CYN}What is the name of your compiled code? \e[2mIf you have made any changes to your code without compiling they will not take affect until it has be re-compiled.\e[22m${NC}"
    read -e PROGRAM
    echo ""
fi
touch tmpout.txt
touch tmpPass.txt
touch tmpDiff.txt
echo -e "${BLU}|	The corresponding lines differ.
<	You are missing this line of output.
>	You have have added this additional output.
(	You have fewer lines than expected.
)	You have more lines than expected.
\	You have extra output on this line.
/	You are missing some output on this line
${NC}"
passoff () {
    ./$PROGRAM $1 > tmpout.txt
    sdiff -l $2 tmpout.txt | cat -n | grep -v -e '($' > tmpDiff.txt
    if [ -s tmpDiff.txt ]
    then
        echo -e "${RED}Case $1 ${BRED}FAILED!${NC}";
        echo -e "${BRED}FAILED!${NC}
	
	" > tmpPass.txt
	touch tmpallpass.txt
    fi
   [ -s tmpDiff.txt ] && echo -e "$(cat tmpDiff.txt)"
}
echo -e "${CYN}Running Passoff cases on compiled code${NC}"

echo -e "${BBRN}Example Test Cases:${NC}"
echo -e "${BGRN}PASSED!${NC}

" > tmpPass.txt
for (( i=0; i < ${#EXAMPLEIN[@]}; i++ ))
do
IN=${EXAMPLEIN[$i]}
OUT=${EXAMPLEOUT[$i]}
passoff "$IN" "$OUT"
done
echo -e "${BBRN}Example Test Cases:${NC}${BRN} have ${NC}$(cat tmpPass.txt)"

echo -e "${BBLU}Passoff Cases: 80%${NC}"
echo -e "${BGRN}PASSED!${NC}

" > tmpPass.txt
for (( i=0; i < ${#PASSOFF80IN[@]}; i++ ))
do
IN=${PASSOFF80IN[$i]}
OUT=${PASSOFF80OUT[$i]}
passoff "$IN" "$OUT"
done
echo -e "${BBLU}Passoff Cases: 80%${NC}${BLU} have ${NC}$(cat tmpPass.txt)"

echo -e "${BPUR}Passoff Cases: 100%${NC}"
echo -e "${BGRN}PASSED!${NC}

" > tmpPass.txt
for (( i=0; i < ${#PASSOFF100IN[@]}; i++ ))
do
IN=${PASSOFF100IN[$i]}
OUT=${PASSOFF100OUT[$i]}
passoff "$IN" "$OUT"
done
echo -e "${BPUR}Passoff Cases: 100%${NC}${PUR} have ${NC}$(cat tmpPass.txt)"
echo -e "${CYN}Test cases complete!${NC}"
if ! [ -f tmpallpass.txt ]
then
	temp_cnt=10
	while [[ ${temp_cnt} -gt 0 ]];
	do
	    printf "\rBreathe you'll be done in %2d seconds... " ${temp_cnt}
	    sleep 1
	    ((temp_cnt--))
	done
	clear
fi
CONGRATS="
                    ${YEL}==                     ==${CYN}
                 <^\()/^>               <^\()/^>
                  \/  \/                 \/  \/
                   /__\      ${YEL}.${CYN}  ${YEL}'${CYN}  ${YEL}.${CYN}      /__\ 
      ${YEL}==${CYN}            /\    ${YEL}.     |     ${YEL}.${CYN}    /\            ${YEL}==${CYN}
   <^\()/^>       !_\/       ${YEL}'  |  ${YEL}'${CYN}       \/_!       <^\()/^>
    \/  \/     !_/I_||  ${YEL}.${CYN}  ${YEL}'   \'/   ${YEL}'${CYN}  ${YEL}.${CYN}  ||_I\_!     \/  \/
     /__\     /I_/| ||      ${YEL}-== ${NC}${BYEL}+${CYN} ${YEL}==-${CYN}      || |\_I\     /__\    
     /_ \   !//|  | ||  ${YEL}'${CYN}  ${YEL}.   /${YEL}.\   ${YEL}.${CYN}  ${YEL}'${CYN}  || |  |\'\!  /_ \    
    (-   ) /I/ |  | ||       ${YEL}.  |  ${YEL}.${CYN}       || |  | \I\ (=   )
     \__/!//|  |  | ||    ${YEL}'     |     ${YEL}'${CYN}    || |  |  |\'\!\__/
     /  \I/ |  |  | ||       ${YEL}'${CYN}  ${YEL}.${CYN}  ${YEL}'${CYN}    ${YEL}*${CYN}  || |  |  | \I/  \    
    {_ __}  |  |  | ||   ${BCYN}Congratulations!${CYN}  || |  |  |  {____}
 _!__|= ||  |  |  | ||   ${YEL}*${CYN}      ${NC}${BYEL}+${CYN}          || |  |  |  ||  |__!_
 _I__|  ||__|__|__|_||          A          ||_|__|__|__||- |__I_
 -|--|- ||--|--|--|-||       __/_\__  ${YEL}*${CYN}    ||-|--|--|--||= |--|-
  |  |  ||  |  |  | ||      /\-${YEL}'o'${CYN}-/\      || |  |  |  ||  |  |
  |  |= ||  |  |  | ||     _||${YEL}:<_>:${CYN}||_     || |  |  |  ||= |  |
  |  |- ||  |  |  | || ${YEL}*${CYN}  /\_/=====\_/\  ${YEL}*${CYN} || |  |  |  ||= |  |
  |  |- ||  |  |  | ||  __|:_:_${YEL}[I]${CYN}_:_:|__  || |  |  |  ||- |  | 
 _|__|  ||__|__|__|_||${GRN}:::::::::::::::::::::${CYN}||_|__|__|__||  |__|_
 -|--|= ||--|--|-${BLU}You passed all of the test cases${CYN}|--|--||- |--|-
  jgs|- ||  |  |  | ||::${BLU}You may now enter${CYN}::|| |  |  |  ||= |  | 
${RED}~~~~~~~~~^^^^^^^^^^^^^^^${BRED}The Passoff Queue${RED}^^^^^^^^^^^^^^^^^^^~~~~~~~~~${NC}"
[ -f tmpallpass.txt ] || echo -e "${CYN} $CONGRATS ${NC}"
rm -f tmpDiff.txt
rm -f tmpPass.txt
rm -f tmpout.txt
rm -f tmpallpass.txt
[ -f tmpallpass.txt ] && rm -f tmpallpass.txt
