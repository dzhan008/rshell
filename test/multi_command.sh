echo "User enters two working commands with ;"
ls ; pwd
echo "User enters two working commands with ;(no space)"
ls;pwd
echo "User enters two commmands with ;. First fails."
la ; pwd
echo "User enters two commands with ;. Second fails."
ls ; ped
echo "User enters two working commands with ||"
ls || pwd
echo "User enters two working commands with ||(no space)"
ls||pwd
echo "User enters two commands with ||. First fails."
la || pwd
echo "User enters two commands with ||. Second fails."
ls || ped
echo "User enters two working commands with &&"
ls && pwd
echo "User enters two working commands with &&(no space)"
ls&&pwd
echo "User enters two working commands with &&. First fails."
la && pwd
echo "User enters two working commands with &&. Second fails."
ls && ped
echo "User enters three commands. First ||, then &&."
ls || pwd && ls -a
echo "User enters three commands. First ||, then &&. First fails."
la || pwd && ls -a
echo "User enters three commands. First ||, then &&. Second fails."
ls || ped && ls -a
echo "User enters three commands. First ||, then &&. Third fails."
ls || pwd && la -a
echo "User enters three commands. First ||, then &&. 1,2 fails."
la || ped && ls -a
echo "User enters three commands. First ||, then &&. 2,3 fails."
ls || ped && la -a
echo "User enters three commands. First ||, then &&. 1,2,3 fails."
la || ped && la -a
echo "User enters three commands. First &&, then ||."
ls && pwd || ls -a
echo "User enters three commands. First && then ||. First fails."
la && pwd || ls -a
echo "User enters three commands. First &&, then ||. Second fails."
ls && ped || ls -a
echo "User enters three commands. First &&, then ||. Third fails."
ls && pwd || la -a
echo "User enters three commands. First &&, then ||. 1,2 fails."
la && ped || ls -a
echo "User enters three commands. First &&, then ||. 2,3 fails."
ls && ped || la -a
echo "User enters three commands. First &&, then ||. 1,2,3, fails."
la && ped || la -a
echo "User enters three commands. First &&, then &&."
ls && pwd && ls -a
echo "User enters three commands. First && then &&. First fails."
la && pwd && ls -a
echo "User enters three commands. First &&, then &&. Second fails."
ls && ped && ls -a
echo "User enters three commands. First &&, then &&. Third fails."
ls && pwd && la -a
echo "User enters three commands. First &&, then &&. 1,2 fails."
la && ped && ls -a
echo "User enters three commands. First &&, then &&. 2,3 fails."
ls && ped && la -a
echo "User enters three commands. First &&, then &&. 1,2,3, fails."
la && ped && la -a
echo "User enters three commands. First ||, then ||."
ls || pwd || ls -a
echo "User enters three commands. First || then ||. First fails."
la || pwd || ls -a
echo "User enters three commands. First ||, then ||. Second fails."
ls || ped || ls -a
echo "User enters three commands. First ||, then ||. Third fails."
ls || pwd || la -a
echo "User enters three commands. First ||, then ||. 1,2 fails."
la || ped || ls -a
echo "User enters three commands. First ||, then ||. 2,3 fails."
ls || ped || la -a
echo "User enters three commands. First ||, then ||. 1,2,3, fails."
la || ped || la -a
