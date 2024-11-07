#!/bin/bash

clear

make fclean > /dev/null

norm_errors=$(norminette | grep 'Error:' | wc -l)

yellow='\033[38;2;234;179;8m'
reset='\e[0m'
bold='\033[1m'
green='\033[38;2;0;255;0m'

if [ "$norm_errors" -ne 0 ]; then
    printf "[$yellow WARNING $reset]: Il y a$yellow $norm_errors$reset erreurs de norminette.\n"
    read -p "Voulez-vous afficher les erreurs? (Y/n): " show_errors
    show_errors=${show_errors:-Y}
    if [[ $show_errors =~ ^[Yy]$ ]]; then
        norminette | grep 'Error'
    fi
    read -p "Voulez-vous sortir du script? (Y/n): " exit_script
    exit_script=${exit_script:-Y}
    if [[ $exit_script =~ ^[Yy]$ ]]; then
        echo "Script annulé."
        exit 1
    fi
else
    printf "[$green INFO $reset]: Aucune erreur de norminette.\n"
fi

read -p "Entrez le message de commit: " commit_message
echo "Message de commit: '$commit_message'"
read -p "Confirmez-vous ce message de commit? (Y/n): " confirmation

confirmation=${confirmation:-Y}

if [[ $confirmation =~ ^[Yy]$ ]]; then
    git add *
    git commit -m "$commit_message"
    git push
else
    echo "Commit annulé."
fi