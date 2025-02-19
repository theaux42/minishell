#include <unistd.h>    // Pour les fonctions système comme fork(), pipe(), dup2()
#include <sys/wait.h>  // Pour waitpid()
#include <string.h>    // Pour strcmp()
#include <stdlib.h>    // Pour exit()

// Fonction utilitaire pour écrire sur la sortie d'erreur (fd 2)
// str: message principal, arg: argument optionnel à ajouter après le message
void ft_putstr_fd2(char *str, char *arg) {
    while (*str)
        write(2, str++, 1);    // Écrit caractère par caractère le message principal
    if (arg)
        while(*arg)
            write(2, arg++, 1); // Écrit l'argument si présent
    write(2, "\n", 1);         // Ajoute un retour à la ligne
}

// Fonction pour exécuter une commande dans un processus fils
// argv[]: tableau d'arguments de la commande
// i: index de fin des arguments (où se trouve le ; ou |)
// tmp_fd: descripteur de fichier pour l'entrée standard
// env[]: variables d'environnement
void ft_execute(char *argv[], int i, int tmp_fd, char *env[]) {
    argv[i] = NULL;                    // Termine le tableau d'arguments avec NULL
    dup2(tmp_fd, STDIN_FILENO);        // Redirige l'entrée standard vers tmp_fd
    close(tmp_fd);                     // Ferme le descripteur car plus nécessaire
    execve(argv[0], argv, env);        // Exécute la commande
    ft_putstr_fd2("error: cannot execute ", argv[0]); // Message d'erreur si execve échoue
    exit(1);                           // Termine le processus fils en cas d'échec
}

int main(int argc, char *argv[], char *env[]) {
    int i;                  // Index pour parcourir les arguments
    int fd[2];             // Descripteurs pour le pipe ([0]=lecture, [1]=écriture)
    int tmp_fd;            // Stocke temporairement l'entrée standard
    (void)argc;            // Évite l'avertissement de compilation pour argc non utilisé

    i = 0;
    tmp_fd = dup(STDIN_FILENO);    // Sauvegarde l'entrée standard originale

    // Boucle principale qui traite chaque commande
    while (argv[i] && argv[i + 1]) {
        argv = &argv[i + 1];        // Avance au début de la prochaine commande
        i = 0;
        
        // Compte le nombre d'arguments jusqu'au prochain ; ou |
        while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
            i++;

        // Gestion de la commande cd (cas spécial car commande interne)
        if (strcmp(argv[0], "cd") == 0) {
            if (i != 2)             // cd doit avoir exactement un argument
                ft_putstr_fd2("error: cd: bad arguments", NULL);
            else if (chdir(argv[1]) != 0)  // Tente de changer de répertoire
                ft_putstr_fd2("error: cd: cannot change directory to ", argv[1]);
        }
        // Exécution d'une commande simple (se terminant par ; ou fin de ligne)
        else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0)) {
            if (fork() == 0)        // Crée un processus fils pour exécuter la commande
                ft_execute(argv, i, tmp_fd, env);
            else {
                close(tmp_fd);       // Le parent ferme l'ancien descripteur
                while(waitpid(-1, NULL, WUNTRACED) != -1)  // Attend la fin de tous les fils
                    ;
                tmp_fd = dup(STDIN_FILENO);  // Restaure l'entrée standard pour la prochaine commande
            }
        }
        // Exécution d'une commande avec pipe (|)
        else if(i != 0 && strcmp(argv[i], "|") == 0) {
            pipe(fd);               // Crée un nouveau pipe
            if (fork() == 0) {      // Processus fils (écrivain)
                dup2(fd[1], STDOUT_FILENO);  // Redirige sa sortie standard vers le pipe
                close(fd[0]);       // Ferme l'extrémité de lecture inutilisée
                close(fd[1]);       // Ferme le descripteur dupliqué
                ft_execute(argv, i, tmp_fd, env);
            }
            else {                  // Processus parent
                close(fd[1]);       // Ferme l'extrémité d'écriture
                close(tmp_fd);      // Ferme l'ancien descripteur d'entrée
                tmp_fd = fd[0];     // Conserve l'extrémité de lecture pour la prochaine commande
            }
        }
    }
    close(tmp_fd);                 // Ferme le dernier descripteur
    return (0);
}
