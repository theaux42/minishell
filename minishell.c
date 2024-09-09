/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:44:13 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/09 13:06:06 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_printsplit(char **split) {
    int i = 0;
    while (split[i] != NULL) {
        printf("%s\n", split[i]);
        i++;
    }
}

int main() {
    char *line;
    char **args;
    while (1) {
        line = readline(PROMPT);
        if (strcmp(line, "exit") == 0) {
            break;
        }

        pid_t pid = fork();
        if (pid == 0) {
            args = ft_split(line, ' ');
            ft_printsplit(args);
            args++;
            execlp(args[0], args[0], &args, NULL);
            perror("exec");
            ft_freesplit(args);
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}
