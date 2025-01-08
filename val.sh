#!/bin/bash
# make
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --suppressions="other/valgrind-rules" ./minishell