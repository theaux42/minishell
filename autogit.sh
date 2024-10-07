#!/bin/bash

# Clean build files
make fclean

# Ask for commit message
read -p "Entrez le message de commit: " commit_message

# Confirm the commit message
echo "Message de commit: '$commit_message'"
read -p "Confirmez-vous ce message de commit? (Y/n): " confirmation

# Default to 'Y' if no input is given
confirmation=${confirmation:-Y}

if [[ $confirmation =~ ^[Yy]$ ]]; then
    git add *
    git commit -m "$commit_message"
    git push
else
    echo "Commit annulé."
fi